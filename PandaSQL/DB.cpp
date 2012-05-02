#include "stdafx.h"

#include "DB.h"
#include "VFS/VFS.h"
#include "VFS/WinVFS.h"

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
{
}

DB::~DB()
{
	mpVFS->CloseFile(mpMainFile);
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


}	// PandaSQL
