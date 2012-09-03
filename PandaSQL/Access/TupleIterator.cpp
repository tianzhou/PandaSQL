#include "stdafx.h"

#include "TupleIterator.h"

#include "Utils/Debug.h"

namespace PandaSQL
{

TupleIterator::TupleIterator(const ColumnDefList &inColumnDefList)
:
mLastError(Status::kOK)
,mColumnDefList(inColumnDefList)
{
}

}	// PandaSQL