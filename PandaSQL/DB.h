#ifndef PANDASQL_DB_H
#define PANDASQL_DB_H

#include "Utils/Status.h"

#include <string>

namespace PandaSQL
{

class DB
{
public:
	
	struct Options
	{
		bool create_if_missing;

		Options();
	};

	static Status Open(const std::string &inDBPath, const Options &inOptions, DB **o_pDB);
	~DB();

private:
	DB();
	DB(const DB &rhs);
	DB& operator=(const DB &rhs);
};

}	// PandaSQL

#endif	// PANDASQL_DB_H