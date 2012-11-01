#ifndef PANDASQL_COLUMN_H
#define PANDASQL_COLUMN_H

#include <map>
#include <set>
#include <string>
#include <vector>

#include "Access/Tuple.h"

#include "Utils/Status.h"
#include "Utils/Types.h"

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
	bool operator==(const ColumnQualifiedName &rhs) const;
	bool operator<(const ColumnQualifiedName &rhs) const;
};

struct ColumnDef
{
	ColumnQualifiedName qualifiedName;
	uint32_t index;
	DataType dataType;
	ConstraintType constraintType;
	ColumnDef();

	bool operator==(const ColumnDef &rhs) const;
};

typedef std::vector<ColumnDef> ColumnDefList;
typedef std::vector<uint32_t> ColumnIndexList;

typedef std::set<std::string> ColumnNameSet;
//<TableName, ColumnSet>
typedef std::map<std::string, ColumnNameSet> TableAndColumnSetMap;

void AddOneColumnToMap(const ColumnQualifiedName &inColumnQualifiedName, TableAndColumnSetMap *io_tableAndColumnSetMap);

void TupleStringToValue(const TupleDescElement &descElement, const std::string &inString, uint32_t *io_offset, Value *o_value);
void TupleStringToValueList(const TupleDesc &desc, const std::string &inString, ValueList *o_valueList);

void ProjectTuple(const ColumnDefList &inAllColDefList, const ColumnDefList &inProjectColDefList, const ValueList &inTupleValue, ValueList *o_projectTupleValue);

void ColumnDefListToTupleDesc(const ColumnDefList &colDefList, TupleDesc *io_tupleDesc);

//class ColumnDataFunctor : public TupleFunctor
//{
//
//public:
//
//	ColumnDataFunctor(const ColumnDefList &inColumnDefList, ValueList *io_valueList);
//	virtual ~ColumnDataFunctor();
//
//	virtual void operator()(const std::string &inTupleData);
//
//private:
//
//	const ColumnDefList &mColumnDefList;
//	ValueList *mpValueList;
//
//
//};


}	// PandaSQL

#endif	// PANDASQL_TABLE_H	