// PandaSQL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Parser/ParserDriver.h"
#include "DB.h"
#include "VFS/VFS.h"
#include "Utils/Status.h"

#include <iostream>

int _tmain(int argc, _TCHAR* argv[])
{
	std::string inQueryString;
	PandaSQL::Status result;

	PandaSQL::DB db;
	PandaSQL::DB::Options openOptions;
	openOptions.create_if_missing = true;
	result = db.Open("C:\\Users\\Tianzhou\\Desktop\\PD_Data\\testDB", openOptions);

	if (result.OK())
	{
		PandaSQL::ParserDriver parserDriver(&db);

		char buf[1024];
		while (0)
		{
			std::cout << "sql>";

			std::cin.getline(buf, 1024);
	
			result = parserDriver.ParseQuery(buf);

			if (strcmp(buf, "q") == 0)
			{
				break;
			}

			if (result.OK())
			{
				result = parserDriver.Execute();

				if (!result.OK())
				{
					std::cout << "error: " << result.GetCode() << std::endl;
				}
			}
		}

#if 1
		PandaSQL::File *inputFile = NULL;
		result = db.GetVFS().OpenFile("./insert.txt", false, &inputFile);
		
		if (result.OK())
		{
			parserDriver.LoadFromFile(inputFile);
		}
#else

		inQueryString = ("SELECT t1.field1, t2.field2 FROM t1;");
		//result = parserDriver.ParseQuery(inQueryString);

		inQueryString = ("UPDATE t1 SET t1.field1=1, t2.field2='hello';");
		//result = parserDriver.ParseQuery(inQueryString);

		inQueryString = ("INSERT INTO t1 (t1.field1, t2.field2) VALUES (1, 'hello');");
		//result = parserDriver.ParseQuery(inQueryString);

		inQueryString = ("DELETE FROM t1;");
		//result = parserDriver.ParseQuery(inQueryString);

		inQueryString = ("CREATE TABLE t1 (field1 INT PRIMARY KEY, field2 TEXT NOT NULL);");
		result = parserDriver.ParseQuery(inQueryString);

		inQueryString = ("CREATE INDEX idx1 ON t1 (field1);");
		//result = parserDriver.ParseQuery(inQueryString);

		inQueryString = ("DROP TABLE t1;");
		//result = parserDriver.ParseQuery(inQueryString);

		inQueryString = ("DROP INDEX idx;");
		//result = parserDriver.ParseQuery(inQueryString);

		result = parserDriver.Execute();
#endif

		result = db.Close();

		system("pause");
	}

    return 0;
}

