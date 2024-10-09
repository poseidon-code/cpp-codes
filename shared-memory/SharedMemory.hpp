#pragma once

#include <string>
#include <system_error>

#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>




template <typename T>
class SharedMemory {
private:
    std::string name;
    uint64_t size;
    int memory;
    bool is_owner;
    bool read_only;

    void close_memory() {
        if (this->memory != -1) {
            close(this->memory);
            this->memory = -1;
        }
    }

public:
    SharedMemory(const std::string& name, bool create = false, bool read_only = true)
        : name(name),
        size(sizeof(T)),
        memory(-1),
        is_owner(create),
        read_only(read_only)
    {
        int oflag = 0;

        if (create) oflag |= O_CREAT | O_RDWR;
        if (!create && this->read_only) oflag |= O_RDONLY;
        if (!create && !this->read_only) oflag |= O_RDWR;

        this->memory = shm_open(this->name.c_str(), oflag, 0666);

        if (this->memory == -1) {
            throw std::runtime_error("failed to open shared memory");
        }

        if (create) {
            if (ftruncate(this->memory, this->size) == -1) {
                close_memory();
                throw std::runtime_error("failed to set the size of shared memory");
            }
        }
    }

    ~SharedMemory() {
        close_memory();
        if (this->is_owner) {
            shm_unlink(this->name.c_str());
        }
    }

    SharedMemory(const SharedMemory& o) = delete;

    SharedMemory& operator=(const SharedMemory& o) = delete;

    SharedMemory(SharedMemory&& o) noexcept
        : name(std::move(o.name)),
        size(o.size),
        memory(o.memory),
        is_owner(o.is_owner),
        read_only(o.read_only)
    {
        o.memory = -1;
    }

    SharedMemory& operator=(SharedMemory&& o) noexcept {
        if (this != &o) {
            close_memory();

            this->name = std::move(o.name);
            this->size = o.size;
            this->memory = o.memory;
            this->is_owner = o.is_owner;
            this->read_only = o.read_only;

            o.memory = -1;
        }
        return *this;
    }

    T* Map() {
        void* p = mmap(
            nullptr,
            this->size,
            this->read_only ? PROT_READ : PROT_READ | PROT_WRITE,
            MAP_SHARED,
            this->memory,
            0
        );

        if (p == MAP_FAILED) {
            throw std::runtime_error("failed to map shared memory");
        }

        return static_cast<T*>(p);
    }

    void Unmap(T* o) {
        if (munmap(o, this->size) == -1) {
            throw std::runtime_error("failed to unmap shared memory");
        }
    }

    auto GetDeleter() {
        return [this](T* p) -> void {
            if (p) {
                this->Unmap(p);
            }
        };
    }
};
