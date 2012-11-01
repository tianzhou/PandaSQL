#ifndef PANDASQL_TUPLE_H
#define PANDASQL_TUPLE_H

#include <vector>

#include "Utils/Status.h"
#include "Utils/Types.h"
#include "Utils/Value.h"

namespace PandaSQL
{

class TupleFunctor
{

public:

	TupleFunctor() {}
	virtual ~TupleFunctor() {}

	virtual void operator()(const std::string &inTupleData) = 0;
};

class TupleDescElement
{
public:
	DataType mDataType;
};

typedef std::vector<TupleDescElement> TupleDesc;

void TupleElementToString(const TupleDescElement &descElement, const Value &tupleValue, std::string *o_string);
void TupleToString(const TupleDesc &tupleDesc, const ValueList &tupleValueList, std::string *o_string);


#ifdef PDDEBUG
void PrintTuple(const ValueList &data);
#endif

}	// PandaSQL

#endif	// PANDASQL_TUPLE_H