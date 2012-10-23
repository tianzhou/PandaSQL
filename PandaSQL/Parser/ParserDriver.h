#ifndef PANDASQL_PARSER_DRIVER_H
#define PANDASQL_PARSER_DRIVER_H

#include <string>
#include <vector>

#include <antlr3.h>

#include "Catalog/Column.h"

#include "Database/DBImpl.h"
#include "Database/Statement.h"

#include "Expr/BooleanExpr.h"
#include "Expr/Expr.h"

#include "VFS/File.h"

#include "Utils/Status.h"
#include "Utils/Types.h"

namespace PandaSQL
{

class ParserDriver
{
public:
	ParserDriver(DBImpl *io_pDB);
	~ParserDriver();

	void CreateStatement();
	void ReleaseStatement();

	Status LoadFromFile(File *inFile);
	Status ParseQuery(std::string inQueryString);
	Status Execute();
	void PrintCurrentState();
	void SetLoadTable(bool isLoadTable) { mLoadTable = isLoadTable; }
	bool IsLoadTable() const { return mLoadTable; }

	static void GetString(ANTLR3_BASE_TREE *tree, std::string *o_str);
	static void GetNumber(ANTLR3_BASE_TREE *tree, int32_t *o_num);
	static void GetStringFromAntlrString(const ANTLR3_STRING &inOpString, std::string *io_string);

	static void GetExprForText(ANTLR3_BASE_TREE *tree, Expr *o_expr);
	static void GetExprForNumber(ANTLR3_BASE_TREE *tree, Expr *o_expr);

	Expr* CreateExprForNumericLiteral(ANTLR3_BASE_TREE *numericTree);
	Expr* CreateExprForBinaryOp(const ANTLR3_STRING &inOpString, const Expr &inLeftOperand, const Expr &inRightOperand);
	Expr* CreateExprForColumnReference(const ColumnQualifiedName &inColumnQualifiedName);
	BooleanExpr* CreateExprForBooleanPrimary(const Expr &inSubExpr);
	BooleanExpr* CreateExprForBooleanAndList();
	BooleanExpr* CreateExprForBooleanOrList();

privileged:

	const Statement& GetStatement() const { return *mpStmt; }
	Statement& GetStatement() { return *mpStmt; }

private:

	class SQLParserSelect
	{
	public:
		SQLParserSelect();
		~SQLParserSelect();
	};

	ParserDriver(const ParserDriver &rhs);
	ParserDriver& operator=(const ParserDriver &rhs);


	DBImpl *mpDB;
	Statement *mpStmt;
	
	//True when we are reading statment from table file to load table def.
	bool mLoadTable;
};

}	// namespace PandaSQL

#endif	// PANDASQL_PARSER_DRIVER_H
