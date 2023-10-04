#include <numeric>
#include <iostream>
#include <iomanip>
#include <vector>



template <typename T>
void print(const std::vector<T>& a, const size_t& m, const size_t& n) {
    for (size_t i = 0; i < m; i++) {
        for (size_t j = i*n; j < i*n + n; j++) {
            std::cout.precision(8);
            std::cout << std::setw(10) << a[j];
        }
        std::cout << "\n";
    }
}


template <typename T>
std::vector<T> subs(const std::vector<T>& matrix, const size_t& m, const size_t& n, const unsigned int subs_factor) {
    const size_t subs_m = m / subs_factor;
    const size_t subs_n = n / subs_factor; 
    const size_t subs_mn = subs_m * subs_n;
    
    std::vector<T> subs_matrix;

    for (size_t i = 0; i < m; i += subs_factor) {
        for (size_t j = 0; j < n; j += subs_factor) {
            T sum = 0;
            size_t count = 0;

            for (size_t ii = i; ii < std::min(i + subs_factor, m); ++ii) {
                for (size_t jj = j; jj < std::min(j + subs_factor, n); ++jj) {
                    sum += matrix[ii * n + jj];
                    ++count;
                }
            }

            subs_matrix.push_back(sum / count);
        }
    }

    return subs_matrix;
}


int main() {
    const unsigned int subs_factor = 2;
    const size_t m = 4, n = 5, mn = m*n;
    
    std::vector<double> matrix(mn, 0);
    std::iota(matrix.begin(), matrix.end(), 1.0);
    print(matrix, m, n);


    std::vector<double> subs_matrix = subs(matrix, m, n, subs_factor);
    print(subs_matrix, m / subs_factor + (m % subs_factor), n / subs_factor + (n % subs_factor));
}
