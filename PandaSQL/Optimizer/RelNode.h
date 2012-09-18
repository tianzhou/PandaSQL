#ifndef PANDASQL_REL_NODE_H
#define PANDASQL_REL_NODE_H

#include "Optimizer/Node.h"

#include "Utils/Value.h"

namespace PandaSQL
{

class Table;

typedef std::vector<uint32_t> ProjectionList;
typedef std::vector<uint32_t> PredicateList;

class RelNode : public Node
{

public:

	enum RelType
	{
		kRelUnknown = 0,
		kRelBase = 1,
		kRelJoin = 2
	};

	RelNode(const Table *inTable);
	~RelNode();

	const Table* GetTable() const { return mpTable; }
	ValueList* GetTupleValue() { return &mTupleValueList; }

	const ProjectionList& GetProjectionList() const { return mProjectionList; }
	void SetProjectionList(const ProjectionList &inProjectionList) { mProjectionList = inProjectionList; }

	const PredicateList& GetPredicateList() const { return mPredicateList; }
	void SetPredicateList(const PredicateList &inPredicateList) { mPredicateList = inPredicateList; }

private:

	const Table *mpTable;
	ValueList mTupleValueList;

	ProjectionList mProjectionList;
	PredicateList mPredicateList;


};

}	// namespace PandaSQL

#endif	// PANDASQL_REL_NODE_H