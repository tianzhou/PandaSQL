// PandaSQL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Parser/SQLParserDriver.h"
#include "Utils/Status.h"


int _tmain(int argc, _TCHAR* argv[])
{
	PandaSQL::SQLParserDriver parserDriver;

#if 0
	std::string inQueryString("./input");

	PandaSQL::Status result = parserDriver.PerformQuery(inQueryString, true);
#else
	std::string inQueryString("select * from field1;");

	PandaSQL::Status result = parserDriver.PerformQuery(inQueryString, false);
#endif

	system("pause");

    return 0;
}

