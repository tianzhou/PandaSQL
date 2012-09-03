#include "stdafx.h"

#include "PandaDB.h"
#include "Storage/IDBBackend.h"

#include "Parser/ParserDriver.h"

#include "Catalog/Table.h"

#include "Storage/IStorage.h"

#include "Access/TupleIterator.h"
#include "Access/Tuple.h"

#include "VFS/WinVFS.h"

#include "Utils/Common.h"
#include "Utils/Debug.h"
#include "Utils/Expr/BooleanExpr.h"
#include "Utils/Expr/ExprContext.h"
#include "Utils/Predicate.h"

#include <iostream>

namespace PandaSQL
{

//static
IVFS* PandaDB::CreateVFS()
{
	//Only have windows for now
	return new WinVFS();
}

PandaDB::Options::Options()
:
create_if_missing(false)
{
}

PandaDB::PandaDB(StorageType inStorageType)
:
mStorageType(inStorageType)
,mpBackend(NULL)
,mIsOpen(false)
{
}

PandaDB::~PandaDB()
{
	PDASSERT(!mpBackend);
}

Status PandaDB::Open(const std::string &inDBPath, const Options &inOptions)
{
	Status result;

	if (mIsOpen)
	{
		result = Status::kInvalidCommand;
	}
	else
	{
		PDASSERT(!mpBackend);

		mpBackend = IDBBackend::CreateBackend(inDBPath, mStorageType);
	
		result = mpBackend->Open();

		mIsOpen = true;
	}

	return result;
}

Status PandaDB::Close()
{
	PDASSERT(mIsOpen);

	Status result;

	result = mpBackend->Close();

	PDASSERT(result.OK());

	delete mpBackend;
	mpBackend = NULL;

	return result;
}

Status PandaDB::CreateTable(const std::string &tableName, const ColumnDefList &columnList)
{
	Status result;

	TableMap::iterator iter = mTableMap.find(tableName);

	PDASSERT(iter == mTableMap.end());

	if (iter == mTableMap.end())
	{
		Table *pTable = new Table();
		pTable->SetName(tableName);

		ColumnDefList::const_iterator colIter = columnList.begin();

		for (; colIter != columnList.end(); colIter++)
		{		
			pTable->AddColumnDef(*colIter);
		}
		
		mTableMap.insert(TableMapEntry(tableName, pTable));

		result = mpBackend->CreateTable(tableName, columnList);
	}
	else
	{
		result = Status::kTableAlreadyExists;
	}

	return result;
}

Status PandaDB::OpenTable(const std::string &tableName)
{
	Status result;

	result = mpBackend->OpenTable(tableName);
	
	return result;
}

Status PandaDB::InsertData(const std::string &tableName, const ColumnDefList &columnList, const ExprList &columnExprList)
{
	Status result;

	TupleDesc tupleDesc;
	TupleData tupleData;
	ColumnDefListToTupleDesc(columnList, &tupleDesc);
	Expr::EvalExprList(columnExprList, tupleDesc, &tupleData);
	
	result = mpBackend->InsertData(tableName, tupleDesc, tupleData, -1);

	return result;
}

Status PandaDB::DeleteData(const std::string &tableName, const BooleanExpr *inBooleanExpr /* = NULL */)
{
	Status result;

	Table *theTable = NULL;

	result = this->GetTableByName(tableName, &theTable);

	if (result.OK())
	{
		std::cout << "****** Delete Table:" << tableName << " ******" << std::endl;
		//result = mpBackend->DeleteData(tableName, inTuplePredicate);
	}

	return result;
}

Status PandaDB::SelectData(const Table::TableRefList &tableList, const JoinList &joinList, const ColumnDefList &projectColumnList, const BooleanExpr *inWhereExpr /*= NULL*/)
{
	Status result;

	if (tableList.size() == 1)
	{
		Table *theTable = NULL;

		result = this->GetTableByName(tableList[0], &theTable);
   
		if (result.OK())
		{
			std::cout << "****** Select Table:" << tableList[0] << " ******" << std::endl;
			//result = mpBackend->SelectData(tableList[0], columnList, inTuplePredicate);
			const ColumnDefList &allColumnList = theTable->GetAllColumns();
			//TupleDesc tupleDesc;
			//TupleDesc projectTupleDesc;

			//ColumnDefListToTupleDesc(allColumnList, &tupleDesc);
			//ColumnDefListToTupleDesc(projectColumnList, &projectTupleDesc);
			
			TupleIterator *theIter = mpBackend->CreateScanIterator(tableList[0], allColumnList, NULL);
			
			ValueList tupleValue;
			ValueList projectTupleValue;
			ExprContext exprContext;

			while (theIter->Valid())
			{
				result = theIter->GetValue(&tupleValue);

				if (!result.OK())
				{
					break;
				}

				exprContext.UpdateTupleValue(allColumnList, tupleValue);

				if (!inWhereExpr || inWhereExpr->IsTrue(&exprContext))
				{
					//TODO: This is slow when doing every time
					ProjectTuple(allColumnList, projectColumnList, tupleValue, &projectTupleValue);			

#ifdef PDDEBUG
					PrintTuple(projectTupleValue);
#endif
				}

				theIter->Next();
			}

			delete theIter;	
		}
	}
	else if (tableList.size() == 2)
	{
		//Table *outerTable = NULL;
		//Table *innerTable = NULL;

		//result = this->GetTableByName(joinList[0].tableName, &outerTable);

		//if (result.OK())
		//{
		//	result = this->GetTableByName(joinList[1].tableName, &innerTable);

		//	if (result.OK())
		//	{
		//		Iterator *outerScan = outerTable->CreateScanIterator();

		//		PDASSERT(outerScan);

		//		while (outerScan->Valid())
		//		{
		//			TupleData outerTuple;

		//			result = outerScan->GetValue(&outerTuple);

		//			if (!result.OK())
		//			{
		//				break;
		//			}

		//			std::vector<TupleEntry> tupleContext;
		//			TupleEntry outerTupleEntry = {outerTable->GetName(), outerTuple};
		//			tupleContext.push_back(outerTupleEntry);

		//			Iterator *innerScan = innerTable->CreateScanIterator();

		//			PDASSERT(innerScan);

		//			while (innerScan->Valid())
		//			{
		//				TupleData innerTuple;

		//				result = innerScan->GetValue(&innerTuple);

		//				if (!result.OK())
		//				{
		//					break;
		//				}

		//				//if (!inTuplePredicate
		//				//	|| inTuplePredicate->Eval(tupleContext))
		//				//{
		//				//	std::cout << outerTuple.ToString() << std::endl;
		//				//}

		//				innerScan->Next();
		//			}

		//			delete innerScan;

		//			if (!result.OK())
		//			{
		//				break;
		//			}

		//			outerScan->Next();
		//		}

		//		delete outerScan;
		//	}
		//}
	}
	else
	{
		PDASSERT(0);
	}

	return result;
}

Status PandaDB::GetColumnDefFromQualifiedName(const Table::TableRefList &inTableRefList, const ColumnQualifiedName &inQualifiedName, ColumnDef *io_columnDef) const
{
	PDASSERT(io_columnDef);

	Status result;

	if (inQualifiedName.tableName.empty())
	{
		Table::TableRefList::const_iterator iter = inTableRefList.begin();

		for (; iter != inTableRefList.end(); iter++)
		{
			Table *theTable;

			result = this->GetTableByName(*iter, &theTable);

			if (result.OK())
			{
				Status localStatus = theTable->GetColumnByName(inQualifiedName.columnName, io_columnDef);

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
			if (*iter == inQualifiedName.tableName)
			{
				Table *theTable;

				result = this->GetTableByName(*iter, &theTable);

				if (result.OK())
				{
					Status localStatus = theTable->GetColumnByName(inQualifiedName.columnName, io_columnDef);

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

Status PandaDB::GetTableByName(const std::string &name, Table **o_table) const
{
	Status result;

	TableMap::const_iterator iter = mTableMap.find(name);

	if (iter != mTableMap.end())
	{
		*o_table = iter->second;
	}
	else
	{
		*o_table = NULL;

		result = Status::kTableMissing;
	}

	return result;
}

void PandaDB::ClearTableMap_Private()
{
	TableMap::iterator iter = mTableMap.begin();

	for (; iter != mTableMap.end(); iter++)
	{
		delete iter->second;
	}

	mTableMap.clear();
}

Table* PandaDB::GetTableByID(uint32_t inTableID) const
{
	Table *result = NULL;

	return result;
}

uint32_t PandaDB::GetTableIDByName(const std::string &inTableName) const
{
	uint32_t result = kUnknownID;

	return result;
}

uint32_t PandaDB::GetColumnIDByName(const std::string &inColumnName) const
{
	uint32_t result = kUnknownID;

	return result;
}


}	// PandaSQL
