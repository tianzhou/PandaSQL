#include "stdafx.h"

#include "Iterator.h"

namespace PandaSQL
{

Iterator::Iterator(const TuplePredicate *inTuplePredicate)
:
mpTuplePredicate(inTuplePredicate)
{
}

}	// PandaSQL