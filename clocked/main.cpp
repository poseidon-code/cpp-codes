#include <atomic>
#include <chrono>
#include <csignal>
#include <iostream>
#include <mutex>
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




std::mutex global_mutex; // global mutex for synchronisation
const unsigned short int CLOCK_RATE = 40; // milliseconds
const unsigned short int SIMULATED_OPERATION_COMPLETION_TIME = 100; // milliseconds


// the maximum number of threads detached and active every second can be calculated by : 
// (probable maximum time the expensive operation takes / clock rate) * 10^x units
// i.e. (`SIMULATED_OPERATION_COMPLETION_TIME` milliseconds / `CLOCK_RATE` milliseconds) * 1000 milliseconds
// where the time measurement units must be respected 
// i.e. for this implementation - (100 ms / 40 ms) * 1000 ms (1s = 1000ms) = 2500 threads maximum (worst case) every second
// e.g. (4 ms / 40 ms) * 1000 ms = 100 threads maximum (worst case) every second
int expensive_operation(unsigned long long int count) {
    // synchorise data/execution between detached threads
    std::lock_guard<std::mutex> lg(global_mutex);

    // simulating time expensive operation
    int wait = std::rand() % SIMULATED_OPERATION_COMPLETION_TIME; // wait atleast: <random> milliseconds
    std::this_thread::sleep_for(std::chrono::milliseconds(wait));

    // "actual" time expensive operation
    std::cout << tick.load(std::memory_order_relaxed) << " : " << count << "\n";
    
    // the operation should return to destroy the thread
    return EXIT_SUCCESS;
}




int main() {
    // register system signal and its handler
    std::signal(SIGINT, handle_interrupt);

    // start system clock tick thread
    std::jthread ticker_thread(ticker);


    unsigned long long int count = 0;

    while (!sSIGINT.load(std::memory_order_relaxed)) {
        count++;

        // call the time expensive operation on a seperate thread and detach it
        // the thread will be destroyed after completion (both successfull & failed) of the operation
        std::jthread(expensive_operation, count).detach();

        // handles clocked execution of the operation irrespective of the time taken by the operation independently
        // i.e. whatever may the time the operation takes, it will be atleast 40 milliseconds apart from the previous execution of the same operation
        std::this_thread::sleep_for(std::chrono::milliseconds(CLOCK_RATE));
    }

    // an infinite loop on a different thread can be stopped using stop tokens
    ticker_thread.request_stop();

    return 0;
}
