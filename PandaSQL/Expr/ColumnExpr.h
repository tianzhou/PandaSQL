#ifndef PANDASQL_COLUMN_EXPR_H
#define PANDASQL_COLUMN_EXPR_H

#include "Expr/Expr.h"

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
	virtual Status Eval(const ExprContext &inExprContext, Value *io_value) const;

	virtual Expr* CreateSubExprForPushdown(const std::vector<std::string> &inTableNameList) const;

	virtual void Walk(ExprWalker *io_walker) const;
	virtual void MutableWalk(MutableExprWalker *io_walker);
	virtual Expr* Clone() const;

private:
	
	ColumnQualifiedName mQualifiedColumnName;
};

}	// PandaSQL


#endif //PANDASQL_COLUMN_EXPR_H
