#ifndef PANDASQL_COLUMN_EXPR_H
#define PANDASQL_COLUMN_EXPR_H

#include "Expr.h"

namespace PandaSQL
{

class ColumnExpr : public Expr
{
public:

	ColumnExpr();
	~ColumnExpr();

	std::string GetColumnName() const;
	void SetColumnName(const std::string &inColumnName);

	std::string GetTableName() const;
	void SetTableName(const std::string &inTableName);

	virtual Status GetValue(ExprContext *io_exprContext, Value *io_value);

private:
	
	ColumnQualifiedName mQualifiedColumnName;
};

}	// PandaSQL


#endif //PANDASQL_COLUMN_EXPR_H
