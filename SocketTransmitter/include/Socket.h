#pragma once

#include <arpa/inet.h>

class Socket {
private:
    sockaddr_in serverAddress{};
    sockaddr_in clientAddress{};
    int serverSocket;

public:
    Socket(
        const char* ccServerIP, unsigned short usServerPort,
        const char* ccClientIP, unsigned short usClientPort
    );
    ~Socket();

    int Send(const char* ccData);
};

extern "C" {
    Socket* SocketConstructor(
        const char* ccServerIP, unsigned short usServerPort,
        const char* ccClientIP, unsigned short usClientPort
    );
}