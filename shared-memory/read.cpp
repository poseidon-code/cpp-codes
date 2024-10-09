#include <chrono>
#include <iostream>
#include <memory>
#include <thread>

#include "SharedMemory.hpp"

int main() {
    struct Data {
        int id;
        char name[20];
    };

    SharedMemory<Data> data_shm("/test-shm-data", false, true);
    std::unique_ptr<Data, decltype(data_shm.GetDeleter())> d(data_shm.Map(), data_shm.GetDeleter());


    for (int i=0; i<20; i++) {
        std::cout << d->id << " : " << d->name << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }


    return 0;
}
