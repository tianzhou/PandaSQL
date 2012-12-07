#include "stdafx.h"

#include "Catalog/Index.h"

namespace PandaSQL
{

Index::Index(void *payload)
:
mPayload(payload)
, mUnique(false)
{
}

Index::~Index()
{
}

}
