#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/epoll.h>

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
    if (serverSocket == -1) {
        std::cerr << "ERROR: Failed to create sender socket." << std::endl;
    } else {
        bind(serverSocket, (sockaddr*)&serverAddress, sizeof(serverAddress));
        std::cout << "SUCCESS: Sender socket created." << std::endl;
    }

    if (loopback) {
        clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
        if (clientSocket == -1) {
            std::cerr << "ERROR: Failed to create receiver socket." << std::endl;
        } else {
            bind(clientSocket, (sockaddr*)&clientAddress, sizeof(clientAddress));
            std::cout << "SUCCESS: Receiver socket created." << std::endl;
        }
    }
};


Socket::Socket(unsigned short usReceivingPort) {
    clientAddress.sin_family = AF_INET;
    clientAddress.sin_port = htons(usReceivingPort);

    clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket == -1) {
        std::cerr << "ERROR: Failed to create receiver only socket." << std::endl;
    } else {
        bind(clientSocket, (sockaddr*)&clientAddress, sizeof(clientAddress));
        std::cout << "SUCCESS: Receiver only socket created." << std::endl;
    }
};


Socket::~Socket() {
    if (serverSocket > 0)
        if (close(serverSocket) >= 0)
            std::cout << "SUCCESS: Sender socket closed." << std::endl;
        else
            std::cerr << "ERROR: Failed to close sender socket." << std::endl;

    if (clientSocket > 0)
        if (close(clientSocket) >= 0)
            std::cout << "SUCCESS: Receiver socket closed." << std::endl;
        else
            std::cerr << "ERROR: Failed to close receiver socket." << std::endl;
};


int Socket::Send(const char* ccData) {
    // no sending socket but receiving socket present (i.e. ReceiverSocket)
    if (serverSocket <= 0 && clientSocket > 0) {
        std::cout << "ERROR: Cannot send data from a revceiver only socket." << std::endl;
        return -1;
    }

    int bytesSent = sendto(
        serverSocket,
        ccData, std::strlen(ccData),
        MSG_CONFIRM | MSG_NOSIGNAL,
        (struct sockaddr*)&clientAddress, sizeof(clientAddress)
    );

    if (bytesSent <= 0)
        std::cerr << "ERROR: Failed to send data, " << strerror(errno) << std::endl;

    return bytesSent;
}


int Socket::Receive(std::function<void(const char*, int)> callback) {
    // no receiveing socket (i.e. SenderSocket-no_loopback)
    if (clientSocket <= 0) {
        std::cout << "ERROR: Cannot receive data from a sender only socket." << std::endl;
        return -2;
    }

    int epollFd = epoll_create1(0);
    if (epollFd == -1) {
        std::cerr << "ERROR: Failed to create epoll, " << strerror(errno) << std::endl;
        return -1;
    }

    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = clientSocket;

    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, clientSocket, &event) == -1) {
        std::cerr << "ERROR: Failed to add socket to epoll, " << strerror(errno) << std::endl;
        close(epollFd);
        return -2;
    }

    struct epoll_event events[10];

    while (true) {
        int numEvents = epoll_wait(epollFd, events, 10, -1);
        if (numEvents == -1) {
            std::cerr << "ERROR: epoll_wait error, " << strerror(errno) << std::endl;
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
                } else {
                    std::cerr << "ERROR: Failed to receive data, " << strerror(errno) << std::endl;
                }
            }
        }
    }

    close(epollFd);
    return 0;
}
