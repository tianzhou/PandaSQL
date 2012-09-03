#ifndef PANDASQL_EXPR_CONTEXT_H
#define PANDASQL_EXPR_CONTEXT_H

#include "Catalog/Column.h"

#include "Utils/Value.h"

#include <map>
#include <string>

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
