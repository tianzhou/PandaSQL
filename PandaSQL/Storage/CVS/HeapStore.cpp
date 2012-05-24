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
	Status result;

	uint32_t pageCount;
	result = mpPageProxy->GetPageCount(&pageCount);

	if (result.OK())
	{
		char *pageData = NULL;
		if (pageCount == 0)
		{
			result = mpPageProxy->NewPage(&pageData);
		}
		else
		{
			//Get last page, page num is 0 based
			result = mpPageProxy->GetPage(pageCount - 1
				, PageProxy::kPageRead | PageProxy::kPageWrite
				, &pageData);

			if (result.OK())
			{

			}
		}
	}
	//result = mpHostedFile->WriteAppend(length, data, NULL);

	return result;
}

} // PandaSQL