#ifndef PANDASQL_TABLE_H
#define PANDASQL_TABLE_H

#include "VFS/IVFS.h"

#include "Storage/IStorage.h"

#include "Utils/Types.h"
#include "Utils/Status.h"

#include <vector>

namespace PandaSQL
{

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

class Iterator;

class Table
{

public:

	typedef std::vector<std::string> TableRefList;
	typedef std::vector<ColumnDef> ColumnDefList;
	typedef std::vector<std::string> ColumnRefList;
	typedef std::vector<Expr> ColumnValueList;

	Table(const std::string &inDBRootPath, IStorage::StorageType inType, IVFS *io_VFS);
	~Table();
	
	void SetName(const std::string &inName) { mName = inName; }
	std::string GetName() const { return mName; }
	void AddColumnDef(const ColumnDef &inColumDef);

	Status Open(IStorage::OpenMode openMode);
	Status InsertRecord(const ColumnRefList &columnList, const ColumnValueList &columnValueList);
	Status SelectRecords(const ColumnRefList &columnList);

private:
	
	Table(const Table &rhs);
	Table& operator=(const Table &rhs);

	Iterator* GetScanIterator();

	std::string mName;
	ColumnDefList mColumnList;

	IVFS *mpVFS;
	IStorage *mpDataHost;
	Iterator *mpScanIterator; //Always use GetScanIterator to access it.
};

}	// PandaSQL

#endif	// PANDASQL_TABLE_H