#include "stdafx.h"

#include "HeapStore.h"

namespace PandaSQL
{

HeapStore::HeapStore(File *io_hostedFile)
:mpHostedFile(io_hostedFile)
{
}

HeapStore::~HeapStore()
{
}

Status HeapStore::InsertRecord(File::Size length, const void *data)
{
	Status result;

	result = mpHostedFile->WriteAppend(length, data, NULL);

	return result;
}

} // PandaSQL