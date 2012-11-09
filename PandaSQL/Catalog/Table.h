#ifndef PANDASQL_TABLE_H
#define PANDASQL_TABLE_H

#include <string>
#include <vector>

#include "Catalog/Column.h"

#include "Expr/Expr.h"

#include "Storage/IStorage.h"

#include "Utils/Types.h"
#include "Utils/Status.h"

namespace PandaSQL
{

class Iterator;

class Table
{

public:

	typedef std::vector<std::string> TableRefList;

	Table();
	~Table();
	
	void SetName(const std::string &inName) { mName = inName; }
	std::string GetName() const { return mName; }
	void AddColumnDef(const ColumnDef &inColumDef);

	const ColumnDefList& GetAllColumns() const { return mColumnList; }
	Status GetColumnByName(const std::string &name, ColumnDef *o_columnDef) const;

	Status Open(IStorage::OpenMode openMode);
	Status AddRecord(const ColumnDefList &columnList, const ColumnValueList &columnValueList);
	Status DeleteRecord(const TuplePredicate *inTuplePredicate = NULL);
	Status SelectRecords(const ColumnDefList &columnList, const TuplePredicate *inTuplePredicate = NULL);

	Iterator* CreateScanIterator(const TuplePredicate *inTuplePredicate = NULL);
	Iterator* CreateIndexIterator(); 

private:
	
	Table(const Table &rhs);
	Table& operator=(const Table &rhs);

	Iterator* GetScanIterator();

	std::string mName;
	ColumnDefList mColumnList;

	IStorage *mpDataHost;
};

}	// PandaSQL

#endif	// PANDASQL_TABLE_H