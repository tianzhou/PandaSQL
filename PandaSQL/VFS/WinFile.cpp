#include "stdafx.h"

#include "WinFile.h"

namespace PandaSQL
{

WinFile::WinFile(std::fstream *inFileStream)
:pFileStream(inFileStream)
{
}

WinFile::~WinFile()
{
	if (pFileStream)
	{
		pFileStream->close();
	}

	delete pFileStream;
}

Status WinFile::Read(PandaSQL::uint64_t offset, PandaSQL::uint64_t amount, void *o_buf, PandaSQL::uint64_t *o_bytesRead)
{
	Status result;

	return result;
}

Status WinFile::Write(PandaSQL::uint64_t offset, PandaSQL::uint64_t amount, const void *inBuf, PandaSQL::uint64_t *o_bytesWritten)
{
	Status result;

	return result;
}

}	// FileSystem