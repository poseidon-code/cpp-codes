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
std::vector<std::vector<T>> subs(const std::vector<std::vector<T>>& matrix, const size_t m, const size_t n, const unsigned int subs_factor) {
    const size_t subs_m = m / subs_factor;
    const size_t subs_n = n / subs_factor; 
    const size_t subs_mn = subs_m * subs_n;
    
    std::vector<std::vector<T>> subs_matrix;

    for (size_t i = 0; i < m; i += subs_factor) {
        std::vector<T> row;

        for (size_t j = 0; j < n; j += subs_factor) {
            T sum = 0;
            size_t count = 0;

            for (size_t ii = i; ii < std::min(i + subs_factor, m); ++ii) {
                for (size_t jj = j; jj < std::min(j + subs_factor, n); ++jj) {
                    sum += matrix[ii][jj];
                    ++count;
                }
            }

            row.push_back(sum / count);
        }

        subs_matrix.push_back(row);
    }

    return subs_matrix;
}


int main() {
    const unsigned int subs_factor = 2;
    const size_t m = 4, n = 5, mn = m*n;
    
    std::vector<std::vector<double>> matrix(m, std::vector<double>(n, 0));
    for (int i = 0; i < m; i++) for (int j = 0; j < n; j++) matrix[i][j] = i*n + j + 1;
    print(matrix);


    std::vector<std::vector<double>> subs_matrix = subs(matrix, m, n, subs_factor);
    print(subs_matrix);
}
