#ifndef PANDASQL_REL_NODE_H
#define PANDASQL_REL_NODE_H

#include "Optimizer/Node.h"

#include "Utils/Value.h"

namespace PandaSQL
{

class Table;

class RelNode : public Node
{

public:

	RelNode(const Table *inTable);
	~RelNode();

	const Table* GetTable() const { return mpTable; }
	ValueList* GetTupleValue() { return &mTupleValueList; }

private:

	const Table *mpTable;
	ValueList mTupleValueList;


};

}	// namespace PandaSQL

#endif	// PANDASQL_REL_NODE_H