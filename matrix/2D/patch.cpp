#include <algorithm>
#include <iomanip>
#include <iostream>
#include <vector>


template <typename T>
void print(const std::vector<std::vector<T>>& a) {
    for (const auto& row : a) {
        for (const auto& value : row) {
            std::cout << std::setw(4) << value;
        }
        std::cout << "\n";
    }
}


template <typename T>
std::vector<std::vector<T>> patch(const std::vector<std::vector<T>>& matrix, const size_t m, const size_t n, const size_t index, const size_t radius) {
    std::vector<std::vector<T>> patch;

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
        std::vector<T> row_patch;
        for (size_t j = c_start; j < c_end; ++j) {
            row_patch.push_back(matrix[i][j]);
        }
        patch.push_back(row_patch);
    }

    return patch;
}


int main() {
    const size_t m = 10, n = 10, mn = m*n;
    
    std::vector<std::vector<int>> matrix(m, std::vector<int>(n, 0));
    for (int i = 0; i < m; i++) for (int j = 0; j < n; j++) matrix[i][j] = i*n + j + 1;
    print(matrix);

    size_t radius = 3;
    size_t center_index = 55;
    std::vector<std::vector<int>> patch_matrix = patch(matrix, m, n, center_index, radius);
    print(patch_matrix);


    return 0;
}
