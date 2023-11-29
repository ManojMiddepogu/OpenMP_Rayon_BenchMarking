#include <iostream>
#include <vector>
#include <omp.h>
#include <string>
#include <map>
#include "../utils/utils.h"


int total_runs;
int num_threads;
int size;
bool row_parallel;
bool block_parallel;
int block_size;
bool strassens;
bool linear_matrix;
bool linear_matrix_block;

double calculateMean(const std::vector<double>& data) {
    double sum = 0.0;
    for (const double& value : data) {
        sum += value;
    }
    return sum / static_cast<double>(data.size());
}

double calculateVariance(const std::vector<double>& data, double mean) {
    double sumSquaredDiff = 0.0;
    for (const double& value : data) {
        double diff = value - mean;
        sumSquaredDiff += diff * diff;
    }
    return sumSquaredDiff / static_cast<double>(data.size());
}

double row_parallel_matrixMultiply(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B, std::vector<std::vector<int>>& result) {
    int size = A.size();
    // int numThreads = omp_get_num_threads();
    double start_time, end_time;
    start_time = omp_get_wtime();
    #pragma omp parallel for collapse(2) schedule(static)
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            double sum = 0;
            #pragma omp simd reduction(+:sum)
            for (int k = 0; k < size; ++k) {
                sum += A[i][k] * B[k][j];
            }
            result[i][j] = sum;
        }
    }
    end_time = omp_get_wtime(); 
    return (end_time - start_time);
}

template <typename T>
double linear_matrixMultiply(const std::vector<T>& A, const std::vector<T>& B, std::vector<T>& result, int size) {
    double start_time, end_time;
    start_time = omp_get_wtime();
    #pragma omp parallel for collapse(2) schedule(static)
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            T sum = 0;
            #pragma omp simd reduction(+:sum)
            for (int k = 0; k < size; ++k) {
                sum += A[i * size + k] * B[k * size + j];
            }
            result[i * size + j] = sum;
        }
    }
    end_time = omp_get_wtime(); 
    return (end_time - start_time);
}

template <typename T>
double linear_matrixMultiply_Block(const std::vector<T>& A, const std::vector<T>& B, std::vector<T>& result, int size, int blockSize) {
    double start_time, end_time;
    start_time = omp_get_wtime();

    #pragma omp parallel for collapse(2) schedule(static)
    for (int i = 0; i < size; i += blockSize) {
        for (int j = 0; j < size; j += blockSize) {
            for (int k = 0; k < size; k += blockSize) {
                for (int ii = 0; ii < blockSize; ++ii) {
                    for (int jj = 0; jj < blockSize; ++jj) {
                        T sum = 0;
                        #pragma omp simd reduction(+:sum)
                        for (int kk = 0; kk < blockSize; ++kk) {
                            sum += A[(i + ii) * size + k + kk] * B[(k + kk) * size + j + jj];
                        }
                        result[(i + ii) * size + j + jj] += sum;
                    }
                }
            }
        }
    }

    end_time = omp_get_wtime(); 
    return (end_time - start_time);
}

template <typename T>
double matrixMultiplyBlocks(const std::vector<std::vector<T>>& A,
                                   const std::vector<std::vector<T>>& B,
                                   std::vector<std::vector<T>>& C,
                                   int size, int blockSize) {
    double start_time, end_time;
    start_time = omp_get_wtime();
    #pragma omp parallel for collapse(2) schedule(static)
    for (int i = 0; i < size; i += blockSize) {
        for (int j = 0; j < size; j += blockSize) {
            for (int k = 0; k < size; k += blockSize) {
                for (int ii = i; ii < i + blockSize; ++ii) {
                    for (int jj = j; jj < j + blockSize; ++jj) {
                        T sum = 0.0;

                        // Trying Loop unrolling
                        #pragma omp simd reduction(+:sum)
                        for (int kk = k; kk < k + blockSize; kk += 4) {
                            sum += A[ii][kk] * B[kk][jj]
                                        + A[ii][kk + 1] * B[kk + 1][jj]
                                        + A[ii][kk + 2] * B[kk + 2][jj]
                                        + A[ii][kk + 3] * B[kk + 3][jj];
                        }

                        C[ii][jj] += sum;
                    }
                }
            }
        }
    }

    end_time = omp_get_wtime(); 
    return (end_time - start_time);
}

template <typename T>
void addMatrix(const std::vector<std::vector<T>>& A, const std::vector<std::vector<T>>& B, std::vector<std::vector<T>>& C, int size) {
    #pragma omp parallel for
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
}

template <typename T>
void subtractMatrix(const std::vector<std::vector<T>>& A, const std::vector<std::vector<T>>& B, std::vector<std::vector<T>>& C, int size) {
    #pragma omp parallel for
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            C[i][j] = A[i][j] - B[i][j];
        }
    }
}

template <typename T>
double strassenMultiply(const std::vector<std::vector<T>>& A, const std::vector<std::vector<T>>& B, std::vector<std::vector<T>>& C, int size) {
    
    double start_time, end_time;
    start_time = omp_get_wtime();
    if(size <= 64){
        return matrixMultiplyBlocks(A, B, C, size, block_size);
    }
    int newSize = size / 2;

    // Create submatrices
    std::vector<std::vector<T>> A11(newSize, std::vector<T>(newSize));
    std::vector<std::vector<T>> A12(newSize, std::vector<T>(newSize));
    std::vector<std::vector<T>> A21(newSize, std::vector<T>(newSize));
    std::vector<std::vector<T>> A22(newSize, std::vector<T>(newSize));

    std::vector<std::vector<T>> B11(newSize, std::vector<T>(newSize));
    std::vector<std::vector<T>> B12(newSize, std::vector<T>(newSize));
    std::vector<std::vector<T>> B21(newSize, std::vector<T>(newSize));
    std::vector<std::vector<T>> B22(newSize, std::vector<T>(newSize));

    std::vector<std::vector<T>> C11(newSize, std::vector<T>(newSize));
    std::vector<std::vector<T>> C12(newSize, std::vector<T>(newSize));
    std::vector<std::vector<T>> C21(newSize, std::vector<T>(newSize));
    std::vector<std::vector<T>> C22(newSize, std::vector<T>(newSize));

    #pragma omp parallel for
    for (int i = 0; i < newSize; ++i) {
        for (int j = 0; j < newSize; ++j) {
            A11[i][j] = A[i][j];
            A12[i][j] = A[i][j + newSize];
            A21[i][j] = A[i + newSize][j];
            A22[i][j] = A[i + newSize][j + newSize];

            B11[i][j] = B[i][j];
            B12[i][j] = B[i][j + newSize];
            B21[i][j] = B[i + newSize][j];
            B22[i][j] = B[i + newSize][j + newSize];
        }
    }

    // Calculate products
    std::vector<std::vector<T>> P1(newSize, std::vector<T>(newSize));
    std::vector<std::vector<T>> P2(newSize, std::vector<T>(newSize));
    std::vector<std::vector<T>> P3(newSize, std::vector<T>(newSize));
    std::vector<std::vector<T>> P4(newSize, std::vector<T>(newSize));
    std::vector<std::vector<T>> P5(newSize, std::vector<T>(newSize));
    std::vector<std::vector<T>> P6(newSize, std::vector<T>(newSize));
    std::vector<std::vector<T>> P7(newSize, std::vector<T>(newSize));

    #pragma omp parallel sections
    {
        #pragma omp section
        {
            addMatrix(A11, A22, C11, newSize);
            addMatrix(B11, B22, P1, newSize);
            strassenMultiply(C11, P1, P2, newSize);
        }

        #pragma omp section
        {
            addMatrix(A21, A22, C21, newSize);
            strassenMultiply(C21, B11, P3, newSize);
        }

        #pragma omp section
        {
            subtractMatrix(B12, B22, P4, newSize);
            strassenMultiply(A11, P4, P5, newSize);
        }

        #pragma omp section
        {
            subtractMatrix(B21, B11, P5, newSize);
            strassenMultiply(A22, P5, P6, newSize);
        }

        #pragma omp section
        {
            addMatrix(A11, A12, C12, newSize);
            strassenMultiply(C12, B22, P7, newSize);
        }

        #pragma omp section
        {
            subtractMatrix(A21, A11, P1, newSize);
            addMatrix(B11, B12, P2, newSize);
            strassenMultiply(P1, P2, P3, newSize);
        }

        #pragma omp section
        {
            subtractMatrix(A12, A22, P3, newSize);
            addMatrix(B21, B22, P4, newSize);
            strassenMultiply(P3, P4, P5, newSize);
        }

        #pragma omp section
        {
            addMatrix(A21, A22, P1, newSize);
            subtractMatrix(B12, B22, P2, newSize);
            strassenMultiply(P1, P2, P6, newSize);
        }

        #pragma omp section
        {
            subtractMatrix(A11, A21, P1, newSize);
            addMatrix(B11, B21, P2, newSize);
            strassenMultiply(P1, P2, P7, newSize);
        }
    }

    // Calculate C matrix
    #pragma omp parallel for
    for (int i = 0; i < newSize; ++i) {
        for (int j = 0; j < newSize; ++j) {
            C[i][j] = P1[i][j] + P2[i][j] - P5[i][j] + P6[i][j];
            C[i][j + newSize] = P4[i][j] + P5[i][j];
            C[i + newSize][j] = P6[i][j] - P3[i][j];
            C[i + newSize][j + newSize] = P1[i][j] - P3[i][j] + P7[i][j];
        }
    }

    end_time = omp_get_wtime(); 
    return (end_time - start_time);
}


int main() {
    string config_file_pth = "../Config/config_matrix_multiply_cpp.json";
    string output_folder = "../output_csvs/";

    // Parse JSON and create a map
    map<string, int> args = readJsonFile(config_file_pth);

    total_runs = args["runs"];
    num_threads = args["num_threads"];
    size = args["size"];
    row_parallel = args["row_parallel"];
    block_parallel = args["block_parallel"];
    block_size = args["block_size"];
    strassens = args["strassens"];
    linear_matrix = args["linear_matrix"];
    linear_matrix_block = args["linear_matrix_block"];

    omp_set_num_threads(num_threads);

    // Initialize matrices A, B, and result
    std::vector<std::vector<int>> A(size, std::vector<int>(size, 1));
    std::vector<std::vector<int>> B(size, std::vector<int>(size, 2));
    std::vector<std::vector<int>> result(size, std::vector<int>(size, 0));

    if(row_parallel){
        std::vector<double> data;
        for(int i = 0; i< total_runs; i++){
            data.push_back(row_parallel_matrixMultiply(A, B, result));
        }
        double mean = calculateMean(data);
        double variance = calculateVariance(data, mean);
        cout<< mean <<" "<< variance << endl;
    } else if(block_parallel){
        std::vector<double> data;
        for(int i = 0; i< total_runs; i++){
            data.push_back(matrixMultiplyBlocks(A, B, result, size, block_size));
        }
        double mean = calculateMean(data);
        double variance = calculateVariance(data, mean);
        cout<< mean <<" "<< variance << endl;
    } else if(strassens){
        std::vector<double> data;
        for(int i = 0; i< total_runs; i++){
            data.push_back(strassenMultiply(A, B, result, size));
        }
        double mean = calculateMean(data);
        double variance = calculateVariance(data, mean);
        cout<< mean <<" "<< variance << endl;
    } else if(linear_matrix){
        std::vector<double> data;
        std::vector<int> A(size * size, 1), B(size * size, 1), result(size * size, 1);
        for(int i = 0; i< total_runs; i++){
            data.push_back(linear_matrixMultiply(A, B, result, size));
        }
        double mean = calculateMean(data);
        double variance = calculateVariance(data, mean);
        cout<< mean <<" "<< variance << endl;
    } else if(linear_matrix_block){
        std::vector<double> data;
        std::vector<int> A(size * size, 1), B(size * size, 1), result(size * size, 1);
        for(int i = 0; i< total_runs; i++){
            data.push_back(linear_matrixMultiply_Block(A, B, result, size, block_size));
        }
        double mean = calculateMean(data);
        double variance = calculateVariance(data, mean);
        cout<< mean <<" "<< variance << endl;
    }
    return 0;
}