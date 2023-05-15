#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/epoll.h>
#include <system_error>

#include "Socket.h"

extern "C" Socket* SenderSocketConstructor(
    const char* ccServerIP, unsigned short usServerPort, 
    const char* ccClientIP, unsigned short usClientPort, 
    bool loopback
) {
    return new Socket(ccServerIP, usServerPort, ccClientIP, usClientPort, loopback);
}

extern "C" Socket* ReceiverSocketConstructor(unsigned short usReceivingPort) {
    return new Socket(usReceivingPort);
}


Socket::Socket(const char* ccServerIP, unsigned short usServerPort, const char* ccClientIP, unsigned short usClientPort, bool loopback) {
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(usServerPort);
    serverAddress.sin_addr.s_addr = inet_addr(ccServerIP);
    
    clientAddress.sin_family = AF_INET;
    clientAddress.sin_port = htons(usClientPort);
    clientAddress.sin_addr.s_addr = inet_addr(ccClientIP);

    serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket == -1)
        throw std::runtime_error("failed to create sender socket");
    else
        bind(serverSocket, (sockaddr*)&serverAddress, sizeof(serverAddress));

    if (loopback) {
        clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
        if (clientSocket == -1)
            throw std::runtime_error("failed to create receiver socket");
        else
            bind(clientSocket, (sockaddr*)&clientAddress, sizeof(clientAddress));
    }
};


Socket::Socket(unsigned short usReceivingPort) {
    clientAddress.sin_family = AF_INET;
    clientAddress.sin_port = htons(usReceivingPort);

    clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket == -1)
        throw std::runtime_error("failed to create receiver only socket");
    else
        bind(clientSocket, (sockaddr*)&clientAddress, sizeof(clientAddress));
};


Socket::~Socket() {
    if (serverSocket > 0)
        close(serverSocket);

    if (clientSocket > 0)
        close(clientSocket);
};


int Socket::Send(const char* ccData) {
    // no sending socket but receiving socket present (i.e. ReceiverSocket)
    if (serverSocket <= 0 && clientSocket > 0) return -2;

    int bytesSent = sendto(
        serverSocket,
        ccData, std::strlen(ccData),
        MSG_CONFIRM | MSG_NOSIGNAL,
        (struct sockaddr*)&clientAddress, sizeof(clientAddress)
    );

    return bytesSent;
}


int Socket::Receive(std::function<void(const char*, int)> callback) {
    // no receiveing socket (i.e. SenderSocket-no_loopback)
    if (clientSocket <= 0) return -2;

    int epollFd = epoll_create1(0);
    if (epollFd == -1) return -1;

    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = clientSocket;

    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, clientSocket, &event) == -1) {
        close(epollFd);
        return -2;
    }

    struct epoll_event events[10];

    while (true) {
        int numEvents = epoll_wait(epollFd, events, 10, -1);
        if (numEvents == -1) {
            close(epollFd);
            return -3;
        }

        for (int i = 0; i < numEvents; ++i) {
            if (events[i].data.fd == clientSocket) {
                char buffer[1024];
                socklen_t clientAddrLen = sizeof(clientAddress);
                
                int bytesRead = recvfrom(
                    clientSocket,
                    buffer, sizeof(buffer), 
                    0,
                    (struct sockaddr*)&clientAddress, &clientAddrLen);

                if (bytesRead > 0) {
                    buffer[bytesRead] = '\0';
                    callback(buffer, bytesRead);
                }
            }
        }
    }

    close(epollFd);
    return 0;
}
