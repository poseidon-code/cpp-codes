#include <iostream>
#include <future>
#include <thread>
#include <chrono>
#include <vector>


int a(int v) {
    std::this_thread::sleep_for(std::chrono::milliseconds((int)(std::rand() % 1000)));
    std::cout << "called - " << v << "\n";
    return v * 10;
}

double mean(const std::vector<double>& data) {
    double sum = 0;
    for (auto v : data) {
        sum += v;
    }

    return sum / data.size();
}


int main() {
    std::vector<double> data;
    for (double i=0 ; i<1000000; i += (std::rand() % 10)) {
        data.push_back(i);
    };

    std::future<double> m = std::async(mean, data);

    std::future<int> v1 = std::async(std::launch::deferred, a, 10);
    std::future<int> v2 = std::async(a, 11);
    std::future<int> v3 = std::async(a, 12);
    std::future<int> v4 = std::async(a, 13);
    std::future<int> v5 = std::async(a, 14);
    std::future<int> v6 = std::async(a, 15);
    std::cout << v2.get() << std::endl;
    std::cout << v3.get() << std::endl;
    std::cout << v1.get() << std::endl;
    std::cout << v4.get() << std::endl;
    std::cout << v5.get() << std::endl;
    std::cout << v6.get() << std::endl;

    std::cout << m.get() << std::endl;


    return 0;
}
