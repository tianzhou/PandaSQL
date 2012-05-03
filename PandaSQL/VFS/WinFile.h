#ifndef PANDASQL_WINFILE_H
#define PANDASQL_WINFILE_H


#include "File.h"

#include "Utils/Status.h"

namespace PandaSQL
{

class WinFile : public File
{
public:

	WinFile(FILE *io_pFile);
	virtual ~WinFile();

	virtual Status Read(File::Offset offset, File::Size amount, void *o_buf, File::Size *o_bytesRead);
	virtual Status Write(File::Offset offset, File::Size amount, const void *inBuf, File::Size *o_bytesWritten);
	virtual Status WriteAppend(File::Size amount, const void *inBuf, File::Size *o_bytesWritten);
	virtual Status Flush();

private:
	WinFile(const WinFile &rhs);
	WinFile& operator=(const WinFile &rhs);

	FILE *mpFile;
};

}	// PandaSQL

#endif	// PANDASQL_WINFILE_H