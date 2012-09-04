#ifndef PANDASQL_NEST_LOOP_NODE_H
#define PANDASQL_NEST_LOOP_NODE_H

#include "Optimizer/Plan/PlanNode.h"

namespace PandaSQL
{

class JoinPath;

class NestLoopNode : public PlanNode
{

public:

	NestLoopNode(const JoinPath &inJoinPath);

	const PlanNode& GetOuterNode() const;
	const PlanNode& GetInnerNode() const;

private:


};

}	// namespace PandaSQL

#endif	// PANDASQL_NEST_LOOP_NODE_H