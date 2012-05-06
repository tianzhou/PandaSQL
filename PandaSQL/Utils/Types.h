#ifndef PANDASQL_TYPES_H
#define PANDASQL_TYPES_H

namespace PandaSQL
{

#if _WIN32 || _WIN64
#if _WIN64
#define PANDASQL_64
#else
#define PANDASQL_32
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

//To denote internal use
#ifndef privileged
#define privileged public
#endif

//DB Types
enum DataType
{
	kInt = 0,
	kText = 1,
};

}	// namespace PandaSQL

#endif	// PANDASQL_TYPES_H