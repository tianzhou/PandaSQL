#ifndef PANDASQL_TABLE_H
#define PANDASQL_TABLE_H

#include <vector>

namespace PandaSQL
{

enum DataType
{
	kInt = 0,
	kText = 1,
};

enum ConstraintType
{
	kConstraintNone = 0,
	kConstraintPrimaryKey = 1,
	kConstraintUnique = 2,
	kConstraintNotNULL = 3,
};

struct ColumnDef
{
	std::string columnName;
	DataType dataType;
	ConstraintType constraintType;
};

enum ExprType
{
	kExprNumber = 0,
	kExprText = 1,
	kExprColumnRef = 2
};

struct Expr
{
	ExprType type;
	std::string text;
};


class Table
{
public:

	typedef std::vector<std::string> TableRefList;
	typedef std::vector<ColumnDef> ColumnDefList;
	typedef std::vector<std::string> ColumnRefList;
	typedef std::vector<Expr> ColumnValueList;

	Table();
	~Table();
	
	void SetName(const std::string &inName) { mName = inName; }
	std::string GetName() const { return mName; }
	void AddColumnDef(const ColumnDef &inColumDef);

private:
	
	Table(const Table &rhs);
	Table& operator=(const Table &rhs);

	std::string mName;
	ColumnDefList mColumnList;
};

}	// PandaSQL

#endif	// PANDASQL_TABLE_H