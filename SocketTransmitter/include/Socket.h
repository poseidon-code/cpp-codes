#pragma once

#include <arpa/inet.h>

class Socket {
private:
    sockaddr_in serverAddress{};
    sockaddr_in clientAddress{};
    int serverSocket;

    int handleSocketClose();

public:
    Socket(const char* ccServerAddress, unsigned short usServerPort, const char* ccClientAddress, unsigned short usClientPort);
    ~Socket();

    int Create();
    int Close();
    int Send(const char* ccData);
};
