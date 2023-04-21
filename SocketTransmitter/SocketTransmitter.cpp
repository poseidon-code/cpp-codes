#include <chrono>
#include <csignal>
#include <ctime>
#include <iostream>
#include <thread>
#include <atomic>

#include "Socket.h"

#define CL "\r\033[K"       // clear terminal line
#define DATARATE 200        // data rate (in miliseconds)


// global signal variables
volatile sig_atomic_t sSIGINT = 0;

void handleInterrupt(int signal) {
    switch (signal) {
        case SIGINT: sSIGINT = 1; break;
    }
}


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

    Socket* socket = new Socket("127.0.0.1", 8000);
    socket->Create();

    int c = 0;

    while (!sSIGINT) {
        currentTick = tick.load(std::memory_order_relaxed);

        if (currentTick >= previousTick + DATARATE) {
            previousTick = currentTick;

            std::cout << CL << "Data Sent : " << ++c << std::flush;

            const char *data = "Hello, client!";
            socket->Send(data);
        }
    } std::cout << std::endl;


    socket->Close();
    oTickThread.join();

    return 0;
}
