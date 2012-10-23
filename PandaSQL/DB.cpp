#include "stdafx.h"

#include "DB.h"

#include <iostream>

#include "Database/DBImpl.h"
#include "Database/Statement.h"

#include "Parser/ParserDriver.h"

namespace PandaSQL
{

DB::DB(StorageType inStorageType)
:
mpDBImpl(new DBImpl(inStorageType))
{
}

DB::~DB()
{
	delete mpDBImpl;
	mpDBImpl = NULL;
}

Status DB::Open(const std::string &inDBPath, const OpenOptions &inOptions)
{
	Status result;

	result = mpDBImpl->Open(inDBPath, inOptions);

	return result;
}

Status DB::Close()
{
	Status result;

	result = mpDBImpl->Close();

	return result;
}

Status DB::Execute(const std::string &inQuery)
{
	Status result;

	PandaSQL::ParserDriver parserDriver(mpDBImpl);

	//TODO: Create/Release in better place
	parserDriver.CreateStatement();

	result = parserDriver.ParseQuery(inQuery);

	if (result.OK())
	{
		result = parserDriver.Execute();

		if (!result.OK())
		{
			std::cout << inQuery << std::endl;
			std::cout << "ERROR: " << result.GetCode() << std::endl;
		}
	}

	parserDriver.ReleaseStatement();
	
	return result;
}

Status DB::Prepare(const std::string &inQuery, Statement **io_statement)
{
	Status result;

	return result;
}

Status DB::Step(Statement *io_statement)
{
	Status result;

	return result;
}

Status DB::Finalize(Statement *io_statement)
{
	Status result;

	return result;
}

}	// PandaSQL
