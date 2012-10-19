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

class DependentColumnListWalker : public ExprWalker
{

public:

	DependentColumnListWalker(TableAndColumnSetMap *io_tableAndColumnSetMap);
	virtual ~DependentColumnListWalker();

	virtual void Visit(const Expr *io_expr);

private:

	TableAndColumnSetMap *pTableAndColumnSetMap;
};

}	// PandaSQL


#endif //PANDASQL_EXPR_H