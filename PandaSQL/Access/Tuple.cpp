#include "stdafx.h"

#include "Access/Tuple.h"

#include "Utils/Common.h"
#include "Utils/Debug.h"

namespace PandaSQL
{

/**********************TupleDesc*******************/

//TupleDesc::TupleDesc(const ColumnQualifiedName &inQualifiedName)
//:
//mQualifiedName(inQualifiedName)
//{
//}
//
//TupleDesc::~TupleDesc()
//{
//}

/**********************TupleData*******************/

//TupleData::TupleData()
//{
//}
//
//TupleData::~TupleData()
//{
//}
//
//uint32_t TupleData::Count() const
//{
//	return (uint32_t)mValueList.size();
//}
//
//void TupleData::GetDataAtIndex(uint32_t index, std::string *o_data) const
//{
//	PDASSERT(index < this->Count());
//
//	*o_data = mValueList[index];
//}
//
//void TupleData::AppendData(const std::string &inData)
//{
//	mValueList.push_back(inData);
//}
//
//void TupleData::SetDataAtIndex(uint32_t index, const std::string &inData)
//{
//	PDASSERT(index < this->Count());
//
//	mValueList[index] = inData;
//}
//
////static
//void TupleData::ValueListToString(const ColumnDefList &inColumnDefList, const ColumnValueList &inColumnValueList, std::string *io_string)
//{
//	PDASSERT(inColumnDefList.size() == inColumnValueList.size());
//
//	io_string->clear();
//
//	ColumnDefList::const_iterator defIter = inColumnDefList.begin();
//	ColumnValueList::const_iterator valueIter = inColumnValueList.begin();
//
//	//TODO: No coercing at this point
//	while (defIter != inColumnDefList.end() && valueIter != inColumnValueList.end())
//	{
//		if (valueIter->valueType == kInt)
//		{
//			if (defIter->dataType == kInt)
//			{
//				io_string->append((char *)&valueIter->number, sizeof(valueIter->number));
//			}
//			else
//			{
//				PDASSERT(0);
//			}
//		}
//		else if (valueIter->valueType == kText)
//		{
//			if (defIter->dataType == kText)
//			{
//				io_string->append(valueIter->text);
//			}
//			else
//			{
//				PDASSERT(0);
//			}
//		}
//		else
//		{
//			PDASSERT(0);
//		}
//
//		defIter++;
//		valueIter++;
//	}
//}
//
////static
//void TupleData::StringToValueList(const ColumnDefList &inColumnDefList, std::string &inString, ColumnValueList *io_columnValueList)
//{
//}

#ifdef PDDEBUG
void PrintTuple(const ValueList &data)
{
	std::string tupleString;
	std::string oneString;
	for (size_t i = 0; i < data.size(); i++)
	{
		if (i > 0)
		{
			tupleString.append(", ");
		}

		oneString = data[i].GetAsString();

		tupleString.append(oneString);
	}

	PDDebugOutput(tupleString.c_str());
}
#endif

};	// PandaSQL