#include "simplemath.h"
#include <iostream>
#include <vector>


struct data {
    int a;
    int b;
    int answer;
};

void subtract_test(std::vector<data> samples) {
    for (auto data : samples) {
        auto out = sm::subtract(data.a, data.b);
        if (out == data.answer) {
            std::cout << "PASSED:\t" << "sm::subtract(" << data.a << ", " << data.b << ") = " << out << "\t(== " << data.answer << ")" << std::endl;
        } else {
            std::cout << "FAILED:\t" << "sm::subtract(" << data.a << ", " << data.b << ") = " << out << "\t(!= " << data.answer << ")" << std::endl;
        }
    }
}


int main() {
    std::vector<data> samples = {
        {2, 2, 0},
        {6, 9, -3},
        {1, 6, -6}
    };

    subtract_test(samples);

    return 0;
}