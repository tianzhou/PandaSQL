// PandaSQL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <string>

#include "DB.h"

#include "Utils/Status.h"

#include "../VFS/IVFS.h"
#include "../VFS/File.h"


PandaSQL::Status ReadSQLScript(const char *filePath, PandaSQL::IVFS *io_VFS, PandaSQL::DB *io_db)
{
	PandaSQL::Status result;

	PandaSQL::File *inputFile = NULL;

	result = io_VFS->OpenFile(filePath, false, &inputFile);

	if (result.OK())
	{
		PandaSQL::File::Offset offset = 0;
		PandaSQL::File::Size amount = 512;
		PandaSQL::File::Size o_bytesRead = 0;
		char buf[513]; //Add Null terminator

		do
		{
			result = inputFile->ReadToDelimiter(offset, 512, ";", true, buf, &o_bytesRead);

			if (o_bytesRead > 0)
			{
				char *pBuf = buf;
				pBuf[o_bytesRead] = '\0';

				while (*pBuf == '\n' || *pBuf == '\r')
				{
					pBuf++;
				}

				std::string query(pBuf);

				result = io_db->Execute(query);
			}

			offset += o_bytesRead;

		}while (result.OK());

		if (result.IsEOF())
		{
			result = PandaSQL::Status::kOK;
		}

		result = io_VFS->CloseFile(inputFile);
	}

	return result;
}

int _tmain(int argc, _TCHAR* argv[])
{
	std::string inQueryString;
	PandaSQL::Status result;

	PandaSQL::IVFS *pVFS = PandaSQL::IVFS::CreateVFS();

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
		//result = ReadSQLScript("TestScriptTestScript/delete.txt", pVFS, &db);

		//result = ReadSQLScript("TestScript/drop_table.txt", pVFS, &db);

		//result = ReadSQLScript("TestScript/create_table.txt", pVFS, &db);

		//result = ReadSQLScript("TestScript/insert.txt", pVFS, &db);

		//result = ReadSQLScript("TestScript/select.txt", pVFS, &db);

		result = ReadSQLScript("TestScript/select_join_2table.txt", pVFS, &db);

		//result = ReadSQLScript("TestScript/select_join_3table.txt", pVFS, &db);

		//result = ReadSQLScript("TestScript/select_where.txt", pVFS, &db);

		//result = ReadSQLScript("TestScript/update.txt", pVFS, &db);

		//result = ReadSQLScript("TestScript/select.txt", pVFS, &db);

		//result = ReadSQLScript("TestScript/select_index.txt", pVFS, &db);
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

