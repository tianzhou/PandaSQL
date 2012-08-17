#include "stdafx.h"

#include "BDBBackend.h"

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
		PDDebugOutput(db_strerror(ret));
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
			PDDebugOutput(db_strerror(ret));
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

Status BDBBackend::CreateTable(const std::string &tableName, const Table::ColumnDefList &columnList)
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

Status BDBBackend::InsertData(const std::string &tableName, const std::string *keyStr, const std::string &dataStr)
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
		if (keyStr)
		{		
			key.data = (void *)keyStr->c_str();
			key.size = keyStr->length();
		}

		memset(&data, 0, sizeof(data));
		data.data = (void *)dataStr.c_str();
		data.size = dataStr.length();

		ret = pTable->put(pTable, NULL, &key, &data, DB_APPEND);

		if (ret != 0)
		{
			PDDebugOutput(db_strerror(ret));
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

	return result;
}

Status BDBBackend::SelectData(const Table::TableRefList &tableList, const JoinList &joinList, const Table::ColumnDefList &columnList, const TuplePredicate *inTuplePredicate /*= NULL*/)
{
	Status result;

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

	u_int32_t flags = 0;

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
		PDDebugOutput(db_strerror(ret));
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