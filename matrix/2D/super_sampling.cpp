#include <numeric>
#include <iostream>
#include <iomanip>
#include <vector>



template <typename T>
void print(const std::vector<std::vector<T>>& a) {
    for (const auto& row : a) {
        for (const auto& value : row) {
            std::cout << std::setw(10) << value;
        }
        std::cout << "\n";
    }
}


template <typename T>
std::vector<std::vector<T>> sups(const std::vector<std::vector<T>>& matrix, const size_t m, const size_t n, const unsigned int sups_factor) {
    const size_t sups_m = m * sups_factor - sups_factor + 1;
    const size_t sups_n = n * sups_factor - sups_factor + 1; 
    const size_t sups_mn = sups_m * sups_n;

    std::vector<std::vector<T>> sups_matrix;

    for (int i = 0; i < sups_m; ++i) {
        std::vector<T> row;

        for (int j = 0; j < sups_n; ++j) {
            double r = i / static_cast<double>(sups_factor);
            double c = j / static_cast<double>(sups_factor);

            int matrix_r = static_cast<int>(r);
            int matrix_c = static_cast<int>(c);

            double u = r - matrix_r;
            double v = c - matrix_c;


            T interpolated_value = static_cast<T>(
                (1 - u) * (1 - v) * matrix[matrix_r][matrix_c] +
                ((matrix_r + 1 < m) ? u * (1 - v) * matrix[matrix_r + 1][matrix_c] : 0) +
                ((matrix_c + 1 < n) ? (1 - u) * v * matrix[matrix_r][matrix_c + 1] : 0) +
                (((matrix_r + 1 < m) && (matrix_c + 1 < n)) ? u * v * matrix[matrix_r + 1][matrix_c + 1] : 0)
            );

            row.push_back(interpolated_value);
        }
        
        sups_matrix.push_back(row);
    }

    return sups_matrix;
}


int main() {
    const unsigned int sups_factor = 2;
    const size_t m = 3, n = 3, mn = m*n;
    
    std::vector<std::vector<double>> matrix(m, std::vector<double>(n, 0));
    for (int i = 0; i < m; i++) for (int j = 0; j < n; j++) matrix[i][j] = i*n + j + 1;
    print(matrix);


    std::vector<std::vector<double>> sups_matrix = sups(matrix, m, n, sups_factor);
    print(sups_matrix);
}
