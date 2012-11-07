#include "stdafx.h"

#include "Storage/BerkeleyDB/BDBScanIterator.h"
#include "Storage/BerkeleyDB/BDBTypes.h"
#include "Storage/BerkeleyDB/Transaction/BDBTransaction.h"

#include "Utils/Debug.h"
#include "Utils/Predicate.h"

namespace PandaSQL
{

static char kDeleteMark[] = " ";

BDBScanIterator::BDBScanIterator(const TupleDesc &inTupleDesc, DB *io_dbTable, DB_ENV *io_dbEnv)
:TupleIterator(inTupleDesc)
,mpDBTable(io_dbTable)
,mpDBCursor(NULL)
,mpDBTXN(NULL)
{
	DBC *dbcp = NULL;
	int ret;

	//Must use transaction, otherwise del will fail
	ret = TransactionBegin(io_dbEnv, NULL, &mpDBTXN);

	if (ret != 0)
	{
		PDDebugOutputVerbose(db_strerror(ret));
		mLastError = Status::kInternalError;
	}

	if (mLastError.OK())
	{
		ret = mpDBTable->cursor(mpDBTable, mpDBTXN, &mpDBCursor, 0);

		if (ret != 0)
		{
			PDDebugOutputVerbose(db_strerror(ret));
			mLastError = Status::kInternalError;
		}

		mJustReset = true;
	}
}

BDBScanIterator::~BDBScanIterator()
{
	int ret;

	//No need to close the cursor here. Transaction commit will first
	//close the cursor and commit if succesful or abort on error
	ret = TransactionCommit(mpDBTXN);

	if (ret != 0)
	{
		PDDebugOutputVerbose(db_strerror(ret));
	}
}

#pragma message ("Is still valid if reaching end record")
bool BDBScanIterator::Valid() const
{
	return mpDBCursor && mLastError.OK();
}

void BDBScanIterator::Reset()
{
	mJustReset = true;
}

bool BDBScanIterator::Next()
{
	if (!mLastError.OK())
	{
		return false;
	}

	bool result;

	if (mJustReset)
	{
		mJustReset = false;
		result = MoveCursor_Private(DB_FIRST);
	}
	else
	{
		result = MoveCursor_Private(DB_NEXT);
	}

	return result;
}

bool BDBScanIterator::Prev()
{	
	return MoveCursor_Private(DB_PREV);
}

bool BDBScanIterator::GetValue(ValueList *o_tupleValueList) const
{
	if (!mLastError.OK())
	{
		return false;
	}

	bool result = true;

	PDASSERT(mpDBCursor);

	DBT key;
	DBT data;

	memset(&key, 0, sizeof(key));
	memset(&data, 0, sizeof(data));

	int ret = mpDBCursor->get(mpDBCursor, &key, &data, DB_CURRENT);

	if (ret != 0)
	{
		if (ret != DB_NOTFOUND)
		{
			PDDebugOutputVerbose(db_strerror(ret));
			mLastError = Status::kInternalError;
		}

		result = false;
	}
	else
	{
		std::string rowString;	
		rowString.append((const char *)data.data, data.size);

		StringToTuple(mTupleDesc, rowString, o_tupleValueList);
	}

	return result;
}

bool BDBScanIterator::Remove()
{
	if (!mLastError.OK())
	{
		return false;
	}

	bool result = true;

	PDASSERT(mpDBCursor);

	int ret = mpDBCursor->del(mpDBCursor, 0);

	if (ret != 0)
	{
		PDDebugOutputVerbose(db_strerror(ret));
		mLastError = Status::kInternalError;
		result = false;
	}

	return result;
}

bool BDBScanIterator::MoveCursor_Private(u_int32_t flags)
{
	bool result = true;

	PDASSERT(mpDBCursor);

	DBT key;
	DBT data;

	memset(&key, 0, sizeof(key));
	memset(&data, 0, sizeof(data));

	int ret = mpDBCursor->get(mpDBCursor, &key, &data, flags);

	if (ret != 0)
	{
		if (ret != DB_NOTFOUND)
		{
			PDDebugOutputVerbose(db_strerror(ret));
			mLastError = Status::kInternalError;
		}

		result = false;
	}

	return result;
}

}	// PandaSQL