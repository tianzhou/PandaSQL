#ifndef PANDASQL_TUPLEIMPL_H
#define PANDASQL_TUPLEIMPL_H

#include "ITuple.h"

#include <vector>

namespace PandaSQL
{

class TupleImpl : public ITuple
{
public:

	struct FieldInfo
	{
		DataType type;
		std::string value;

		FieldInfo();
		FieldInfo(DataType inType, const std::string &inValue);
	};

	TupleImpl();
	virtual ~TupleImpl();

	virtual uint32_t Count() const;
	virtual DataType GetTypeOfField(uint32_t index) const;
	virtual void GetDataOfField(uint32_t index, std::string *o_data) const;

	virtual void AppendFieldData(DataType inType, const std::string &inData);
	virtual void SetFieldData(uint32_t index, DataType inType, const std::string &inData);

private:
	TupleImpl(const TupleImpl &rhs);
	TupleImpl& operator=(const TupleImpl &rhs);

	std::vector<FieldInfo> mValueList;
};

}	// PandaSQL

#endif	// PANDASQL_TUPLEIMPL_H