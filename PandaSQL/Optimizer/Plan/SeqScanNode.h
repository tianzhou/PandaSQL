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

	virtual void Reset();
	virtual bool Step();
	virtual void End();

	virtual void SetupProjection(const TableAndColumnSetMap &inRequiredColumns);
	virtual void SetupPredicate_Recursive(const BooleanExpr &inPredicateExpr, Bitmask *io_tableMask);

private:

	uint32_t mRelIndex;
	TupleIterator *mpTupleIterator;
	ColumnIndexList mProjectionList;
};

}	// namespace PandaSQL

#endif	// PANDASQL_SEQ_SCAN_NODE_H