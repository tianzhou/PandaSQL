#ifndef PANDASQL_BUFMGR_H
#define PANDASQL_BUFMGR_H

#include "Utils/Types.h"
#include "Utils/Status.h"

namespace PandaSQL
{

class Page;

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

class BufMgr
{

public:

	BufMgr();
	~BufMgr();

	Status PinPage(uint32_t pageNum, Page **o_page);
	Status UnpinPage(uint32_t pageNum);

private:

};

}	// PandaSQL

#endif	// PANDASQL_BUFFER_MGR_H