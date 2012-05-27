#include "stdafx.h"

#include "WinFile.h"

#include <io.h>
#include <sys/stat.h>

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

Status WinFile::ReadToDelimiter(File::Offset offset, File::Size amount, const char *delimiter, bool includeDelimiter, void *o_buf, File::Size *o_bytesRead)
{
	Status result;

	*o_bytesRead = 0;

	File::Size bytesRead = 0;

	result = this->Read(offset, amount, o_buf, &bytesRead);

	if (result.OK() || result.IsEOF())
	{
		//TODO: This one is not optimal, but we have to construct a null terminator
		//to be used by strstr
		std::string theStr((const char *)o_buf, bytesRead);
		const char *pch = strstr(theStr.c_str(), delimiter);

		if (pch)
		{
			*o_bytesRead = pch - theStr.c_str();

			if (includeDelimiter)
			{
				*o_bytesRead += strlen(delimiter);
			}

			//We want the next read starts after the separator
			if (result.IsEOF()
				&&*o_bytesRead < bytesRead)
			{
				result = Status::kOK;
			}
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

Status WinFile::Append(File::Size amount, const void *inBuf, File::Size *o_bytesWritten)
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

Status WinFile::GetSize(File::Size *o_size)
{
	Status result;
	struct stat stbuf;

	int fd = _fileno(mpFile);
	
	if (fstat(fd, &stbuf) == -1)
	{
		result = Status::kIOError;
	}
	else
	{
		*o_size = stbuf.st_size;
	}

	return result;
}


}	// FileSystem