#include <atomic>
#include <chrono>
#include <csignal>
#include <iostream>
#include <stop_token>
#include <thread>


// signal variables
volatile sig_atomic_t sigint = 0;

// handle interrupt signals
void handle_interrupt(int signal) {
    switch (signal) {
        case SIGINT: sigint = 1; break;
        default: sigint = 1; break;
    }
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




const unsigned short int CLOCK_RATE = 40; // milliseconds
const unsigned short int SIMULATED_OPERATION_COMPLETION_TIME = 100; // milliseconds


void expensive_operation(unsigned long long int count) {
    // simulating time expensive operation
    int wait = std::rand() % SIMULATED_OPERATION_COMPLETION_TIME; // wait atleast: <random> milliseconds
    std::this_thread::sleep_for(std::chrono::milliseconds(wait));

    // "actual" time expensive operation
    std::cout << tick.load(std::memory_order_relaxed) << " : " << count << "\n";
}




int main() {
    // register system signal and its handler
    std::signal(SIGINT, handle_interrupt);

    // start system clock tick thread
    std::jthread ticker_thread(ticker);


    unsigned long long int count = 0;

    while (!sigint) {
        auto next_clock = std::chrono::steady_clock::now() + std::chrono::milliseconds(CLOCK_RATE);
        count++;

        // call the time expensive operation
        expensive_operation(count);

        // handles clocked execution of the operation respecting the time taken by the operation (i.e. ensures synchronisation)
        // next execution will be always <CLOCK_RATE> time-units apart from the previous execution
        // if the current execution takes more than <CLOCK_RATE> time-units, then it will call the next execution without waiting, 
        // after finishing the current execution (synchronisation)
        std::this_thread::sleep_until(next_clock);
    }

    // an infinite loop on a different thread can be stopped using stop tokens
    ticker_thread.request_stop();

    return 0;
}
