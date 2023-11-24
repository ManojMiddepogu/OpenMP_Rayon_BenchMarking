#include <iostream>
#include <vector>
#include <omp.h>

// Function to multiply two matrices using OpenMP
void matrixMultiply(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B, std::vector<std::vector<int>>& result) {
    int size = A.size();

    #pragma omp parallel for
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            result[i][j] = 0;
            for (int k = 0; k < size; ++k) {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }
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

    // Display the result
    std::cout << "Matrix A:" << std::endl;
    for (const auto& row : A) {
        for (int element : row) {
            std::cout << element << ' ';
        }
        std::cout << std::endl;
    }

    std::cout << "Matrix B:" << std::endl;
    for (const auto& row : B) {
        for (int element : row) {
            std::cout << element << ' ';
        }
        std::cout << std::endl;
    }

    std::cout << "Result:" << std::endl;
    for (const auto& row : result) {
        for (int element : row) {
            std::cout << element << ' ';
        }
        std::cout << std::endl;
    }

    return 0;
}
