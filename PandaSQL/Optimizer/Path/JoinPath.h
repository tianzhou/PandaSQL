#ifndef PANDASQL_JOIN_PATH_H
#define PANDASQL_JOIN_PATH_H

#include "Optimizer/RelNode.h"

#include <vector>

namespace PandaSQL
{

typedef std::vector<uint32_t> JoinPath;

struct JoinInfo
{
};

}	// namespace PandaSQL

#endif	// PANDASQL_JOIN_PATH_H