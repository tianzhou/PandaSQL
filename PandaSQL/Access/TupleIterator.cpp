#include "stdafx.h"

#include "Access/TupleIterator.h"

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