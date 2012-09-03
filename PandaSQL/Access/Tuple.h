#ifndef PANDASQL_TUPLE_H
#define PANDASQL_TUPLE_H

#include "Catalog/Column.h"

#include "Utils/Status.h"
#include "Utils/Types.h"
#include "Utils/Value.h"

#include <vector>

namespace PandaSQL
{


//class TupleDesc
//{
//
//public:
//
//	TupleDesc(const ColumnQualifiedName &inQualifiedName);
//	~TupleDesc();
//
//	ColumnQualifiedName GetColumnQualifiedName() const { return mQualifiedName; }
//
//private:
//
//	const ColumnQualifiedName &mQualifiedName;
//
//};

class TupleDescElement
{
public:
	DataType mDataType;
};

typedef std::vector<TupleDescElement> TupleDesc;

class TupleDataElement
{
public:
	int32_t mNumber;

	std::string mText;
};

typedef std::vector<TupleDataElement> TupleData;

void TupleElementToString(const TupleDescElement &descElement, const TupleDataElement &dataElement, std::string *o_string);
void TupleToString(const TupleDesc &desc, const TupleData &data, std::string *o_string);
void TupleStringToValue(const ColumnDef &inColumnDef, const std::string &inString, uint32_t *io_offset, Value *o_value);
void TupleStringToValueList(const ColumnDefList &inColumnDefList, const std::string &inString, ValueList *o_valueList);

void ProjectTuple(const ColumnDefList &inAllColDefList, const ColumnDefList &inProjectColDefList, const ValueList &inTupleValue, ValueList *o_projectTupleValue);

void ColumnDefListToTupleDesc(const ColumnDefList &colDefList, TupleDesc *io_tupleDesc);

#ifdef PDDEBUG
void PrintTuple(const ValueList &data);
#endif
//class TupleData 
//{
//public:
//
//	TupleData();
//	~TupleData();
//
//	uint32_t Count() const;
//	void GetDataAtIndex(uint32_t index, std::string *o_data) const;
//
//	void AppendData(const std::string &inData);
//	void SetDataAtIndex(uint32_t index, const std::string &inData);
//
//	//static void ValueListToString(const ColumnDefList &inColumnDefList, const ColumnValueList &inColumnValueList, std::string *io_string);
//	//static void StringToValueList(const ColumnDefList &inColumnDefList, std::string &inString, ColumnValueList *io_columnValueList);
//
//private:
//	//TupleData(const TupleData &rhs);
//	//TupleData& operator=(const TupleData &rhs);
//
//	std::vector<std::string> mValueList;
//};
//
//struct TupleEntry
//{
//	std::string tableName;
//	TupleData tupleData;
//};

}	// PandaSQL

#endif	// PANDASQL_TUPLE_H