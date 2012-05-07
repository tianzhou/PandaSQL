#ifndef PANDASQL_HEAPSTORE_H
#define PANDASQL_HEAPSTORE_H

#include "VFS/File.h"

namespace PandaSQL
{

class HeapStore
{
public:

	HeapStore(File *io_hostedFile);
	~HeapStore();

	Status InsertRecord(File::Size length, const void *data);

protected:

	HeapStore(const HeapStore &rhs);
	HeapStore& operator=(const HeapStore &rhs);

	File *mpHostedFile;
};

}	// PandaSQL

#endif	// PANDASQL_HEAPSTORE_H