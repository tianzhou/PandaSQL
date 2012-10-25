#ifndef PANDASQL_EXPR_WALKER_H
#define PANDASQL_EXPR_WALKER_H

#include "Catalog/Column.h"

namespace PandaSQL
{

class Expr;

class ExprWalker
{

public:

	ExprWalker() {}
	virtual ~ExprWalker() {}

	virtual void Visit(const Expr *io_expr) = 0;
};

class MutableExprWalker
{

public:

	MutableExprWalker() {}
	virtual ~MutableExprWalker() {}

	virtual void Visit(const Expr *io_expr) = 0;
	virtual void MutableVisit(Expr *io_expr) = 0;

};

class DependentColumnListWalker : public ExprWalker
{

public:

	DependentColumnListWalker(TableAndColumnSetMap *io_tableAndColumnSetMap);
	virtual ~DependentColumnListWalker();

	virtual void Visit(const Expr *io_expr);

private:

	TableAndColumnSetMap *mpTableAndColumnSetMap;
};

class AmendColumnWalker : public MutableExprWalker
{

public:

	AmendColumnWalker(const TableAndColumnSetMap &inValidTableAndColumnSetMap);
	virtual ~AmendColumnWalker();

	virtual void Visit(const Expr *io_expr);
	virtual void MutableVisit(Expr *io_expr);

private:

	const TableAndColumnSetMap &mTableAndColumnSetMap;
};

}	// PandaSQL


#endif //PANDASQL_EXPR_H