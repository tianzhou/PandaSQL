#ifndef PANDASQL_JOIN_PATH_H
#define PANDASQL_JOIN_PATH_H

#include <vector>

#include "Utils/Types.h"

namespace PandaSQL
{

class RelNode;
class BooleanExpr;

typedef std::vector<uint32_t> JoinPath;

struct JoinInfo
{
};

typedef std::vector<JoinInfo> JoinInfoList;

//void GenerateJoinInfo(const RelNode &inLeftNode, const RelNode &inRightNode, const BooleanExpr &inPredicateExpr, JoinInfo *io_joinInfo);

}	// namespace PandaSQL

#endif	// PANDASQL_JOIN_PATH_H