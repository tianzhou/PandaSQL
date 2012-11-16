#ifndef PANDASQL_TUPLE_H
#define PANDASQL_TUPLE_H

#include <vector>

#include "Utils/Status.h"
#include "Utils/Types.h"
#include "Utils/Value.h"

namespace PandaSQL
{

class TupleDescElement
{
public:
	DataType mDataType;
};

typedef std::vector<TupleDescElement> TupleDesc;

void StringToTupleElmentOffsetAndLength(const TupleDesc &desc, const std::string &inString, uint32_t inElementIndex, uint32_t *o_offset, uint32_t *o_length);

void StringToTupleElement(const TupleDescElement &descElement, const std::string &inString, uint32_t *io_offset, Value *o_value);
void StringToTuple(const TupleDesc &desc, const std::string &inString, ValueList *o_tupleValueList);

void TupleElementToString(const TupleDescElement &descElement, const Value &tupleValue, std::string *o_string);
void TupleToString(const TupleDesc &tupleDesc, const ValueList &tupleValueList, std::string *o_string);

#ifdef PDDEBUG
void PrintTuple(const ValueList &tupleValueList);
#endif

}	// PandaSQL

#endif	// PANDASQL_TUPLE_H