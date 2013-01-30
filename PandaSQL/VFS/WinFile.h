#ifndef PANDASQL_WINFILE_H
#define PANDASQL_WINFILE_H


#include "VFS/File.h"

#include "Utils/Status.h"

namespace PandaSQL
{

class WinFile : public File
{
public:

	WinFile(FILE *io_pFile);
	virtual ~WinFile();

	virtual Status Read(File::Offset offset, File::Size amount, void *o_buf, File::Size *o_bytesRead);
	//virtual Status ReadToDelimiter(File::Offset offset, File::Size amount, const char *delimiter, bool includeDelimiter, void *o_buf, File::Size *o_bytesRead);
	virtual Status Write(File::Offset offset, File::Size amount, const void *inBuf, File::Size *o_bytesWritten);
	virtual Status Append(File::Size amount, const void *inBuf, File::Size *o_bytesWritten);
	virtual Status Flush();
	virtual Status GetSize(File::Size *o_size);

private:
	WinFile(const WinFile &rhs);
	WinFile& operator=(const WinFile &rhs);

	FILE *mpFile;
};

}	// PandaSQL

#endif	// PANDASQL_WINFILE_H