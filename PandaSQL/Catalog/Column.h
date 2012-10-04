#ifndef PANDASQL_COLUMN_H
#define PANDASQL_COLUMN_H

#include "Utils/Status.h"
#include "Utils/Types.h"


#include <string>
#include <vector>

namespace PandaSQL
{

struct ColumnValue
{
	DataType valueType;

	int32_t number;

	std::string text;
};

typedef std::vector<ColumnValue> ColumnValueList;

struct ColumnQualifiedName
{
	std::string tableName;
	std::string columnName;

	ColumnQualifiedName();
	ColumnQualifiedName(const std::string inTableName, const std::string inColumnName);
	bool operator<(const ColumnQualifiedName &rhs) const;
};

struct ColumnDef
{
	ColumnQualifiedName qualifiedName;
	uint32_t index;
	DataType dataType;
	ConstraintType constraintType;
	ColumnDef();
};

typedef std::vector<ColumnDef> ColumnDefList;

}	// PandaSQL

#endif	// PANDASQL_TABLE_H