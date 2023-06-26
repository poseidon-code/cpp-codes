#include <atomic>
#include <csignal>
#include <iostream>
#include <memory>
#include <sys/epoll.h>
#include <thread>

#include "Socket.h"


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

// tick counter for data rate simulation
std::atomic<unsigned long> tick{0};
void tickThread(std::atomic<unsigned long>& tick) {
    while (!sSIGINT) {
        tick.store(
            std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count(),
            std::memory_order_relaxed
        );
    }
}


int senderThread(std::unique_ptr<Socket> socket, const Network& sendto_network) {
    unsigned long previousTick = 0, currentTick = 0;

    unsigned long long c = 0;

    while (!sSIGINT) {
        currentTick = tick.load(std::memory_order_relaxed);

        if (currentTick >= previousTick + 500) {
            previousTick = currentTick;

            const char* data = std::to_string(c).c_str();
            socket->Send(data, sendto_network);

            std::cout << "\nData Sent : " << ++c << std::flush;
        }
    } std::cout << std::endl;

    return 0;
}



void callback(char* ccData, int bytesRead) {
    std::cout << "\nData Received : " << ccData;
}

int receiverThread(std::unique_ptr<Socket> socket) {
    int epollFd = epoll_create1(0);
    if (epollFd == -1) return 1;

    struct epoll_event event;
    struct epoll_event events[2];

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
        int numEvents = epoll_wait(epollFd, events, 2, -1);
        if (numEvents == -1) close(epollFd); // TRUE when SIGINT is given, exits the loop after closing epollFd

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

    return 0;
}




int main() {
    signal(SIGINT, handleInterrupt);

    
    if (pipe(pipe_fds) == -1) {
        std::cerr << "pipe creation failed." << std::endl;
        return 1;
    }

    Network thisNetworkSender("127.0.0.1", 8000);
    Network thisNetworkReceiver("127.0.0.1", 1110);

    std::unique_ptr<Socket> socketSend = std::make_unique<Socket>(thisNetworkSender);
    std::unique_ptr<Socket> socketReceive = std::make_unique<Socket>(thisNetworkReceiver);

    std::thread oTickThread(tickThread, std::ref(tick));
    std::thread oSenderThread(senderThread, std::move(socketSend), thisNetworkReceiver);
    std::thread oReceiverThread(receiverThread, std::move(socketReceive));


    oReceiverThread.join();
    oSenderThread.join();
    oTickThread.join();


    close(pipe_fds[0]);
    close(pipe_fds[1]);

    return 0;
}
