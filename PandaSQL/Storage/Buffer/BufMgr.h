#ifndef PANDASQL_BUFMGR_H
#define PANDASQL_BUFMGR_H

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

	uint32_t Lookup(uint32_t inPageNum) const;
	void Remove(uint32_t inPageNum, uint32_t inBufNum);
	void Insert(uint32_t inPageNum, uint32_t inBufNum);

private:

	uint32_t Hash_Private(uint32_t inValue) const;

	struct HashEntry
	{
		uint32_t pageNum;
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
	uint32_t mPageNum;
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

	uint32_t mPageNum;
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

	Status PinPage(uint32_t inPageNum, Page *o_page);
	Status UnpinPage(uint32_t inPageNum);
	Status NewPage(uint32_t *o_pageNum, Page *o_page);

	Status GetTotalPages(uint32_t *o_pageNum);

private:
	BufHashTable *mpBufHash;
	BufDesc *mpBufDescs;
	Replacer *mpReplacer;
	char **mppBufData;

	uint32_t mBufCount;
	uint32_t mPageSize;
	File *mpFile;
};

}	// PandaSQL

#endif	// PANDASQL_BUFFER_MGR_H