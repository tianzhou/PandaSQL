#include "stdafx.h"

#include "HeapStore.h"
#include "Storage/Buffer/PageProxy.h"

namespace PandaSQL
{

HeapStore::HeapStore(PageProxy *io_pageProxy)
:mpPageProxy(io_pageProxy)
{
}

HeapStore::~HeapStore()
{
}

Status HeapStore::InsertRecord(File::Size length, const void *data)
{
	//TODO: Hard code
	//Don't exceed page boundary
	PDASSERT(length <= 4096);

	Status result;

	uint32_t pageCount;
	uint32_t pageNum;
	result = mpPageProxy->GetPageCount(&pageCount);

	if (result.OK())
	{
		char *pageData = NULL;
		if (pageCount == 0)
		{
			result = mpPageProxy->NewPage(&pageNum);
		}
		else
		{
			//Get last page, page num is 0 based
			pageNum = pageCount - 1;
		}

		if (result.OK())
		{
			result = mpPageProxy->GetPage(pageNum
				, PageProxy::kPageRead | PageProxy::kPageWrite
				, &pageData);

			char *pch =strchr(pageData, '\0');
			
			if (pch)
			{
				//TODO: Hard code
				if ((File::Size)(pch - pageData) <= 4096 - length)
				{
					memcpy(pch, data, length);
				}
				else
				{
					//TODO: Need concatenate between two pages.
					//No enough space, add a new page
					result = mpPageProxy->NewPage(&pageNum);

					if (result.OK())
					{
						result = mpPageProxy->GetPage(pageNum
							, PageProxy::kPageRead | PageProxy::kPageWrite
							, &pageData);

						if (result.OK())
						{
							memcpy(pageData, data, length);
						}
					}
				}
			}

			if (result.OK())
			{
				mpPageProxy->PutPage(pageNum, true, pageData);
			}
		}
	}

	return result;
}

} // PandaSQL