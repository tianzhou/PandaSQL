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

private:

	std::string mColumnName;
	std::string mTableName;
};

}	// PandaSQL


#endif //PANDASQL_COLUMN_EXPR_H
