#include "stdafx.h"

#include "BufMgr.h"
#include "Page.h"

#include "VFS/File.h"

namespace PandaSQL
{

static const uint32_t kInvalidBufNum = -1;


//**************BufHashTable*****************

BufHashTable::BufHashTable()
{
}

BufHashTable::~BufHashTable()
{

}

uint32_t BufHashTable::Lookup(uint32_t inPageNum)
{
	return kInvalidBufNum;
}

void BufHashTable::Remove(uint32_t inBufNum)
{
}
	
void BufHashTable::Insert(uint32_t inPageNum, uint32_t inBufNum)
{

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
mRefCount(0)
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
mpBufHash(new BufHashTable())
,mpBufDescs(new BufDesc[inBufCount])
,mpReplacer(new Replacer(inBufCount, mpBufDescs))
,mpBufData(new char*[inBufCount])
,mBufCount(inBufCount)
,mPageSize(inPageSize)
,mpFile(io_file)
{
	for (uint32_t i = 0; i < mBufCount; i++)
	{
		mpBufData[i] = new char[mPageSize];
	}
}

BufMgr::~BufMgr()
{
	for (uint32_t i = 0; i < mBufCount; i++)
	{
		delete []mpBufData[i];
	}
	delete []mpBufData;

	delete []mpBufDescs;
	delete mpReplacer;
	delete mpBufHash;
}

Status BufMgr::PinPage(uint32_t inPageNum, Page *o_page)
{
	Status result;

	uint32_t bufNum = mpBufHash->Lookup(inPageNum);

	if (bufNum == kInvalidBufNum)
	{
		uint32_t victimBuf = mpReplacer->PickVictim();

		PDASSERT(victimBuf != kInvalidBufNum);

		BufDesc *pBufDesc = &mpBufDescs[victimBuf];
		
		//TODO: Needs lock
		if (pBufDesc->mDirty)
		{
			File::Size bytesWritten = 0;
			mpFile->Write(pBufDesc->mPageNum * mPageSize, mPageSize, mpBufData[victimBuf], &bytesWritten);

			PDASSERT(bytesWritten == mPageSize);
		}

		bufNum = victimBuf;
			
		File::Size bytesRead = 0;
		result = mpFile->Read(inPageNum * mPageSize, mPageSize, mpBufData[bufNum], &bytesRead);
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

	o_page->mPageData = mpBufData[bufNum];
	o_page->mPageNum = mpBufDescs[bufNum].mPageNum;

	mpReplacer->IncreaseUsageCount(bufNum);

	return result;
}

Status BufMgr::UnpinPage(uint32_t inPageNum)
{
	Status result;

	uint32_t bufNum = mpBufHash->Lookup(inPageNum);

	PDASSERT(bufNum != kInvalidBufNum);

	PDASSERT(mpBufDescs[bufNum].mRefCount >= 1);

	mpBufDescs[bufNum].mRefCount--;
	mpReplacer->DecreaseUsageCount(bufNum);

	return result;
}

}	// PandaSQL
