#include "stdafx.h"

#include "DB.h"
#include "VFS/VFS.h"
#include "VFS/WinVFS.h"
#include "VFS/File.h"

namespace PandaSQL
{

static VFS* create_vfs()
{
	//Only have windows for now
	return new WinVFS();
}

DB::Options::Options()
:
create_if_missing(false)
{
}

DB::DB()
:
mpVFS(create_vfs())
,mpMainFile(NULL)
,mpTableFile(NULL)
{
}

DB::~DB()
{
	mpVFS->CloseFile(mpMainFile);
	mpVFS->CloseFile(mpTableFile);
	delete mpVFS;
}

Status DB::Open(const std::string &inDBPath, const Options &inOptions)
{
	Status result;

	if (!mpVFS->IsFileExist(inDBPath))
	{
		result = mpVFS->CreateDir(inDBPath);	
	}

	bool create_if_missing = inOptions.create_if_missing;

	if (result.OK())
	{
		result = mpVFS->OpenFile(inDBPath + "\\testDB.pdm", create_if_missing, &mpMainFile);

		if (result.OK())
		{
			result = mpVFS->OpenFile(inDBPath + "\\testDB.pdt", create_if_missing, &mpTableFile);
		}
	}

	return result;
}

Status DB::CreateTable(const std::string &inCreateStmt)
{
	Status result;

	std::string theString = inCreateStmt + '\n';
	result = mpTableFile->WriteAppend(theString.length(), theString.c_str(), NULL);
	
	if (result.OK())
	{
		result = mpTableFile->Flush();
	}

	return result;
}


}	// PandaSQL
