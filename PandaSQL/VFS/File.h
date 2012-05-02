#ifndef PANDASQL_FILE_H
#define PANDASQL_FILE_H

#include "Utils/Status.h"
#include "Utils/Types.h"

namespace PandaSQL
{

class File
{
public:

	File();
	virtual ~File() = 0 {}
	virtual Status Read(PandaSQL::uint64_t offset, PandaSQL::uint64_t amount, void *o_buf, PandaSQL::uint64_t *o_bytesRead) = 0;
	virtual Status Write(PandaSQL::uint64_t offset, PandaSQL::uint64_t amount, const void *inBuf, PandaSQL::uint64_t *o_bytesWritten) = 0;

private:
	File(const File &rhs);
	File& operator=(const File &rhs);
};

}	// PandaSQL

#endif	// PANDASQL_WINVFS_H