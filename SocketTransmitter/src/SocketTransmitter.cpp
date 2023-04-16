#include <arpa/inet.h>
#include <chrono>
#include <csignal>
#include <cstring>
#include <ctime>
#include <iostream>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

// Global variable to track interrupt signal
volatile sig_atomic_t SIGINTReceived = 0;

// Signal handler for Ctrl+C interrupt
void handleInterrupt(int signal) {
    if (signal == SIGINT) SIGINTReceived = 1;
}

int main() {
    signal(SIGINT, handleInterrupt);

    // Create Socket
    int serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        return -1;
    } else std::cout << "Socket Created" << std::endl;

    // Create Socket Address structure
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8000);
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Set data rate
    const int dataRate_ms = 1000;
    unsigned long long int streamedDataCount = 0;

    // Continuously send data to the client at the desired data rate
    while (!SIGINTReceived) {
        const char *data = "Hello, client!";

        // Send data to the client
        int bytesSent = sendto(
            serverSocket,
            data, std::strlen(data),
            MSG_CONFIRM | MSG_NOSIGNAL,
            (struct sockaddr*)&serverAddress, sizeof(serverAddress)
        );
        if (bytesSent <= 0) {
            std::cerr << "Error sending data: " << strerror(errno) << std::endl;
            return -2;
        }

        std::cout << "Data Streamed : " << ++streamedDataCount << "\n";

        // Wait for the remaining time until the next data packet
        std::this_thread::sleep_for(std::chrono::milliseconds(dataRate_ms));
    }

    // Close the sockets
    close(serverSocket);
    std::cout << "Socket Closed !" << std::endl;

    return 0;
}
