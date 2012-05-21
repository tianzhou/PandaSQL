#include "stdafx.h"

#include "BufMgr.h"

namespace PandaSQL
{

//**************BufHashTable*****************

BufHashTable::BufHashTable()
{
}

BufHashTable::~BufHashTable()
{

}

//**************Frame*****************

Frame::Frame()
{
}

Frame::~Frame()
{
}

//**************BufDesc*****************

BufDesc::BufDesc()
{
}

BufDesc::~BufDesc()
{
}

//**************BufMgr*****************

BufMgr::BufMgr(uint32_t inPageSize, uint32_t inBufCount, File *io_file)
:
mpBufs(new BufDesc[inBufCount])
,mBufCount(inBufCount)
{
}

BufMgr::~BufMgr()
{
	delete []mpBufs;
}

Status BufMgr::PinPage(uint32_t pageNum, Page **o_page)
{
	Status result;

	return result;
}

Status BufMgr::UnpinPage(uint32_t pageNum)
{
	Status result;

	return result;
}

}	// PandaSQL
