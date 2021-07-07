#ifndef PARAMETER_FLOW
#define PARAMETER_FLOW
#define IN
#define OUT
#define INOUT
#endif    //PARAMETER_FLOW

#ifndef BASE_TYPE_DEF
#define BASE_TYPE_DEF
#include <stdint.h>
typedef int16_t          SHORT;
typedef uint16_t         USHORT;
typedef int32_t          INT;
typedef uint32_t         UINT;
typedef int64_t          DLONG;
typedef uint64_t         DULONG;
typedef void             VOID;
typedef bool             BOOL;
typedef char             CHAR;
typedef unsigned char    UCHAR;
typedef float            FLOAT;
typedef double           DOUBLE;
#endif    //BASE_TYPE_DEF