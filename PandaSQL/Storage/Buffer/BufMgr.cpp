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

//**************BufMgr*****************

BufMgr::BufMgr()
{
}

BufMgr::~BufMgr()
{
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
