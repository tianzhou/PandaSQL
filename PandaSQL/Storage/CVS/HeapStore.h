#ifndef PANDASQL_HEAPSTORE_H
#define PANDASQL_HEAPSTORE_H

#include "VFS/File.h"

namespace PandaSQL
{

class PageProxy;

class HeapStore
{
public:

	HeapStore(PageProxy *io_pageProxy);
	~HeapStore();

	Status InsertRecord(File::Size length, const void *data);

protected:

	HeapStore(const HeapStore &rhs);
	HeapStore& operator=(const HeapStore &rhs);

	PageProxy *mpPageProxy;
};

}	// PandaSQL

#endif	// PANDASQL_HEAPSTORE_H