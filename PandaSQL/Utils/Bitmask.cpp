#include "stdafx.h"

#include "Utils/Bitmask.h"

#include "Utils/Debug.h"

namespace PandaSQL
{

Bitmask::Bitmask(size_t inLength)
:
mLength(inLength)
,mpBits(new bool[mLength])
{
	this->ClearAll();
}

Bitmask::~Bitmask()
{
	delete []mpBits;
	mpBits = NULL;
}

bool Bitmask::GetBit(size_t index) const
{
	PDASSERT(index < mLength);

	return mpBits[index];
}

void Bitmask::SetBit(size_t index, bool bit)
{
	PDASSERT(index < mLength);
	mpBits[index] = bit;
}

void Bitmask::SetBit(size_t low, size_t high, bool bit)
{
	PDASSERT(low <= high);
	PDASSERT(high < mLength);

	size_t index;
	for (index = low; index <= high; index++)
	{
		mpBits[index] = bit;
	}
}

bool Bitmask::IsClear() const
{
	bool result = true;

	size_t i;
	for (i = 0; i < this->GetLength(); i++)
	{
		if (GetBit(i))
		{
			result = false;
			break;
		}
	}

	return result;
}

void Bitmask::ClearAll()
{
	SetBit(0, this->GetLength() - 1, 0);
}

void Bitmask::Union(const Bitmask &left, const Bitmask &right)
{
	PDASSERT(left.GetLength() == right.GetLength());
	PDASSERT(this->GetLength() == left.GetLength());

	size_t i;
	for (i = 0; i < this->GetLength(); i++)
	{
		this->SetBit(i, left.GetBit(i) | right.GetBit(i));
	}
}

}	// PandaSQL
