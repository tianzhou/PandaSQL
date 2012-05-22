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
	BufHashTable();
	~BufHashTable();

	uint32_t Lookup(uint32_t inPageNum);
	void Remove(uint32_t inBufNum);
	void Insert(uint32_t inPageNum, uint32_t inBufNum);

private:



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

private:
	BufHashTable *mpBufHash;
	BufDesc *mpBufDescs;
	Replacer *mpReplacer;
	char **mpBufData;

	uint32_t mBufCount;
	uint32_t mPageSize;
	File *mpFile;
};

}	// PandaSQL

#endif	// PANDASQL_BUFFER_MGR_H