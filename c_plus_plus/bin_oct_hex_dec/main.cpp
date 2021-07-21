#include "BinOctHexDec.hpp"
#include <iostream>

using namespace std;

int main() {
    vector<uint16_t> res1 = BinOctHexDec::Dec2Bin<uint16_t>(123);
    cout << "res1: ";
    for (const auto& e:res1) {
        cout << e;
    }
    cout << endl;

    vector<uint16_t> res2 = BinOctHexDec::Dec2Oct<uint16_t>(123);
    cout << "res2: ";
    for (const auto& e:res2) {
        cout << e;
    }
    cout << endl;

    string res3 = BinOctHexDec::Dec2Hex(123);
    cout << "res3: " << res3 << endl;

    uint64_t res4 = BinOctHexDec::Oct2Dec(res2);
    cout << "res4: " << res4 << endl;

    uint64_t res5 = BinOctHexDec::Hex2Dec(res3);
    cout << "res5: " << res5 << endl;

    uint64_t res6 = BinOctHexDec::Bin2Dec(res1);
    cout << "res6: " << res6 << endl;

    return 0;
}