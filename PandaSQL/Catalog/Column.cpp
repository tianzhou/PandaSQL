#include "stdafx.h"

#include "Column.h"


namespace PandaSQL
{
	
ColumnDef::ColumnDef()
:
qualifiedName()
,index(kInvalidColumnIndex)
,dataType(kUnknownType)
,constraintType(kConstraintNone)
{
	
}

}	// PandaSQL
