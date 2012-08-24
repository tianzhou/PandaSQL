#ifndef PANDASQL_DEBUG_H
#define PANDASQL_DEBUG_H

#include "Utils/Types.h"

namespace PandaSQL
{

#ifdef PDDEBUG
	
void DebugString(const char *str);
void DebugStringVerbose(const char *fileName, int32_t lineNum, const char *str);

#define PDDebugOutput(x) DebugString(x)
#define PDDebugOutputVerbose(x) DebugStringVerbose(__FILE__, __LINE__, x)

#define PDASSERT(x) assert(x)

#else

#define PDDebugOutput(x)		((void) 0)
#define PDDebugOutputVerbose(x)	((void) 0)
#define PDASSERT(x)				((void) 0)

#endif

}	// namespace PandaSQL

#endif	// PANDASQL_DEBUG_H