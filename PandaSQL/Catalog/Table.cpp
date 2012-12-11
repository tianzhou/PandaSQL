#include "stdafx.h"

#include "Catalog/Table.h"

#include "Access/Tuple.h"

#include "Utils/Common.h"
#include "Utils/Debug.h"

namespace PandaSQL
{

Table::Table(void *payload)
:
mPayload(payload)
{
}

Table::~Table()
{
	
}

void Table::AddColumnDef(const ColumnDef &inColumDef)
{
	PDASSERT(inColumDef.qualifiedName.tableName == this->GetName());

	mColumnList.push_back(inColumDef);
}

Status Table::GetColumnByName(const std::string &name, ColumnDef *o_columnDef) const
{
	Status result;

	ColumnDefList::const_iterator iter = mColumnList.begin();

	uint32_t index = 0;
	for(; iter != mColumnList.end(); iter++)
	{
		if (name == iter->qualifiedName.columnName)
		{
			*o_columnDef = *iter;
			o_columnDef->index = index;
			break;
		}
		index++;
	}

	if (iter == mColumnList.end())
	{
		result = Status::kColumnMissing;
	}

	return result;
}

}	// PandaSQL