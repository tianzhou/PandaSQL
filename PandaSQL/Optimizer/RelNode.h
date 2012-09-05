#ifndef PANDASQL_REL_NODE_H
#define PANDASQL_REL_NODE_H

#include "Optimizer/Node.h"

namespace PandaSQL
{

class Table;

class RelNode : public Node
{

public:

	RelNode(const Table *inTable);
	~RelNode();

private:

	const Table *mpTable;


};

}	// namespace PandaSQL

#endif	// PANDASQL_REL_NODE_H