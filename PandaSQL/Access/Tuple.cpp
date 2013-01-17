#include "stdafx.h"

#include "Access/Tuple.h"

#include "Utils/Common.h"
#include "Utils/Debug.h"

namespace PandaSQL
{

void StringToTupleElmentOffsetAndLength(const TupleDesc &desc, const std::string &inString, uint32_t inElementIndex, uint32_t *o_offset, uint32_t *o_length)
{
	*o_offset = 0;
	*o_length = 0;

	PDASSERT(inElementIndex < desc.size());

	uint32_t currentElmentOffset = 0;

	for (size_t i = 0; i <= inElementIndex; i++)
	{
		const TupleDescElement &element = desc[i];

		if (element.mDataType == kInt)
		{
			*o_offset = currentElmentOffset;
			*o_length = sizeof(int32_t);

			currentElmentOffset += sizeof(int32_t);
		}
		else if (element.mDataType == kText)
		{
			*o_offset = currentElmentOffset;

			size_t length = *(const size_t *)(inString.c_str() + *o_offset);
			*o_offset += sizeof(length);
			currentElmentOffset += sizeof(length);

			*o_length = length;
			currentElmentOffset += *o_length;
		}
		else
		{
			PDASSERT(0);
		}	
	}
}

void StringToTupleElement(const TupleDescElement &descElement, const std::string &inString, uint32_t *io_offset, Value *o_tupleValue)
{
	if (descElement.mDataType == kInt)
	{
		int32_t numberValue = *(const int32_t *)(inString.c_str() + *io_offset);
		o_tupleValue->SetAsNumber(numberValue);
		*io_offset += sizeof(int32_t);
	}
	else if (descElement.mDataType == kText)
	{
		std::string stringValue;

		size_t length = *(const size_t *)(inString.c_str() + *io_offset);
		*io_offset += sizeof(length);

		stringValue.append(inString.c_str() + *io_offset, length);
		o_tupleValue->SetAsString(stringValue);
		
		*io_offset += length;
	}
	else
	{
		PDASSERT(0);
	}
}

void StringToTuple(const TupleDesc &desc, const std::string &inString, ValueList *o_tupleValueList)
{
	size_t offset = 0;

	o_tupleValueList->clear();

	for (size_t i = 0; i < desc.size(); i++)
	{
		Value oneValue;

		StringToTupleElement(desc[i], inString, &offset, &oneValue);

		o_tupleValueList->push_back(oneValue);
	}

	//Should consume the whole string
	PDASSERT(offset == inString.length());
}


void TupleElementToString(const TupleDescElement &descElement, const Value &tupleValue, std::string *o_string)
{
	if (descElement.mDataType == kInt)
	{
		int32_t numberValue = tupleValue.GetAsNumber();

		o_string->append((const char *)&numberValue, sizeof(numberValue));
	}
	else if (descElement.mDataType == kText)
	{
		size_t length = tupleValue.GetAsString().length();
		
		o_string->append((const char *)&length, sizeof(length));

		o_string->append(tupleValue.GetAsString());
	}
	else
	{
		PDASSERT(0);
	}
}

void TupleToString(const TupleDesc &tupleDesc, const ValueList &tupleValueList, std::string *o_string)
{
	PDASSERT(tupleDesc.size() == tupleValueList.size());

	o_string->clear();

	for (size_t i = 0; i < tupleDesc.size(); i++)
	{
		TupleElementToString(tupleDesc[i], tupleValueList[i], o_string);
	}
}

#ifdef PDDEBUG
void PrintTuple(const ValueList &tupleValueList)
{
	std::string tupleString;
	std::string oneString;
	for (size_t i = 0; i < tupleValueList.size(); i++)
	{
		if (i > 0)
		{
			tupleString.append(", ");
		}

		oneString = tupleValueList[i].GetAsString();

		tupleString.append(oneString);
	}

	PDDebugOutput(tupleString.c_str());
}
#endif

};	// PandaSQL