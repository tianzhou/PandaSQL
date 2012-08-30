#ifndef PANDASQL_CONSTANT_EXPR_H
#define PANDASQL_CONSTANT_EXPR_H

#include "Expr.h"

namespace PandaSQL
{

class ConstantExpr : public Expr
{
public:

	enum 
	{
		kConstantUnknown = 0,
		kConstantInt = 1,
		kConstantText = 2,
	};

	typedef uint8_t ConstantType;

	ConstantExpr();
	~ConstantExpr();

	ConstantType GetType() const;
	void SetType(ConstantType inType);

	int32_t GetInt() const;
	void SetInt(int32_t inIntValue);
	
	std::string GetText() const;
	void SetText(const std::string inTextValue);

private:

	ConstantType mConstantType;
	
	//mConstantType == kConstantInt
	int32_t mIntValue;

	//mConstantType == kConstantText
	std::string mTextValue;
};

}	// PandaSQL


#endif //PANDASQL_CONSTANT_EXPR_H
