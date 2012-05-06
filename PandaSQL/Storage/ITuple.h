#ifndef PANDASQL_ITUPLE_H
#define PANDASQL_ITUPLE_H

#include "Utils/Status.h"
#include "Utils/Types.h"

namespace PandaSQL
{

class ITuple
{
public:

	ITuple();
	virtual ~ITuple() = 0 {}

	virtual uint32_t Count() = 0;
	virtual DataType GetTypeOfField(uint32_t index) = 0;
	virtual void GetDataOfField(uint32_t index, std::string *o_data) = 0;

	virtual void AppendFieldData(DataType inType, const std::string &inData) = 0;
	virtual void SetFieldData(uint32_t index, DataType inType, const std::string &inData) = 0;

private:
	ITuple(const ITuple &rhs);
	ITuple& operator=(const ITuple &rhs);
};

}	// PandaSQL

#endif	// PANDASQL_STORAGE_H