#include "stdafx.h"

#include "DB.h"
#include "VFS/VFS.h"
#include "VFS/WinVFS.h"
#include "VFS/File.h"

#include "Parser/ParserDriver.h"

#include "Catalog/Table.h"

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
	PDASSERT(!mpVFS);
	PDASSERT(!mpMainFile);
	PDASSERT(!mpTableFile);
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
		std::string mainFilePath = inDBPath + "\\testDB.pdm";
		result = mpVFS->OpenFile(mainFilePath, create_if_missing, &mpMainFile);

		if (result.OK())
		{
			std::string tableFilePath = inDBPath + "\\testDB.pdt";
			result = mpVFS->OpenFile(tableFilePath, create_if_missing, &mpTableFile);

			if (result.OK())
			{
				ParserDriver parser(this);
				parser.SetLoadTable(true);
				result = parser.LoadFromFile(mpTableFile);
			}
		}
	}

	return result;
}

Status DB::Close()
{
	Status result;

	TableList::iterator iter = mTableList.begin();

	for (; iter != mTableList.end(); iter++)
	{
		delete *iter;
	}

	mpVFS->CloseFile(mpMainFile);
	mpMainFile = NULL;
	mpVFS->CloseFile(mpTableFile);
	mpTableFile = NULL;

	delete mpVFS;
	mpVFS = NULL;

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

Status DB::AddTable(Table *pTable)
{
	Status result;

	mTableList.push_back(pTable);

	return result;
}

Status DB::InsertData(const Table::ColumnRefList &columnList, const Table::ColumnValueList &columnValueList)
{
	Status result;

	return result;
}


}	// PandaSQL
