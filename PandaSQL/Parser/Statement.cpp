#include "stdafx.h"

#include "Parser/Statement.h"

#include <iostream>

#include "Executor/Executor.h"

#include "Node/PlanNode.h"

#include "Optimizer/Plan/Planner.h"

#include "PandaDB.h"

#include "Utils/Debug.h"

namespace PandaSQL
{

Statement::Statement(PandaDB *io_pDB)
:
mpDB(io_pDB)
,mStmtType(kStmtUnknown)
,mAllColumns(false)
,mpWhereExpr(NULL)
{
}

Statement::~Statement()
{
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

	result = mPredicate.Prepare(*mpDB, mTableRefs);
	mPredicate.TransformToCNF();

	if (mAllColumns)
	{
		PDASSERT(mColumnDefs.empty());

		Table::TableRefList::const_iterator iter = mTableRefs.begin();
		Table *theTable;
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
	
	this->PrintStatement();

	if (!result.OK())
	{
		std::cout << "Error:" << result.GetCode() << std::endl;
	}

	return result;
}

Status Statement::Execute(bool loadTable)
{
	Status result;

	switch (mStmtType)
	{
	case kStmtCreateTable:
		{
			if (loadTable)
			{
				result = mpDB->OpenTable(mTableRefs[0]);
			}
			else
			{
				result = mpDB->CreateTable(mTableRefs[0], mColumnDefs);
			}

			break;
		}
	case kStmtInsert:
		{
			result = mpDB->InsertData(mTableRefs[0], mColumnDefs, mSetExprList);
			break;
		}
	case kStmtSelect:
		{
			//ColumnQualifiedName join1 = {"Master", "master_id"};
			//ColumnQualifiedName join2 = {"Detail", "detail_id"};
			//mJoinList.push_back(join1);
			//mJoinList.push_back(join2);

			Planner thePlanner(*this, mpDB);

			PlanNode *thePlan = thePlanner.GeneratePlan();

			Executor theExecutor;
			result = theExecutor.ExecutePlan(thePlan);

			delete thePlan;

			//TuplePredicate tuplePredicate;
			//result = mpDB->SelectData(mTableRefs, mJoinList, mColumnDefs, mpWhereExpr);
			break;
		}
	case kStmtDelete:
		{
			//Predicate thePredicate;
			//PredicateItem predicateItem;

			//PandaSQL::Expr lExpr;
			//lExpr.type = kExprColumnDef;

			//ColumnQualifiedName qualifiedName = {"", "name"};
			//ColumnDef theDef;
			//theDef.qualifiedName = qualifiedName;
			//theDef.index = kInvalidColumnIndex;
			//theDef.dataType = kText;
			//theDef.constraintType = kConstraintNone;
			//
			//lExpr.columnDef = theDef;

			//PandaSQL::Expr rExpr;
			//rExpr.type = kExprText;
			//rExpr.text = "\"Peter\"";

			//predicateItem.SetFormat(lExpr, rExpr, PredicateItem::kEqual);
			//thePredicate.SetSinglePredicateItem(predicateItem);
			//result = mpDB->DeleteData(mTableRefs[0], &thePredicate);
			result = mpDB->DeleteData(mTableRefs[0], NULL);
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

void Statement::SetWhereClauseExpression(const BooleanExpr *inWhereExpr)
{
	mpWhereExpr = inWhereExpr;
}


}	// PandaSQL