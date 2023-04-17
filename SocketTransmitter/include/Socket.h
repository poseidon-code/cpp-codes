#pragma once

#include <arpa/inet.h>

class Socket {
private:
    sockaddr_in address{};
    int serversocket;

    int handleSocketClose();

public:
    Socket(const char* ccAddress, unsigned short usPort);
    ~Socket();

    int Create();
    int Close();
    int Send(const char* ccData);
};