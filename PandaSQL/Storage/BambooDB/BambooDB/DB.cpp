#include "stdafx.h"

#include "BambooDB/DB.h"

//#include "Utils/Debug.h"
//#include "Utils/Common.h"

namespace BambooDB
{

DB::DB(const std::string &inDBPath)
	:
mIsOpen(false)
{

}

DB::~DB()
{
}

Error DB::Open()
{
	Error result;

	return result;
}

Error DB::Close()
{
	Error result;

	return result;
}

}	// BambooDB