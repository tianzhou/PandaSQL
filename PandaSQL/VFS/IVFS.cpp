#include "stdafx.h"

#include "VFS/IVFS.h"
#include "VFS/WinVFS.h"

namespace PandaSQL
{

//static
IVFS* IVFS::CreateVFS()
{
	//Only have windows for now
	return new WinVFS();
}

IVFS::IVFS()
{
}

}	// FileSystem