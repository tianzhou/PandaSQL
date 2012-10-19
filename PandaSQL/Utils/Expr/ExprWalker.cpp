#include "stdafx.h"

#include "ExprWalker.h"

#include "ColumnExpr.h"

#include "Utils/Debug.h"

namespace PandaSQL
{

//***************************************************
//***********DependentColumnListWalker***************
//***************************************************
DependentColumnListWalker::DependentColumnListWalker(TableAndColumnSetMap *io_tableAndColumnSetMap)
:
pTableAndColumnSetMap(io_tableAndColumnSetMap)
{
}

DependentColumnListWalker::~DependentColumnListWalker()
{
}

void DependentColumnListWalker::Visit(const Expr *io_expr)
{
	switch (io_expr->GetType())
	{
	case Expr::kExprColumnRef:
		{
			const ColumnExpr *pColExpr = dynamic_cast<const ColumnExpr *>(io_expr);

			PDASSERT(pColExpr);

			if (pColExpr)
			{
				AddOneColumnToMap(pColExpr->GetQualifiedColumnName(), pTableAndColumnSetMap);
			}
			break;
		}
	default:
		break;
	}
}


}	// PandaSQL