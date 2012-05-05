#ifndef PANDASQL_PARSER_DRIVER_H
#define PANDASQL_PARSER_DRIVER_H

#include "DB.h"

#include "Catalog/Table.h"

#include "VFS/File.h"

#include "Utils/Status.h"
#include "Utils/Types.h"

#include <string>
#include <vector>
#include <antlr3.h>

namespace PandaSQL
{

class Statement
{
public:
	Statement(DB *io_pDB);
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

	void SetOriginalStmtText(const std::string inStmtText);

	void SetStatementType(StatementType inStmtType) { mStmtType = inStmtType; }
	StatementType GetStatementType() const { return mStmtType; }

	//For create_index_stmt, indexed column[1]
	//For select_stmt, selected column[1..N]
	//For insert_stmt, affected column[1..N]
	//For update_stmt, affected column[1..N]
	void AddColumnRef(const std::string &inColumnRef);

	//For create_table_stmt, created table[1]
	//For create_index_stmt, affected table[1]
	//For drop_table_stmt, affected table[1]
	//For insert_stmt, affected table[1]
	//For delete_stmt, affected table[1]
	void AddTableRef(const std::string &inTableRef);

	//For update_stmt, applied value[1..N]
	//For insert_stmt, applied value[1..N]
	void AddExprRef(const Expr &inExpr);

	//For cerate_table_stmt, column def[1..N]
	void AddColumnDef(const ColumnDef &inDef);

	//For create_index_stmt
	void SetIndexRef(const std::string &inIndexRef);

	Status Execute(bool loadTable);
	void PrintStatement();

private:
	DB *mpDB;

	std::string	mOrigStmtText;

	StatementType mStmtType;
	Table::ColumnRefList mSelectColumnRefs;
	Table::TableRefList mTableRefs;

	Table::ColumnValueList mSetExprList;

	Table::ColumnDefList mColumnDefs;

	std::string mIndexRef;
};

class ParserDriver
{
public:
	ParserDriver(DB *io_pDB);
	~ParserDriver();

	Status LoadFromFile(File *inFile);
	Status ParseQuery(std::string inQueryString);
	Status Execute();
	void PrintCurrentState();
	void SetLoadTable(bool isLoadTable) { mLoadTable = isLoadTable; }
	bool IsLoadTable() const { return mLoadTable; }

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


	DB *mpDB;
	Statement mStmt;
	
	//True when we are reading statment from table file to load table def.
	bool mLoadTable;
};

}	// namespace PandaSQL

#endif	// PANDASQL_PARSER_DRIVER_H
