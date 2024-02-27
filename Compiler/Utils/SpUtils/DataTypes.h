//Inspired by LLVM-C DataTypes.h, change from C to C++
#ifndef Rtcc_DataTypes_h
#define Rtcc_DataTypes_h

#include <cinttypes>
#include <cstdint>
#include <stdint-gcc.h>

#ifdef _WIN64
typedef int64_t ssize_t;
#else
typedef int32_t ssize_t;
#endif

#ifndef INT64_MAX
#define INT64_MAX 9223372036854775807LL
#endif

#ifndef INT64_MIN
#define INT64_MIN ((-INT64_MAX) - 1)
#endif

#ifndef UINT64_MAX
#define UINT64_MAX 0xffffffffffffffffULL
#endif

#endif //Rtcc_DataTypes_h