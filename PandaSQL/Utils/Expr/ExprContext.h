#ifndef PANDASQL_EXPR_CONTEXT_H
#define PANDASQL_EXPR_CONTEXT_H

#include "Utils/Value.h"

#include <map>
#include <string>

namespace PandaSQL
{

struct ColumnDef;
struct ColumnQualifiedName;

class ExprContext
{

public:

	void UpdateColumnWithTupleData(const ColumnQualifiedName &inColumnQualifiedName, const Value &inValue); 
	void GetColumnValue(const ColumnQualifiedName &inColumnQualifiedName, Value *io_value) const;

private:

	typedef std::map<const ColumnQualifiedName, const Value> ColumnValueMap;
	typedef std::pair<const ColumnQualifiedName, const Value> ColumnValuePair;

	ColumnValueMap mColumnValueMap;
	
};

}	// PandaSQL


#endif //PANDASQL_EXPR_CONTEXT_H
