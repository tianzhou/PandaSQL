#ifndef PANDASQL_COMMON_H
#define PANDASQL_COMMON_H

#include "Utils/Types.h"

namespace PandaSQL
{

void StringToNumber(const std::string &inStr, uint32_t *o_num);
void NumberToString(const uint32_t inNum, std::string *o_str);

}	// PandaSQL


#endif	// PANDASQL_WINUTILS_H