#ifndef PANDASQL_BUFMGR_H
#define PANDASQL_BUFMGR_H

#include "Utils/Types.h"
#include "Utils/Status.h"

namespace PandaSQL
{

class Page;
class File;
class BufMgr;

class BufHashTable
{
public:
	BufHashTable();
	~BufHashTable();

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

class BufDesc
{
private:
	friend class BufMgr;
	BufDesc();
	~BufDesc();

	uint32_t mPageNum;
	uint8_t mBufNum;
};

class BufMgr
{

public:

	BufMgr(uint32_t inPageSize, uint32_t inBufCount, File *io_file);
	~BufMgr();

	Status PinPage(uint32_t pageNum, Page **o_page);
	Status UnpinPage(uint32_t pageNum);

private:

	BufDesc *mpBufs;
	uint32_t mBufCount;
};

}	// PandaSQL

#endif	// PANDASQL_BUFFER_MGR_H