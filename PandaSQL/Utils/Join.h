#ifndef PANDASQL_JOIN_H
#define PANDASQL_JOIN_H

#include "Catalog/Column.h"

#include "Utils/Types.h"

#include <vector>

namespace PandaSQL
{

//struct JoinSpec
//{
//	ColumnQualifiedName leftColumn;
//	ColumnQualifiedName rightColumn;
//};

typedef std::vector<ColumnQualifiedName> JoinList;

}	// namespace PandaSQL

#endif	// PANDASQL_JOIN_H