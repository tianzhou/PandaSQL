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
		kUnknownNode = 0,
	};
	Node();
	~Node();

	NodeType mType;

};

}	// PandaSQL

#endif	// PANDASQL_NODE_H