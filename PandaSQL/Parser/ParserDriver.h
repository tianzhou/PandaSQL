#ifndef PANDASQL_PARSER_DRIVER_H
#define PANDASQL_PARSER_DRIVER_H

#include "PandaDB.h"

#include "Catalog/Table.h"

#include "VFS/File.h"

#include "Utils/Status.h"
#include "Utils/Types.h"
#include "Utils/Predicate.h"
#include "Utils/Join.h"

#include <string>
#include <vector>
#include <antlr3.h>

namespace PandaSQL
{

class Statement
{
public:
	Statement(PandaDB *io_pDB);
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

	void Clear();

	void SetOriginalStmtText(const std::string inStmtText);

	void SetStatementType(StatementType inStmtType) { mStmtType = inStmtType; }
	StatementType GetStatementType() const { return mStmtType; }

	//For create_table_stmt, created table[1]
	//For create_index_stmt, affected table[1]
	//For drop_table_stmt, affected table[1]
	//For update_stmt, affected table[1]
	//For insert_stmt, affected table[1]
	//For delete_stmt, affected table[1]
	//For select_stmt, affected table[1..N]
	void AddTableRef(const std::string &inTableRef);

	//For update_stmt, applied value[1..N]
	//For insert_stmt, applied value[1..N]
	void AddExprRef(const Expr &inExpr);

	//For create_index_stmt, indexed column[1]
	//For select_stmt, selected column[1..N]
	//For insert_stmt, affected column[1..N]
	//For update_stmt, affected column[1..N]
	void AddColumnDefWithName(const ColumnQualifiedName &inQualifiedName);

	//For cerate_table_stmt, column def[1..N]
	void AddColumnDef(const ColumnDef &inDef); 

	//For create_index_stmt
	void SetIndexRef(const std::string &inIndexRef);

	void SetPredicate(const Predicate &inPredicate);

	//Rewrite statment. e.g. Translate to fully qualified column name
	Status Prepare();

	Status Execute(bool loadTable);
	void PrintStatement();

private:
	PandaDB *mpDB;

	std::string	mOrigStmtText;

	StatementType mStmtType;
	Table::TableRefList mTableRefs;

	ExprList mSetExprList;

	ColumnDefList mColumnDefs;

	JoinList mJoinList;

	Predicate mPredicate;

	std::string mIndexRef;
};

class ParserDriver
{
public:
	ParserDriver(PandaDB *io_pDB);
	~ParserDriver();

	Status LoadFromFile(File *inFile);
	Status ParseQuery(std::string inQueryString);
	Status Execute();
	void PrintCurrentState();
	void SetLoadTable(bool isLoadTable) { mLoadTable = isLoadTable; }
	bool IsLoadTable() const { return mLoadTable; }

	static void GetString(ANTLR3_BASE_TREE *tree, std::string *o_str);
	static void GetNumber(ANTLR3_BASE_TREE *tree, int32_t *o_num);

	static void GetExprForText(ANTLR3_BASE_TREE *tree, Expr *o_expr);
	static void GetExprForNumber(ANTLR3_BASE_TREE *tree, Expr *o_expr);
	static void GetExprForColumnDef(const ColumnQualifiedName &inQualifiedName, Expr *o_expr);

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


	PandaDB *mpDB;
	Statement mStmt;
	
	//True when we are reading statment from table file to load table def.
	bool mLoadTable;
};

}	// namespace PandaSQL

#endif	// PANDASQL_PARSER_DRIVER_H
