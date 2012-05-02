// PandaSQL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Parser/ParserDriver.h"
#include "DB.h"
#include "Utils/Status.h"


int _tmain(int argc, _TCHAR* argv[])
{
	PandaSQL::ParserDriver parserDriver;

#if 0
	std::string inQueryString("./input");

	PandaSQL::Status result = parserDriver.PerformQuery(inQueryString, true);
#else
	std::string inQueryString;
	PandaSQL::Status result;

	inQueryString = ("SELECT t1.field1, t2.field2 FROM t1;");
	//result = parserDriver.PerformQuery(inQueryString, false);

	inQueryString = ("UPDATE t1 SET t1.field1=1, t2.field2='hello';");
	//result = parserDriver.PerformQuery(inQueryString, false);

	inQueryString = ("INSERT INTO t1 (t1.field1, t2.field2) VALUES (1, 'hello');");
	//result = parserDriver.PerformQuery(inQueryString, false);

	inQueryString = ("DELETE FROM t1;");
	//result = parserDriver.PerformQuery(inQueryString, false);

	inQueryString = ("CREATE TABLE t1 (field1 INT PRIMARY KEY, field2 TEXT NOT NULL);");
	//result = parserDriver.PerformQuery(inQueryString, false);

	inQueryString = ("CREATE INDEX idx1 ON t1 (field1);");
	//result = parserDriver.PerformQuery(inQueryString, false);

	inQueryString = ("DROP TABLE t1;");
	//result = parserDriver.PerformQuery(inQueryString, false);

	inQueryString = ("DROP INDEX idx;");
	//result = parserDriver.PerformQuery(inQueryString, false);

	PandaSQL::DB db;

	PandaSQL::DB::Options openOptions;
	openOptions.create_if_missing = true;
	result = db.Open("C:\\Users\\Tianzhou\\Desktop\\PD_Data\\testDB", openOptions);

#endif

	system("pause");

    return 0;
}

