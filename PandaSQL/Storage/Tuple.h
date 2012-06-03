#ifndef PANDASQL_TUPLE_H
#define PANDASQL_TUPLE_H

#include "Utils/Status.h"
#include "Utils/Types.h"

#include <vector>

namespace PandaSQL
{

class Tuple
{
public:

	struct FieldInfo
	{
		DataType type;
		std::string value;

		FieldInfo();
		FieldInfo(DataType inType, const std::string &inValue);
	};

	Tuple();
	virtual ~Tuple();

	virtual uint32_t Count() const;
	virtual DataType GetTypeOfField(uint32_t index) const;
	virtual void GetDataOfField(uint32_t index, std::string *o_data) const;

	virtual void AppendFieldData(DataType inType, const std::string &inData);
	virtual void SetFieldData(uint32_t index, DataType inType, const std::string &inData);

	virtual std::string ToString() const;

private:
	Tuple(const Tuple &rhs);
	Tuple& operator=(const Tuple &rhs);

	std::vector<FieldInfo> mValueList;
};

}	// PandaSQL

#endif	// PANDASQL_TUPLE_H