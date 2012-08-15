#include "stdafx.h"

#include "IStorage.h"
#include "CVS/CVSStorage.h"
#include "BerkeleyDB/BDBStorage.h"

namespace PandaSQL
{

IStorage *IStorage::CreateStorage(const std::string &inDBRootPath, StorageType inType)
{
	IStorage *result;

	switch (inType)
	{
	case kCVS:
		//result = new CVSStorage(io_VFS, inDBRootPath);
		break;
	case kBDB:
		result = new BDBStorage(inDBRootPath);
		break;
	default:
		break;
	}

	return result;
}

IStorage::IStorage(const std::string &inRootPath)
:mRootPath(inRootPath)
{
}

}	// PandaSQL