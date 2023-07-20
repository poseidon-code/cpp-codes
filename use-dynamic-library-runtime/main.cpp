#include <atomic>
#include <chrono>
#include <csignal>
#include <ctime>
#include <dlfcn.h>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include "Socket.h"



#define CL "\r\033[K"       // clear terminal line
#define DATARATE 500        // data rate (in miliseconds)


// global signal variables
volatile sig_atomic_t sSIGINT = 0;


// handle interrupt signals
void handleInterrupt(int signal) {
    switch (signal) {
        case SIGINT: sSIGINT = 1; break;
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




int main() {
    signal(SIGINT, handleInterrupt);

    unsigned long previousTick = 0, currentTick = 0;
    std::thread oTickThread(tickThread, std::ref(tick));


    // dynamically lazy load 'Socket' library at runtime (path relative of executable)
    void* libSocket = dlopen("lib/libSocket.so", RTLD_LAZY);
    if (!libSocket) {
        std::cerr << "failed to load 'libSocket' at runtime. (" << dlerror() << ")" << std::endl;
        dlclose(libSocket);
        return 1;
    }

    // declare type of the NetworkConstructor of 'Socket' library
    typedef Network (*NetworkConstructor)(const char* ccIP, const unsigned short int cusiPort);
    // point to the NetworkConstructor in the dynamic library
    NetworkConstructor fpNetworkConstructor = reinterpret_cast<NetworkConstructor>(dlsym(libSocket, "NetworkConstructor"));

    // declare type of the SocketConstructor of 'Socket' library
    typedef Socket* (*SocketConstructor)(const Network& network);
    // point to the SocketConstructor in the dynamic library
    SocketConstructor fpSocketConstructor = reinterpret_cast<SocketConstructor>(dlsym(libSocket, "SocketConstructor"));


    // call that NetworkConstructor to create a new Network object
    Network thisNetwork = fpNetworkConstructor("127.0.0.1", 8000);
    Network sendtoNetwork = fpNetworkConstructor("127.0.0.1", 1110);

    // call that SocketConstructor to create a new Socket object
    Socket* pSocket = fpSocketConstructor(thisNetwork);
    // (optional) wrapping with `unique_ptr` for modern resource management
    std::unique_ptr<Socket, void (*)(Socket*)> socket(pSocket, [](Socket* ptr) { delete ptr; });


    int c = 0;

    while (!sSIGINT) {
        currentTick = tick.load(std::memory_order_relaxed);

        if (currentTick >= previousTick + DATARATE) {
            previousTick = currentTick;

            const char* data = std::to_string(c).c_str();
            socket->Send(data, sizeof(data), sendtoNetwork);

            std::cout << CL << "Data Sent : " << ++c << std::flush;
        }
    } std::cout << std::endl;


    oTickThread.join();
    dlclose(libSocket); // close library handle
    
    return 0;
}
