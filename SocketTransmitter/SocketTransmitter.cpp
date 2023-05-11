#include <chrono>
#include <csignal>
#include <ctime>
#include <iostream>
#include <thread>
#include <atomic>
#include <string>
#include <memory>

#include "Socket.h"

#define CL "\r\033[K"       // clear terminal line
#define DATARATE 500        // data rate (in miliseconds)


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

    // std::unique_ptr<Socket> senderSocket(new Socket("127.0.0.1", 8000, "127.0.0.1", 1110, false)); // Socket::Receive() will NOT work
    // std::unique_ptr<Socket> loopbackSocket(new Socket("127.0.0.1", 8000, "127.0.0.1", 1110, true)); // both Socket::Send() & Socket::Receive() will work
    // std::unique_ptr<Socket> receiverSocket(new Socket(1110)); // Socket::Send() will NOT work

    // since data receiving (Socket::Receive()) is not required, hence the `loopback` is set to FALSE, although it is a loopback usecase
    std::unique_ptr<Socket> socket = std::make_unique<Socket>("127.0.0.1", 8000, "127.0.0.1", 1110, false);

    int c = 0;

    while (!sSIGINT) {
        currentTick = tick.load(std::memory_order_relaxed);

        if (currentTick >= previousTick + DATARATE) {
            previousTick = currentTick;

            const char* data = std::to_string(c).c_str();
            socket->Send(data);

            std::cout << CL << "Data Sent : " << ++c << std::flush;
        }
    } std::cout << std::endl;


    oTickThread.join();
    return 0;
}
