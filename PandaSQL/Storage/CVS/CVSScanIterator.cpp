#include "stdafx.h"

#include "CVSScanIterator.h"
#include "Storage/Buffer/PageProxy.h"
#include "Storage/Buffer/Page.h"
#include "Storage/ITuple.h"

namespace PandaSQL
{

CVSScanIterator::CVSScanIterator(PageProxy *io_pageProxy)
:mpPageProxy(io_pageProxy)
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
		char *pageData = NULL;
		uint32_t pageCount = kInvalidPageNum;

		do
		{
			result = mpPageProxy->GetPage(mSeekPos.pageNum
				, PageProxy::kPageRead
				, &pageData);

			if (!result.OK())
			{
				break;
			}

			std::string theStr(pageData + mSeekPos.offset, mpPageProxy->GetPageSize() - mSeekPos.offset);
			const char *pch = strstr(theStr.c_str(), kNewLineSymbol);

			if (pch)
			{
				mValuePos = mSeekPos;

				mSeekPos.offset += pch - theStr.c_str();

				mSeekPos.offset += strlen(kNewLineSymbol);

				mPosAfterLast = false;

				break;
			}

			if (pageCount == kInvalidPageNum)
			{
				result = mpPageProxy->GetPageCount(&pageCount);
			}

			if (result.OK())
			{
				if (pageCount > mSeekPos.pageNum + 1)
				{
					result = mpPageProxy->PutPage(mSeekPos.pageNum
						, false
						, pageData);

					if (result.OK())
					{
						//Search for next page
						mSeekPos.pageNum++;
						mSeekPos.offset = 0;
					}
				}
				else
				{
					result = Status::kEOF;
				}
			}

		}while(result.OK());

		if (result.IsEOF())
		{
			this->SeekAfterLast();
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

Status CVSScanIterator::InsertValue(const ITuple &inTuple)
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

		inTuple.GetDataOfField(i, &o_data);
		rowData += o_data;
	}

	rowData += kNewLineSymbol;

	PDASSERT(rowData.length() <= mpPageProxy->GetPageSize());

	uint32_t pageNum;
	uint32_t pageCount;

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

Status CVSScanIterator::UpdateValue(const ITuple &inTuple)
{
	Status result;

	//Don't allow update
	PDASSERT(0);
	
	return result;
}

Status CVSScanIterator::GetValue(ITuple *o_tuple) const
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

		PDASSERT(mSeekPos.offset > mValuePos.offset);

		char *pageData = NULL;

		result = mpPageProxy->GetPage(mSeekPos.pageNum
			, PageProxy::kPageRead | PageProxy::kPageWrite
			, &pageData);

		//TODO: Hard code
		char tempStr[4096];
		size_t length = mSeekPos.offset - mValuePos.offset;
		memcpy(tempStr, &pageData[mValuePos.offset], length);
		tempStr[length] = '\0';
		
		char *nextToken;
		char *pch = strtok_s(tempStr, ",", &nextToken);

		while (pch != NULL)
		{
			o_tuple->AppendFieldData(kText, pch);
			pch = strtok_s(NULL, ",", &nextToken);
		}

		mpPageProxy->PutPage(mSeekPos.pageNum, false, pageData);
	}
	
	return result;
}

}	// PandaSQL