#include <thread>
#include <chrono>
#include <iostream>

#include "Memory.hpp"

int main() {
    struct Data {
        int id;
        char name[20];
    };

    SharedMemory<Data> data_shm("/test-shm-data", false);
    Data* d = data_shm.Map();

    for (int i=0; i<20; i++) {
        std::cout << d->id << " : " << d->name << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    data_shm.Unmap(d);

    return 0;
}
