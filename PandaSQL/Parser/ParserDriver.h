#ifndef PANDASQL_PARSER_DRIVER_H
#define PANDASQL_PARSER_DRIVER_H

#include "Utils/Status.h"
#include "Utils/Types.h"

#include <string>
#include <vector>
#include <antlr3.h>

namespace PandaSQL
{

typedef enum
{
	kExprNumber = 0,
	kExprText = 1,
	kExprColumnRef = 2
}ExprType;

typedef struct
{
	ExprType type;
	std::string text;
}Expr;

class Statement
{
public:
	Statement();
	~Statement();

	typedef enum
	{
		kStmtUnknown = 0,
		kStmtSelect = 1,
		kStmtUpdate = 2,
		kStmtInsert = 3,
		kStmtDelete = 4,
		kStmtCreate = 5
	}StatementType;

	static const std::string kNoTable;

	void SetStatementType(StatementType inStmtType) { mStmtType = inStmtType; }
	StatementType GetStatementType() const { return mStmtType; }

	void AddColumnRef(const std::string& inColumnRef);
	void AddTableRef(const std::string& inTableName);
	void AddExprRef(const Expr& inExpr);

	void PrintStatement();
private:
	StatementType mStmtType;
	std::vector<std::string> mSelectColumnRefs;
	std::vector<std::string> mTableRefs;

	std::vector<Expr> mSetExprList;
};

class ParserDriver
{
public:
	ParserDriver();
	~ParserDriver();

	Status PerformQuery(std::string inQueryString, bool fromFile);
	void PrintCurrentState();

	static std::string GetColumnRef(const std::string& inTableName, const std::string& inColumnName);
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

	ParserDriver(const ParserDriver& rhs);
	ParserDriver& operator=(const ParserDriver& rhs);


	Statement mStmt;
};

}	// namespace PandaSQL

#endif	// PANDASQL_PARSER_DRIVER_H
