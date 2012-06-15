#ifndef PANDASQL_COMMON_H
#define PANDASQL_COMMON_H

#include "Utils/Status.h"

#include <vector>

namespace PandaSQL
{

class DB;
typedef std::vector<std::string> TableRefList;
struct ColumnDef;

Status AmendColumnDef(const DB &inDB, const TableRefList &inTableRefList, ColumnDef *io_columnDef);

}	// PandaSQL


#endif	// PANDASQL_WINUTILS_H