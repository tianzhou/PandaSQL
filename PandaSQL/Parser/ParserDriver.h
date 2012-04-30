#ifndef PANDASQL_PARSER_DRIVER_H
#define PANDASQL_PARSER_DRIVER_H

#include "Utils/Status.h"
#include "Utils/Types.h"

#include <string>
#include <vector>
#include <antlr3.h>

namespace PandaSQL
{

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

class Statement
{
public:
	Statement();
	~Statement();

	enum StatementType
	{
		kStmtUnknown = 0,
		kStmtSelect = 1,
		kStmtUpdate = 2,
		kStmtInsert = 3,
		kStmtDelete = 4,
		kStmtCreateTable = 5,
		kStmtCreateIndex = 6,
		kStmtDropTable = 7,
		kStmtDropIndex = 8,
	};

	static const std::string kNoTable;

	void SetStatementType(StatementType inStmtType) { mStmtType = inStmtType; }
	StatementType GetStatementType() const { return mStmtType; }

	void AddColumnRef(const std::string &inColumnRef);
	void AddTableRef(const std::string &inTableRef);
	void AddExprRef(const Expr &inExpr);

	void AddColumnDef(const ColumnDef &inDef);

	void SetIndexRef(const std::string &inIndexRef);

	void PrintStatement();
private:
	StatementType mStmtType;
	std::vector<std::string> mSelectColumnRefs;
	std::vector<std::string> mTableRefs;

	std::vector<Expr> mSetExprList;

	std::vector<ColumnDef> mColumnDefs;

	std::string mIndexRef;
};

class ParserDriver
{
public:
	ParserDriver();
	~ParserDriver();

	Status PerformQuery(std::string inQueryString, bool fromFile);
	void PrintCurrentState();

	static std::string GetColumnRef(const std::string &inTableName, const std::string &inColumnName);
	static void GetIdentifier(ANTLR3_BASE_TREE *tree, std::string &o_str);
privileged:

	const Statement& GetStatement() const { return mStmt; }
	Statement& GetStatement() { return mStmt; }

private:

	class SQLParserSelect
	{
	public:
		SQLParserSelect();
		~SQLParserSelect();
	};

	ParserDriver(const ParserDriver &rhs);
	ParserDriver& operator=(const ParserDriver &rhs);


	Statement mStmt;
};

}	// namespace PandaSQL

#endif	// PANDASQL_PARSER_DRIVER_H
