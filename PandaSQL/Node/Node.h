#ifndef PANDASQL_NODE_H
#define PANDASQL_NODE_H

#include "Utils/Types.h"
#include "Utils/Status.h"

namespace PandaSQL
{

class Node
{
public:

	enum NodeType
	{
		kNodeUnknown = 0,

		//Base node
		kNodeSeqScan = 1,
		kNodeIndexScan = 2,

		//Intermediate node
		kNodeNestLoop = 10,

		//Object node
		kNodeRelation = 100,
	};

	Node(NodeType inNodeType);
	virtual ~Node();

	NodeType GetNodeType() const { return mType; }

protected:

	NodeType mType;

};

}	// PandaSQL

#endif	// PANDASQL_NODE_H