#ifndef PANDASQL_DB_H
#define PANDASQL_DB_H

#include "Utils/Status.h"
#include "Utils/Types.h"

namespace PandaSQL
{

class Statement;
class DBImpl;

class DB
{
public:

	DB(StorageType inStorageType);
	~DB();

	Status Open(const std::string &inDBPath, const OpenOptions &inOptions);
	Status Close();

	Status Execute(const std::string &inQuery);

	Status Prepare(const std::string &inQuery, Statement **io_statement);
	Status Step(Statement *io_statement);
	Status Finalize(Statement *io_statement);

private:
	DB(const DB &rhs);
	DB& operator=(const DB &rhs);

	DBImpl	*mpDBImpl;
};

}	// PandaSQL

#endif	// PANDASQL_PANDADB_H