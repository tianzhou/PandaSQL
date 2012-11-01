#include "stdafx.h"

#include "Access/TupleIterator.h"

#include "Utils/Debug.h"

namespace PandaSQL
{

TupleIterator::TupleIterator(const TupleDesc &inTupleDesc)
:
mTupleDesc(inTupleDesc)
,mLastError(Status::kOK)
{
}

}	// PandaSQL