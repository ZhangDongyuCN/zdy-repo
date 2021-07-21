#ifndef _BIN_OCT_HEX_DEC_
#define _BIN_OCT_HEX_DEC_

#include <vector>
#include <string>
#include <cstdint>
#include <algorithm>
#include <sstream>
#include <cmath>

class BinOctHexDec {
public:
    template<class T>
    static std::vector<T> Dec2Bin(uint64_t dec, size_t alignWidth = -1);

    template<class T>
    static std::vector<T> Dec2Oct(uint64_t dec, size_t alignWidth = -1);

    static std::string Dec2Hex(uint64_t dec, size_t alignWidth = -1);

    template<class T>
    static uint64_t Bin2Dec(const std::vector<T>& bin);

    template<class T>
    static uint64_t Oct2Dec(const std::vector<T>& oct);

    static uint64_t Hex2Dec(const std::string& hex);
};

template<class T>
std::vector<T> BinOctHexDec::Dec2Bin(uint64_t dec, size_t alignWidth) {
    //转换成二进制
    std::vector<T> binary;
    while (dec != 0) {
        binary.emplace_back(dec % 2);
        dec /= 2;
    }
    std::reverse(binary.begin(), binary.end());

    //对齐到指定宽度
    if (alignWidth == -1 || alignWidth <= binary.size()) {
        return binary;
    } else {
        std::vector<T> newBinary(alignWidth, false);
        std::move(binary.begin(), binary.end(), newBinary.begin() + (alignWidth - binary.size()));
        return newBinary;
    }
}

template<class T>
std::vector<T> BinOctHexDec::Dec2Oct(uint64_t dec, size_t alignWidth) {
    //转换成八进制
    std::vector<T> oct;
    while (dec != 0) {
        oct.emplace_back(dec % 8);
        dec /= 8;
    }
    std::reverse(oct.begin(), oct.end());

    //对齐到指定宽度
    if (alignWidth == -1 || alignWidth <= oct.size()) {
        return oct;
    } else {
        std::vector<T> newOct(alignWidth, 0);
        std::move(oct.begin(), oct.end(), newOct.begin() + (alignWidth - newOct.size()));
        return newOct;
    }
}

std::string BinOctHexDec::Dec2Hex(uint64_t dec, size_t alignWidth) {
    std::stringstream ss;
    std::string hex;
    ss << std::uppercase << std::hex << dec;
    ss >> hex;
    return hex;
}

template<class T>
uint64_t BinOctHexDec::Bin2Dec(const std::vector<T>& bin) {
    uint64_t dec = 0;
    size_t base = bin.size();
    for (const auto& e:bin) {
        dec += e * pow(2, base - 1);
        base--;
    }
    return dec;
}

template<class T>
uint64_t BinOctHexDec::Oct2Dec(const std::vector<T>& oct) {
    uint64_t dec = 0;
    size_t base = oct.size();
    for (const auto& e:oct) {
        dec += e * pow(8, base - 1);
        base--;
    }
    return dec;
}

uint64_t BinOctHexDec::Hex2Dec(const std::string& hex) {
    std::stringstream ss;
    uint64_t dec;
    ss << std::hex << hex;
    ss >> dec;
    return dec;
}

#endif