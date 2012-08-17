#ifndef PANDASQL_DEBUG_H
#define PANDASQL_DEBUG_H

#include "Utils/Types.h"

namespace PandaSQL
{

#ifdef PDDEBUG

void DebugString(const char *fileName, int32_t lineNum, const char *str);


#define PDDebugOutput(x) DebugString(__FILE__, __LINE__, x)

#define PDASSERT(x) assert(x)

#else

#define PDDebugOutput(x)	((void) 0)
#define PDASSERT(x)			((void) 0)

#endif

}	// namespace PandaSQL

#endif	// PANDASQL_DEBUG_H