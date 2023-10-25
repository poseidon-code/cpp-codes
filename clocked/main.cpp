#include <atomic>
#include <chrono>
#include <csignal>
#include <iostream>
#include <stop_token>
#include <thread>



// signal variables
std::atomic<bool> sSIGINT = false;

// handle interrupt signals
void handle_interrupt(int signal) {
    if (signal == SIGINT)
        sSIGINT.store(true, std::memory_order_relaxed);
}

// independent thread for setting clock ticks
std::atomic<unsigned long long int> tick = 0;
void ticker(std::stop_token stop) {
    while (!stop.stop_requested()) {
        tick.store(
            std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count(),
            std::memory_order_relaxed
        );
        std::this_thread::sleep_for(std::chrono::milliseconds(10)); // handle busy-waiting
    }
}




int expensive_operation() {
    // simulating time expensive operation
    int wait = std::rand() % 1000 + 3000; // wait atleast: <random> milliseconds + 3 second
    std::this_thread::sleep_for(std::chrono::milliseconds(wait));

    // "actual" time expensive operation
    std::cout << tick.load(std::memory_order_relaxed) << "\n";
    
    // the operation should return to destroy the thread
    return EXIT_SUCCESS;
}




int main() {
    // register system signal and its handler
    std::signal(SIGINT, handle_interrupt);

    // start system clock tick thread
    std::jthread ticker_thread(ticker);


    while (!sSIGINT.load(std::memory_order_relaxed)) {
        // call the time expensive operation on a seperate thread and detach it
        // the thread will be destroyed after completion (both successfull & failed) of the operation
        std::jthread(expensive_operation).detach();

        // handles busy-waiting
        // handles clocked execution of the operation irrespective of the time taken by the operation independently
        // i.e. whatever may the time the operation takes, it will be atleast 40 milliseconds apart from the previous execution of the same operation
        std::this_thread::sleep_for(std::chrono::milliseconds(40));
    }

    // an infinite loop on a different thread can be stopped using stop tokens
    ticker_thread.request_stop();

    return 0;
}
