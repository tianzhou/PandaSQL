// PandaSQL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Parser/ParserDriver.h"
#include "Utils/Status.h"


int _tmain(int argc, _TCHAR* argv[])
{
	PandaSQL::ParserDriver parserDriver;

#if 0
	std::string inQueryString("./input");

	PandaSQL::Status result = parserDriver.PerformQuery(inQueryString, true);
#else
	std::string inQueryString("SELECT t2.field1, t1.field2 FROM t1;");

	PandaSQL::Status result = parserDriver.PerformQuery(inQueryString, false);
#endif

	system("pause");

    return 0;
}

