#include "stdafx.h"

#include "BDBScanIterator.h"

#include "Utils/Debug.h"
#include "Utils/Predicate.h"

namespace PandaSQL
{

static char kDeleteMark[] = " ";

BDBScanIterator::BDBScanIterator(const ColumnDefList &inColumnDefList, DB *io_dbTable)
:TupleIterator(inColumnDefList)
,mpDBTable(io_dbTable)
,mpDBCursor(NULL)
{
	this->SeekToFirst();
}

BDBScanIterator::~BDBScanIterator()
{
	int ret = mpDBCursor->close(mpDBCursor);

	if (ret != 0)
	{
		PDDebugOutputVerbose(db_strerror(ret));
	}
}

bool BDBScanIterator::Valid() const
{
	return mpDBCursor && mLastError.OK();
}

Status BDBScanIterator::SeekToFirst()
{
	Status result;

	DBC *dbcp = NULL;
	int ret;

	if (!mpDBCursor)
	{
		ret = mpDBTable->cursor(mpDBTable, NULL, &mpDBCursor, 0);

		if (ret != 0)
		{
			PDDebugOutputVerbose(db_strerror(ret));
			result = Status::kInternalError;
		}
	}

	if (result.OK())
	{
		result = MoveCursor_Private(DB_FIRST);
	}

	mLastError = result;

	return result;
}

Status BDBScanIterator::Next()
{
	Status result;

	result = MoveCursor_Private(DB_NEXT);

	mLastError = result;

	return result;
}

Status BDBScanIterator::Prev()
{
	Status result;
	
	result = MoveCursor_Private(DB_PREV);

	mLastError = result;

	return result;
}

Status BDBScanIterator::GetValue(ValueList *o_valueList) const
{
	Status result;

	PDASSERT(mpDBCursor);

	DBT key;
	DBT data;

	memset(&key, 0, sizeof(key));
	memset(&data, 0, sizeof(data));

	int ret = mpDBCursor->get(mpDBCursor, &key, &data, DB_CURRENT);

	if (ret != 0)
	{
		if (ret == DB_NOTFOUND)
		{
			result = Status::kEOF;
		}
		else
		{
			PDDebugOutputVerbose(db_strerror(ret));
			result = Status::kInternalError;
		}
	}
	else
	{
		std::string rowString;	
		rowString.append((const char *)data.data, data.size);
		
		TupleStringToValueList(mColumnDefList, rowString, o_valueList);
	}

	mLastError = result;

	return result;
}

Status BDBScanIterator::MoveCursor_Private(u_int32_t flags)
{
	Status result;

	PDASSERT(mpDBCursor);

	DBT key;
	DBT data;

	memset(&key, 0, sizeof(key));
	memset(&data, 0, sizeof(data));

	int ret = mpDBCursor->get(mpDBCursor, &key, &data, flags);

	if (ret != 0)
	{
		if (ret == DB_NOTFOUND)
		{
			result = Status::kEOF;
		}
		else
		{
			PDDebugOutputVerbose(db_strerror(ret));
			result = Status::kInternalError;
		}
	}

	return result;
}

}	// PandaSQL