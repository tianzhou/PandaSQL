#ifndef PANDASQL_DB_H
#define PANDASQL_DB_H

#include "Utils/Status.h"

#include <vector>

namespace PandaSQL
{

class VFS;
class File;

class DB
{
public:
	
	struct Options
	{
		bool create_if_missing;

		Options();
	};

	DB();
	~DB();
	Status Open(const std::string &inDBPath, const Options &inOptions);
	Status CreateTable(const std::string &inCreateStmt);

private:
	
	DB(const DB &rhs);
	DB& operator=(const DB &rhs);

	VFS	*mpVFS;
	File *mpMainFile;
	File *mpTableFile;
	std::vector<File*> mDataFileList;
	std::vector<File*> mIndexFileList;
};

}	// PandaSQL

#endif	// PANDASQL_DB_H