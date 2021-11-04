#pragma once

#include <memory>

/** 
 * Represents a fixed size chunk of memory to which data can be written.
 * Access to the memory is provided by the MemoryProviderType template argument,
 * allowing to implement various storage backends, e.g. raw memory, file access, etc.
 * @tparam MemoryProviderType A storage provider backend. Must implement the following 
 *                            public member functions:
 *                                  // returns the maximum number of bytes that can be written
 *                                  size_t getAvailableSize() const;
 *                                  // writes some data to the memory and advances the write
 *                                  // position so that multiple consecutive calls append
 *                                  // the data continuously.
 *                                  void write(const void* srcData, size_t sizeToWrite);
 */
template <typename MemoryProviderType>
class WritableMemory
{
public:
    /**
     * @param rawMem    Instance of the storage provider backend,
     *                  which is captured by reference (so it must stay alive
     *                  until this WritableMemory is deconstructed).
     */
    WritableMemory(MemoryProviderType& rawMem) :
        rawMem_(rawMem),
        size_(rawMem.getAvailableSize())
    {
    }

    ~WritableMemory() {}

    /** 
     * Returns the number of bytes that can still be written 
     */
    size_t getRemainingSize() const { return size_; }

    /** 
     * Attempts to write to the memory.
     * If enough bytes are available, the data is written and true is returned.
     * Otherwise, false is returned and the data is not written.
     */
    bool writeRaw(const void* src, size_t sizeToWrite)
    {
        if (size_ < sizeToWrite)
            return false;

        rawMem_.write(src, sizeToWrite);
        size_ -= sizeToWrite;
        return true;
    }

    /** 
     * Attempts to write a value to the memory.
     * If enough bytes are available, the data is written and true is returned.
     * Otherwise, false is returned and the data is not written.
     */
    template <typename T>
    bool writeItem(const T& value)
    {
        return writeRaw((const void*) &value, sizeof(T));
    }

    /** 
     * Attempts to write multiple values to the memory.
     * If enough bytes are available to store all values, they're written and true is returned.
     * Otherwise, false is returned and none of the values is written.
     */
    template <typename... T>
    bool writeItems(T const&... values)
    {
        const auto totalSize = (size_t(0) + ... + sizeof(T));
        if (size_ < totalSize)
            return false;

        return (writeItem(values) && ...);
    }

private:
    WritableMemory(const WritableMemory&) = delete;
    WritableMemory& operator=(const WritableMemory&) = delete;
    MemoryProviderType& rawMem_;
    size_t size_;
};

/** 
 * Represents a fixed size chunk of memory from which data can be read.
 * Access to the memory is provided by the MemoryProviderType template argument,
 * allowing to implement various storage backends, e.g. raw memory, file access, etc.
 * @tparam MemoryProviderType A storage provider backend. Must implement the following 
 *                            public member functions:
 *                                  // returns the maximum number of bytes that can be read
 *                                  size_t getAvailableSize() const;
 *                                  // reads some data from the memory and advances the read
 *                                  // position
 *                                  void read(void* dest, size_t sizeToRead);
 */
template <typename MemoryProviderType>
class ReadableMemory
{
public:
    /**
     * @param rawMem    Instance of the storage provider backend,
     *                  which is captured by reference (so it must stay alive
     *                  until this ReadableMemory is deconstructed).
     */
    ReadableMemory(MemoryProviderType& rawMem) :
        rawMem_(rawMem),
        size_(rawMem.getAvailableSize())
    {
    }
    ~ReadableMemory() {}

    /** 
     * Returns the number of bytes that can still be read 
     */
    size_t getRemainingSize() const { return size_; }

    /**
     * Attempts to read from the memory.
     * If enough bytes are available, the data is read and true is returned.
     * Otherwise, false is returned and `dest` remains unchanged.
     */
    bool readRaw(void* dest, size_t sizeToRead)
    {
        if (size_ < sizeToRead)
            return false;

        rawMem_.read(dest, sizeToRead);
        size_ -= sizeToRead;
        return true;
    }

    /** 
     * Attempts to read a value from the memory.
     * If enough bytes are available, the data is read and true is returned.
     * Otherwise, false is returned and the value is unchanged.
     */
    template <typename T>
    bool readItem(T& value)
    {
        return readRaw((void*) &value, sizeof(T));
    }

    /** 
     * Attempts to read multiple values from the memory.
     * If enough bytes are available to read all values, they're read and true is returned.
     * Otherwise, false is returned and all values are unchanged.
     */
    template <typename... T>
    bool readItems(T&... values)
    {
        const auto totalSize = (size_t(0) + ... + sizeof(T));
        if (size_ < totalSize)
            return false;

        return (readItem(values) && ...);
    }

private:
    ReadableMemory(const ReadableMemory&) = delete;
    ReadableMemory& operator=(const ReadableMemory&) = delete;
    MemoryProviderType& rawMem_;
    size_t size_;
};

/** 
 * A memory provider that allocates a fixed size buffer
 * on the Stack. For use with ReadableMemory and WritableMemory
 */
template <size_t size>
struct StackMemoryProvider
{
    std::array<uint8_t, size> arr;
    uint8_t* ptr = arr.data();

    constexpr size_t getAvailableSize() const { return size; }

    void write(const void* srcData, size_t sizeToWrite)
    {
        memcpy(ptr, srcData, sizeToWrite);
        ptr += sizeToWrite;
    }

    void read(void* destData, size_t sizeToRead)
    {
        memcpy(destData, ptr, sizeToRead);
        ptr += sizeToRead;
    }
};