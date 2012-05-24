#include "stdafx.h"

#include "Page.h"

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
