#ifndef PANDA_SQL_PARSER_DRIVER_H
#define PANDA_SQL_PARSER_DRIVER_H

#include <string>

#include "Utils/Status.h"

namespace PandaSQL
{

class SQLParserDriver
{
public:
	SQLParserDriver();
	~SQLParserDriver();

	Status PerformQuery(std::string inQueryString, bool fromFile);

	void PrintCurrentState();

private:
	SQLParserDriver(const SQLParserDriver& rhs);
	SQLParserDriver& operator=(const SQLParserDriver& rhs);
};

}	// namespace PandaSQL

#endif	// PANDA_SQL_PARSER_DRIVER_H
