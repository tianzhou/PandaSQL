#include "stdafx.h"

#include "CVSScanIterator.h"
#include "VFS/File.h"

namespace PandaSQL
{

CVSScanIterator::CVSScanIterator(File *io_file)
:mpFile(io_file)
{
	this->SeekBeforeFirst();
}

CVSScanIterator::~CVSScanIterator()
{
}

bool CVSScanIterator::Valid() const
{
	return mPosValid;
}

Status CVSScanIterator::SeekBeforeFirst()
{
	mFilePos.mark = kBegin;
	mFilePos.offset = 0;
	mPosValid = false;

	return mStatus;
}

Status CVSScanIterator::SeekAfterLast()
{
	mFilePos.mark = kEnd;
	mFilePos.offset = 0;
	mPosValid = false;

	return mStatus;
}

//Status CVSScanIterator::SeekToKey(const std::string &inKey)
//{
//	mFilePos.mark = kNormal;
//	mFilePos.key = inKey;
//	mFilePos.offset = 0;
//	mPosValid = false;
//
//	return mStatus;
//}

Status CVSScanIterator::Next()
{
	Status result;

	if (mFilePos.mark == kEnd)
	{
		result = Status::kEOF;
	}
	else
	{
		File::Size o_bytesRead = 0;
		char buf[513];	//Add Null terminator

		result = mpFile->ReadToDelimiter(mFilePos.offset, 512, "\n", true, buf, &o_bytesRead);

		if (result.IsEOF())
		{
			this->SeekAfterLast();
		}

		if (o_bytesRead > 0)
		{
			buf[o_bytesRead] = '\0';
			mFilePos.offset += o_bytesRead;
			mPosValid = true;
			mCurentData = buf;

			result = Status::kOK;
		}
	}

	return result;
}

Status CVSScanIterator::Prev()
{
	Status result;
	
	return result;
}

//Status CVSScanIterator::GetKey(std::string *o_key) const
//{
//	return mStatus;
//}

Status CVSScanIterator::GetValue(std::string *o_value) const
{
	*o_value = mCurentData;
	return mStatus;
}

}	// PandaSQL