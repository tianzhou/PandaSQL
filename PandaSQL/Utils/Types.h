#ifndef PANDASQL_TYPES_H
#define PANDASQL_TYPES_H

#include <string>
#include <vector>

#include "PandaSQLDef.h"

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

typedef std::vector<uint32_t> UInt32List;

//To denote internal use
#ifndef privileged
#define privileged public
#endif

//DB Types
enum DataType
{
	kUnknownType = 0,
	kInt = 1,
	kText = 2,
};

enum ConstraintType
{
	kConstraintNone = 0,
	kConstraintPrimaryKey = 1,
	kConstraintUnique = 2,
	kConstraintNotNULL = 3,
};

enum StorageType
{
	kBamboo,
	kBDB
};

extern char *kNewLineSymbol; 

extern uint32_t kUnknownID;
extern uint32_t kBeginID;

extern uint32_t kInvalidColumnIndex;

}	// namespace PandaSQL

#endif	// PANDASQL_TYPES_H