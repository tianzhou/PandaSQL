#ifndef PANDASQL_COLUMN_H
#define PANDASQL_COLUMN_H

#include "Utils/Types.h"
#include "Utils/Status.h"

#include <string>

namespace PandaSQL
{

struct ColumnDef
{
	ColumnQualifiedName qualifiedName;
	uint32_t index;
	DataType dataType;
	ConstraintType constraintType;
	ColumnDef();
};

}	// PandaSQL

#endif	// PANDASQL_TABLE_H