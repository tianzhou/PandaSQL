#include "stdafx.h"

#include "TupleImpl.h"
#include "Utils/Types.h"

namespace PandaSQL
{

TupleImpl::FieldInfo::FieldInfo()
{
}

TupleImpl::FieldInfo::FieldInfo(DataType inType, const std::string &inValue)
:type(inType)
,value(inValue)
{
}

TupleImpl::TupleImpl()
{
}

TupleImpl::~TupleImpl()
{
}

uint32_t TupleImpl::Count()
{
	return mValueList.size();
}

DataType TupleImpl::GetTypeOfField(uint32_t index)
{
	PDASSERT(index < this->Count());

	return mValueList[index].type;
}

void TupleImpl::GetDataOfField(uint32_t index, std::string *o_data)
{
	PDASSERT(index < this->Count());

	*o_data = mValueList[index].value;
}

void TupleImpl::AppendFieldData(DataType inType, const std::string &inData)
{
	mValueList.push_back(FieldInfo(inType, inData));
}

void TupleImpl::SetFieldData(uint32_t index, DataType inType, const std::string &inData)
{
	PDASSERT(index < this->Count());

	mValueList[index] = FieldInfo(inType, inData);
}

};	// PandaSQL