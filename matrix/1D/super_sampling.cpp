#include <numeric>
#include <iostream>
#include <iomanip>
#include <vector>



template <typename T>
void print(const std::vector<T>& a, const size_t& m, const size_t& n) {
    for (size_t i = 0; i < m; i++) {
        for (size_t j = i*m; j < i*m + n; j++) {
            std::cout.precision(8);
            std::cout << std::setw(10) << a[j];
        }
        std::cout << "\n";
    }
}


template <typename T>
std::vector<T> sups(const std::vector<T>& matrix, const size_t& m, const size_t& n, const unsigned int sups_factor) {
    const size_t sups_m = m * sups_factor - sups_factor + 1;
    const size_t sups_n = n * sups_factor - sups_factor + 1; 
    const size_t sups_mn = sups_m * sups_n;

    std::vector<T> sups_matrix(sups_mn, 0);

    for (int i = 0; i < sups_m; ++i) {
        for (int j = 0; j < sups_n; ++j) {
            double r = i / static_cast<double>(sups_factor);
            double c = j / static_cast<double>(sups_factor);

            int matrix_r = static_cast<int>(r);
            int matrix_c = static_cast<int>(c);

            double u = r - matrix_r;
            double v = c - matrix_c;

            T interpolated_value = static_cast<T>(
                (1-u) * (1-v) * matrix[matrix_r * n + matrix_c] +
                u * (1-v) * matrix[(matrix_r + 1) * n + matrix_c] +
                (1-u) * v * matrix[matrix_r * n + (matrix_c + 1)] +
                u * v * matrix[(matrix_r + 1) * n + (matrix_c + 1)]
            );

            sups_matrix[i * sups_n + j] = interpolated_value;
        }
    }

    return sups_matrix;
}


int main() {
    const unsigned int sups_factor = 2;
    const size_t m = 3, n = 3, mn = m*n;
    
    std::vector<double> matrix(mn, 0);
    std::iota(matrix.begin(), matrix.end(), 1.0);
    print(matrix, m, n);


    std::vector<double> sups_matrix = sups(matrix, m, n, sups_factor);
    print(sups_matrix, m * sups_factor - sups_factor + 1, n * sups_factor - sups_factor + 1);
}
