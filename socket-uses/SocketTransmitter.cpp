#include <atomic>
#include <chrono>
#include <csignal>
#include <ctime>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include "Socket.h"

#define CL "\r\033[K"        // clear terminal line
#define DATARATE 1000        // data rate (in miliseconds)


// global signal variables
volatile sig_atomic_t sSIGINT = 0;

// handle interrupt signals
void handleInterrupt(int signal) {
    switch (signal) {
        case SIGINT: sSIGINT = 1; break;
    }
}


// tick counter for data rate simulation
std::atomic<unsigned long> tick{0};
void tickThread(std::atomic<unsigned long>& tick) {
    while (!sSIGINT) {
        tick.store(
            std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count(),
            std::memory_order_relaxed
        );
    }
}


int main() {
    signal(SIGINT, handleInterrupt);

    unsigned long previousTick = 0, currentTick = 0;
    std::thread oTickThread(tickThread, std::ref(tick));

    Network thisNetwork("127.0.0.1", 8000);
    Network sendToNetwork("192.168.8.129", 1110);

    // same system can have sockets bind same network to multiple ports
    // same system can have sockets bind multiple networks to same port
    // same system can have sockets bind multiple networks to multiple ports
    // same system should not (but can) have multiple sockets bind to same network and same port (random port will be assigned)
    std::unique_ptr<Socket> socket = std::make_unique<Socket>(thisNetwork);

    int c = 0;

    while (!sSIGINT) {
        currentTick = tick.load(std::memory_order_relaxed);

        if (currentTick >= previousTick + DATARATE) {
            previousTick = currentTick;
            const char* data = std::to_string(c).c_str();

            // same socket can send data to same network with multiple ports
            // same socket can send data to multiple networks with same ports
            // same socket can send data to multiple networks with multiple ports
            // same socket should not (but can) send data to same networks with same ports (this causes duplicate data transmission)
            int bytes_sent = socket->Send(data, sizeof(data), sendToNetwork);

            std::cout << CL << "Data Sent : " << (bytes_sent > 0 ? ++c : c) << std::flush;
        }
    } std::cout << std::endl;


    oTickThread.join();
    return 0;
}
