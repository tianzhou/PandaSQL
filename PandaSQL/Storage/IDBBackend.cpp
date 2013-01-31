#include "stdafx.h"

#include "Storage/IDBBackend.h"

#include "Storage/BambooDB/BambooBackend.h"
#include "Storage/BerkeleyDB/BDBBackend.h"

namespace PandaSQL
{

IDBBackend *IDBBackend::CreateBackend(const std::string &inRootPath, StorageType inType)
{
	IDBBackend *result;

	switch (inType)
	{
	case kBamboo:
		result = new BambooBackend(inRootPath);
		break;
	case kBDB:
		result = new BDBBackend(inRootPath);
		break;
	default:
		break;
	}

	return result;
}

IDBBackend::IDBBackend(const std::string &inRootPath)
:mRootPath(inRootPath)
{
}

}	// PandaSQL