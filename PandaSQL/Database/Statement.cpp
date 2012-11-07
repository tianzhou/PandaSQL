#include "stdafx.h"

#include "Database/Statement.h"

#include <iostream>

#include "Database/DBImpl.h"

#include "Executor/Executor.h"

#include "Expr/BooleanExpr.h"
#include "Expr/ExprWalker.h"

#include "Node/PlanNode.h"

#include "Optimizer/Plan/Planner.h"

#include "Utils/Debug.h"

namespace PandaSQL
{

Statement::Statement(StatementType inStmtType, DBImpl *io_pDB)
:
mpDB(io_pDB)
,mStmtType(inStmtType)
,mAllColumns(false)
,mpWhereExpr(NULL)
{
}

Statement::~Statement()
{
	delete mpWhereExpr;
}

void Statement::Clear()
{
	mOrigStmtText = std::string();
	mStmtType = kStmtUnknown;
	mTableRefs.clear();
	mSetExprList.clear();
	mColumnDefs.clear();
	mIndexRef = std::string();
}

void Statement::SetOriginalStmtText(const std::string inStmtText)
{
	mOrigStmtText = inStmtText;

	//Replace line feed with space

	std::string space(" ");
	std::string crlf("\r\n");
	std::string lf("\n");

	size_t pos = 0;
	while((pos = mOrigStmtText.find(crlf, pos)) != std::string::npos)
	{
		mOrigStmtText.replace(pos, crlf.length(), space);
		pos += space.length();
	}

	pos = 0;
	while((pos = mOrigStmtText.find(lf, pos)) != std::string::npos)
	{
		mOrigStmtText.replace(pos, lf.length(), space);
		pos += space.length();
	}
}

void Statement::AddTableRef(const std::string &inTableRef)
{
	mTableRefs.push_back(inTableRef);
}

void Statement::AddExprRef(const Expr &inExpr)
{
	mSetExprList.push_back(inExpr);
}

void Statement::AddColumnWithQualifiedName(const ColumnQualifiedName &inQualifiedName)
{
	ColumnDef theColumnDef;

	Status result = mpDB->GetColumnDefFromQualifiedName(mTableRefs, inQualifiedName, &theColumnDef);
	
	if (result.OK())
	{
		mColumnDefs.push_back(theColumnDef);
	}
}

void Statement::AddColumnDef(const ColumnDef &inDef)
{
	mColumnDefs.push_back(inDef);
}

void Statement::AddAllColumns()
{
	mAllColumns = true;
}

void Statement::SetIndexRef(const std::string &inIndexRef)
{
	mIndexRef = inIndexRef;
}

void Statement::SetPredicate(const Predicate &inPredicate)
{
	mPredicate = inPredicate;
}

Status Statement::Prepare()
{
	Status result;

	if (mStmtType != kStmtUnknown
		&& mStmtType != kStmtEmpty)
	{
		result = mPredicate.Prepare(*mpDB, mTableRefs);
		mPredicate.TransformToCNF();

		if (mpWhereExpr)
		{
			TableAndColumnSetMap validTableAndColumnSetMap;

			Table::TableRefList::const_iterator iter = mTableRefs.begin();
			const Table *pTable;
			for (; iter != mTableRefs.end(); iter++)
			{
				result = mpDB->GetTableByName(*iter, &pTable);

				if (result.OK())
				{
					const ColumnDefList& theColumnDefList = pTable->GetAllColumns();

					ColumnDefList::const_iterator columnIter = theColumnDefList.begin();
					ColumnNameSet columnSet;

					for (; columnIter != theColumnDefList.end(); columnIter++)
					{
						columnSet.insert(columnIter->qualifiedName.columnName);
					}

					validTableAndColumnSetMap.insert(TableAndColumnSetMap::value_type(pTable->GetName(), columnSet));
				}
				else
				{
					break;
				}
			}

			if (result.OK())
			{
				AmendColumnWalker amendColumnWalker(validTableAndColumnSetMap);	
				mpWhereExpr->MutableWalk(&amendColumnWalker);
			}
		}

		if (result.OK())
		{
			if (mAllColumns)
			{
				PDASSERT(mColumnDefs.empty());

				Table::TableRefList::const_iterator iter = mTableRefs.begin();
				const Table *theTable;
				for (; iter != mTableRefs.end(); iter++)
				{
					result = mpDB->GetTableByName(*iter, &theTable);

					if (result.OK())
					{
						const ColumnDefList& theColumnDefList = theTable->GetAllColumns();
						mColumnDefs.insert(mColumnDefs.end(), theColumnDefList.begin(), theColumnDefList.end());
					}
					else
					{
						break;
					}
				}
			}
			else
			{
				if (mStmtType == kStmtCreateTable)
				{
					ColumnDefList::iterator iter = mColumnDefs.begin();
					uint32_t columnIndex = 0;

					for (; iter != mColumnDefs.end(); iter++)
					{
						iter->qualifiedName.tableName = mTableRefs[0];
						iter->index = columnIndex;
						columnIndex++;
					}
				}
				else
				{
					ColumnDefList::iterator iter = mColumnDefs.begin();

					for (; iter != mColumnDefs.end(); iter++)
					{	
						//result = mpDB->AmendColumnDef(mTableRefs, &(*iter));				
					}
				}
			}
		}

		this->PrintStatement();

		if (!result.OK())
		{
			std::cout << "Error:" << result.GetCode() << std::endl;
		}
	}

	return result;
}

Status Statement::Execute(bool createTable /* = true */)
{
	Status result;

	switch (mStmtType)
	{
	case kStmtCreateTable:
		{
			if (createTable)
			{
				//It will call Statement::Execute again and call mpDB->OpenTable
				//to actually open the table
				result = mpDB->CreateOpenTable(mTableRefs[0], mColumnDefs, mOrigStmtText);
			}
			else
			{
				result = mpDB->OpenTable(mTableRefs[0], mColumnDefs);				
			}

			break;
		}
	case kStmtDropTable:
		{
			result = mpDB->DropTable(mTableRefs[0]);
			break;
		}
	case kStmtInsert:
		{
			result = mpDB->InsertData(mTableRefs[0], mColumnDefs, mSetExprList);
			break;
		}
	case kStmtSelect:
		{
			Planner thePlanner(*this, mpDB);

			PlanNode *thePlan = thePlanner.GeneratePlan();

			Executor theExecutor;
			result = theExecutor.ExecutePlan(thePlan);

			delete thePlan;
			break;
		}
	case kStmtDelete:
		{
			result = mpDB->DeleteData(mTableRefs[0], mpWhereExpr);
			break;
		}

	default:
		break;
	}

	return result;
}

void Statement::PrintStatement()
{
	mPredicate.Print(0);

	std::cout << "*********ColumnDef*********" << std::endl;

	for (size_t i = 0; i < mColumnDefs.size(); i++)
	{
		std::cout << "Column " << i << ": " << mColumnDefs[i].qualifiedName.tableName << "." << mColumnDefs[i].qualifiedName.columnName << std::endl;
	}
}

void Statement::SetWhereClauseExpression(BooleanExpr *inWhereExpr)
{
	mpWhereExpr = inWhereExpr;
}


}	// PandaSQL