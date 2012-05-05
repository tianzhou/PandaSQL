#include "stdafx.h"

#include "Table.h"

namespace PandaSQL
{

Table::Table()
{
}

Table::~Table()
{
}

void Table::AddColumnDef(const ColumnDef &inColumDef)
{
	mColumnList.push_back(inColumDef);
}

}	// PandaSQL