#ifndef PANDASQL_EXPR_CONTEXT_H
#define PANDASQL_EXPR_CONTEXT_H

#include <map>
#include <string>

#include "Catalog/Column.h"

#include "Utils/Value.h"

namespace PandaSQL
{

class ExprContext
{

public:

	void UpdateTupleValue(const ColumnDefList &inColumnDefList, const ValueList &inValueList); 
	void GetColumnValue(const ColumnQualifiedName &inColumnQualifiedName, Value *io_value) const;

private:

	typedef std::map<ColumnQualifiedName, Value> ColumnValueMap;

	ColumnValueMap mColumnValueMap;
	
};

}	// PandaSQL


#endif //PANDASQL_EXPR_CONTEXT_H
