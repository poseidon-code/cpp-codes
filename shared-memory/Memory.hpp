#pragma once

#include <cstring>
#include <fcntl.h>
#include <string>
#include <system_error>
#include <sys/mman.h>
#include <unistd.h>


template <typename T>
class SharedMemory {
private:
    std::string name;
    uint64_t size;
    int memory;

public:
    SharedMemory(std::string name, bool create) {
        this->name = name;
        this->size = sizeof(T);

        if (create) {
            this->memory = shm_open(this->name.c_str(), O_CREAT | O_RDWR, 0666);
            
            if (this->memory == -1) {
                throw std::runtime_error("failed to create shared memory");
            }

            if (ftruncate(this->memory, this->size) == -1) {
                throw std::runtime_error("failed to set the size of shared memory");
            }
        } else {
            this->memory = shm_open(this->name.c_str(), O_RDWR, 0666);

            if (this->memory == -1) {
                throw std::runtime_error("failed to open shared memory");
            }
        }
    }


    ~SharedMemory() {
        close(this->memory);
        shm_unlink(this->name.c_str());
    }

    T* Map() {
        void *p = mmap(nullptr, this->size, PROT_READ | PROT_WRITE, MAP_SHARED, this->memory, 0);
        
        if (p == MAP_FAILED) {
            throw std::runtime_error("failed to map shared memory");
        }

        return static_cast<T*>(p);
    }

    void Unmap(T *object) {
        if (munmap(object, this->size) == -1) {
            throw std::runtime_error("failed to unmap shared memory");
        }
    }
};
