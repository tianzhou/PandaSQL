#ifndef BAMBOO_DB_H
#define BAMBOO_DB_H

#include <string>

#include "BambooDBDef.h"

#include "BambooDB/Utils/Error.h"
#include "BambooDB/Utils/Types.h"

namespace BambooDB
{

class BAMBOODB_API DB
{
public:

	DB(const std::string &inDBPath);
	~DB();

	Error Open();
	Error Close();

privileged:


private:

	DB(const DB &rhs);
	DB& operator=(const DB &rhs);

	bool mIsOpen;
};


}	// BambooDB

#endif	// PANDASQL_BAMBOO_DB_H