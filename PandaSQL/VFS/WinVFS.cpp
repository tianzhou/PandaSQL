#include "stdafx.h"

#include "VFS/WinVFS.h"

#include "VFS/WinFile.h"

#include "Utils/Debug.h"
#include "Utils/WinUtils.h"

namespace PandaSQL
{

WinVFS::WinVFS()
{
}

WinVFS::~WinVFS()
{
}

Status WinVFS::OpenFile(const std::string &inPath, bool create_if_missing, File **file)
{
	Status result;

	if (!this->IsFileExist(inPath))
	{
		if (create_if_missing)
		{
			FILE *newFile = NULL;

			//Create the file
			if (fopen_s(&newFile, inPath.c_str(), "w+b") != 0)
			{
				result = Status::kIOError;
			}
			else
			{
				//Close it, we will open it below
				fclose(newFile);
			}
		}
		else
		{
			result = Status::kIOError;
		}	
	}
	
	if (result.OK())
	{
		FILE *newFile = NULL;

		//Open it in binary mode, otherwise, CR LN is replaced by LN
		if (fopen_s(&newFile, inPath.c_str(), "r+b") != 0)
		{
			result = Status::kIOError;
		}
		else
		{
			WinFile *pFile = new WinFile(newFile);
			*file = pFile;
		}
	}

	return result;
}

Status WinVFS::CloseFile(File *file)
{
	Status result;

	PDASSERT(file);

	//Destructor will close the internal file descryptor
	delete file;

	return result;
}

Status WinVFS::CreateDir(const std::string &inPath)
{
	Status result;

	std::wstring wPath = s2ws(inPath);
	if (::CreateDirectory(wPath.c_str(), NULL) == 0)
	{
		result = Status::kIOError;
	}

	return result;
}

bool WinVFS::IsFileExist(const std::string &inPath)
{
	bool result = true;

	std::wstring wPath = s2ws(inPath);
	if(::GetFileAttributes(wPath.c_str()) == INVALID_FILE_ATTRIBUTES
		&& ::GetLastError() == ERROR_FILE_NOT_FOUND)
	{
		result = false;
	}

	return result;
}

}	// PandaSQL