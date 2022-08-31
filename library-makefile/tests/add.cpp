#include "simplemath.h"
#include <iostream>
#include <vector>


struct data {
    int a;
    int b;
    int answer;
};

void add_test(std::vector<data> samples) {
    for (auto data : samples) {
        auto out = sm::add(data.a, data.b);
        if (out == data.answer) {
            std::cout << "PASSED:\t" << "sm::add(" << data.a << ", " << data.b << ") = " << out << "\t(== " << data.answer << ")" << std::endl;
        } else {
            std::cout << "FAILED:\t" << "sm::add(" << data.a << ", " << data.b << ") = " << out << "\t(!= " << data.answer << ")" << std::endl;
        }
    }
}


int main() {
    std::vector<data> samples = {
        {1, 2, 3},
        {6, 9, 15},
        {1, 6, 8}
    };

    add_test(samples);

    return 0;
}