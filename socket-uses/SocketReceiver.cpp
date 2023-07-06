#include <csignal>
#include <iostream>
#include <atomic>
#include <string>
#include <memory>
#include <sys/epoll.h>

#include "Socket.h"


#define EPOLL_EVENTS 2


// global signal variables
volatile sig_atomic_t sSIGINT = 0;

// dummy pipe for exiting epoll_wait()
int pipe_fds[2];

// handle interrupt signals
void handleInterrupt(int signal) {
    switch (signal) {
        case SIGINT: sSIGINT = 1;
            char byte = 0;
            write(pipe_fds[1], &byte, sizeof(byte));
            break;
    }
}


void callback(unsigned char* ccData, int bytesRead) {
     std::cout << "\r\033[K" << "Data Received : " << ccData << std::flush;
}


int main() {
    signal(SIGINT, handleInterrupt);

    if (pipe(pipe_fds) == -1) {
        std::cerr << "pipe creation failed." << std::endl;
        return 3;
    }

    Network thisNetwork("192.168.8.129", 1110);

    std::unique_ptr<Socket> socket = std::make_unique<Socket>(thisNetwork);


    int epollFd = epoll_create1(0);
    if (epollFd == -1) return 1;

    struct epoll_event event;
    struct epoll_event events[EPOLL_EVENTS];

    // adding event listener for `socket->udpsocket`
    event.events = EPOLLIN;
    event.data.fd = socket->udpsocket;
    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, socket->udpsocket, &event) == -1) {
        close(epollFd);
        return 1;
    }

    // the `pipe_fds` is added for exiting from epoll_wait() 
    event.events = EPOLLIN;
    event.data.fd = pipe_fds[0];
    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, pipe_fds[0], &event) == -1) {
        close(epollFd);
        return 1;
    }


    while (!sSIGINT) {
        int numEvents = epoll_wait(epollFd, events, EPOLL_EVENTS, -1);
        if (numEvents == -1) {
            // TRUE when SIGINT is given, exits the loop after closing epollFd
            close(epollFd);
        }

        for (int i = 0; i < numEvents; ++i) {
            if (events[i].data.fd == socket->udpsocket) {
                socket->Receive(callback, 100);
            }

            // the `pipe_fds` is used to exit the epoll_wait(), as the thread keeps waiting even after SIGINT
            else if (events[i].data.fd == pipe_fds[0]) {
                char byte;
                read(pipe_fds[0], &byte, sizeof(byte));
            }
        }
    } std::cout << std::endl;


    close(pipe_fds[0]);
    close(pipe_fds[1]);

    return 0;
}
