#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <stop_token>
#include <csignal>

volatile sig_atomic_t sigint = 0;

void signal_handler(int signal) {
    if (signal == SIGINT) sigint = 1;
}

std::vector<int> data(1, 0);

void f(std::stop_token stop) {
    while (!stop.stop_requested()) {
        ++data[0];
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void a(const std::vector<int> d) {
    for (int i=0; i<10; i++) {
        std::cout << "inside a() : " << d[0] << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main() {
    std::jthread t(f);

    while (!sigint) {
        std::cout << "inside main() : " << data[0] << "\n";
        a(data);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }


    t.request_stop();

    return 0;
}
