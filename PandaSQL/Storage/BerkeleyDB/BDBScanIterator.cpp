#include "stdafx.h"

#include "BDBScanIterator.h"

#include "Utils/Debug.h"
#include "Utils/Predicate.h"

namespace PandaSQL
{

static char kDeleteMark[] = " ";

BDBScanIterator::BDBScanIterator(const TuplePredicate *inTuplePredicate, DB *io_dbTable)
:Iterator(inTuplePredicate)
,mpDBTable(io_dbTable)
,mpDBCursor(NULL)
,mLastError(Status::kOK)
{
	this->SeekToFirst();
}

BDBScanIterator::~BDBScanIterator()
{
	int ret = mpDBCursor->close(mpDBCursor);

	if (ret != 0)
	{
		PDDebugOutput(db_strerror(ret));
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
			PDDebugOutput(db_strerror(ret));
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

Status BDBScanIterator::SeekAfterLast()
{
	Status result;

	mLastError = result;

	return result;
}

Status BDBScanIterator::SeekToPredicate(const TuplePredicate *inTuplePredicate)
{
	Status result;

	if (inTuplePredicate)
	{
		
	}
	else
	{
		result = this->SeekToFirst();
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

Status BDBScanIterator::InsertValue(const TupleData &inTuple)
{
	Status result;

	return result;
}

Status BDBScanIterator::UpdateValue(const TupleData &inTuple)
{
	Status result;

	//Don't allow update
	PDASSERT(0);
	
	return result;
}

Status BDBScanIterator::DeleteValue()
{
	Status result;

	return result;
}

Status BDBScanIterator::GetValue(TupleData *o_tuple) const
{
	Status result;
	
	return result;
}

Status BDBScanIterator::GetValue(std::string *o_rowString) const
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
		PDDebugOutput(db_strerror(ret));
		result = Status::kInternalError;
	}
	else
	{
		o_rowString->append((const char *)data.data, data.size);
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
		PDDebugOutput(db_strerror(ret));

		if (ret == DB_NOTFOUND)
		{
			result = Status::kEOF;
		}
		else
		{
			result = Status::kInternalError;
		}
	}

	return result;
}

}	// PandaSQL