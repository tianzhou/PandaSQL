#include "stdafx.h"

#include "Page.h"

namespace PandaSQL
{

Page::Page()
:
mPageData(NULL)
,mPageNum(INVALID_PAGE_NUM)
,mRefCount(0)
{
}

Page::~Page()
{
}

}	// PandaSQL
