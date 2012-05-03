#ifndef PANDASQL_FILE_H
#define PANDASQL_FILE_H

#include "Utils/Status.h"
#include "Utils/Types.h"

namespace PandaSQL
{

class File
{
public:

#ifdef PANDASQL_64
	typedef uint64_t Offset;
	typedef uint64_t Size;
#else
	typedef uint32_t Offset;
	typedef uint32_t Size;
#endif

	File();
	virtual ~File() = 0 {}

	virtual Status Read(File::Offset offset, File::Size amount, void *o_buf, File::Size *o_bytesRead) = 0;
	virtual Status Write(File::Offset offset, File::Size amount, const void *inBuf, File::Size *o_bytesWritten) = 0;
	virtual Status WriteAppend(File::Size amount, const void *inBuf, File::Size *o_bytesWritten) = 0;
	virtual Status Flush() = 0;

private:
	File(const File &rhs);
	File& operator=(const File &rhs);
};

}	// PandaSQL

#endif	// PANDASQL_WINVFS_H