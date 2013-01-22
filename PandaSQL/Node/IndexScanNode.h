#ifndef PANDASQL_INDEX_SCAN_NODE_H
#define PANDASQL_INDEX_SCAN_NODE_H

#include "Node/ScanNode.h"

#include "Utils/Value.h"

namespace PandaSQL
{

class Index;

class IndexScanNode : public ScanNode
{

public:

	IndexScanNode(PlanContext *io_pPlanContext, uint32_t inRelIndex, const Index &inIndex);
	virtual ~IndexScanNode();

	//Derived from ScanNode
	//virtual void Reset();
	//virtual bool Step();
	//virtual void End();

	//virtual void SetupProjection(const TableAndColumnSetMap &inRequiredColumns);
	//virtual void SetupPredicate_Recursive(const BooleanExpr &inPredicateExpr, Bitmask *io_tableMask);

protected:

	virtual TupleIterator* CreateScanIterator();

private:

	const Index &mIndex;
};

}	// namespace PandaSQL

#endif	// PANDASQL_INDEX_SCAN_NODE_H