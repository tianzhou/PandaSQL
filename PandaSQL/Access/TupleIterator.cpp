#include "stdafx.h"

#include "Access/TupleIterator.h"

#include "Utils/Debug.h"

namespace PandaSQL
{

TupleIterator::TupleIterator()
:
mpTupleFunctor(NULL)
,mLastError(Status::kOK)
{
}

}	// PandaSQL