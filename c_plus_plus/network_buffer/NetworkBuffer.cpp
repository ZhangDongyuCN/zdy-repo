#include "NetworkBuffer.h"
#include <algorithm>
#include <netinet/in.h>

using std::fill;
using std::copy;

namespace sinicheveen
{
    NetworkBuffer::NetworkBuffer(size_t capacity /*= 1024*/)
    {
        if (capacity <= min_capacity_)
        {
            buffer_ = new char[min_capacity_];
            capacity_ = min_capacity_;
            size_ = 0;
        }
        else
        {
            buffer_ = new char[capacity];
            capacity_ = capacity;
            size_ = 0;
        }
    }    //NetworkBuffer()

    NetworkBuffer::NetworkBuffer(char* copiedCharBuffer, size_t length)
    {
        if (length <= min_capacity_)
        {
            buffer_ = new char[min_capacity_];
            copy(copiedCharBuffer, copiedCharBuffer + length, buffer_);
            capacity_ = min_capacity_;
            size_ = length;
        }
        else
        {
            buffer_ = new char[length];
            copy(copiedCharBuffer, copiedCharBuffer + length, buffer_);
            capacity_ = length;
            size_ = length;
        }
    }    //NetworkBuffer()

    NetworkBuffer::~NetworkBuffer()
    {
        if (nullptr != buffer_)
        {
            delete[] buffer_;
            buffer_ = nullptr;
        }
    }

    char* NetworkBuffer::data()
    {
        return buffer_;
    }    //data()

    const char* NetworkBuffer::cdata() const
    {
        return buffer_;
    }    //cdata()

    char* NetworkBuffer::begin()
    {
        return buffer_;
    }    //begin()

    const char* NetworkBuffer::cbegin() const
    {
        return buffer_;
    }    //cbegin()

    char* NetworkBuffer::end()
    {
        return buffer_ + size_;
    }    //end()

    const char* NetworkBuffer::cend() const
    {
        return buffer_ + size_;
    }    //cend()

    size_t NetworkBuffer::size()
    {
        return size_;
    }    //size()

    size_t NetworkBuffer::capacity()
    {
        return capacity_;
    }    //capacity()

    void NetworkBuffer::deleteNetworkBuffer()
    {
        if (nullptr != buffer_)
        {
            delete[] buffer_;
            buffer_ = nullptr;
        }
    }    //deleteNetworkBuffer()

    void NetworkBuffer::clearNetworkBuffer()
    {
        fill(begin(), end(), 0);
        size_ = 0;
    }    //clearNetworkBuffer()

    void NetworkBuffer::copyNetworkBuffer(const NetworkBuffer& copiedNetworkBuffer)
    {
        if (capacity_ >= copiedNetworkBuffer.size_)
        {
            copy(copiedNetworkBuffer.cbegin(), copiedNetworkBuffer.cend(), buffer_);
            size_ = copiedNetworkBuffer.size_;
        }
        else
        {
            delete[] buffer_;
            buffer_ = new char[copiedNetworkBuffer.size_];
            copy(copiedNetworkBuffer.cbegin(), copiedNetworkBuffer.cend(), buffer_);
            size_ = copiedNetworkBuffer.size_;
            capacity_ = size_;
        }
    }    //copyNetworkBuffer()

    void NetworkBuffer::appendUint8(uint8_t value)
    {
        if (capacity_ < size_ + sizeof(uint8_t))
        {
            grow(sizeof(uint8_t));
        }

        uint8_t* offset = reinterpret_cast<uint8_t*>(buffer_ + size_);
        *offset = value;
        size_ += sizeof(uint8_t);
    }    //appendUint8()

    void NetworkBuffer::appendUint16(uint16_t value)
    {
        if (capacity_ < size_ + sizeof(uint16_t))
        {
            grow(sizeof(uint16_t));
        }

        uint16_t* offset = reinterpret_cast<uint16_t*>(buffer_ + size_);
        *offset = htons(value);
        size_ += sizeof(uint16_t);
    }    //appendUint16()

    void NetworkBuffer::appendUint32(uint32_t value)
    {
        if (capacity_ < size_ + sizeof(uint32_t))
        {
            grow(sizeof(uint32_t));
        }

        uint32_t* offset = reinterpret_cast<uint32_t*>(buffer_ + size_);
        *offset = htonl(value);
        size_ += sizeof(uint32_t);
    }    //appendUint32()

    void NetworkBuffer::appendString(string str)
    {
        if (capacity_ < size_ + str.length())
        {
            grow(str.length());
        }

        copy(str.begin(), str.end(), buffer_ + size_);
        size_ += str.length();
    }    //appendString()

    void NetworkBuffer::grow(size_t incrementCapacity)
    {
        size_t newCapacity = capacity_ + incrementCapacity > capacity_ * 2 ?
            capacity_ + incrementCapacity : capacity_ * 2;
        char* newBuffer = new char[newCapacity];
        copy(buffer_, buffer_ + size_, newBuffer);
        delete[] buffer_;
        buffer_ = newBuffer;
        newBuffer = nullptr;
        capacity_ = newCapacity;
    }    //grow()
}    //namespace sinicheveen

