#ifndef PANDASQL_BITMASK_H
#define PANDASQL_BITMASK_H

#include "Utils/Types.h"

namespace PandaSQL
{

class Bitmask
{

public:

	Bitmask(uint32_t inLength);
	~Bitmask();

	bool GetBit(uint32_t index) const;
	void SetBit(uint32_t index, bool bit);
	void SetBit(uint32_t low, uint32_t high, bool bit);
	
	bool IsClear() const;
	void ClearAll();
	uint32_t GetLength() const { return mLength; }

	void Union(const Bitmask &left, const Bitmask &right);
	
private:

	uint32_t mLength;
	bool *mpBits;

};


}	// PandaSQL


#endif	// PANDASQL_BITMASK_H