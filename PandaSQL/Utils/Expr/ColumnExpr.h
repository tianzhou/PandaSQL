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

	virtual bool IsTrue(ExprContext *io_exprContext) const;
	virtual Status GetValue(ExprContext *io_exprContext, Value *io_value) const;

	virtual void PopulateDependentColumns(TableAndColumnSetMap *io_tableAndColumnSetMap) const;

private:
	
	ColumnQualifiedName mQualifiedColumnName;
};

}	// PandaSQL


#endif //PANDASQL_COLUMN_EXPR_H
