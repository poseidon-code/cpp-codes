#include <algorithm>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <vector>


template <typename T>
void print(const std::vector<T>& a, const size_t& m, const size_t& n) {
    for (size_t i = 0; i < m; i++) {
        for (size_t j = i*m; j < i*m + n; j++) {
            std::cout << std::setw(4) << a[j];
        }
        std::cout << "\n";
    }
}


template <typename T>
std::vector<T> patch(const std::vector<T>& matrix, size_t m, size_t n, size_t index, size_t radius) {
    std::vector<T> patch;

    size_t r = index / n;
    size_t c = index % n;

    size_t r_start = (r >= radius) ? r - radius : 0;
    size_t r_end = std::min(r + radius + 1, m);
    size_t c_start = (c >= radius) ? c - radius : 0;
    size_t c_end = std::min(c + radius + 1, n);

    if (r_start + r_end < 2 * radius + 1) {
        r_start = 0;
        r_end = std::min(m, 2 * radius + 1);
    }

    if (c_start + c_end < 2 * radius + 1) {
        c_start = 0;
        c_end = std::min(n, 2 * radius + 1);
    }

    if (r + radius >= m) {
        r_start = std::max<size_t>(m - 2 * radius - 1, 0);
        r_end = m;
    }

    if (c + radius >= n) {
        c_start = std::max<size_t>(n - 2 * radius - 1, 0);
        c_end = n;
    }

    for (size_t i = r_start; i < r_end; ++i) {
        for (size_t j = c_start; j < c_end; ++j) {
            patch.push_back(matrix[i * n + j]);
        }
    }

    return patch;
}


int main() {
    const size_t m = 10, n = 10, mn = m*n;
    
    std::vector<int> matrix(mn, 0);
    std::iota(matrix.begin(), matrix.end(), 0);
    print(matrix, m, n);
    std::cout << "\n";

    size_t radius = 3;
    size_t center_index = 55;
    std::vector<int> patch_matrix = patch(matrix, m, n, center_index, radius);
    print(patch_matrix, radius * 2 + 1, radius * 2 + 1);


    return 0;
}
