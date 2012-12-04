#ifndef PANDASQL_COMMON_H
#define PANDASQL_COMMON_H

#include "Utils/Types.h"

#include <vector>

namespace PandaSQL
{

void StringToNumber(const std::string &inStr, int32_t *o_num);
void NumberToString(const int32_t inNum, std::string *o_str);

void StringToIntList(const std::string &inStr, std::vector<int32_t> *o_intList);
void IntListToString(const std::vector<int32_t> &inIntList, std::string *o_str);

}	// PandaSQL


#endif	// PANDASQL_WINUTILS_H