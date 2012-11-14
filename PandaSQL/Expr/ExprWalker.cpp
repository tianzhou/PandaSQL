#include "stdafx.h"

#include "Expr/ExprWalker.h"

#include "Expr/ColumnExpr.h"

#include "Utils/Debug.h"

namespace PandaSQL
{

//***************************************************
//***********DependentColumnListWalker***************
//***************************************************
DependentColumnListWalker::DependentColumnListWalker(TableAndColumnSetMap *io_tableAndColumnSetMap)
:
mpTableAndColumnSetMap(io_tableAndColumnSetMap)
{
}

DependentColumnListWalker::~DependentColumnListWalker()
{
}

void DependentColumnListWalker::Visit(const Expr *io_expr)
{
	switch (io_expr->GetExprType())
	{
	case Expr::kExprColumnRef:
		{
			const ColumnExpr *pColExpr = dynamic_cast<const ColumnExpr *>(io_expr);

			PDASSERT(pColExpr);

			if (pColExpr)
			{
				AddOneColumnToMap(pColExpr->GetQualifiedColumnName(), mpTableAndColumnSetMap);
			}
			break;
		}
	default:
		break;
	}
}

//***************************************************
//***********DependentColumnListWalker***************
//***************************************************
AmendColumnWalker::AmendColumnWalker(const TableAndColumnSetMap &inValidTableAndColumnSetMap)
:
mTableAndColumnSetMap(inValidTableAndColumnSetMap)
{
}

AmendColumnWalker::~AmendColumnWalker()
{
}

void AmendColumnWalker::Visit(const Expr *io_expr)
{
}

void AmendColumnWalker::MutableVisit(Expr *io_expr)
{
	switch (io_expr->GetExprType())
	{
	case Expr::kExprColumnRef:
		{
			Status result;

			ColumnExpr *pColExpr = dynamic_cast<ColumnExpr *>(io_expr);

			PDASSERT(pColExpr);

			if (pColExpr)
			{
				ColumnQualifiedName qualifiedName = pColExpr->GetQualifiedColumnName();
				if (qualifiedName.tableName.empty())
				{
					bool hasMatch = false;
					std::string ownedTableName;
					TableAndColumnSetMap::const_iterator setIter = mTableAndColumnSetMap.begin();

					for (; setIter != mTableAndColumnSetMap.end(); setIter++)
					{
						ColumnNameSet::const_iterator columnIter = setIter->second.begin();

						for (; columnIter != setIter->second.end(); columnIter++)
						{
							if (*columnIter == qualifiedName.columnName)
							{
								if (hasMatch)
								{
									result = Status::kAmbiguousColumn;
									break;
								}
								else
								{
									ownedTableName = setIter->first;
									hasMatch = true;
								}
							}
						}
					}

					if (result.OK())
					{
						if (hasMatch)
						{
							qualifiedName.tableName = ownedTableName;
							pColExpr->SetQualifiedColumnName(qualifiedName);
						}
					}
				}
			}
			break;
		}
	default:
		break;
	}
}


}	// PandaSQL