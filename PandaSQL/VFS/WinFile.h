#ifndef PANDASQL_WINFILE_H
#define PANDASQL_WINFILE_H


#include "File.h"

#include "Utils/Status.h"

#include <iostream>
#include <fstream>

namespace PandaSQL
{

class WinFile : public File
{
public:

	WinFile(std::fstream *inFileStream);
	virtual ~WinFile();

	virtual Status Read(PandaSQL::uint64_t offset, PandaSQL::uint64_t amount, void *o_buf, PandaSQL::uint64_t *o_bytesRead);
	virtual Status Write(PandaSQL::uint64_t offset, PandaSQL::uint64_t amount, const void *inBuf, PandaSQL::uint64_t *o_bytesWritten);

private:
	WinFile(const WinFile &rhs);
	WinFile& operator=(const WinFile &rhs);

	std::fstream *pFileStream;
};

}	// PandaSQL

#endif	// PANDASQL_WINFILE_H