use rayon::prelude::*;
use std::time::Instant;
use std::collections::HashMap;
use rayon::current_num_threads;

mod utils;

fn main() {
    // Read JSON from file
    let config_file_path = "../Config/profile.json";
    let output_folder = "../output_csvs/";

    // Parse JSON and create a map
    let mut args: HashMap<String, i32> = HashMap::new();
    match utils::read_json_file(config_file_path) {
        Ok(result_map) => {
            // Now `result_map` is a HashMap<String, i32> representing your JSON data.
            println!("{:?}", result_map);
            args = result_map;
        }
        Err(e) => {
            eprintln!("{}", e);
        }
    }

    let total_runs = args["runs"];
    let num_threads = args["num_threads"];
    rayon::ThreadPoolBuilder::new().num_threads(num_threads.try_into().unwrap()).build_global().unwrap();
    let threads = current_num_threads();
    println!("Threads: {}", threads);

    let mut thread_creation_times: Vec<Vec<f64>> = Vec::new();
    let mut thread_execution_times: Vec<Vec<f64>> = Vec::new();
    let mut thread_total_times: Vec<Vec<f64>> = Vec::new();

    for runs in 0..total_runs {
        let start_time = Instant::now();

        let (creation_times, execution_times): (Vec<_>, Vec<_>) = (0..num_threads)
            .into_par_iter()
            .map(|thread_no| {
                let thread_creation_time = start_time.elapsed().as_secs_f64();
                // println!(
                //     "Run {}, Thread {} creation time: {} seconds",
                //     runs, thread_no, thread_creation_time
                // );

                // Work goes here
                std::thread::sleep(std::time::Duration::from_secs(1));

                let thread_execution_time = start_time.elapsed().as_secs_f64();
                // println!(
                //     "Run {}, Thread {} execution time: {} seconds",
                //     runs, thread_no, thread_execution_time
                // );

                (thread_creation_time, thread_execution_time)
            })
            .unzip();

        // Total execution time
        let total_time: Vec<_> = vec![start_time.elapsed().as_secs_f64()];
        println!(
            "Runs {}, Threads {}, total time: {:?} seconds",
            runs, num_threads, total_time
        );

        // Group times
        thread_creation_times.push(creation_times);
        thread_execution_times.push(execution_times);
        thread_total_times.push(total_time);
        let flattened_times: Vec<f64> = thread_creation_times.iter().flat_map(|row| row.iter()).cloned().collect();

        // Calculate the sum of all elements
        let sum: f64 = flattened_times.iter().sum();

        // Calculate the average
        let average = sum / (flattened_times.len() as f64);

        println!("Average: {}", average);
    }

    // Pushing data to csv
    let thread_creation_filename = format!(
        "{}rayon_thread_creation{}{}.csv",
        output_folder,
        utils::args_to_str(&args),
        ".csv"
    );
    let thread_execution_filename = format!(
        "{}rayon_thread_execution{}{}.csv",
        output_folder,
        utils::args_to_str(&args),
        ".csv"
    );
    let thread_totaltime_filename = format!(
        "{}rayon_thread_total_times{}{}.csv",
        output_folder,
        utils::args_to_str(&args),
        ".csv"
    );

    utils::write_csv(&thread_creation_filename, &thread_creation_times);
    utils::write_csv(&thread_execution_filename, &thread_execution_times);
    utils::write_csv(&thread_totaltime_filename, &thread_total_times);
}

// Placeholder for the missing functions (read_json_file, args_to_str, write_csv)
