#include "stdafx.h"

#include "Expr/BinaryExpr.h"

#include "Utils/Debug.h"
#include "Utils/Value.h"

namespace PandaSQL
{

BinaryExpr::BinaryExpr()
:
Expr(kExprBinary)
,mOpType(kBinaryUnknown)
,mpLeftOperand(NULL)
,mpRightOperand(NULL)
{
}

BinaryExpr::~BinaryExpr()
{
	delete mpLeftOperand;
	mpLeftOperand = NULL;
	delete mpRightOperand;
	mpRightOperand = NULL;
}

BinaryExpr::BinaryOpType BinaryExpr::GetOpType() const
{
	return mOpType;
}

void BinaryExpr::SetOpType(BinaryExpr::BinaryOpType inOpType)
{
	mOpType = inOpType;
}

Expr* BinaryExpr::GetLeftOperand() const
{
	return mpLeftOperand;
}

void BinaryExpr::SetLeftOperand(Expr *inLeftOperand)
{
	mpLeftOperand = inLeftOperand;
}

Expr* BinaryExpr::GetRightOperand() const
{
	return mpRightOperand;
}

void BinaryExpr::SetRightOperand(Expr *inRightOperand)
{
	mpRightOperand = inRightOperand;
}

bool BinaryExpr::IsTrue(const ExprContext &inExprContext) const
{
	bool result = true;

	PDASSERT(mpLeftOperand && mpRightOperand);

	if (mpLeftOperand && mpRightOperand)
	{
		Value leftValue, rightValue;
		
		mpLeftOperand->Eval(inExprContext, &leftValue);
		mpRightOperand->Eval(inExprContext, &rightValue);

		if (leftValue.GetType() == kUnknownType
			|| rightValue.GetType() == kUnknownType)
		{
			// It's a logic error if the current context doesn't include
			// the value for the operand, this means predicate(probably pushdown)
			// is not setup properly
			PDASSERT(0);
		}
		else
		{
			switch (mOpType)
			{
			case kBinaryEqual:
				{
					result = (leftValue == rightValue);
					break;
				}
			case kBinaryNotEqual:
				{
					result = (leftValue != rightValue);
					break;
				}
			case kBinaryGreater:
				{
					result = (leftValue > rightValue);
					break;
				}
			case kBinaryGreaterEqual:
				{
					result = (leftValue >= rightValue);
					break;
				}
			case kBinaryLess:
				{
					result = (leftValue < rightValue);
					break;
				}
			case kBinaryLessEqual:
				{
					result = (leftValue <= rightValue);
					break;
				}
			default:
				PDASSERT(0);
				break;
			}
		}
	}

	return result;
}

Expr* BinaryExpr::CreateSubExprForPushdown(const std::vector<std::string> &inTableNameList) const
{
	PDASSERT(mpLeftOperand && mpRightOperand);

	Expr *result = NULL;

	Expr *leftExpr = NULL;
	if (mpLeftOperand)
	{
		leftExpr = mpLeftOperand->CreateSubExprForPushdown(inTableNameList);
	}

	if (leftExpr)
	{
		Expr *rightExpr = NULL;

		if (mpRightOperand)
		{
			rightExpr = mpRightOperand->CreateSubExprForPushdown(inTableNameList);
		}

		if (rightExpr)
		{
			BinaryExpr *newBinaryExpr = new BinaryExpr();
			
			newBinaryExpr->SetOpType(this->GetOpType());
			newBinaryExpr->SetLeftOperand(leftExpr);
			newBinaryExpr->SetRightOperand(rightExpr);

			result = newBinaryExpr;
		}
		else
		{
			delete leftExpr;
			leftExpr = NULL;
		}
	}

	return result;
}

void BinaryExpr::Walk(ExprWalker *io_walker) const
{
	PDASSERT(mpLeftOperand && mpRightOperand);

	if (mpLeftOperand)
	{
		mpLeftOperand->Walk(io_walker);
	}
	
	if (mpRightOperand)
	{
		mpRightOperand->Walk(io_walker);
	}
}

void BinaryExpr::MutableWalk(MutableExprWalker *io_walker)
{
	PDASSERT(mpLeftOperand && mpRightOperand);

	if (mpLeftOperand)
	{
		mpLeftOperand->MutableWalk(io_walker);
	}
	
	if (mpRightOperand)
	{
		mpRightOperand->MutableWalk(io_walker);
	}
}

Expr* BinaryExpr::Clone() const
{
	BinaryExpr *result = new BinaryExpr();

	result->mOpType = mOpType;

	if (mpLeftOperand)
	{
		result->mpLeftOperand = mpLeftOperand->Clone();
	}

	if (mpRightOperand)
	{
		result->mpRightOperand = mpRightOperand->Clone();
	}

	return result;
}

void BinaryExpr::Print(uint32_t level) const
{
	this->PrintIndention(level);
	level++;

	switch (mOpType)
	{
	case kBinaryUnknown:
		{
			std::cout << "Unknown" << std::endl;
			break;
		}
	case kBinaryEqual:
		{
			std::cout << "=" << std::endl;
			break;
		}
	case kBinaryNotEqual:
		{
			std::cout << "!=" << std::endl;
			break;
		}
	case kBinaryGreater:
		{
			std::cout << ">" << std::endl;
			break;
		}
	case kBinaryGreaterEqual:
		{
			std::cout << ">=" << std::endl;
			break;
		}
	case kBinaryLess:
		{
			std::cout << "<" << std::endl;
			break;
		}
	case kBinaryLessEqual:
		{
			std::cout << "<=" << std::endl;
			break;
		}
	default:
		break;
	}

	if (mpLeftOperand)
	{
		mpLeftOperand->Print(level);
	}

	if (mpRightOperand)
	{
		mpRightOperand->Print(level);
	}
}

}	// PandaSQL