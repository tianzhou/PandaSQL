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

	//result = mpHostedFile->WriteAppend(length, data, NULL);

	return result;
}

} // PandaSQL