#include "stdafx.h"

#include "Table.h"
#include "Storage/IStorage.h"
#include "Storage/TupleImpl.h"

namespace PandaSQL
{

Table::Table()
:mpDataHost(NULL)
{
	mpDataHost = IStorage::CreateStorage(IStorage::kCVS);
}

Table::~Table()
{
	delete mpDataHost;
}

void Table::AddColumnDef(const ColumnDef &inColumDef)
{
	mColumnList.push_back(inColumDef);
}

Status Table::InsertRow(const ColumnRefList &columnList, const ColumnValueList &columnValueList)
{
	Status result;

	TupleImpl oneTuple;

	ColumnValueList::const_iterator iter = columnValueList.begin();

	TupleImpl::FieldInfo oneField;

	DataType theType;

	for (; iter != columnValueList.end(); iter++)
	{
		if (iter->type == kExprNumber)
		{
			theType = kInt;
		}
		else if (iter->type == kExprText)
		{
			theType = kText;
		}
		else if (iter->type == kExprColumnRef)
		{
			PDASSERT(0);
		}

		oneTuple.AppendFieldData(theType, iter->text);
	}

	mpDataHost->InsertRow(oneTuple);

	return result;
}

}	// PandaSQL