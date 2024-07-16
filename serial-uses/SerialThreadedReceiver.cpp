#include <iostream>
#include <stop_token>
#include <thread>

#include "Serial.h"


void callback(unsigned char* buffer, const ssize_t bytes_read) {
    std::cout << bytes_read << std::endl;
};


int main() {
    Serial s("/dev/ttys0", B115200, 64);

    const auto f = [&s](std::stop_token stop){s.Read(stop, callback, 0xAA, 0xBB);};
    std::jthread t(f);

    std::this_thread::sleep_for(std::chrono::seconds(10));
    t.request_stop();

    return EXIT_SUCCESS;
}
