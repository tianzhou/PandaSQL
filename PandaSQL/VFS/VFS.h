#ifndef PANDASQL_VFS_H
#define PANDASQL_VFS_H

#include "Utils/Status.h"

namespace PandaSQL
{

class File;

class VFS
{
public:

	VFS();
	virtual ~VFS() = 0 {}
	virtual Status OpenFile(const std::string &inPath, bool create_if_missing, File **file) = 0;
	virtual Status CloseFile(File *file) = 0;
	virtual Status CreateDir(const std::string &inPath) = 0;
	virtual bool IsFileExist(const std::string &inPath) = 0;

private:
	
	VFS(const VFS &rhs);
	VFS& operator=(const VFS &rhs);
};

}	// PandaSQL

#endif	// PANDASQL_VFS_H