#include "stdafx.h"

#include "Tuple.h"

namespace PandaSQL
{

Tuple::FieldInfo::FieldInfo()
{
}

Tuple::FieldInfo::FieldInfo(DataType inType, const std::string &inValue)
:type(inType)
,value(inValue)
{
}

Tuple::Tuple()
{
}

Tuple::~Tuple()
{
}

uint32_t Tuple::Count() const
{
	return mValueList.size();
}

DataType Tuple::GetTypeOfField(uint32_t index) const
{
	PDASSERT(index < this->Count());

	return mValueList[index].type;
}

void Tuple::GetDataOfField(uint32_t index, std::string *o_data) const
{
	PDASSERT(index < this->Count());

	*o_data = mValueList[index].value;
}

void Tuple::AppendFieldData(DataType inType, const std::string &inData)
{
	mValueList.push_back(FieldInfo(inType, inData));
}

void Tuple::SetFieldData(uint32_t index, DataType inType, const std::string &inData)
{
	PDASSERT(index < this->Count());

	mValueList[index] = FieldInfo(inType, inData);
}

std::string Tuple::ToString() const
{
	std::string resultString;
	std::string oneValue;

	for (uint32_t i = 0; i < this->Count(); i++)
	{
		if (i != 0)
		{
			resultString += ' ';
		}

		this->GetDataOfField(i, &oneValue);

		resultString += oneValue;
	}

	return resultString;
}

};	// PandaSQL