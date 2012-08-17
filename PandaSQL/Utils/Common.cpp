
#include "stdafx.h"

#include "Common.h"

#include "Debug.h"

#include "DB.h"
#include "Catalog/Table.h"
#include "Catalog/Column.h"

#include <sstream>

namespace PandaSQL
{

Status AmendColumnDef(const DB &inDB, const TableRefList &inTableRefList, ColumnDef *io_columnDef)
{
	PDASSERT(io_columnDef);

	Status result;

	if (io_columnDef->qualifiedName.tableName.empty())
	{
		Table::TableRefList::const_iterator iter = inTableRefList.begin();

		for (; iter != inTableRefList.end(); iter++)
		{
			Table *theTable;

			//TODO: Figure out right call
			//result = inDB.GetTableByName(*iter, &theTable);

			if (result.OK())
			{
				Status localStatus = theTable->GetColumnByName(io_columnDef->qualifiedName.columnName, io_columnDef);

				if (localStatus.OK())
				{
					break;
				}
			}
		}

		if (iter == inTableRefList.end())
		{
			result = Status::kColumnMissing;
		}
	}
	else
	{
		Table::TableRefList::const_iterator iter = inTableRefList.begin();

		for (; iter != inTableRefList.end(); iter++)
		{
			if (*iter == io_columnDef->qualifiedName.tableName)
			{
				Table *theTable;

				//TODO: Figure out right call
				//result = inDB.GetTableByName(*iter, &theTable);

				if (result.OK())
				{
					Status localStatus = theTable->GetColumnByName(io_columnDef->qualifiedName.columnName, io_columnDef);

					if (!localStatus.OK())
					{
						result = Status::kColumnMissing;
					}

					break;
				}
			}
		}

		if (iter == inTableRefList.end())
		{
			result = Status::kTableMissing;
		}
	}

	return result;

}

void StringToNumber(const std::string &inStr, uint32_t *o_num)
{
	*o_num = atoi(inStr.c_str());
}

void NumberToString(const uint32_t inNum, std::string *o_str)
{
	std::stringstream convert;

	convert << inNum;

	*o_str = convert.str();
}


}	// PandaSQL
