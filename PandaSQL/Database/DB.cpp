#include "stdafx.h"

#include "DB.h"

#include "Database/DBImpl.h"
#include "Database/Statement.h"

#include "Parser/ParserDriver.h"

#include "Utils/Debug.h"

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

	Statement *pTopStatement = NULL;

	result = parserDriver.ParseQuery(inQuery, &pTopStatement);

	if (result.OK())
	{
		PDASSERT(pTopStatement);

		result = pTopStatement->Execute();

		if (!result.OK())
		{
			std::cout << "ERROR: " << result.GetCode() << std::endl;
		}

		if (pTopStatement->GetStatementType() != Statement::kStmtUnknown
			&& pTopStatement->GetStatementType() != Statement::kStmtEmpty)
		{
			std::cout << "********************" << std::endl;
		}

		delete pTopStatement;
	}
	
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
