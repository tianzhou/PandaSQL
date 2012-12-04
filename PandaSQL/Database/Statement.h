#ifndef PANDASQL_STATEMENT_H
#define PANDASQL_STATEMENT_H

#include "Catalog/Table.h"

#include "Optimizer/Join/Join.h"

namespace PandaSQL
{

class DBImpl;
class BooleanExpr;

class Statement
{
public:
	enum StatementType
	{
		kStmtUnknown = 0,
		kStmtEmpty = 1,
		kStmtSelect = 2,
		kStmtInsert = 3,
		kStmtUpdate = 4,
		kStmtDelete = 5,
		kStmtCreateTable = 6,
		kStmtDropTable = 7,
		kStmtCreateIndex = 8,
		kStmtDropIndex = 9,
	};

	Statement(StatementType inStmtType, DBImpl *io_pDB);
	~Statement();

	void Clear();

	void SetOriginalStmtText(const std::string inStmtText);

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
	void AddExprRef(Expr *inExpr);

	//For create_index_stmt, indexed column[1]
	//For select_stmt, selected column[1..N]
	//For insert_stmt, affected column[1..N]
	//For update_stmt, affected column[1..N]
	void AddColumnWithQualifiedName(const ColumnQualifiedName &inQualifiedName);

	//For cerate_table_stmt, column def[1..N]
	void AddColumnDef(const ColumnDef &inDef);
	void AddAllColumns();

	//For create_index_stmt
	void SetIndexRef(const std::string &inIndexRef);
	void SetUniqueIndex(bool isUnique);

	//Rewrite statment. e.g. Translate to fully qualified column name
	Status Prepare();

	//We use "CREATE TABLE/INDEX ..." to both create and load table/index.
	Status Execute(bool createTableOrIndex = true);
	void PrintStatement();

	void SetWhereClauseExpression(BooleanExpr *inWhereExpr);

private:

	friend class ParserDriver;
	friend class Planner;

	const Table::TableRefList& GetTableRefList() const { return mTableRefs; }
	const ColumnDefList& GetTargetColumnDefList() const { return mColumnDefs; }
	const BooleanExpr* GetWhereExpr() const { return mpWhereExpr; }
	const DBImpl* GetDB() const { return mpDB; }

	DBImpl *mpDB;

	std::string	mOrigStmtText;

	StatementType mStmtType;
	Table::TableRefList mTableRefs;

	ExprList mSetExprList;

	ColumnDefList mColumnDefs;
	bool	mAllColumns;

	JoinList mJoinList;

	std::string mIndexRef;
	bool	mUniqueIndex;

	BooleanExpr *mpWhereExpr;
};


}	// PandaSQL


#endif //PANDASQL_STATEMENT_H
