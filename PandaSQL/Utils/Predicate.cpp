#include "stdafx.h"

#include "Predicate.h"
#include "Utils/Types.h"

namespace PandaSQL
{

Predicate::PredicateItem::PredicateItem()
{
}

Predicate::PredicateItem::PredicateItem(uint32_t inFieldNum, DataType inValueType, const std::string &inValue, PredicateType inPredicateType)
:
fieldNum(inFieldNum)
,valueType(inValueType)
,value(inValue)
,predicateType(inPredicateType)
{
}

Predicate::Predicate()
{
}

Predicate::~Predicate()
{
}

uint32_t Predicate::Count() const
{
	return mPredicateList.size();
}

uint32_t Predicate::GetFieldNumOfItem(uint32_t index) const
{
	PDASSERT(index < this->Count());

	return mPredicateList[index].fieldNum;
}

DataType Predicate::GetDataTypeOfItem(uint32_t index) const
{
	PDASSERT(index < this->Count());

	return mPredicateList[index].valueType;
}

void Predicate::GetDataOfItem(uint32_t index, std::string *o_data) const
{
	PDASSERT(index < this->Count());

	*o_data = mPredicateList[index].value;
}

Predicate::PredicateType Predicate::GetPredicateTypeOfItem(uint32_t index) const
{
	PDASSERT(index < this->Count());

	return mPredicateList[index].predicateType;
}

void Predicate::AppendPredicateItem(uint32_t inFieldNum, DataType inValueType, const std::string &inData, PredicateType inPredicateType)
{
	mPredicateList.push_back(PredicateItem(inFieldNum, inValueType, inData, inPredicateType));
}

void Predicate::SetPredicateItem(uint32_t index, uint32_t inFieldNum, DataType inType, const std::string &inData, PredicateType inPredicateType)
{
	PDASSERT(index < this->Count());

	mPredicateList[index] = PredicateItem(inFieldNum, inType, inData, inPredicateType);
}

};	// PandaSQL