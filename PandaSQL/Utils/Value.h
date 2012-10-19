#ifndef PANDASQL_VALUE_H
#define PANDASQL_VALUE_H

#include <vector>

#include "Utils/Types.h"

namespace PandaSQL
{

class Value
{
public:

	Value();
	bool operator==(const Value &rhs) const;

	DataType GetType() const { return mValueType; }

	int32_t GetAsNumber() const;
	void SetAsNumber(int32_t inNumber);

	std::string GetAsString() const;
	void SetAsString(const std::string inString);

	bool GetAsBool() const;

private:

	DataType mValueType;

	int32_t mNumber;
	std::string mText;

};

typedef std::vector<Value> ValueList;

}	// namespace PandaSQL

#endif	// PANDASQL_VALUE_H