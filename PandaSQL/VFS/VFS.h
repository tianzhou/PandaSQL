#ifndef PANDASQL_VFS_H
#define PANDASQL_VFS_H

#include "Utils/Status.h"

#include <string>

namespace PandaSQL
{

class File;

class VFS
{
public:

	VFS();
	virtual ~VFS() = 0 {}

	//Always pass in the relative path according to inRootPath
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