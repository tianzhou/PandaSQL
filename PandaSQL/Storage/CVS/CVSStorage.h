#ifndef PANDASQL_CVSSTORAGE_H
#define PANDASQL_CVSSTORAGE_H

#include "Storage/IStorage.h"

#include <map>

namespace PandaSQL
{

class File;
class HeapStore;

class CVSStorage : public IStorage
{
public:

	CVSStorage(IVFS *io_VFS, const std::string &inRootPath);
	virtual ~CVSStorage();

	virtual Iterator *CreateScanIterator();
	virtual Iterator *CreateIndexIterator();

	virtual void ReleaseScanIterator(Iterator *iter);
	virtual void ReleaseIndexIterator(Iterator *iter); 

	virtual Status OpenTable(const std::string &inTableName, OpenMode inMode);
	virtual Status InsertRecord(const ITuple &inTuple);
	virtual Status FindFirstRecordWithPredicate(const Predicate *inPredicate, Iterator **o_iterator);

private:

	CVSStorage(const CVSStorage &rhs);
	CVSStorage& operator=(const CVSStorage &rhs);

	std::string DataFilePathFromTableName(const std::string &tableName);

	File *mpDataFile;
	OpenMode mDataFileMode;

	HeapStore *mpHeapStore;

	Iterator *mpScanIterator;
};

}	// PandaSQL

#endif	// PANDASQL_CVSSTORAGE_H