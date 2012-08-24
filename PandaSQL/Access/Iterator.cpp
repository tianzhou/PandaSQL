#include "stdafx.h"

#include "Iterator.h"

namespace PandaSQL
{

Iterator::Iterator(const TupleDesc &inTupleDesc, const TuplePredicate *inTuplePredicate)
:
mpTupleDesc(inTupleDesc)
,mpTuplePredicate(inTuplePredicate)
{
}

}	// PandaSQL