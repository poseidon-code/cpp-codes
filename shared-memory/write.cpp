#include <thread>
#include <chrono>

#include "Memory.hpp"

int main() {
    struct Data {
        int id;
        char name[20];
    };

    SharedMemory<Data> data_shm("/test-shm-data", true);
    Data* d = data_shm.Map();

    for (int i=0; i<100; i++) {
        d->id = i;
        std::strncpy(d->name, "poseidon", sizeof(d->name) - 1);
        d->name[sizeof(d->name) - 1] = '\0';

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    data_shm.Unmap(d);

    return 0;
}
