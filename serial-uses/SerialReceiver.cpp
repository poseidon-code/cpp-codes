#include <iostream>
#include <chrono>
#include <csignal>
#include <thread>
#include <sys/epoll.h>

#include "Serial.h"


const auto DATARATE = std::chrono::milliseconds(40);

// signal variables
volatile sig_atomic_t sigint = 0;

// dummy pipe for exiting epoll_wait()
int pipe_fds[2];

// events count
const short int EPOLL_EVENTS = 2;

// handle interrupt signals
void signal_handler(int signal) {
    switch (signal) {
        case SIGINT: sigint = 1; break;
        default: sigint = 1; break;
    }
}



void callback(unsigned char* data, const int bytes_read) {
     std::cout << "\r\033[K" << "Data Received : " << data << std::flush;
}



int main() {
    signal(SIGINT, signal_handler);

    Serial serial("/dev/ttyS1", B115200, sizeof(unsigned long long int));


    if (pipe(pipe_fds) == -1) {
        std::cerr << "pipe creation failed." << std::endl;
        return EXIT_FAILURE;
    }

    int epollFd = epoll_create1(0);
    if (epollFd == -1) return EXIT_FAILURE;

    struct epoll_event event;
    struct epoll_event events[EPOLL_EVENTS];

    // adding event listener for `serial.serial_port`
    event.events = EPOLLIN;
    event.data.fd = serial.serial_port;
    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, serial.serial_port, &event) == -1) {
        close(epollFd);
        return EXIT_FAILURE;
    }

    // the `pipe_fds` is added for exiting from epoll_wait() 
    event.events = EPOLLIN;
    event.data.fd = pipe_fds[0];
    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, pipe_fds[0], &event) == -1) {
        close(epollFd);
        return EXIT_FAILURE;
    }


    while (!sigint) {
        int numEvents = epoll_wait(epollFd, events, EPOLL_EVENTS, -1);
        if (numEvents == -1) {
            // TRUE when SIGINT is given, exits the loop after closing epollFd
            close(epollFd);
        }

        for (int i = 0; i < numEvents; ++i) {
            if (events[i].data.fd == serial.serial_port) {
                serial.Read(callback);
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

    return EXIT_SUCCESS;
}
