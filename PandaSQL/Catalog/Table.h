#ifndef PANDASQL_TABLE_H
#define PANDASQL_TABLE_H

#include <string>
#include <vector>

#include "Catalog/Column.h"

#include "Expr/Expr.h"

#include "Utils/Types.h"
#include "Utils/Status.h"

namespace PandaSQL
{

class Iterator;

class Table
{

public:

	typedef std::vector<std::string> TableRefList;

	Table(void *payload);
	~Table();

	void* GetPayload() const { return mPayload; }
	
	void SetName(const std::string &inName) { mName = inName; }
	std::string GetName() const { return mName; }
	void AddColumnDef(const ColumnDef &inColumDef);

	const ColumnDefList& GetAllColumns() const { return mColumnList; }
	Status GetColumnByName(const std::string &name, ColumnDef *o_columnDef) const;

private:
	
	Table(const Table &rhs);
	Table& operator=(const Table &rhs);

	Iterator* GetScanIterator();

	//payload is not owned by this class, 
	//so it's reasonable to make it mutable
	mutable void *mPayload;

	std::string mName;
	ColumnDefList mColumnList;
};

}	// PandaSQL

#endif	// PANDASQL_TABLE_H