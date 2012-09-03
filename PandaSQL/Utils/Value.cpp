#include "stdafx.h"

#include "Value.h"

#include "Utils/Common.h"
#include "Utils/Debug.h"

namespace PandaSQL
{

Value::Value()
:
mValueType(kUnknownType)
{
}

bool Value::operator==(const Value &rhs) const
{
	bool result = true;

	switch (mValueType)
	{
	case kInt:
		{
			result = (mNumber == rhs.GetAsNumber());
			break;
		}
	case kText:
		{
			result = (mText == rhs.GetAsString());
			break;
		}
	default:
		PDASSERT(0);
		break;
	}

	return result;
}

int32_t Value::GetAsNumber() const
{
	int32_t result;

	switch (mValueType)
	{
	case kInt:
		{
			result = mNumber;
			break;
		}
	case kText:
		{
			StringToNumber(mText, &result);
			break;
		}
	default:
		PDASSERT(0);
		break;
	}

	return result;
}

void Value::SetAsNumber(int32_t inNumber)
{
	mValueType = kInt;
	mNumber = inNumber;
}

std::string Value::GetAsString() const
{
	std::string result;

	switch (mValueType)
	{
	case kInt:
		{
			NumberToString(mNumber, &result);
			break;
		}
	case kText:
		{
			result = mText;
			break;
		}
	default:
		PDASSERT(0);
		break;
	}

	return result;
}

void Value::SetAsString(const std::string inString)
{
	mValueType = kText;
	mText = inString;
}


}	// PandaSQL