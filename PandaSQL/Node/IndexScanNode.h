#ifndef PANDASQL_INDEX_SCAN_NODE_H
#define PANDASQL_INDEX_SCAN_NODE_H

#include "Node/PlanNode.h"

#include "Utils/Value.h"

namespace PandaSQL
{

class RelNode;
class TupleIterator;

class IndexScanNode : public PlanNode
{

public:

	IndexScanNode(PlanContext *io_pPlanContext, uint32_t inRelIndex);
	virtual ~IndexScanNode();

	virtual void Reset();
	virtual bool Step();
	virtual void End();

	virtual void SetupProjection(const TableAndColumnSetMap &inRequiredColumns);
	virtual void SetupPredicate_Recursive(const BooleanExpr &inPredicateExpr, Bitmask *io_tableMask);

private:

	uint32_t mRelIndex;
	TupleIterator *mpTupleIterator;
	TupleDesc mTupleDesc;
	UInt32List mProjectionList;
};

}	// namespace PandaSQL

#endif	// PANDASQL_INDEX_SCAN_NODE_H