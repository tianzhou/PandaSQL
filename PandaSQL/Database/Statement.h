#ifndef PANDASQL_STATEMENT_H
#define PANDASQL_STATEMENT_H

#include "Catalog/Table.h"

#include "Optimizer/Join/Join.h"

#include "Utils/Predicate.h"

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
		kStmtSelect = 1,
		kStmtUpdate = 2,
		kStmtInsert = 3,
		kStmtDelete = 4,
		kStmtCreateTable = 5,
		kStmtCreateIndex = 6,
		kStmtDropTable = 7,
		kStmtDropIndex = 8,
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
	void AddExprRef(const Expr &inExpr);

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

	void SetPredicate(const Predicate &inPredicate);

	//Rewrite statment. e.g. Translate to fully qualified column name
	Status Prepare();

	Status Execute(bool loadTable);
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

	Predicate mPredicate;

	std::string mIndexRef;

	BooleanExpr *mpWhereExpr;
};


}	// PandaSQL


#endif //PANDASQL_STATEMENT_H
