#include "stdafx.h"

#include "DB.h"

namespace PandaSQL
{

DB::Options::Options()
:
create_if_missing(false)
{
}

Status DB::Open(const std::string &inDBPath, const Options &inOptions, DB **o_pDB)
{
	Status result;

	return result;
}

DB::~DB()
{
}


}	// PandaSQL
