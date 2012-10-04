#ifndef PANDASQL_VALUE_H
#define PANDASQL_VALUE_H

#include "Utils/Types.h"

#include <vector>

namespace PandaSQL
{

class Value
{
public:

	Value();
	bool operator==(const Value &rhs) const;

	int32_t GetAsNumber() const;
	void SetAsNumber(int32_t inNumber);

	std::string GetAsString() const;
	void SetAsString(const std::string inString);

private:

	DataType mValueType;

	int32_t mNumber;
	std::string mText;

};

typedef std::vector<Value> ValueList;

}	// namespace PandaSQL

#endif	// PANDASQL_VALUE_H