#ifndef BAMBOO_TYPES_H
#define BAMBOO_TYPES_H

#include <string>
#include <vector>

namespace BambooDB
{

#if _WIN32 || _WIN64
#if _WIN64
#define BAMBOODB_64
#else
#define BAMBOODB_32
#endif
#endif


typedef char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef unsigned int uint32_t;
typedef long long int64_t;
typedef unsigned long long uint64_t;

typedef std::vector<uint32_t> UInt32List;

//To denote internal use
#ifndef privileged
#define privileged public
#endif

}	// namespace BambooDB

#endif	// BAMBOO_TYPES_H