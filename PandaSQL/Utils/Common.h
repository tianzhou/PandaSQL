#ifndef PANDASQL_COMMON_H
#define PANDASQL_COMMON_H

#include "Utils/Status.h"
#include "Utils/Types.h"

#include <vector>

namespace PandaSQL
{

class DB;
typedef std::vector<std::string> TableRefList;
struct ColumnDef;

Status AmendColumnDef(const DB &inDB, const TableRefList &inTableRefList, ColumnDef *io_columnDef);

void StringToNumber(const std::string &inStr, uint32_t *o_num);
void NumberToString(const uint32_t inNum, std::string *o_str);


}	// PandaSQL


#endif	// PANDASQL_WINUTILS_H