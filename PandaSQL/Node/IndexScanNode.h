#ifndef PANDASQL_INDEX_SCAN_NODE_H
#define PANDASQL_INDEX_SCAN_NODE_H

#include "Node/ScanNode.h"

#include "Utils/Value.h"

namespace PandaSQL
{

class IndexScanNode : public ScanNode
{

public:

	IndexScanNode(PlanContext *io_pPlanContext, uint32_t inRelIndex);
	virtual ~IndexScanNode();

	//Derived from ScanNode
	//virtual void Reset();
	//virtual bool Step();
	//virtual void End();

	//virtual void SetupProjection(const TableAndColumnSetMap &inRequiredColumns);
	//virtual void SetupPredicate_Recursive(const BooleanExpr &inPredicateExpr, Bitmask *io_tableMask);

protected:

	virtual TupleIterator* CreateScanIterator(const Table *pTable);

private:

};

}	// namespace PandaSQL

#endif	// PANDASQL_INDEX_SCAN_NODE_H