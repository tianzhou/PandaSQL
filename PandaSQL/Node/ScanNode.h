#ifndef PANDASQL_SCAN_NODE_H
#define PANDASQL_SCAN_NODE_H

#include "Node/PlanNode.h"

#include "Utils/Value.h"

namespace PandaSQL
{

class RelNode;
class Table;
class TupleIterator;

class ScanNode : public PlanNode
{

public:

	ScanNode(NodeType inNodeType, PlanContext *io_pPlanContext, uint32_t inRelIndex);
	virtual ~ScanNode() = 0;

	virtual void Reset();
	virtual bool Step();
	virtual void End();

	virtual void SetupProjection(const TableAndColumnSetMap &inRequiredColumns);
	virtual void SetupPredicate_Recursive(const BooleanExpr &inPredicateExpr, Bitmask *io_tableMask);

protected:

	//Derive class might return NULL if fail to create.
	//In that case, mLastStatus should be set properly
	virtual TupleIterator* CreateScanIterator(const Table *pTable) = 0;

private:

	void InitTupleIteratorIfNeeded();

protected:

	uint32_t mRelIndex;
	TupleIterator *mpTupleIterator;
	TupleDesc mTupleDesc;
	UInt32List mProjectionList;
};

}	// namespace PandaSQL

#endif	// PANDASQL_SCAN_NODE_H