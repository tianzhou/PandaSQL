#include "stdafx.h"

#include "IStorage.h"
#include "CVS/CVSStorage.h"

namespace PandaSQL
{

IStorage *IStorage::CreateStorage(const std::string &inDBRootPath, StorageType inType, VFS *io_VFS)
{
	IStorage *result;

	switch (inType)
	{
	case kCVS:
		result = new CVSStorage(io_VFS, inDBRootPath);
		break;
	default:
		break;
	}

	return result;
}

IStorage::IStorage(const std::string &inRootPath, VFS *io_VFS)
:mRootPath(inRootPath)
,mpVFS(io_VFS)
{
}

}	// PandaSQL