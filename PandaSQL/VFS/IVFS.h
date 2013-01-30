#ifndef PANDASQL_IVFS_H
#define PANDASQL_IVFS_H

#include <string>

#include "PandaSQLDef.h"
#include "Utils/Status.h"

namespace PandaSQL
{

class File;

class IVFS
{
public:

	static IVFS* CreateVFS();

	virtual ~IVFS() = 0 {}

	//Always pass in the relative path according to inRootPath
	virtual Status OpenFile(const std::string &inPath, bool create_if_missing, File **file) = 0;
	virtual Status CloseFile(File *file) = 0;
	virtual Status CreateDir(const std::string &inPath) = 0;
	virtual bool IsFileExist(const std::string &inPath) = 0;

protected:

	IVFS();

private:
	
	IVFS(const IVFS &rhs);
	IVFS& operator=(const IVFS &rhs);
};

}	// PandaSQL

#endif	// PANDASQL_IVFS_H