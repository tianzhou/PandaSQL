#ifndef PANDASQL_PAGE_H
#define PANDASQL_PAGE_H

#include "Utils/Types.h"
#include "Utils/Status.h"

namespace PandaSQL
{

#define kInvalidPageNum -1

class Page
{
public:
	Page();
	~Page();

	char *mPageData;
	uint32_t mPageNum;
	uint32_t mRefCount;

};

}	// PandaSQL

#endif	// PANDASQL_PAGE_H