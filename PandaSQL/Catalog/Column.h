#ifndef PANDASQL_COLUMN_H
#define PANDASQL_COLUMN_H

#include "Utils/Types.h"
#include "Utils/Status.h"

#include <string>

namespace PandaSQL
{

struct ColumnQualifiedName
{
	std::string tableName;
	std::string columnName;
};

struct ColumnDef
{
	ColumnQualifiedName qualifiedName; 
	DataType dataType;
	ConstraintType constraintType;
};

}	// PandaSQL

#endif	// PANDASQL_TABLE_H