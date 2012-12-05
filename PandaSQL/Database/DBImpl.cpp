#include "stdafx.h"

#include "Database/DBImpl.h"

#include "Access/TupleIterator.h"
#include "Access/Tuple.h"

#include "Expr/BooleanExpr.h"
#include "Expr/ExprContext.h"

#include "Parser/ParserDriver.h"

#include "Storage/IDBBackend.h"
#include "Storage/IStorage.h"

#include "Utils/Common.h"
#include "Utils/Debug.h"
#include "Utils/Value.h"

namespace PandaSQL
{

bool s_initSystemTable = false;

const std::string kSchemaTableName = "PD_Table";

enum
{
	kSchemaTableNameIndex = 0,
	kSchemaTableCreationStmtIndex = 1
};

ColumnDefList s_schemaTableColumnDefList;
const ColumnDefList *s_pSchemaTableColumnDefList = &s_schemaTableColumnDefList;
TupleDesc s_schemaTableTupleDesc;

//Index schema

const std::string kSchemaIndexName = "PD_Index";

enum
{
	kSchemaIndexNameIndex = 0,
	kSchemaIndexTableNameIndex = 1,
	kSchemaIndexCreationStmtIndex = 2
};

ColumnDefList s_schemaIndexColumnDefList;
const ColumnDefList *s_pSchemaIndexColumnDefList = &s_schemaIndexColumnDefList;
TupleDesc s_schemaIndexTupleDesc;

DBImpl::DBImpl(StorageType inStorageType)
:
mStorageType(inStorageType)
,mpBackend(NULL)
,mIsOpen(false)
{
	//TODO: Lock

	if (!s_initSystemTable)
	{
		ColumnDef schemaColumn;
		TupleDescElement tupleDescElement;
		uint32_t index = 0;

		//table_name;
		PDASSERT(index == kSchemaTableNameIndex);
		schemaColumn.qualifiedName.tableName = kSchemaTableName;
		schemaColumn.qualifiedName.columnName = "table_name";
		schemaColumn.index = index++;
		schemaColumn.dataType = kText;
		schemaColumn.constraintType = kConstraintNone;
		s_schemaTableColumnDefList.push_back(schemaColumn);

		tupleDescElement.mDataType = schemaColumn.dataType;
		s_schemaTableTupleDesc.push_back(tupleDescElement);

		//create_stmt
		PDASSERT(index == kSchemaTableCreationStmtIndex);
		schemaColumn.qualifiedName.tableName = kSchemaTableName;
		schemaColumn.qualifiedName.columnName = "create_stmt";
		schemaColumn.index = index++;
		schemaColumn.dataType = kText;
		schemaColumn.constraintType = kConstraintNone;
		s_schemaTableColumnDefList.push_back(schemaColumn);

		tupleDescElement.mDataType = schemaColumn.dataType;
		s_schemaTableTupleDesc.push_back(tupleDescElement);


		index = 0;

		//index_name;
		PDASSERT(index == kSchemaIndexNameIndex);
		schemaColumn.qualifiedName.tableName = kSchemaIndexName;
		schemaColumn.qualifiedName.columnName = "index_name";
		schemaColumn.index = index++;
		schemaColumn.dataType = kText;
		schemaColumn.constraintType = kConstraintNone;
		s_schemaIndexColumnDefList.push_back(schemaColumn);

		tupleDescElement.mDataType = schemaColumn.dataType;
		s_schemaIndexTupleDesc.push_back(tupleDescElement);

		//table_name
		PDASSERT(index == kSchemaIndexTableNameIndex);
		schemaColumn.qualifiedName.tableName = kSchemaIndexName;
		schemaColumn.qualifiedName.columnName = "table_name";
		schemaColumn.index = index++;
		schemaColumn.dataType = kText;
		schemaColumn.constraintType = kConstraintNone;
		s_schemaIndexColumnDefList.push_back(schemaColumn);

		tupleDescElement.mDataType = schemaColumn.dataType;
		s_schemaIndexTupleDesc.push_back(tupleDescElement);

		//create_index_stmt
		PDASSERT(index == kSchemaIndexCreationStmtIndex);
		schemaColumn.qualifiedName.tableName = kSchemaIndexName;
		schemaColumn.qualifiedName.columnName = "create_index_stmt";
		schemaColumn.index = index++;
		schemaColumn.dataType = kText;
		schemaColumn.constraintType = kConstraintNone;
		s_schemaIndexColumnDefList.push_back(schemaColumn);

		tupleDescElement.mDataType = schemaColumn.dataType;
		s_schemaIndexTupleDesc.push_back(tupleDescElement);

		s_initSystemTable = true;
	}
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

		if (result.OK())
		{
			result = mpBackend->OpenTable(kSchemaTableName, IDBBackend::kCreate);

			if (result.OK())
			{
				this->AddTable_Private(kSchemaTableName, *s_pSchemaTableColumnDefList);

				TupleDesc tupleDesc;

				ColumnDefListToTupleDesc(*s_pSchemaTableColumnDefList, &tupleDesc);

				TupleIterator *schemaTupleIter = mpBackend->CreateScanIterator(kSchemaTableName, tupleDesc);

				if (schemaTupleIter)
				{				
					ValueList tableInfoValueList;

					schemaTupleIter->Reset();

					while(schemaTupleIter->Next())
					{
						if (schemaTupleIter->GetValue(&tableInfoValueList))
						{
							result = this->OpenTableWithCreationStmt_Private(tableInfoValueList[kSchemaTableCreationStmtIndex].GetAsString());

							if (!result.OK())
							{
								break;
							}
						}
					}

					delete schemaTupleIter;
				}	
			}

			if (result.OK())
			{
				result = mpBackend->OpenTable(kSchemaIndexName, IDBBackend::kCreate);

				if (result.OK())
				{
					this->AddTable_Private(kSchemaIndexName, *s_pSchemaIndexColumnDefList);

					TupleDesc tupleDesc;

					ColumnDefListToTupleDesc(*s_pSchemaIndexColumnDefList, &tupleDesc);

					TupleIterator *schemaTupleIter = mpBackend->CreateScanIterator(kSchemaIndexName, tupleDesc);

					if (schemaTupleIter)
					{				
						ValueList indexInfoValueList;

						schemaTupleIter->Reset();

						while(schemaTupleIter->Next())
						{
							if (schemaTupleIter->GetValue(&indexInfoValueList))
							{
								result = this->OpenIndexWithCreationStmt_Private(indexInfoValueList[kSchemaIndexCreationStmtIndex].GetAsString());

								if (!result.OK())
								{
									break;
								}
							}
						}

						delete schemaTupleIter;
					}	
				}
			}

			if (result.OK())
			{
				mIsOpen = true;
			}
			else
			{
				this->Close_Private(true);
			}
		}
	}

	return result;
}

Status DBImpl::Close()
{
	return this->Close_Private(false);
}

Status DBImpl::CreateOpenTable(const std::string &tableName, const ColumnDefList &columnList, const std::string &creationStmt)
{
	Status result;

	const Table *pTable = mTableCatalog.GetTableByName(tableName);

	if (pTable == NULL)
	{
		IDBBackend::OpenMode openMode = IDBBackend::kCreate | IDBBackend::kErrorIfExists;

		result = mpBackend->OpenTable(tableName, openMode);

		if (result.OK())
		{
			ValueList tupleValueList;
			Value oneValue;

			oneValue.SetAsString(tableName);
			tupleValueList.push_back(oneValue);

			oneValue.SetAsString(creationStmt);
			tupleValueList.push_back(oneValue);

			//Insert this new table info into schema table
			result = mpBackend->InsertData(kSchemaTableName, s_schemaTableTupleDesc, tupleValueList);
		
			if (result.OK())
			{
				this->AddTable_Private(tableName, columnList);
			}
		}		
	}
	else
	{
		result = Status::kTableAlreadyExists;
	}

	return result;
}

Status DBImpl::OpenTable(const std::string &tableName, const ColumnDefList &columnList)
{
	Status result;

	IDBBackend::OpenMode openMode = 0;

	result = mpBackend->OpenTable(tableName, openMode);

	if (result.OK())
	{
		this->AddTable_Private(tableName, columnList);
	}
	
	return result;
}

Status DBImpl::DropTable(const std::string &tableName)
{
	//TODO: Make whole method atomic
	Status result;

	const Table *pTable = NULL;

	result = this->GetTableByName(tableName, &pTable);

	//Remove the entry from schema table
	if (result.OK())
	{
		const Table *pSchemaTable = NULL;

		result = this->GetTableByName(kSchemaTableName, &pSchemaTable);

		if (result.OK())
		{
			TupleIterator *pTupleIterator = this->CreateTupleIteratorForTable(*pSchemaTable, s_schemaTableTupleDesc);

			result = pTupleIterator->GetLastError();

			if (result.OK())
			{
				while (pTupleIterator->Next())
				{
					ValueList theValueList;
					if (pTupleIterator->GetValue(&theValueList))
					{
						if (theValueList[kSchemaTableNameIndex].GetAsString() == tableName)
						{
							if (!pTupleIterator->Remove())
							{
								result = Status::kInternalError;
							}

							break;
						}
					}
				}

				if (!pTupleIterator->Valid())
				{
					//We should find a matching item
					PDASSERT(0);

					result = Status::kInternalError;
				}
			}

			delete pTupleIterator;
		}
	}

	//Drop all indexes for the table
	if (result.OK())
	{
		std::vector<std::string> indexNameList;
		mIndexCatalog.GetIndexNameListForTable(tableName, &indexNameList);

		std::vector<std::string>::const_iterator indexNameIter = indexNameList.begin();

		for (; indexNameIter != indexNameList.end(); indexNameIter++)
		{
			result = this->DropIndex(*indexNameIter, tableName);

			if (!result.OK())
			{
				break;
			}
		}
	}

	//Close the table
	if (result.OK())
	{
		result = mpBackend->CloseTable(tableName);
	}

	//Drop the table
	if (result.OK())
	{
		result = mpBackend->DropTable(tableName);
	}	

	if (result.OK())
	{
		mTableCatalog.RemoveTable(tableName);		
	}

	return result;
}

Status DBImpl::CreateOpenIndex(const std::string &indexName, const std::string &tableName, const ColumnDefList &columnList, bool isUnique, const std::string &creationStmt)
{
	Status result;

	const Table *pTable = NULL;

	result = this->GetTableByName(tableName, &pTable);

	if (result.OK())
	{
		TupleDesc tupleDesc;
		const ColumnDefList &columnDefList = pTable->GetAllColumns();

		ColumnDefListToTupleDesc(columnDefList, &tupleDesc); 
		
		std::vector<int32_t> indexList;

		for (ColumnDefList::const_iterator iter = columnList.begin()
			; iter != columnList.end()
			; iter++)
		{
			indexList.push_back(iter->index);
		}

		IDBBackend::OpenMode openMode = IDBBackend::kCreate | IDBBackend::kErrorIfExists;

		result = mpBackend->OpenIndex(indexName, tableName, tupleDesc, indexList, isUnique, openMode);

		if (result.OK())
		{
			ValueList tupleValueList;
			Value oneValue;

			oneValue.SetAsString(indexName);
			tupleValueList.push_back(oneValue);

			oneValue.SetAsString(tableName);
			tupleValueList.push_back(oneValue);

			oneValue.SetAsString(creationStmt);
			tupleValueList.push_back(oneValue);

			//Insert this new index info into schema table
			result = mpBackend->InsertData(kSchemaIndexName, s_schemaIndexTupleDesc, tupleValueList);

			if (result.OK())
			{
				this->AddIndex_Private(indexName, tableName, indexList, isUnique);
			}
		}
	}
	else
	{
		result = Status::kTableMissing;
	}

	return result;
}

Status DBImpl::OpenIndex(const std::string &indexName, const std::string &tableName, const ColumnDefList &columnList, bool isUnique)
{
	Status result;

	const Table *pTable = NULL;

	result = this->GetTableByName(tableName, &pTable);

	if (result.OK())
	{
		TupleDesc tupleDesc;
		const ColumnDefList &columnDefList = pTable->GetAllColumns();

		ColumnDefListToTupleDesc(columnDefList, &tupleDesc); 

		std::vector<int32_t> indexList;

		for (ColumnDefList::const_iterator iter = columnList.begin()
			; iter != columnList.end()
			; iter++)
		{
			indexList.push_back(iter->index);
		}

		IDBBackend::OpenMode openMode = 0;

		result = mpBackend->OpenIndex(indexName, tableName, tupleDesc, indexList, isUnique, openMode);

		if (result.OK())
		{
			this->AddIndex_Private(indexName, tableName, indexList, isUnique);
		}
	}
	else
	{
		result = Status::kTableMissing;
	}
	
	return result;
}

Status DBImpl::DropIndex(const std::string &indexName, const std::string &tableName)
{
	Status result;
	const Table *pTable = NULL;

	result = this->GetTableByName(tableName, &pTable);

	//Remove the entry from schema table
	if (result.OK())
	{
		const Table *pSchemaIndexTable = NULL;

		result = this->GetTableByName(kSchemaIndexName, &pSchemaIndexTable);

		if (result.OK())
		{
			TupleIterator *pTupleIterator = this->CreateTupleIteratorForTable(*pSchemaIndexTable, s_schemaIndexTupleDesc);

			result = pTupleIterator->GetLastError();

			if (result.OK())
			{
				while (pTupleIterator->Next())
				{
					ValueList theValueList;
					if (pTupleIterator->GetValue(&theValueList))
					{
						if (theValueList[kSchemaIndexNameIndex].GetAsString() == indexName
							&& theValueList[kSchemaIndexTableNameIndex].GetAsString() == tableName)
						{
							if (!pTupleIterator->Remove())
							{
								result = Status::kInternalError;
							}

							break;
						}
					}
				}

				if (!pTupleIterator->Valid())
				{
					//We should find a matching item
					PDASSERT(0);

					result = Status::kInternalError;
				}
			}

			delete pTupleIterator;
		}
	}

	//Drop the index
	if (result.OK())
	{
		result = mpBackend->DropIndex(indexName, tableName);
	}	

	if (result.OK())
	{
		mIndexCatalog.RemoveIndex(indexName, tableName);		
	}

	return result;
}

Status DBImpl::InsertData(const std::string &tableName, const ColumnDefList &columnList, const ExprList &columnExprList)
{
	Status result;

	TupleDesc tupleDesc;
	ValueList tupleValueList;

	ColumnDefListToTupleDesc(columnList, &tupleDesc);

	//For now, columnExprList should be constant.
	//And we provide empty context here.
	ExprContext exprContext;
	Expr::EvalExprList(columnExprList, exprContext, &tupleValueList);
	
	result = mpBackend->InsertData(tableName, tupleDesc, tupleValueList);

	return result;
}

struct DBImpl_UpdateContext
{
	DBImpl_UpdateContext(const ColumnDefList &inColumnDefList, const ExprList &inColumnExprList);
	const ColumnDefList &columnList;
	const ExprList &columnExprList;
};

DBImpl_UpdateContext::DBImpl_UpdateContext(const ColumnDefList &inColumnDefList, const ExprList &inColumnExprList)
:
columnList(inColumnDefList)
,columnExprList(inColumnExprList)
{
}

Status UpdateCurrent(TupleIterator *io_iterator, void *io_ctx)
{
	Status result;

	DBImpl_UpdateContext *ctx = (DBImpl_UpdateContext *)io_ctx;
	const ColumnDefList &columnList = ctx->columnList;
	const ExprList &columnExprList = ctx->columnExprList;

	ValueList newValueList;

	if (io_iterator->GetValue(&newValueList))
	{
		//For now, columnExprList should be constant.
		//And we provide empty context here.
		ExprContext exprContext;
		Value newColumnValue;

		ColumnDefList::const_iterator columnIter = columnList.begin();

		size_t index = 0;
		for (; columnIter != columnList.end(); columnIter++)
		{
			result = columnExprList[index]->Eval(exprContext, &newColumnValue);

			if (!result.OK())
			{
				break;
			}

			newValueList[columnIter->index] = newColumnValue;
			index++;
		}

		if (result.OK())
		{
			if (!io_iterator->Update(newValueList))
			{
				result = Status::kInternalError;
			}
		}
	}
	else
	{
		result = Status::kInternalError;
	}

	return result;
}

Status DeleteCurrent(TupleIterator *io_iterator, void *io_ctx)
{
	Status result;

	if (!io_iterator->Remove())
	{
		result = Status::kInternalError;
	}

	return result;
}

Status DBImpl::UpdateData(const std::string &tableName, const ColumnDefList &columnList, const ExprList &columnExprList, const BooleanExpr *inPredicateExpr /* = NULL */)
{
	Status result;

	DBImpl_UpdateContext context(columnList, columnExprList);

	result = this->PerformIterator_Private(tableName, inPredicateExpr, UpdateCurrent, &context);

	return result;
}

Status DBImpl::DeleteData(const std::string &tableName, const BooleanExpr *inPredicateExpr /* = NULL */)
{
	Status result;

	result = this->PerformIterator_Private(tableName, inPredicateExpr, DeleteCurrent, NULL);

	return result;
}

Status DBImpl::PerformIterator_Private(const std::string &tableName, const BooleanExpr *inPredicateExpr, PerformIterator performer, void *io_ctx)
{
	PDASSERT(performer);

	Status result;

	const Table *pTable = NULL;

	result = this->GetTableByName(tableName, &pTable);

	if (result.OK())
	{
		TupleDesc tupleDesc;
		const ColumnDefList &columnDefList = pTable->GetAllColumns();

		ColumnDefListToTupleDesc(columnDefList, &tupleDesc); 

		TupleIterator *pTupleIterator = this->CreateTupleIteratorForTable(*pTable, tupleDesc);

		result = pTupleIterator->GetLastError();

		if (result.OK())
		{
			ExprContext exprContext;

			bool matchPredicate = false;

			while (pTupleIterator->Next())
			{
				matchPredicate = (inPredicateExpr == NULL);

				if (!matchPredicate)
				{
					ValueList theValueList;
					if (pTupleIterator->GetValue(&theValueList))
					{
						exprContext.UpdateTupleValue(columnDefList, theValueList);

						matchPredicate = inPredicateExpr->IsTrue(exprContext);
					}
				}

				if (matchPredicate)
				{
					result = performer(pTupleIterator, io_ctx);
					
					if (!result.OK())
					{
						break;
					}
				}
			}
		}

		delete pTupleIterator;
	}

	return result;
}

Status DBImpl::SelectData(const Table::TableRefList &tableList, const JoinList &joinList, const ColumnDefList &projectColumnList, const BooleanExpr *inWhereExpr /*= NULL*/)
{
	Status result;

//	if (tableList.size() == 1)
//	{
//		Table *theTable = NULL;
//
//		result = this->GetTableByName(tableList[0], &theTable);
//   
//		if (result.OK())
//		{
//			std::cout << "****** Select Table:" << tableList[0] << " ******" << std::endl;
//			//result = mpBackend->SelectData(tableList[0], columnList, inTuplePredicate);
//			const ColumnDefList &allColumnList = theTable->GetAllColumns();
//			TupleDesc tupleDesc;
//			//TupleDesc projectTupleDesc;
//
//			ColumnDefListToTupleDesc(allColumnList, &tupleDesc);
//			//ColumnDefListToTupleDesc(projectColumnList, &projectTupleDesc);
//			
//			TupleIterator *theIter = mpBackend->CreateScanIterator(tableList[0], tupleDesc, NULL);
//			
//			ValueList tupleValue;
//			ValueList projectTupleValue;
//			ExprContext exprContext;
//
//			while (theIter->Valid())
//			{
//				if (!theIter->GetValue(&tupleValue))
//				{
//					break;
//				}
//
//				exprContext.UpdateTupleValue(allColumnList, tupleValue);
//
//				if (!inWhereExpr || inWhereExpr->IsTrue(exprContext))
//				{
//					//TODO: This is slow when doing every time
//					ProjectTuple(allColumnList, projectColumnList, tupleValue, &projectTupleValue);			
//
//#ifdef PDDEBUG
//					PrintTuple(projectTupleValue);
//#endif
//				}
//
//				theIter->Next();
//			}
//
//			delete theIter;	
//		}
//	}
//	else if (tableList.size() == 2)
//	{
//		Table *outerTable = NULL;
//		Table *innerTable = NULL;
//
//		result = this->GetTableByName(tableList[0], &outerTable);
//
//		if (result.OK())
//		{
//			result = this->GetTableByName(tableList[1], &innerTable);
//
//			if (result.OK())
//			{
//				const ColumnDefList &outerTableAllColumnList = outerTable->GetAllColumns();
//				const ColumnDefList &innerTableAllColumnList = innerTable->GetAllColumns();
//				ColumnDefList allColumnList = outerTableAllColumnList;
//				allColumnList.insert(allColumnList.end(), innerTableAllColumnList.begin(), innerTableAllColumnList.end());
//
//				TupleIterator *outerScan = mpBackend->CreateScanIterator(tableList[0], NULL);
//
//				PDASSERT(outerScan);
//
//				ValueList projectTupleValue;
//				ExprContext exprContext;
//				while (outerScan->Valid())
//				{
//					ValueList outerTupleValue;
//					ValueList outerProjectTupleValue;
//
//					if (!outerScan->GetValue(&outerTupleValue))
//					{
//						break;
//					}
//
//					exprContext.UpdateTupleValue(outerTableAllColumnList, outerTupleValue);
//
//					TupleIterator *innerScan = mpBackend->CreateScanIterator(tableList[1], NULL);
//
//					PDASSERT(innerScan);
//
//					while (innerScan->Valid())
//					{
//						ValueList innerTupleValue;
//						ValueList innerProjectTupleValue;
//
//						if (!innerScan->GetValue(&innerTupleValue))
//						{
//							break;
//						}
//
//						exprContext.UpdateTupleValue(innerTableAllColumnList, innerTupleValue);
//
//						if (!inWhereExpr || inWhereExpr->IsTrue(exprContext))
//						{
//							//TODO: This is slow when doing every time
//							ValueList allTupleValue = outerTupleValue;
//							allTupleValue.insert(allTupleValue.end(), innerTupleValue.begin(), innerTupleValue.end());
//
//							ProjectTuple(allColumnList, projectColumnList, allTupleValue, &projectTupleValue);		
//#ifdef PDDEBUG
//							PrintTuple(projectTupleValue);
//#endif
//						}
//
//						innerScan->Next();
//					}
//
//					delete innerScan;
//
//					if (!result.OK())
//					{
//						break;
//					}
//
//					outerScan->Next();
//				}
//
//				delete outerScan;
//			}
//		}
//	}
//	else
//	{
//		PDASSERT(0);
//	}

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
			const Table *theTable;

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
				const Table *theTable;

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

TupleIterator* DBImpl::CreateTupleIteratorForTable(const Table &inTable, const TupleDesc &inTupleDesc)
{
	const ColumnDefList &allColumnList = inTable.GetAllColumns();

	TupleIterator *theIter = mpBackend->CreateScanIterator(inTable.GetName(), inTupleDesc, NULL);

	return theIter;
}

Status DBImpl::GetTableByName(const std::string &tableName, const Table **o_table) const
{
	Status result;

	*o_table = mTableCatalog.GetTableByName(tableName);

	if (*o_table == NULL)
	{
		result = Status::kTableMissing;
	}

	return result;
}

Status DBImpl::Close_Private(bool forceClose)
{
	Status result;

	if (!forceClose)
	{
		PDASSERT(mIsOpen);
	}

	result = mpBackend->Close();

	PDASSERT(result.OK());

	delete mpBackend;
	mpBackend = NULL;

	return result;
}

Status DBImpl::OpenTableWithCreationStmt_Private(const std::string &inCreationStmt)
{
	Status result;

	ParserDriver parserDriver(this);
	Statement *pStmt = NULL;

	result = parserDriver.ParseQuery(inCreationStmt, &pStmt);

	if (result.OK())
	{
		//We use inCreationStmt to open table only
		result = pStmt->Execute(false); //createTableOrIndex = false;
		
		delete pStmt;
	}

	return result;
}

void DBImpl::AddTable_Private(const std::string &tableName, const ColumnDefList &columnList)
{		
	Table *pTable = new Table();
	pTable->SetName(tableName);

	ColumnDefList::const_iterator colIter = columnList.begin();

	for (; colIter != columnList.end(); colIter++)
	{		
		pTable->AddColumnDef(*colIter);
	}

	mTableCatalog.AddTable(tableName, pTable);
}

Status	DBImpl::OpenIndexWithCreationStmt_Private(const std::string &inCreationStmt)
{	
	Status result;

	ParserDriver parserDriver(this);
	Statement *pStmt = NULL;

	result = parserDriver.ParseQuery(inCreationStmt, &pStmt);

	if (result.OK())
	{
		//We use inCreationStmt to open index only
		result = pStmt->Execute(false); //createTableOrIndex = false;
		
		delete pStmt;
	}

	return result;
}

void DBImpl::AddIndex_Private(const std::string &indexName, const std::string &tableName, std::vector<int32_t> indexList, bool isUnique)
{
	Index *pIndex = new Index();
	pIndex->SetIndexName(indexName);
	pIndex->SetTableName(tableName);
	pIndex->SetIndexList(indexList);
	pIndex->SetIsUnique(isUnique);

	mIndexCatalog.AddIndex(indexName, tableName, pIndex);
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
