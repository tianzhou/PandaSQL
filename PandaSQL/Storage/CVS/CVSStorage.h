#ifndef PANDASQL_CVSSTORAGE_H
#define PANDASQL_CVSSTORAGE_H

#include "Storage/IStorage.h"

#include <map>

namespace PandaSQL
{

class VFS;
class File;
class ITuple;

class CVSStorage : public IStorage
{
public:

	CVSStorage(VFS *io_VFS, const std::string &inRootPath);
	virtual ~CVSStorage();

	virtual Status OpenTable(const std::string &inTableName, OpenMode inMode);
	virtual Status InsertRow(const ITuple &inTuple);

private:

	CVSStorage(const CVSStorage &rhs);
	CVSStorage& operator=(const CVSStorage &rhs);

	std::string DataFilePathFromTableName(const std::string &tableName);

	File *mpDataFile;
	OpenMode mDataFileMode;
};

}	// PandaSQL

#endif	// PANDASQL_CVSSTORAGE_H