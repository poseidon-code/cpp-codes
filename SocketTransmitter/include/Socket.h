#pragma once

#include <arpa/inet.h>
#include <functional>

class Socket {
private:
    sockaddr_in serverAddress{};
    sockaddr_in clientAddress{};
    int serverSocket;
    int clientSocket;

public:
    Socket(
        const char* ccServerIP, unsigned short usServerPort,
        const char* ccClientIP, unsigned short usClientPort,
        bool loopback
    );
    Socket(unsigned short usReceivingPort);
    ~Socket();

    int Send(const char* ccData);
    int Receive(std::function<void(const char*, int)> callback);
};

extern "C" {
    Socket* SenderSocketConstructor(
        const char* ccServerIP, unsigned short usServerPort,
        const char* ccClientIP, unsigned short usClientPort,
        bool loopback
    );
    Socket* ReceiverSocketConstructor(unsigned short usReceivingPort);
}