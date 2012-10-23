#include "stdafx.h"

#include "Node/RelNode.h"

#include "Catalog/Table.h"

namespace PandaSQL
{

RelNode::RelNode(const Table *inTable)
:
Node(kNodeRelation)
,mRelType(kRelBase)
,mpTable(inTable)
{
}

RelNode::~RelNode()
{
}

}	// PandaSQL
