#ifndef PANDASQL_BOOLEAN_EXPR_H
#define PANDASQL_BOOLEAN_EXPR_H

#include "Expr.h"

namespace PandaSQL
{

class BooleanExpr : public Expr
{

public:

	enum
	{
		kBooleanUnknown = 0,
		kBooleanNormal = 1,
		kBooleanAndList = 2,
		kBooleanOrList = 3
	};

	typedef uint8_t BooleanType;

	typedef std::vector<const Expr *> BooleanList;

	BooleanExpr();
	~BooleanExpr();

	void SetNegate(bool isNegate);

	void SetType(BooleanType inType);

	const BooleanList& GetBooleanList() const;
	void AddExpr(const Expr *inExpr); 

private:
	
	BooleanType mType;
	bool		mNegate;
	BooleanList mBooleanList;
};

}	// PandaSQL


#endif //PANDASQL_BOOLEAN_EXPR_H
