#ifndef PANDASQL_PAGE_H
#define PANDASQL_PAGE_H

#include "Utils/Types.h"
#include "Utils/Status.h"

namespace PandaSQL
{

#define INVALID_PAGE_NUM -1

class Page
{
public:
	Page();
	~Page();

private:
	char *mPageData;
	uint32_t mPageNum;

};

}	// PandaSQL

#endif	// PANDASQL_PAGE_H