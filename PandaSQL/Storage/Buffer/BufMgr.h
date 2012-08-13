#ifndef PANDASQL_BUFMGR_H
#define PANDASQL_BUFMGR_H

#include "Storage/StorageTypes.h"

#include "Utils/Types.h"
#include "Utils/Status.h"

namespace PandaSQL
{

class Page;
class File;
class BufMgr;
class BufDesc;

class BufHashTable
{
public:
	BufHashTable(uint32_t inTableSize);
	~BufHashTable();

	uint32_t Lookup(PageNum inPageNum) const;
	void Remove(PageNum inPageNum, uint32_t inBufNum);
	void Insert(PageNum inPageNum, uint32_t inBufNum);

private:

	uint32_t Hash_Private(PageNum inValue) const;

	struct HashEntry
	{
		PageNum pageNum;
		uint32_t bufNum;
		HashEntry *next;
	};

	uint32_t mTableSize;
	HashEntry **mppHash;
};

class Frame
{
public:
	Frame();
	~Frame();

private:
	PageNum mPageNum;
};

class Replacer
{
public:
	Replacer(uint32_t inBufCount, BufDesc *io_pBufDescs);
	~Replacer();

	Status IncreaseUsageCount(uint32_t inBufNum);
	Status DecreaseUsageCount(uint32_t inBufNum);
	uint32_t PickVictim();

private:
	uint32_t mBufCount;
	BufDesc *mpBufDescs;
};

class BufDesc
{
private:
	friend class BufMgr;
	friend class Replacer;
	BufDesc();
	~BufDesc();

	PageNum mPageNum;
	uint32_t mBufNum;
	uint16_t mRefCount;
	uint16_t mUsageCount; //Used by Replacer
	bool mDirty;
};

class BufMgr
{
public:

	BufMgr(uint32_t inBufCount, uint32_t inPageSize, File *io_file);
	~BufMgr();

	Status PinPage(PageNum inPageNum, Page *o_page);
	Status UnpinPage(PageNum inPageNum, bool inDirty);
	Status NewPage(PageNum *o_pageNum);

	Status GetTotalPages(PageNum *o_pageCount);

private:
	BufHashTable *mpBufHash;
	BufDesc *mpBufDescs;
	Replacer *mpReplacer;
	char **mppBufData;
	char *mpTempBuf; //size of mPageSize

	uint32_t mBufCount;
	uint32_t mPageSize;
	File *mpFile;
};

}	// PandaSQL

#endif	// PANDASQL_BUFFER_MGR_H