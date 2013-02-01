// PandaSQL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <fstream>
#include <iostream>
#include <string>

#include "DB.h"

#include "Utils/Status.h"


PandaSQL::Status ReadSQLScript(const char *filePath, PandaSQL::DB *io_db)
{
	PandaSQL::Status result;

	std::fstream fileStream;
	char buf[1025]; //Add Null terminator

	fileStream.open(filePath, std::fstream::in);

	while (fileStream.good())
	{
		fileStream.getline(buf, 1024, ';');

		if (fileStream.good())
		{
			std::string query(buf);

			//The delim is discarded while parser needs it
			query += ';';

			result = io_db->Execute(query);

			if (!result.OK())
			{

			}
		}
	}

	fileStream.close();

	return result;
}

int _tmain(int argc, _TCHAR* argv[])
{
	std::string inQueryString;
	PandaSQL::Status result;

	//PandaSQL::DB db(PandaSQL::kBamboo);
	PandaSQL::DB db(PandaSQL::kBDB);
	PandaSQL::OpenOptions openOptions;
	openOptions.create_if_missing = true;
	result = db.Open("C:\\Users\\Tianzhou\\Desktop\\PD_Data\\testDB\\BDB", openOptions);

	if (result.OK())
	{
		char buf[1024];
		while (0)
		{
			std::cout << "sql>";

			std::cin.getline(buf, 1024);

			std::string query(buf);

			if (strcmp(buf, "q") == 0)
			{
				break;
			}

			db.Execute(query);
		}

#if 1
		//result = ReadSQLScript("TestScriptTestScript/delete.txt", &db);

		//result = ReadSQLScript("TestScript/drop_table.txt", &db);

		//result = ReadSQLScript("TestScript/create_table.txt", &db);

		//result = ReadSQLScript("TestScript/insert.txt", &db);

		//result = ReadSQLScript("TestScript/select.txt", &db);

		result = ReadSQLScript("TestScript/select_join_2table.txt", &db);

		//result = ReadSQLScript("TestScript/select_join_3table.txt", &db);

		//result = ReadSQLScript("TestScript/select_where.txt", &db);

		//result = ReadSQLScript("TestScript/update.txt", &db);

		//result = ReadSQLScript("TestScript/select.txt", &db);

		//result = ReadSQLScript("TestScript/select_index.txt", &db);
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
		
	}

	system("pause");

    return 0;
}

