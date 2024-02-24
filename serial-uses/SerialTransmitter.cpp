#include <iostream>
#include <chrono>
#include <csignal>
#include <thread>
#include <cstring>


#include "Serial.h"


const auto DATARATE = std::chrono::milliseconds(40);

// signal variables
volatile sig_atomic_t sigint = 0;

// handle interrupt signals
void signal_handler(int signal) {
    switch (signal) {
        case SIGINT: sigint = 1; break;
        default: sigint = 1; break;
    }
}



int main() {
    std::signal(SIGINT, signal_handler);

    Serial serial("/dev/ttyS1", B115200, sizeof(unsigned long long int));
    unsigned long long int c = 0;

    while (!sigint) {
        auto next_clock = std::chrono::steady_clock::now() + DATARATE;

        unsigned char data[sizeof(unsigned long long int)] = {0};
        std::memcpy(data, &c, sizeof(unsigned long long int));

        int bytes_sent = serial.Write(data);
        std::cout << "\r\033[K" << "Data Sent : " << (bytes_sent > 0 ? ++c : c) << std::flush;

        std::this_thread::sleep_until(next_clock);
    }

    return 0;
}
