#include "stdafx.h"

#include "Storage/CVS/CVSScanIterator.h"

#include "Storage/Buffer/PageProxy.h"
#include "Storage/Buffer/Page.h"

#include "Access/Tuple.h"

#include "Utils/Debug.h"
#include "Utils/Predicate.h"

namespace PandaSQL
{

static char kDeleteMark[] = " ";

CVSScanIterator::CVSScanIterator(const TuplePredicate *inTuplePredicate, PageProxy *io_pageProxy)
:Iterator(inTuplePredicate)
,mpPageProxy(io_pageProxy)
{
	this->SeekToFirst();
}

CVSScanIterator::~CVSScanIterator()
{
}

bool CVSScanIterator::Valid() const
{
	return !mPosAfterLast;
}

Status CVSScanIterator::Next_Inner()
{
	Status result;

	if (mPosAfterLast)
	{
		result = Status::kEOF;
	}
	else
	{
		bool findValidPos = false;
		char *pageData = NULL;
		PageNum pageCount;
		
		result = mpPageProxy->GetPageCount(&pageCount);

		if (result.OK())
		{
			PageNum currentPageNum = mSeekPos.pageNum;

			if (mSeekPos.pageNum + 1 > pageCount)
			{
				result = Status::kEOF;
			}
			else
			{	
				result = mpPageProxy->GetPage(currentPageNum
					, PageProxy::kPageRead
					, &pageData);
			}

			if (result.OK())
			{

				while (result.OK() && !findValidPos)
				{
					if (currentPageNum != mSeekPos.pageNum)
					{
						result = mpPageProxy->GetPage(currentPageNum
							, PageProxy::kPageRead
							, &pageData);

						if (result.OK())
						{
							currentPageNum = mSeekPos.pageNum;
							result = mpPageProxy->PutPage(currentPageNum
								, false
								, pageData);
						}

						if (!result.OK())
						{
							break;
						}
					}

					uint32_t startPos = mSeekPos.offset;
					bool skipRecord = false;

					if (!memcmp(pageData + startPos, kDeleteMark, sizeof(kDeleteMark)))
					{
						//Skip delete mark
						startPos += sizeof(kDeleteMark);
						skipRecord = true;
					}

					std::string theStr(pageData + startPos, mpPageProxy->GetPageSize() - mSeekPos.offset);
					const char *pch = strstr(theStr.c_str(), kNewLineSymbol);

					if (pch)
					{
						mValuePos = mSeekPos;

						if (skipRecord)
						{
							mSeekPos.offset += sizeof(kDeleteMark);
						}

						mSeekPos.offset += (uint32_t)(pch - theStr.c_str());

						mSeekPos.offset += (uint32_t)(strlen(kNewLineSymbol));

						if (!skipRecord)
						{
							if (mpTuplePredicate)
							{
								TupleData tuple;
								//TODO: Hard code
								char tempStr[4096];
								size_t length = mSeekPos.offset - mValuePos.offset - strlen(kNewLineSymbol);
								memcpy(tempStr, theStr.c_str(), length);
								tempStr[length] = '\0';

								char *nextToken;
								char *pch = strtok_s(tempStr, ",", &nextToken);

								while (pch != NULL)
								{
									tuple.AppendData(pch);
									pch = strtok_s(NULL, ",", &nextToken);
								}

								skipRecord = !mpTuplePredicate->Eval(tuple);
								findValidPos = !skipRecord;
							}
							else
							{
								findValidPos = true;
							}
						}
					}

					if (!findValidPos && !skipRecord)
					{
						if (mSeekPos.pageNum + 1 < pageCount)
						{
							//Search for next page
							mSeekPos.pageNum++;
							mSeekPos.offset = 0;
						}
						else
						{
							result = Status::kEOF;
						}
					}
				}

				Status localStatus = mpPageProxy->PutPage(currentPageNum
					, false
					, pageData);

				//Override only when error happens
				if (!localStatus.OK())
				{
					result = localStatus;
				}
			}

			if (result.IsEOF())
			{
				this->SeekAfterLast();
			}
		}
	}

	return result;
}

Status CVSScanIterator::SeekToFirst()
{
	Status result;

	mSeekPos.pageNum = 0;
	mSeekPos.offset = 0;
	mValuePos.pageNum = 0;
	mValuePos.offset = 0;
	mPosAfterLast = false;

	result = this->Next_Inner();

	return result;
}

Status CVSScanIterator::SeekAfterLast()
{
	Status result;

	mSeekPos.pageNum = 0;
	mSeekPos.offset = 0;
	mValuePos.pageNum = 0;
	mValuePos.offset = 0;
	mPosAfterLast = true;

	return result;
}

//Status CVSScanIterator::SeekToPredicate(const TuplePredicate *inTuplePredicate)
//{
//	Status result;
//
//	if (inTuplePredicate)
//	{
//		
//	}
//	else
//	{
//		result = this->SeekToFirst();
//	}
//
//	return result;
//}

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

	result = this->Next_Inner();

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

Status CVSScanIterator::InsertValue(const TupleData &inTuple)
{
	Status result;

	uint32_t fieldCount = inTuple.Count();
	
	std::string rowData;
	std::string o_data;
	for (uint32_t i = 0; i < fieldCount; i++)
	{
		if (i != 0)
		{
			rowData += ",";
		}

		inTuple.GetDataAtIndex(i, &o_data);
		rowData += o_data;
	}

	rowData += kNewLineSymbol;

	PDASSERT(rowData.length() <= mpPageProxy->GetPageSize());

	PageNum pageNum;
	PageNum pageCount;

	if (mPosAfterLast)
	{
		result = mpPageProxy->GetPageCount(&pageCount);

		if (result.OK())
		{
			if (pageCount == 0)
			{
				result = mpPageProxy->NewPage(&pageNum);
			}
			else
			{
				//Get last page, page num is 0 based
				pageNum = pageCount - 1;
			}
		}
	}
	else
	{
		//Don't allow insert in the middle
		PDASSERT(0);
	}

	if (result.OK())
	{
		char *pageData = NULL;

		result = mpPageProxy->GetPage(pageNum
			, PageProxy::kPageRead | PageProxy::kPageWrite
			, &pageData);

		char *pch =strchr(pageData, '\0');

		if (pch)
		{
			if ((uint32_t)(pch - pageData) <= mpPageProxy->GetPageSize() - rowData.length())
			{
				memcpy(pch, rowData.c_str(), rowData.length());
			}
			else
			{
				//TODO: Need concatenate between two pages.

				//Return back old page
				result = mpPageProxy->PutPage(pageNum, false, pageData);

				if (result.OK())
				{
					//No enough space, add a new page
					result = mpPageProxy->NewPage(&pageNum);

					if (result.OK())
					{
						result = mpPageProxy->GetPage(pageNum
							, PageProxy::kPageRead | PageProxy::kPageWrite
							, &pageData);

						if (result.OK())
						{
							memcpy(pageData, rowData.c_str(), rowData.length());
						}
					}
				}
			}
		}

		if (result.OK())
		{
			mpPageProxy->PutPage(pageNum, true, pageData);
		}
	}

	return result;
}

Status CVSScanIterator::UpdateValue(const TupleData &inTuple)
{
	Status result;

	//Don't allow update
	PDASSERT(0);
	
	return result;
}

Status CVSScanIterator::DeleteValue()
{
	Status result;

	if (mPosAfterLast)
	{
		result = Status::kEOF;
	}
	else
	{
		PDASSERT(mSeekPos.pageNum == mValuePos.pageNum);

		uint32_t rowEnd = mSeekPos.offset - (uint32_t)(strlen(kNewLineSymbol));

		PDASSERT(rowEnd > mValuePos.offset);

		char *pageData = NULL;

		result = mpPageProxy->GetPage(mSeekPos.pageNum
			, PageProxy::kPageRead | PageProxy::kPageWrite
			, &pageData);

		size_t length = sizeof(kDeleteMark);
		memcpy(pageData + mValuePos.offset, kDeleteMark, length);

		mpPageProxy->PutPage(mSeekPos.pageNum, true, pageData);
	}

	return result;
}

Status CVSScanIterator::GetValue(TupleData *o_tuple) const
{
	Status result;

	if (mPosAfterLast)
	{
		result = Status::kEOF;
	}
	else
	{
		//TODO: May expand pages
		PDASSERT(mSeekPos.pageNum == mValuePos.pageNum);

		uint32_t rowEnd = mSeekPos.offset - (uint32_t)(strlen(kNewLineSymbol));

		PDASSERT(rowEnd > mValuePos.offset);

		char *pageData = NULL;

		result = mpPageProxy->GetPage(mSeekPos.pageNum
			, PageProxy::kPageRead | PageProxy::kPageWrite
			, &pageData);

		//TODO: Hard code
		char tempStr[4096];
		size_t length = rowEnd - mValuePos.offset;
		memcpy(tempStr, &pageData[mValuePos.offset], length);
		tempStr[length] = '\0';
		
		char *nextToken;
		char *pch = strtok_s(tempStr, ",", &nextToken);

		while (pch != NULL)
		{
			o_tuple->AppendData(pch);
			pch = strtok_s(NULL, ",", &nextToken);
		}

		mpPageProxy->PutPage(mSeekPos.pageNum, false, pageData);
	}
	
	return result;
}

Status CVSScanIterator::GetValue(std::string *o_rowString) const
{
	Status result;

	return result;
}

}	// PandaSQL