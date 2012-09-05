#ifndef PANDASQL_SEQ_SCAN_NODE_H
#define PANDASQL_SEQ_SCAN_NODE_H

#include "Optimizer/Plan/PlanNode.h"

namespace PandaSQL
{

class RelNode;

class SeqScanNode : public PlanNode
{

public:

	SeqScanNode(PlanContext *inPlanContext, uint32_t inRelIndex);

	virtual void ExecutePlan() const;

private:

	uint32_t mRelIndex;
};

}	// namespace PandaSQL

#endif	// PANDASQL_SEQ_SCAN_NODE_H