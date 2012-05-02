#ifndef PANDASQL_TYPES_H
#define PANDASQL_TYPES_H

namespace PandaSQL
{

typedef char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef unsigned int uint32_t;
typedef long long int64_t;
typedef unsigned long long uint64_t;

//To denote internal use
#ifndef privileged
#define privileged public
#endif

}	// namespace PandaSQL

#endif	// PANDASQL_TYPES_H