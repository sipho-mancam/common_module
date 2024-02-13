#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <iostream>
#include <cstring>

class SharedMemoryManager {
public:
    SharedMemoryManager(const std::string& name, std::size_t size)
        : name_(name), size_(size) {
        createSharedMemory();
        mapSharedMemory();
    }

    ~SharedMemoryManager() {
        boost::interprocess::shared_memory_object::remove(name_.c_str());
    }

    void write(void* data, int len) {
        memcpy(region_.get_address(), data, len);
    }

    const char* read() const {
        const char* data = static_cast<const char*>(region_.get_address());
        return data;
    }

private:
    void createSharedMemory() {
        shm_ = boost::interprocess::shared_memory_object(
            boost::interprocess::open_or_create, name_.c_str(), boost::interprocess::read_write);

        shm_.truncate(size_);
    }

    void mapSharedMemory() {
        region_ = boost::interprocess::mapped_region(shm_, boost::interprocess::read_write);
    }

private:
    std::string name_;
    std::size_t size_;
    boost::interprocess::shared_memory_object shm_;
    boost::interprocess::mapped_region region_;
};


class ReadOnlySharedMemoryReader {
public:
    ReadOnlySharedMemoryReader(const std::string& name)
        : name_(name) {
        openSharedMemory();
        mapSharedMemory();
    }

    ~ReadOnlySharedMemoryReader() = default;

    void* read() const {
        void* data = region_.get_address();
        return data;
    }

private:
    void openSharedMemory() {
        shm_ = boost::interprocess::shared_memory_object(
            boost::interprocess::open_only, name_.c_str(), boost::interprocess::read_only);
    }

    void mapSharedMemory() {
        region_ = boost::interprocess::mapped_region(shm_, boost::interprocess::read_only);
    }

private:
    std::string name_;
    boost::interprocess::shared_memory_object shm_;
    boost::interprocess::mapped_region region_;
};