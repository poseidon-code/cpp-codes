#include "simplemath.h"
#include <iostream>
#include <vector>


struct data {
    int a;
    int b;
    int answer;
};

void multiply_test(std::vector<data> samples) {
    for (auto data : samples) {
        auto out = sm::multiply(data.a, data.b);
        if (out == data.answer) {
            std::cout << "PASSED:\t" << "sm::multiply(" << data.a << ", " << data.b << ") = " << out << "\t(== " << data.answer << ")" << std::endl;
        } else {
            std::cout << "FAILED:\t" << "sm::multiply(" << data.a << ", " << data.b << ") = " << out << "\t(!= " << data.answer << ")" << std::endl;
        }
    }
}


int main() {
    std::vector<data> samples = {
        {2, 2, 4},
        {6, 9, 54},
        {1, 6, 9}
    };

    multiply_test(samples);

    return 0;
}