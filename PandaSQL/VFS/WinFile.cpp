#include "stdafx.h"

#include "WinFile.h"

#include <io.h>

namespace PandaSQL
{

WinFile::WinFile(FILE *io_pFile)
:mpFile(io_pFile)
{
}

WinFile::~WinFile()
{
	if (mpFile)
	{
		fclose(mpFile);
	}
}

Status WinFile::Read(File::Offset offset, File::Size amount, void *o_buf, File::Size *o_bytesRead)
{
	Status result;

	if (fseek(mpFile, offset, SEEK_SET) != 0)
	{
		result = Status::kIOError;
	}
	else
	{
		File::Size bytesRead;
		bytesRead = fread(o_buf, 1, amount, mpFile);

		if (bytesRead != amount)
		{
			if (feof(mpFile))
			{
				result = Status::kEOF;
			}
			else
			{
				result = Status::kIOError;
			}
		}

		if (o_bytesRead)
		{
			*o_bytesRead = bytesRead;
		}
	}

	return result;
}

Status WinFile::Write(File::Offset offset, File::Size amount, const void *inBuf, File::Size *o_bytesWritten)
{
	Status result;

	if (fseek(mpFile, offset, SEEK_SET) != 0)
	{
		result = Status::kIOError;
	}
	else
	{
		File::Size bytesWritten = fwrite(inBuf, 1, amount, mpFile);

		if (bytesWritten != amount)
		{
			result = Status::kIOError;
		}

		if (o_bytesWritten)
		{
			*o_bytesWritten = bytesWritten;
		}
	}

	return result;
}

Status WinFile::WriteAppend(File::Size amount, const void *inBuf, File::Size *o_bytesWritten)
{
	Status result;

	if (fseek(mpFile, 0, SEEK_SET) != 0)
	{
		result = Status::kIOError;
	}
	else
	{
		File::Size bytesWritten = fwrite(inBuf, 1, amount, mpFile);

		if (bytesWritten != amount)
		{
			result = Status::kIOError;
		}

		if (o_bytesWritten)
		{
			*o_bytesWritten = bytesWritten;
		}
	}

	return result;
}

Status WinFile::Flush()
{
	Status result;

	//From file buffer to os buffer
	if (fflush(mpFile) != 0)
	{
		result = Status::kIOError;
	}

	//From os buffer to disk
	int fd = _fileno(mpFile);
	if (_commit(fd) != 0)
	{
		result = Status::kIOError;
	}


	return result;
}

}	// FileSystem