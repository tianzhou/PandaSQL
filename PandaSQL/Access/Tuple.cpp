#include "stdafx.h"

#include "Tuple.h"

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

TupleData::TupleData()
{
}

TupleData::~TupleData()
{
}

uint32_t TupleData::Count() const
{
	return (uint32_t)mValueList.size();
}

void TupleData::GetDataAtIndex(uint32_t index, std::string *o_data) const
{
	PDASSERT(index < this->Count());

	*o_data = mValueList[index];
}

void TupleData::AppendData(const std::string &inData)
{
	mValueList.push_back(inData);
}

void TupleData::SetDataAtIndex(uint32_t index, const std::string &inData)
{
	PDASSERT(index < this->Count());

	mValueList[index] = inData;
}

std::string TupleData::ToString() const
{
	std::string resultString;
	std::string oneValue;

	for (uint32_t i = 0; i < this->Count(); i++)
	{
		if (i != 0)
		{
			resultString += ' ';
		}

		this->GetDataAtIndex(i, &oneValue);

		resultString += oneValue;
	}

	return resultString;
}

};	// PandaSQL