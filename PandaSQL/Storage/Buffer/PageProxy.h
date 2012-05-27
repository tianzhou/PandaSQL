#ifndef PANDASQL_PAGEPROXY_H
#define PANDASQL_PAGEPROXY_H

//#include "VFS/File.h"

#include "Utils/Types.h"
#include "Utils/Status.h"

namespace PandaSQL
{

class File;
class BufMgr;

class PageProxy
{
public:

	PageProxy(uint32_t inPageSize, File *io_pagedFile);
	~PageProxy();

	//Status ReadPage(uint32_t inPageNum, File::Offset inPageOffset, File::Size amount, void *o_buf, File::Size *o_bytesRead);
	//Status WritePage(uint32_t inPageNum, File::Offset inPageOffset, File::Size amount, const void *inBuf, File::Size *o_bytesWritten);

	//For internal locking
	enum AccessMode
	{
		kPageRead = 0x0001,
		kPageWrite = 0x0002,
	};

	typedef int AccessFlags;

	//Page num is zero based
	Status GetPage(uint32_t inPageNum, AccessFlags inAccessMode, char **o_pageData);
	Status PutPage(uint32_t inPageNum, bool inDirty, const char *inPageData);
	Status NewPage(uint32_t *o_pageNum);

	Status GetPageCount(uint32_t *o_pageCount) const;

private:

	//uint32_t mPageSize;
	BufMgr *mpBufMgr;
	//File *mpPagedFile;
	

};

}	// PandaSQL

#endif	// PANDASQL_PAGEPROXY_H