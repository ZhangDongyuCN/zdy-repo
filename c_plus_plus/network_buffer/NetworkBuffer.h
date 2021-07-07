#pragma once

#include <cstdint>
#include <string>

using std::string;


namespace sinicheveen
{
    class NetworkBuffer
    {
    public:

        NetworkBuffer(size_t capacity = 1024);

        NetworkBuffer(char* copiedCharBuffer, size_t length);

        ~NetworkBuffer();

        NetworkBuffer(const NetworkBuffer&) = delete;

        NetworkBuffer(NetworkBuffer&&) = delete;

        NetworkBuffer& operator= (const NetworkBuffer&) = delete;

        NetworkBuffer& operator= (NetworkBuffer&&) = delete;

        char* data();

        const char* cdata() const;

        char* begin();

        const char* cbegin() const;

        char* end();

        const char* cend() const;

        size_t size();

        size_t capacity();

        void deleteNetworkBuffer();

        void clearNetworkBuffer();

        void copyNetworkBuffer(const NetworkBuffer& copiedNetworkBuffer);

        void appendUint8(uint8_t value);

        void appendUint16(uint16_t value);

        void appendUint32(uint32_t value);

        void appendString(string str);

    protected:

        void grow(size_t incrementCapacity);

    private:

        char* buffer_;
        size_t size_;
        size_t capacity_;
        size_t min_capacity_ = 1024;
    };    //class NetworkBuffer
}    //namespace sinicheveen
