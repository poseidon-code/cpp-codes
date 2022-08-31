#include "simplemath.h"
#include <iostream>
#include <vector>


struct data {
    int a;
    int b;
    int answer;
};

void divide_test(std::vector<data> samples) {
    for (auto data : samples) {
        auto out = sm::divide(data.a, data.b);
        if (out == data.answer) {
            std::cout << "PASSED:\t" << "sm::divide(" << data.a << ", " << data.b << ") = " << out << "\t(== " << data.answer << ")" << std::endl;
        } else {
            std::cout << "FAILED:\t" << "sm::divide(" << data.a << ", " << data.b << ") = " << out << "\t(!= " << data.answer << ")" << std::endl;
        }
    }
}


int main() {
    std::vector<data> samples = {
        {2, 2, 1},
        {9, 3, 3},
        {1, 6, 6}
    };

    divide_test(samples);

    return 0;
}