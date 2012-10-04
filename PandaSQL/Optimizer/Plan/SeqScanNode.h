#ifndef PANDASQL_SEQ_SCAN_NODE_H
#define PANDASQL_SEQ_SCAN_NODE_H

#include "Optimizer/Plan/PlanNode.h"

#include "Utils/Value.h"

namespace PandaSQL
{

class RelNode;
class TupleIterator;

class SeqScanNode : public PlanNode
{

public:

	SeqScanNode(PlanContext *io_pPlanContext, uint32_t inRelIndex);
	virtual ~SeqScanNode();

	virtual void Start();
	virtual bool Step();
	virtual void End();

private:

	uint32_t mRelIndex;
	TupleIterator *mpTupleIterator;
};

}	// namespace PandaSQL

#endif	// PANDASQL_SEQ_SCAN_NODE_H