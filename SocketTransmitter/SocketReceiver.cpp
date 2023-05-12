#include <csignal>
#include <iostream>
#include <atomic>
#include <string>
#include <memory>

#include "Socket.h"


// global signal variables
volatile sig_atomic_t sSIGINT = 0;

// handle interrupt signals
void handleInterrupt(int signal) {
    switch (signal) {
        case SIGINT: sSIGINT = 1; break;
    }
}


void callback(const char* ccData, int bytesRead) {
    std::cout << ccData << "\n";
}


int main() {
    signal(SIGINT, handleInterrupt);

    // std::unique_ptr<Socket> senderSocket(new Socket("127.0.0.1", 8000, "127.0.0.1", 1110, false)); // Socket::Receive() will NOT work
    // std::unique_ptr<Socket> loopbackSocket(new Socket("127.0.0.1", 8000, "127.0.0.1", 1110, true)); // both Socket::Send() & Socket::Receive() will work
    // std::unique_ptr<Socket> receiverSocket(new Socket(1110)); // Socket::Send() will NOT work

    std::unique_ptr<Socket> socket(new Socket(1110));

    socket->Receive(callback);

    return 0;
}
