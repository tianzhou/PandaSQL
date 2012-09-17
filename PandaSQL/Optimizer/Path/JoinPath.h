#ifndef PANDASQL_JOIN_PATH_H
#define PANDASQL_JOIN_PATH_H

#include "Optimizer/RelNode.h"

#include <vector>

namespace PandaSQL
{

typedef std::vector<uint32_t> JoinPath;

struct JoinInfo
{
	std::vector<uint32_t> mPredicateIndexList;
};

}	// namespace PandaSQL

#endif	// PANDASQL_JOIN_PATH_H