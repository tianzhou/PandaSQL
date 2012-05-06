#include "stdafx.h"

#include "IStorage.h"
#include "CVS/CVSStorage.h"

namespace PandaSQL
{

IStorage *IStorage::CreateStorage(StorageType inType)
{
	IStorage *result;

	switch (inType)
	{
	case kCVS:
		result = new CVSStorage();
		break;
	default:
		break;
	}

	return result;
}

IStorage::IStorage()
{
}

}	// PandaSQL