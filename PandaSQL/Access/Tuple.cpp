#include "stdafx.h"

#include "Tuple.h"

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

void TupleElementToString(const TupleDescElement &descElement, const TupleDataElement &dataElement, std::string *o_string)
{
	if (descElement.mDataType == kInt)
	{
		o_string->append((const char *)&dataElement.mNumber, sizeof(dataElement.mNumber));
	}
	else if (descElement.mDataType == kText)
	{
		size_t length = dataElement.mText.length();
		o_string->append((const char *)&length, sizeof(size_t));
		o_string->append(dataElement.mText.c_str(), length);
	}
	else
	{
		PDASSERT(0);
	}
}

void TupleToString(const TupleDesc &desc, const TupleData &data, std::string *o_string)
{
	PDASSERT(desc.size() == data.size());

	o_string->clear();

	for (size_t i = 0; i < desc.size(); i++)
	{
		TupleElementToString(desc[i], data[i], o_string);
	}
}

void StringToTupleElement(const TupleDescElement &descElement, const std::string &inString, uint32_t *io_offset, TupleDataElement *o_dataElement)
{
	if (descElement.mDataType == kInt)
	{
		o_dataElement->mNumber = *(const int *)(inString.c_str() + *io_offset);
		*io_offset += sizeof(int);
	}
	else if (descElement.mDataType == kText)
	{
		size_t length = *(const size_t *)(inString.c_str() + *io_offset);
		*io_offset += sizeof(length);

		o_dataElement->mText.append(inString.c_str() + *io_offset, length);
		*io_offset += length;
	}
	else
	{
		PDASSERT(0);
	}
}

void StringToTuple(const TupleDesc &desc, const std::string &inString, TupleData *o_data)
{
	size_t offset = 0;

	for (size_t i = 0; i < desc.size(); i++)
	{
		TupleDataElement oneDataElement;

		StringToTupleElement(desc[i], inString, &offset, &oneDataElement);

		o_data->push_back(oneDataElement);
	}

	//TODO: Check
	PDASSERT(offset <= inString.length());
}

void ColumnDefListToTupleDesc(const ColumnDefList &colDefList, TupleDesc *io_tupleDesc)
{
	ColumnDefList::const_iterator iter = colDefList.begin();

	for (; iter != colDefList.end(); iter++)
	{
		TupleDescElement oneDescElement;

		oneDescElement.mDataType = iter->dataType;
		
		io_tupleDesc->push_back(oneDescElement);
	}
}

#ifdef PDDEBUG
void PrintTuple(const TupleDesc &desc, const TupleData &data)
{
	PDASSERT(desc.size() == data.size());

	std::string tupleString;
	std::string oneString;
	for (size_t i = 0; i < desc.size(); i++)
	{
		if (i > 0)
		{
			tupleString.append(", ");
		}
		if (desc[i].mDataType == kInt)
		{
			NumberToString(data[i].mNumber, &oneString);
		}
		else if (desc[i].mDataType == kText)
		{
			oneString = data[i].mText;
		}
		else
		{
			PDASSERT(0);
		}

		tupleString.append(oneString);
	}

	PDDebugOutput(tupleString.c_str());
}
#endif

};	// PandaSQL