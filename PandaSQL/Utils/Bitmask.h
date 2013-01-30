#ifndef PANDASQL_BITMASK_H
#define PANDASQL_BITMASK_H

#include "Utils/Types.h"

namespace PandaSQL
{

class Bitmask
{

public:

	Bitmask(size_t inLength);
	~Bitmask();

	bool GetBit(size_t index) const;
	void SetBit(size_t index, bool bit);
	void SetBit(size_t low, size_t high, bool bit);
	
	bool IsClear() const;
	void ClearAll();
	size_t GetLength() const { return mLength; }

	void Union(const Bitmask &left, const Bitmask &right);
	
private:

	size_t mLength;
	bool *mpBits;

};


}	// PandaSQL


#endif	// PANDASQL_BITMASK_H