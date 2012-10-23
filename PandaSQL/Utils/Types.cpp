#include "stdafx.h"

#include "Utils/Types.h"

namespace PandaSQL
{

char *kNewLineSymbol = "\r\n";
uint32_t kUnknownID = 0;
uint32_t kBeginID = 1;
uint32_t kInvalidColumnIndex = -1;

OpenOptions::OpenOptions()
:
create_if_missing(false)
{
}

}	// PandaSQL