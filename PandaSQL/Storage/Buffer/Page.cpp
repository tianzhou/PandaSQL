#include "stdafx.h"

#include "Storage/Buffer/Page.h"

namespace PandaSQL
{

Page::Page()
:
mPageData(NULL)
,mPageNum(kInvalidPageNum)
,mRefCount(0)
{
}

Page::~Page()
{
}

}	// PandaSQL
