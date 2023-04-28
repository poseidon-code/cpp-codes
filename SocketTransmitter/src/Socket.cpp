#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include <cstring>

#include "Socket.h"



Socket::Socket(const char* ccServerIP, unsigned short usServerPort, const char* ccClientIP, unsigned short usClientPort) {
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(usServerPort);
    serverAddress.sin_addr.s_addr = inet_addr(ccServerIP);
    
    clientAddress.sin_family = AF_INET;
    clientAddress.sin_port = htons(usClientPort);
    clientAddress.sin_addr.s_addr = inet_addr(ccClientIP);

    serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket == -1) {
        std::cerr << "ERROR:\t Failed to create socket." << std::endl;
    } else {
        bind(serverSocket, (sockaddr*)&serverAddress, sizeof(serverAddress));
        std::cout << "SUCCESS: Socket created." << std::endl;
    }
};


Socket::~Socket() {
    if (serverSocket >= 0)
        if (close(serverSocket) >= 0)
            std::cout << "SUCCESS: Socket closed." << std::endl;
        else
            std::cerr << "ERROR:\t Failed to close socket." << std::endl;
    else
        std::cerr << "ERROR:\t No open sockets found, No sockets were closed." << std::endl;
};


int Socket::Send(const char* ccData) {
    int bytesSent = sendto(
        serverSocket,
        ccData, std::strlen(ccData),
        MSG_CONFIRM | MSG_NOSIGNAL,
        (struct sockaddr*)&clientAddress, sizeof(clientAddress)
    );

    if (bytesSent <= 0)
        std::cerr << "ERROR:\t Failed to send data, " << strerror(errno) << std::endl;

    return bytesSent;
}
