#ifndef PANDASQL_PAGE_H
#define PANDASQL_PAGE_H

#include "Utils/Types.h"
#include "Utils/Status.h"

namespace PandaSQL
{

#define PAGE_SIZE 8192

class Page
{
public:
	Page();
	~Page();

private:
	char mPageData[PAGE_SIZE];

};

}	// PandaSQL

#endif	// PANDASQL_PAGE_H