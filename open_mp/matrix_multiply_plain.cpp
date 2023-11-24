#include <iostream>
#include <vector>
#include <omp.h>
#include <string>
#include <map>
#include "../utils/utils.h"

// Function to multiply two matrices using OpenMP
void matrixMultiply(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B, std::vector<std::vector<int>>& result) {
    int size = A.size();
    int numThreads = omp_get_num_threads();
    double start_time, end_time;
    start_time = omp_get_wtime();
    #pragma omp parallel for
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            result[i][j] = 0;
            for (int k = 0; k < size; ++k) {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    end_time = omp_get_wtime();
    printf("Threads: %d Time Taken:%f\n",numThreads , end_time-start_time); 
}

int main() {
    // Define matrix size
    const int size = 3;

    // Initialize matrices A, B, and result
    std::vector<std::vector<int>> A(size, std::vector<int>(size, 1));
    std::vector<std::vector<int>> B(size, std::vector<int>(size, 2));
    std::vector<std::vector<int>> result(size, std::vector<int>(size, 0));

    // Multiply matrices using OpenMP
    matrixMultiply(A, B, result);
    return 0;
}
