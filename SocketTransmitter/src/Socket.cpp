#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include <cstring>

#include "Socket.h"

Socket::Socket(const char* ccAddress, unsigned short usPort) {
    address.sin_family = AF_INET;
    address.sin_port = htons(usPort);
    address.sin_addr.s_addr = inet_addr(ccAddress);
};

Socket::~Socket() {
    handleSocketClose();
};

int Socket::handleSocketClose() {
    if (serversocket >= 0) {
        if (close(serversocket) >= 0) {
            std::cout << "SUCCESS: Socket closed." << std::endl;
            return 0;
        } else {
            std::cerr << "ERROR:\t Failed to close socket." << std::endl;
            return -1;
        }
    } else {
        std::cerr << "ERROR:\t No open sockets found, No sockets were closed." << std::endl;
        return -1;
    }
}

int Socket::Create() {
    serversocket = socket(AF_INET, SOCK_DGRAM, 0);
    
    if (serversocket == -1) {
        std::cerr << "ERROR:\t Failed to create socket." << std::endl;
        return -1;
    } else {
        std::cout << "SUCCESS: Socket created." << std::endl;
        return 0;
    }
}

int Socket::Close() {
    return handleSocketClose();
}

int Socket::Send(const char* ccData) {
    int bytesSent = sendto(
        serversocket,
        ccData, std::strlen(ccData),
        MSG_CONFIRM | MSG_NOSIGNAL,
        (struct sockaddr*)&address, sizeof(address)
    );

    if (bytesSent <= 0)
        std::cerr << "ERROR:\t Failed to send data, " << strerror(errno) << std::endl;

    return bytesSent;
}
