#include <chrono>
#include <csignal>
#include <ctime>
#include <iostream>
#include <thread>

#include "Socket.h"



volatile sig_atomic_t rSIGINT = 0;

void handleInterrupt(int signal) {
    switch (signal) {
        case SIGINT: rSIGINT = 1; break;
    }
}


int main() {
    signal(SIGINT, handleInterrupt);

    Socket* socket = new Socket("127.0.0.1", 8000);
    socket->Create();

    const int dataRate_ms = 500;
    unsigned long long int streamedDataCount = 0;

    while (!rSIGINT) {
        const char *data = "Hello, client!";
        socket->Send(data);
        
        std::cout << "Data Streamed : " << ++streamedDataCount << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(dataRate_ms));
    }

    socket->Close();
    return 0;
}
