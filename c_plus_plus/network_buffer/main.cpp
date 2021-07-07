#pragma pack(1)

#include <iostream>
#include "NetworkBuffer.h"
#include <cstring>
#include <netinet/in.h>

using namespace std;
using namespace sinicheveen;

struct MyStruct
{
    uint8_t a;
    uint16_t b;
    uint32_t c;
    char d[12] = { '\0' };
};

int main()
{
    NetworkBuffer buffer, buffer1;

    buffer.appendUint8(8);
    buffer.appendUint16(16);
    buffer.appendUint32(32);
    buffer.appendString("zhangdongyu");

    buffer1.copyNetworkBuffer(buffer);

    MyStruct st;
    memcpy(&st, buffer1.cdata(), 18);

    st.b = ntohs(st.b);
    st.c = ntohl(st.c);

    cout << (int)st.a << endl;
    cout << st.b << endl;
    cout << st.c << endl;
    cout << st.d << endl;

    return 0;
}