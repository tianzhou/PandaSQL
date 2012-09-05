#include "stdafx.h"

#include "RelNode.h"

#include "Catalog/Table.h"

namespace PandaSQL
{

RelNode::RelNode(const Table *inTable)
:
Node(kNodeRelation)
,mpTable(inTable)
{
}

RelNode::~RelNode()
{
}

}	// PandaSQL
