#include "stdafx.h"

#include "Database/DBImpl.h"

#include <iostream>

#include "Access/TupleIterator.h"
#include "Access/Tuple.h"

#include "Catalog/Table.h"

#include "Expr/BooleanExpr.h"
#include "Expr/ExprContext.h"

#include "Storage/IDBBackend.h"
#include "Storage/IStorage.h"

#include "Utils/Common.h"
#include "Utils/Debug.h"
#include "Utils/Predicate.h"

namespace PandaSQL
{

DBImpl::DBImpl(StorageType inStorageType)
:
mStorageType(inStorageType)
,mpBackend(NULL)
,mIsOpen(false)
{
}

DBImpl::~DBImpl()
{
	PDASSERT(!mpBackend);
}

Status DBImpl::Open(const std::string &inDBPath, const OpenOptions &inOptions)
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

Status DBImpl::Close()
{
	PDASSERT(mIsOpen);

	Status result;

	result = mpBackend->Close();

	PDASSERT(result.OK());

	delete mpBackend;
	mpBackend = NULL;

	return result;
}

Status DBImpl::CreateTable(const std::string &tableName, const ColumnDefList &columnList)
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

Status DBImpl::OpenTable(const std::string &tableName)
{
	Status result;

	result = mpBackend->OpenTable(tableName);
	
	return result;
}

Status DBImpl::InsertData(const std::string &tableName, const ColumnDefList &columnList, const ExprList &columnExprList)
{
	Status result;

	TupleDesc tupleDesc;
	TupleData tupleData;
	ColumnDefListToTupleDesc(columnList, &tupleDesc);
	Expr::EvalExprList(columnExprList, tupleDesc, &tupleData);
	
	result = mpBackend->InsertData(tableName, tupleDesc, tupleData, -1);

	return result;
}

Status DBImpl::DeleteData(const std::string &tableName, const BooleanExpr *inBooleanExpr /* = NULL */)
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

Status DBImpl::SelectData(const Table::TableRefList &tableList, const JoinList &joinList, const ColumnDefList &projectColumnList, const BooleanExpr *inWhereExpr /*= NULL*/)
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
				if (!theIter->GetValue(&tupleValue))
				{
					break;
				}

				exprContext.UpdateTupleValue(allColumnList, tupleValue);

				if (!inWhereExpr || inWhereExpr->IsTrue(exprContext))
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
		Table *outerTable = NULL;
		Table *innerTable = NULL;

		result = this->GetTableByName(tableList[0], &outerTable);

		if (result.OK())
		{
			result = this->GetTableByName(tableList[1], &innerTable);

			if (result.OK())
			{
				const ColumnDefList &outerTableAllColumnList = outerTable->GetAllColumns();
				const ColumnDefList &innerTableAllColumnList = innerTable->GetAllColumns();
				ColumnDefList allColumnList = outerTableAllColumnList;
				allColumnList.insert(allColumnList.end(), innerTableAllColumnList.begin(), innerTableAllColumnList.end());

				TupleIterator *outerScan = mpBackend->CreateScanIterator(tableList[0], outerTableAllColumnList, NULL);

				PDASSERT(outerScan);

				ValueList projectTupleValue;
				ExprContext exprContext;
				while (outerScan->Valid())
				{
					ValueList outerTupleValue;
					ValueList outerProjectTupleValue;

					if (!outerScan->GetValue(&outerTupleValue))
					{
						break;
					}

					exprContext.UpdateTupleValue(outerTableAllColumnList, outerTupleValue);

					TupleIterator *innerScan = mpBackend->CreateScanIterator(tableList[1], innerTableAllColumnList, NULL);

					PDASSERT(innerScan);

					while (innerScan->Valid())
					{
						ValueList innerTupleValue;
						ValueList innerProjectTupleValue;

						if (!innerScan->GetValue(&innerTupleValue))
						{
							break;
						}

						exprContext.UpdateTupleValue(innerTableAllColumnList, innerTupleValue);

						if (!inWhereExpr || inWhereExpr->IsTrue(exprContext))
						{
							//TODO: This is slow when doing every time
							ValueList allTupleValue = outerTupleValue;
							allTupleValue.insert(allTupleValue.end(), innerTupleValue.begin(), innerTupleValue.end());

							ProjectTuple(allColumnList, projectColumnList, allTupleValue, &projectTupleValue);		
#ifdef PDDEBUG
							PrintTuple(projectTupleValue);
#endif
						}

						innerScan->Next();
					}

					delete innerScan;

					if (!result.OK())
					{
						break;
					}

					outerScan->Next();
				}

				delete outerScan;
			}
		}
	}
	else
	{
		PDASSERT(0);
	}

	return result;
}

Status DBImpl::GetColumnDefFromQualifiedName(const Table::TableRefList &inTableRefList, const ColumnQualifiedName &inQualifiedName, ColumnDef *io_columnDef) const
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

TupleIterator* DBImpl::CreateTupleIteratorForTable(const Table &inTable)
{
	const ColumnDefList &allColumnList = inTable.GetAllColumns();

	TupleIterator *theIter = mpBackend->CreateScanIterator(inTable.GetName(), allColumnList, NULL);

	return theIter;
}

Status DBImpl::GetTableByName(const std::string &name, Table **o_table) const
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

void DBImpl::ClearTableMap_Private()
{
	TableMap::iterator iter = mTableMap.begin();

	for (; iter != mTableMap.end(); iter++)
	{
		delete iter->second;
	}

	mTableMap.clear();
}

Table* DBImpl::GetTableByID(uint32_t inTableID) const
{
	Table *result = NULL;

	return result;
}

uint32_t DBImpl::GetTableIDByName(const std::string &inTableName) const
{
	uint32_t result = kUnknownID;

	return result;
}

uint32_t DBImpl::GetColumnIDByName(const std::string &inColumnName) const
{
	uint32_t result = kUnknownID;

	return result;
}

}	// DBImpl
