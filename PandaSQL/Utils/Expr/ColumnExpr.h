#ifndef PANDASQL_COLUMN_EXPR_H
#define PANDASQL_COLUMN_EXPR_H

#include "Expr.h"

#include "Catalog/Column.h"

namespace PandaSQL
{

class ColumnExpr : public Expr
{
public:

	ColumnExpr();
	~ColumnExpr();

	ColumnQualifiedName GetQualifiedColumnName() const;
	void SetQualifiedColumnName(const ColumnQualifiedName &inQualifiedColumnName);

	virtual bool IsTrue(const ExprContext &inExprContext) const;
	virtual Status GetValue(const ExprContext &inExprContext, Value *io_value) const;

	virtual void PopulateDependentColumns(TableAndColumnSetMap *io_tableAndColumnSetMap) const;

private:
	
	ColumnQualifiedName mQualifiedColumnName;
};

}	// PandaSQL


#endif //PANDASQL_COLUMN_EXPR_H
