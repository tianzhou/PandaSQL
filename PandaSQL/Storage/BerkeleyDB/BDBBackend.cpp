#include "stdafx.h"

#include "BDBBackend.h"
#include "BDBScanIterator.h"
#include "Transaction/BDBTransaction.h"

#include "Catalog/Table.h"

#include "Utils/Debug.h"

namespace PandaSQL
{

BDBBackend::BDBBackend(const std::string &inRootPath)
:
IDBBackend(inRootPath)
,mpDBEnv(NULL)
,mIsOpen(false)
{
	int ret;

	/*
	* Create an environment and initialize it for additional error
	* reporting.
	*/
	if ((ret = db_env_create(&mpDBEnv, 0)) != 0)
	{
		PDDebugOutputVerbose(db_strerror(ret));
	}
}

BDBBackend::~BDBBackend()
{

}

Status BDBBackend::Open()
{
	Status result;

	PDASSERT(mpDBEnv);
	PDASSERT(!mIsOpen);

	if (!mIsOpen)
	{
		int ret = mpDBEnv->open(mpDBEnv
			, mRootPath.c_str()
			, DB_CREATE | DB_INIT_LOG | DB_INIT_LOCK | DB_INIT_MPOOL | DB_INIT_TXN
			, 0);

		if (ret != 0)
		{
			PDDebugOutputVerbose(db_strerror(ret));
		}

		PDASSERT(ret == 0);

		mIsOpen = true;
	}

	return result;
}	

Status BDBBackend::Close()
{
	Status result;

	PDASSERT(mIsOpen);

	if (mIsOpen)
	{
		int ret = mpDBEnv->close(mpDBEnv, 0);
		mpDBEnv = NULL;

		PDASSERT(ret == 0);

		mIsOpen = false;
	}

	return result;
}

Status BDBBackend::CreateTable(const std::string &tableName, const ColumnDefList &columnList)
{
	Status result;

	result = this->OpenTable_Private(tableName, kCreateIfMissing);

	return result;
}

Status BDBBackend::OpenTable(const std::string &tableName)
{
	Status result;

	result = this->OpenTable_Private(tableName, 0);

	return result;
}

Status BDBBackend::InsertData(const std::string &tableName, const TupleDesc &tupleDesc, const TupleData &tupleData, int32_t keyIndex)
{
	Status result;

	DB *pTable = NULL;
	
	result = this->GetTableByName_Private(tableName, &pTable);

	if (result.OK())
	{
		DBT key;
		DBT data;
		int ret;

		db_recno_t recno;
		
		memset(&key, 0, sizeof(key));

		std::string keyString;
		if (keyIndex >= 0)
		{		
			TupleElementToString(tupleDesc[keyIndex], tupleData[keyIndex], &keyString);
			
			key.data = (void *)keyString.c_str();
			key.size = keyString.length();
		}

		memset(&data, 0, sizeof(data));

		std::string rowString;
		TupleToString(tupleDesc, tupleData, &rowString);
		data.data = (void *)rowString.c_str();
		data.size = rowString.length();

		ret = pTable->put(pTable, NULL, &key, &data, DB_APPEND);

		if (ret != 0)
		{
			PDDebugOutputVerbose(db_strerror(ret));
		}
		else
		{
			recno = *(db_recno_t *)(key.data);
			printf("new record number is %u\n", recno);
		}
	}

	return result;
}

Status BDBBackend::DeleteData(const std::string &tableName, const TuplePredicate *inTuplePredicate /*= NULL*/)
{
	Status result;

	DB *pTable = NULL;
	
	result = this->GetTableByName_Private(tableName, &pTable);

	if (result.OK())
	{
		DBC *dbcp = NULL;
		DB_TXN *tid = NULL;
		int ret;

		ret = TransactionBegin(mpDBEnv, NULL, &tid);

		if (ret != 0)
		{
			PDDebugOutputVerbose(db_strerror(ret));
			result = Status::kInternalError;
		}

		if (result.OK())
		{
			ret = pTable->cursor(pTable, tid, &dbcp, 0);

			if (ret != 0)
			{
				PDDebugOutputVerbose(db_strerror(ret));
				result = Status::kInternalError;
			}	

			if (result.OK())
			{
				DBT key;
				DBT data;

				memset(&key, 0, sizeof(key));
				memset(&data, 0, sizeof(data));

				/* Walk through the database and print out the key/data pairs. */
				while ((ret = dbcp->get(dbcp, &key, &data, DB_NEXT)) == 0)
				{
					ret = dbcp->del(dbcp, 0);

					if (ret != 0)
					{
						break;
					}
				}

				if (ret != DB_NOTFOUND)
				{
					PDDebugOutputVerbose(db_strerror(ret));
					result = Status::kInternalError;
				}

				ret = dbcp->close(dbcp);

				if (ret != 0)
				{
					PDDebugOutputVerbose(db_strerror(ret));
					result = Status::kInternalError;
				}

				ret = TransactionCommit(tid);
				
				if (ret != 0)
				{
					PDDebugOutputVerbose(db_strerror(ret));

					if (result.OK())
					{
						result = Status::kInternalError;
					}
				}

			}
		}
	}

	return result;
}

Status BDBBackend::SelectData(const std::string &tableName, const ColumnDefList &columnList, const TuplePredicate *inTuplePredicate /*= NULL*/)
{
	Status result;

	DB *pTable = NULL;
	
	result = this->GetTableByName_Private(tableName, &pTable);

	if (result.OK())
	{
		DBC *dbcp = NULL;
		int ret;

		ret = pTable->cursor(pTable, NULL, &dbcp, 0);

		if (ret != 0)
		{
			PDDebugOutputVerbose(db_strerror(ret));
			result = Status::kInternalError;
		}	

		if (result.OK())
		{
			DBT key;
			DBT data;

			memset(&key, 0, sizeof(key));
			memset(&data, 0, sizeof(data));

			/* Walk through the database and print out the key/data pairs. */
			while ((ret = dbcp->get(dbcp, &key, &data, DB_NEXT)) == 0)
			{
				printf("%u : %.*s\n",
					*(int *)key.data,
					(int)data.size, (char *)data.data);
			}

			if (ret != DB_NOTFOUND)
			{
				PDDebugOutputVerbose(db_strerror(ret));
				result = Status::kInternalError;
			}

			ret = dbcp->close(dbcp);

			if (ret != 0)
			{
				PDDebugOutputVerbose(db_strerror(ret));
				result = Status::kInternalError;
			}
		}
	}

	return result;
}

TupleIterator* BDBBackend::CreateScanIterator(const std::string &tableName, const ColumnDefList &inColumnDefList, const TuplePredicate *inTuplePredicate /*= NULL*/)
{
	TupleIterator *result = NULL;

	DB *pTable = NULL;
	
	Status localResult = this->GetTableByName_Private(tableName, &pTable);

	if (localResult.OK())
	{
		result = new BDBScanIterator(inColumnDefList, pTable);
	}

	return result;
}

Status BDBBackend::GetTableByName_Private(const std::string &name, DB **o_table) const
{
	Status result;

	BDBBackend::TableMap::const_iterator iter = mTableMap.find(name);

	if (iter != mTableMap.end())
	{
		*o_table = iter->second;
	}
	else
	{
		*o_table = NULL;

		result = Status::kTableMissing;
	}

	return result;
}

Status BDBBackend::OpenTable_Private(const std::string &inTableName, OpenMode inMode)
{
	static char *const kDBName = "mydb.panda";

	Status result;

	DB *pTable = NULL;

	int ret = db_create(&pTable, mpDBEnv, 0);

	PDASSERT(ret == 0);

	u_int32_t flags = DB_AUTO_COMMIT;

	if ((inMode & kCreateIfMissing) != 0)
	{
		flags |= DB_CREATE;
	}

	ret = pTable->open(pTable,
		NULL
		, kDBName
		, inTableName.c_str()
		, DB_RECNO
		, flags
		, 0);


	if (ret != 0)
	{
		PDDebugOutputVerbose(db_strerror(ret));
		result = Status::kInternalError;
	}

	if (result.OK())
	{
		TableMapEntry tableEntry(inTableName.c_str(), pTable);
		mTableMap.insert(tableEntry);
	}

	return result;
}

Status BDBBackend::CloseTable_Private(const std::string &inTableName)
{
	Status result;

	TableMap::iterator iter = mTableMap.find(inTableName);

	if (iter != mTableMap.end())
	{
		int ret = iter->second->close(iter->second, 0);

		PDASSERT(ret == 0);

		mTableMap.erase(iter);
	}

	return result;
}

Status BDBBackend::CloseAllTables_Private()
{
	Status result;

	TableMap::iterator iter = mTableMap.begin();

	for (; iter != mTableMap.end(); iter++)
	{
		int ret = iter->second->close(iter->second, 0);

		PDASSERT(ret == 0);
	}

	mTableMap.clear();

	return result;
}

}	// PandaSQL