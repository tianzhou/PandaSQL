#include "stdafx.h"

#include "Storage/BerkeleyDB/BDBScanIterator.h"
#include "Storage/BerkeleyDB/BDBTypes.h"

#include "Utils/Debug.h"
#include "Utils/Predicate.h"

namespace PandaSQL
{

static char kDeleteMark[] = " ";

BDBScanIterator::BDBScanIterator(const TupleDesc &inTupleDesc, DB *io_dbTable)
:TupleIterator(inTupleDesc)
,mpDBTable(io_dbTable)
,mpDBCursor(NULL)
,mJustReset(false)
{
}

BDBScanIterator::~BDBScanIterator()
{
	int ret = mpDBCursor->close(mpDBCursor);

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
	DBC *dbcp = NULL;
	int ret;

	if (!mpDBCursor)
	{
		ret = mpDBTable->cursor(mpDBTable, NULL, &mpDBCursor, 0);

		if (ret != 0)
		{
			PDDebugOutputVerbose(db_strerror(ret));
			mLastError = Status::kInternalError;
		}
	}

	mJustReset = true;
}

bool BDBScanIterator::Next()
{
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

bool BDBScanIterator::GetValue(ValueList *o_valueList) const
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

		TupleStringToValueList(mTupleDesc, rowString, o_valueList);
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