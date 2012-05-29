#include "stdafx.h"

#include "Iterator.h"

namespace PandaSQL
{

Iterator::Iterator(const Predicate *inPredicate)
:
mpPredicate(inPredicate)
{
}

}	// PandaSQL