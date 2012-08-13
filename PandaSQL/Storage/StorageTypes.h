#ifndef PANDASQL_STORAGE_TYPES_H
#define PANDASQL_STORAGE_TYPES_H

#include "Utils/Types.h"


namespace PandaSQL
{

#ifdef PANDASQL_32
	typedef uint32_t PageNum; 
#else
	typedef uint64_t PageNum; 
#endif

}	// namespace PandaSQL

#endif	// PANDASQL_STORAGE_TYPES_H