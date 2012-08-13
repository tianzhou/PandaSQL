#include "stdafx.h"

#include "BufMgr.h"
#include "Page.h"

#include "VFS/File.h"

namespace PandaSQL
{

static const uint32_t kInvalidBufNum = -1;


//**************BufHashTable*****************

BufHashTable::BufHashTable(uint32_t inTableSize)
:
mTableSize(inTableSize)
,mppHash(new HashEntry*[inTableSize])
{
	for (uint32_t i = 0; i < mTableSize; i++)
	{
		mppHash[i] = NULL;
	}
}

BufHashTable::~BufHashTable()
{
	HashEntry *pCurrentEntry = NULL;
	HashEntry *pOldEntry = NULL;
	for (uint32_t i = 0; i < mTableSize; i++)
	{
		pCurrentEntry = mppHash[i];
		while (pCurrentEntry)
		{
			pOldEntry = pCurrentEntry;
			pCurrentEntry = pOldEntry->next;

			delete pOldEntry;
		}

		delete pCurrentEntry;
	}
	delete []mppHash;
}

uint32_t BufHashTable::Hash_Private(PageNum inValue) const
{
	return inValue % mTableSize;
}

uint32_t BufHashTable::Lookup(PageNum inPageNum) const
{
	uint32_t result = kInvalidBufNum;

	uint32_t hash = this->Hash_Private(inPageNum);

	const HashEntry *pOneEntry = mppHash[hash];

	while (pOneEntry
			&& pOneEntry->pageNum != inPageNum)
	{
		pOneEntry = pOneEntry->next;
	}

	if (pOneEntry)
	{
		result = pOneEntry->bufNum;
	}

	return result;
}

void BufHashTable::Remove(PageNum inPageNum, uint32_t inBufNum)
{
	uint32_t hash = this->Hash_Private(inPageNum);

	HashEntry *pOneEntry = mppHash[hash];
	HashEntry *pLastEntry = NULL;

	while (pOneEntry
			&& pOneEntry->bufNum != inBufNum)
	{
		pLastEntry = pOneEntry;
		pOneEntry = pOneEntry->next;
	}

	if (pOneEntry)
	{
		//If it's header
		if (pLastEntry == NULL)
		{
			mppHash[hash]->next = pOneEntry->next;
		}
		else
		{
			pLastEntry->next = pOneEntry->next;
		}

		delete pOneEntry;
	}
}
	
void BufHashTable::Insert(PageNum inPageNum, uint32_t inBufNum)
{
	uint32_t hash = this->Hash_Private(inPageNum);

	HashEntry *pOneEntry = mppHash[hash];
	HashEntry *pLastEntry = NULL;

	while (pOneEntry
			&& pOneEntry->bufNum != inBufNum)
	{
		pLastEntry = pOneEntry;
		pOneEntry = pOneEntry->next;
	}

	if (!pOneEntry)
	{
		pOneEntry = new HashEntry();
		pOneEntry->pageNum = inPageNum;
		pOneEntry->bufNum = inBufNum;
		pOneEntry->next = NULL;

		//If it's header
		if (pLastEntry == NULL)
		{
			mppHash[hash] = pOneEntry;
		}
		else
		{
			pLastEntry->next = pOneEntry;
		}		
	}
}

//**************Frame*****************

Frame::Frame()
{
}

Frame::~Frame()
{
}

//**************Replacer*****************
Replacer::Replacer(uint32_t inBufCount, BufDesc *io_pBufDescs)
:
mBufCount(inBufCount)
,mpBufDescs(io_pBufDescs)
{
}

Replacer::~Replacer()
{

}

Status Replacer::IncreaseUsageCount(uint32_t inBufNum)
{
	Status result;

	PDASSERT(inBufNum < mBufCount);

	mpBufDescs[inBufNum].mUsageCount++;

	return result;
}

Status Replacer::DecreaseUsageCount(uint32_t inBufNum)
{
	Status result;

	PDASSERT(inBufNum < mBufCount);
	PDASSERT(mpBufDescs[inBufNum].mUsageCount > 0);

	mpBufDescs[inBufNum].mUsageCount--;

	return result;
}

uint32_t Replacer::PickVictim()
{
	uint32_t returnBufNum = kInvalidBufNum;

	bool isAllDirty = true;
	for (uint32_t i = 0; i < mBufCount; i++)
	{
		if (!mpBufDescs[i].mDirty
			&& mpBufDescs[i].mUsageCount == 0)
		{
			returnBufNum = i;
			break;
		}

		if (mpBufDescs[i].mDirty)
		{
			isAllDirty = false;
		}
	}

	if (!isAllDirty)
	{
		while (1)
		{
			for (uint32_t i = 0; i < mBufCount; i++)
			{
				if (!mpBufDescs[i].mDirty
					&& mpBufDescs[i].mUsageCount == 0)
				{
					returnBufNum = i;
					break;
				}

				mpBufDescs[i].mUsageCount--;
			}	
		}
	}

	return returnBufNum;
}

//**************BufDesc*****************

BufDesc::BufDesc()
:
mPageNum(kInvalidPageNum)
,mBufNum(kInvalidBufNum)
,mRefCount(0)
,mUsageCount(0)
,mDirty(false)
{
}

BufDesc::~BufDesc()
{
}

//**************BufMgr*****************

BufMgr::BufMgr(uint32_t inBufCount, uint32_t inPageSize, File *io_file)
:
mpBufHash(new BufHashTable(inBufCount))
,mpBufDescs(new BufDesc[inBufCount])
,mpReplacer(new Replacer(inBufCount, mpBufDescs))
,mppBufData(new char*[inBufCount])
,mpTempBuf(new char[inPageSize])
,mBufCount(inBufCount)
,mPageSize(inPageSize)
,mpFile(io_file)
{
	for (uint32_t i = 0; i < mBufCount; i++)
	{
		mppBufData[i] = new char[mPageSize];
	}
}

BufMgr::~BufMgr()
{
	delete []mpTempBuf;

	for (uint32_t i = 0; i < mBufCount; i++)
	{
		delete []mppBufData[i];
	}
	delete []mppBufData;

	delete []mpBufDescs;
	delete mpReplacer;
	delete mpBufHash;
}

Status BufMgr::PinPage(PageNum inPageNum, Page *o_page)
{
	Status result;

	uint32_t bufNum = mpBufHash->Lookup(inPageNum);

	if (bufNum == kInvalidBufNum)
	{
		uint32_t victimBuf = mpReplacer->PickVictim();

		PDASSERT(victimBuf != kInvalidBufNum);

		BufDesc *pBufDesc = &mpBufDescs[victimBuf];

		//TODO: Needs lock
		//If it's not the empty slot
		if (pBufDesc->mBufNum != kInvalidBufNum)
		{
			mpBufHash->Remove(pBufDesc->mPageNum, pBufDesc->mBufNum);
		}
		
		//TODO: Needs lock
		if (pBufDesc->mDirty)
		{
			File::Size bytesWritten = 0;
			mpFile->Write(pBufDesc->mPageNum * mPageSize, mPageSize, mppBufData[victimBuf], &bytesWritten);

			PDASSERT(bytesWritten == mPageSize);
		}

		bufNum = victimBuf;

		//TODO: Needs lock
		mpBufHash->Insert(inPageNum, bufNum);
			
		File::Size bytesRead = 0;
		result = mpFile->Read(inPageNum * mPageSize, mPageSize, mppBufData[bufNum], &bytesRead);
		PDASSERT(result.OK());
		PDASSERT(bytesRead == mPageSize);

		mpBufDescs[bufNum].mPageNum = inPageNum;
		mpBufDescs[bufNum].mBufNum = bufNum;
		mpBufDescs[bufNum].mRefCount = 1;
		mpBufDescs[bufNum].mDirty = false;
	}
	else
	{
		mpBufDescs[bufNum].mRefCount++;
	}

	o_page->mPageData = mppBufData[bufNum];
	o_page->mPageNum = mpBufDescs[bufNum].mPageNum;

	mpReplacer->IncreaseUsageCount(bufNum);

	return result;
}

Status BufMgr::UnpinPage(PageNum inPageNum, bool inDirty)
{
	Status result;

	uint32_t bufNum = mpBufHash->Lookup(inPageNum);

	PDASSERT(bufNum != kInvalidBufNum);

	PDASSERT(mpBufDescs[bufNum].mRefCount >= 1);

	mpBufDescs[bufNum].mDirty = inDirty;
	mpBufDescs[bufNum].mRefCount--;
	mpReplacer->DecreaseUsageCount(bufNum);

	//TODO: Defer write 
	if (mpBufDescs[bufNum].mRefCount == 0
		&& mpBufDescs[bufNum].mDirty)
	{
		//TODO: Needs lock
		File::Size bytesWritten = 0;
		mpFile->Write(inPageNum * mPageSize, mPageSize, mppBufData[bufNum], &bytesWritten);

		PDASSERT(bytesWritten == mPageSize);
	}

	return result;
}

Status BufMgr::NewPage(PageNum *o_pageNum)
{
	Status result;

	memset(mpTempBuf, 0, mPageSize);

	File::Size bytesWritten;
	result = mpFile->Append(mPageSize, mpTempBuf, &bytesWritten);

	if (result.OK())
	{
		//result = mpFile->Flush();

		//if (result.OK())
		{
			PageNum totalPage;
			result = this->GetTotalPages(&totalPage);

			PDASSERT(totalPage > 0);

			//o_pageNum is 0 based
			*o_pageNum = totalPage - 1;
		}
	}

	return result;
}

Status BufMgr::GetTotalPages(PageNum *o_pageCount)
{
	Status result;

	File::Size size;
	result = mpFile->GetSize(&size);

	if (result.OK())
	{
		*o_pageCount = size / mPageSize;

		if (size % mPageSize != 0)
		{
			//Assume file is page aligned
			PDASSERT(0);
			result = Status::kIOError;
		}
	}

	return result;
}

}	// PandaSQL
