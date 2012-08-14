#include "stdafx.h"

#include "BDBBackend.h"

namespace PandaSQL
{

BDBBackend::BDBBackend(const std::string &inRootPath)
:IDBBackend(inRootPath)
{
}

BDBBackend::~BDBBackend()
{

}

Status BDBBackend::CreateTable(const Table &inTable)
{
	Status result;

	return result;
}

Status BDBBackend::OpenTable(const std::string &inTableName, OpenMode inMode)
{
	Status result;

	return result;
}

}	// PandaSQL