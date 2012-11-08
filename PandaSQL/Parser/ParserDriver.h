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
	
	//caller is responsible to delete io_statement
	Status ParseQuery(std::string inQueryString, Statement **io_statement);

	Status LoadFromFile(File *inFile);

privileged:

	void PushNewStatement(Statement::StatementType inType);

	void PrintCurrentState();

	static void GetString(ANTLR3_BASE_TREE *tree, std::string *o_str);
	static void GetNumber(ANTLR3_BASE_TREE *tree, int32_t *o_num);
	static void GetStringFromAntlrString(const ANTLR3_STRING &inOpString, std::string *io_string);

	static Expr* CreateExprForNumericLiteral(ANTLR3_BASE_TREE *numericTree);
	static Expr* CreateExprForStringLiteral(ANTLR3_BASE_TREE *stringTree);
	static Expr* CreateExprForBinaryOp(const ANTLR3_STRING &inOpString, Expr *io_leftOperand, Expr *io_rightOperand);
	static Expr* CreateExprForColumnReference(const ColumnQualifiedName &inColumnQualifiedName);
	static BooleanExpr* CreateExprForBooleanPrimary(Expr *io_inSubExpr);
	static BooleanExpr* CreateExprForBooleanAndList();
	static BooleanExpr* CreateExprForBooleanOrList();

	//TODO: support subquery
	const Statement& GetCurrentStatement() const { return *mpStmt; }
	Statement& GetCurrentStatement() { return *mpStmt; }

	const Statement& GetRootStatement() const { return *mpStmt; }
	Statement& GetRootStatement() { return *mpStmt; }

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

	//Statement is created via ParserDriver, however it is not deleted
	//by it. The root statement is returned via ParseQuery to the caller
	//who is responsible to delete it
	Statement *mpStmt;
};

}	// namespace PandaSQL

#endif	// PANDASQL_PARSER_DRIVER_H
