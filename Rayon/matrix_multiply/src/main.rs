extern crate rayon;

use rayon::current_num_threads;
use rayon::prelude::*;
use std::env;
use std::thread;
use std::time::Duration;
use std::time::Instant;

// Done
fn parallel_matrix_multiply(a: &[Vec<i32>], b: &[Vec<i32>]) -> Vec<Vec<i32>> {
    let size = a.len();
    let mut result = vec![vec![0; size]; size];

    let start_time = Instant::now();
    result.par_iter_mut().enumerate().for_each(|(i, row)| {
        let temp_row: Vec<i32> = (0..size)
            .into_par_iter()
            .map(|j| {
                a[i].iter()
                    .zip(b.iter().map(|row| row[j]))
                    .map(|(x, y)| x * y)
                    .sum()
            })
            .collect();
        row.copy_from_slice(&temp_row);
    });

    let elapsed_time = start_time.elapsed();
    println!("Time taken: {:?}", elapsed_time);

    result
}

fn matrix_multiply(a: &[Vec<i32>], b: &[Vec<i32>]) -> Vec<Vec<i32>> {
    let rows_a = a.len();
    let cols_a = a[0].len();
    let cols_b = b[0].len();
    let start_time = Instant::now();
    let mut result = vec![vec![0; cols_b]; rows_a];
    result.par_iter_mut().enumerate().for_each(|(i, row)| {
        for j in 0..cols_b {
            for k in 0..cols_a {
                row[j] += a[i][k] * b[k][j];
            }
        }
    });
    let elapsed_time = start_time.elapsed();
    println!("Time taken: {:?}", elapsed_time);

    result
}

fn matrix_multiply_block(A: &[Vec<i32>], B: &[Vec<i32>], block_size: usize) -> Vec<Vec<i32>> {
    let n = A.len();
    let p = B.len();
    let m = B[0].len();

    let mut result = vec![vec![0; m]; n];
    result
        .par_chunks_mut(block_size)
        .enumerate()
        .for_each(|(block_row, block_rows)| {
            for block_col in (0..m).step_by(block_size) {
                for i in 0..block_size {
                    if block_row * block_size + i >= n {
                        break;
                    }
                    for j in block_col..std::cmp::min(block_col + block_size, m) {
                        let mut sum = 0;
                        for k in 0..p {
                            sum += A[block_row * block_size + i][k] * B[k][j];
                        }
                        block_rows[i][j] = sum;
                    }
                }
            }
        });
    result
}

fn flat_multiply(a: &[i32], b: &[i32], size: usize) -> Vec<i32> {
    let mut result = vec![0; size * size];

    let start_time = Instant::now();
    result.par_iter_mut().enumerate().for_each(|(i, element)| {
        let row = i / size;
        let col = i % size;

        *element = (0..size)
            .into_par_iter()
            .map(|k| a[row * size + k] * b[k * size + col])
            .sum();
    });
    let elapsed_time = Instant::now().duration_since(start_time).as_secs_f64();
    println!("Time taken: {:?}", elapsed_time);

    result
}

fn add_matrices(a: &[Vec<i32>], b: &[Vec<i32>]) -> Vec<Vec<i32>> {
    a.par_iter()
        .zip(b.par_iter())
        .map(|(row_a, row_b)| {
            row_a
                .iter()
                .zip(row_b.iter())
                .map(|(&x, &y)| x + y)
                .collect()
        })
        .collect()
}

fn subtract_matrices(a: &[Vec<i32>], b: &[Vec<i32>]) -> Vec<Vec<i32>> {
    a.par_iter()
        .zip(b.par_iter())
        .map(|(row_a, row_b)| {
            row_a
                .iter()
                .zip(row_b.iter())
                .map(|(&x, &y)| x - y)
                .collect()
        })
        .collect()
}

fn split_matrix(
    matrix: &[Vec<i32>],
) -> (Vec<Vec<i32>>, Vec<Vec<i32>>, Vec<Vec<i32>>, Vec<Vec<i32>>) {
    let n = matrix.len();
    let m = n / 2;
    let mut a11 = vec![vec![0; m]; m];
    let mut a12 = vec![vec![0; m]; m];
    let mut a21 = vec![vec![0; m]; m];
    let mut a22 = vec![vec![0; m]; m];

    for i in 0..m {
        for j in 0..m {
            a11[i][j] = matrix[i][j];
            a12[i][j] = matrix[i][j + m];
            a21[i][j] = matrix[i + m][j];
            a22[i][j] = matrix[i + m][j + m];
        }
    }

    (a11, a12, a21, a22)
}

fn strassen_multiply(a: &[Vec<i32>], b: &[Vec<i32>]) -> Vec<Vec<i32>> {
    let n = a.len();
    if n <= 128 {
        return matrix_multiply_block(a, b, 16);
    }

    let (a11, a12, a21, a22) = split_matrix(a);
    let (b11, b12, b21, b22) = split_matrix(b);

    let (p1, p2) = rayon::join(
        || strassen_multiply(&add_matrices(&a11, &a22), &add_matrices(&b11, &b22)),
        || strassen_multiply(&add_matrices(&a21, &a22), &b11),
    );

    let (p3, p4) = rayon::join(
        || strassen_multiply(&a11, &subtract_matrices(&b12, &b22)),
        || strassen_multiply(&a22, &subtract_matrices(&b21, &b11)),
    );

    let (p5, p6) = rayon::join(
        || strassen_multiply(&add_matrices(&a11, &a12), &b22),
        || strassen_multiply(&subtract_matrices(&a21, &a11), &add_matrices(&b11, &b12)),
    );

    let p7 = strassen_multiply(&subtract_matrices(&a12, &a22), &add_matrices(&b21, &b22));

    let c11 = add_matrices(&subtract_matrices(&add_matrices(&p1, &p4), &p5), &p7);
    let c12 = add_matrices(&p3, &p5);
    let c21 = add_matrices(&p2, &p4);
    let c22 = add_matrices(&subtract_matrices(&add_matrices(&p1, &p3), &p2), &p6);

    let mut result = vec![vec![0; n]; n];

    let m = n / 2;
    for i in 0..m {
        for j in 0..m {
            result[i][j] = a11[i][j];
            result[i][j + m] = a12[i][j];
            result[i + m][j] = a21[i][j];
            result[i + m][j + m] = a22[i][j];
        }
    }

    result
}

fn main() {
    let args: Vec<String> = env::args().collect();

    if args.len() != 4 {
        eprintln!("Usage: cargo run --release -- <nThreads> <matrix_size> <Algo>");
        std::process::exit(1);
    }

    let size: usize = args[2].parse().expect("Invalid matrix size");
    let nThreads: usize = args[1].parse().expect("Invalid no of Threads");
    rayon::ThreadPoolBuilder::new()
        .num_threads(nThreads)
        .build_global()
        .unwrap();

    let num_threads = current_num_threads();
    println!("Threads: {}, size: {}", num_threads, size);

    let algo: usize = args[3].parse().expect("Invalid Algo");
    if algo == 0 {
        // 2 level Parallel Matrix Multiplication
        let matrix_a: Vec<Vec<i32>> = vec![vec![2; size]; size];
        let matrix_b: Vec<Vec<i32>> = vec![vec![2; size]; size];
        let _ = parallel_matrix_multiply(&matrix_a, &matrix_b);
    }
    if algo == 1 {
        // Block Matrix Multiply
        let start_time = Instant::now();
        let matrix_a: Vec<Vec<i32>> = vec![vec![2; size]; size];
        let matrix_b: Vec<Vec<i32>> = vec![vec![2; size]; size];
        let _ = matrix_multiply_block(&matrix_a, &matrix_b, 16);
        let elapsed_time = Instant::now().duration_since(start_time).as_secs_f64();
        println!("Time taken: {:?}", elapsed_time);
    }
    if algo == 2 {
        // Strassen
        let matrix_a: Vec<Vec<i32>> = vec![vec![2; size]; size];
        let matrix_b: Vec<Vec<i32>> = vec![vec![2; size]; size];
        let start_time = Instant::now();
        let _ = strassen_multiply(&matrix_a, &matrix_b);
        let elapsed_time = Instant::now().duration_since(start_time).as_secs_f64();
        println!("Time taken: {:?}", elapsed_time);
    }
    if algo == 3 {
        // Linear
        let matrix_a: Vec<i32> = vec![2; (size * size).try_into().unwrap()];
        let matrix_b: Vec<i32> = vec![2; (size * size).try_into().unwrap()];
        let answer = flat_multiply(&matrix_a, &matrix_b, size.try_into().unwrap());
    }
    if algo == 4 {
        let matrix_a: Vec<Vec<i32>> = vec![vec![2; size]; size];
        let matrix_b: Vec<Vec<i32>> = vec![vec![2; size]; size];
        let _ = matrix_multiply(&matrix_a, &matrix_b);
    }
}