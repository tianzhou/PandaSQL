#ifndef PANDASQL_WINVFS_H
#define PANDASQL_WINVFS_H

#include "IVFS.h"

#include "Utils/Status.h"

namespace PandaSQL
{

class File;

class WinVFS : public IVFS
{
public:

	WinVFS();
	virtual ~WinVFS();
	virtual Status OpenFile(const std::string &inPath, bool create_if_missing, File **file);
	virtual Status CloseFile(File *file);
	virtual Status CreateDir(const std::string &inPath);
	virtual bool IsFileExist(const std::string &inPath);

private:
	WinVFS(const WinVFS &rhs);
	WinVFS& operator=(const WinVFS &rhs);

	std::string mRootPath;
};

}	// PandaSQL

#endif	// PANDASQL_WINVFS_H