
#include "stdafx.h"

#include "Utils/Common.h"

#include <sstream>

namespace PandaSQL
{

void StringToNumber(const std::string &inStr, int32_t *o_num)
{
	*o_num = atoi(inStr.c_str());
}

void NumberToString(const int32_t inNum, std::string *o_str)
{
	std::stringstream convert;

	convert << inNum;

	*o_str = convert.str();
}

void StringToIntList(const std::string &inStr, std::vector<int32_t> *o_intList)
{
}

void IntListToString(const std::vector<int32_t> &inIntList, std::string *o_str)
{

}


}	// PandaSQL
