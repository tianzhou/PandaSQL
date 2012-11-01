#include "stdafx.h"

#include "Access/Tuple.h"

#include "Utils/Common.h"
#include "Utils/Debug.h"

namespace PandaSQL
{

void TupleElementToString(const TupleDescElement &descElement, const Value &tupleValue, std::string *o_string)
{
	if (descElement.mDataType == kInt)
	{
		int32_t numberValue = tupleValue.GetAsNumber();

		o_string->append((const char *)&numberValue, sizeof(numberValue));
	}
	else if (descElement.mDataType == kText)
	{
		*o_string = tupleValue.GetAsString();
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