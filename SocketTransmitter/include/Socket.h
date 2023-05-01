#pragma once

#include <arpa/inet.h>

class Socket {
private:
    sockaddr_in serverAddress{};
    sockaddr_in clientAddress{};
    int serverSocket;

public:
    Socket(
        const char* ccServerAddress, unsigned short usServerPort,
        const char* ccClientAddress, unsigned short usClientPort
    );
    ~Socket();

    int Send(const char* ccData);
};

extern "C" {
    Socket* SocketConstructor(
        const char* ccServerAddress, unsigned short usServerPort,
        const char* ccClientAddress, unsigned short usClientPort
    );
}