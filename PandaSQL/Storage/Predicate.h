#ifndef PANDASQL_PREDICATE_H
#define PANDASQL_PREDICATE_H

#include "Utils\Types.h"

#include <vector>

namespace PandaSQL
{

class Predicate
{
public:

	enum PredicateType
	{
		kUnknown,
		kEqual,
		kNotEqual,
		kLess,
		kGreater,
		kLessEqual,
		kGreaterEqual
	};

	struct PredicateItem
	{
		uint32_t fieldNum;
		DataType valueType;
		std::string value;
		PredicateType predicateType;

		PredicateItem();
		PredicateItem(uint32_t inFieldNum, DataType inValueType, const std::string &inValue, PredicateType inPredicateType);
	};

	Predicate();
	~Predicate();

	uint32_t Count() const;
	uint32_t GetFieldNumOfItem(uint32_t index) const;
	DataType GetDataTypeOfItem(uint32_t index) const;
	void GetDataOfItem(uint32_t index, std::string *o_data) const;
	PredicateType GetPredicateTypeOfItem(uint32_t index) const;

	virtual void AppendPredicateItem(uint32_t inFieldNum, DataType inValueType, const std::string &inData, PredicateType inPredicateType);
	virtual void SetPredicateItem(uint32_t index, uint32_t inFieldNum, DataType inType, const std::string &inData, PredicateType inPredicateType);

private:
	Predicate(const Predicate &rhs);
	Predicate& operator=(const Predicate &rhs);

	std::vector<PredicateItem> mPredicateList;
};

}	// PandaSQL

#endif	// PANDASQL_PREDICATE_H