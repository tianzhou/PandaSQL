#ifndef BAMBOO_ERROR_H
#define BAMBOO_ERROR_H

#include "BambooDB/BambooDBDef.h"

#include "BambooDB/Utils/Types.h"

namespace BambooDB
{

class BAMBOODB_API Error
{
public:

	enum Code
	{
		kOK = 0,
		kInternalError = 1,
		kEOF = 2,
	};

	Error() : mError(kOK) {}
	Error(Code inCode) : mError(inCode) {}
	~Error() {}

	Error(const Error& rhs);
	Error& operator=(const Error &rhs);

	bool OK() const { return mError == kOK; }
	bool IsEOF() const { return mError == kEOF; }

	Code GetError() const { return mError; }
	void SetError(Code inError) { mError = inError; }

private:

	Code mError;
};

inline Error::Error(const Error& rhs)
{
	mError = rhs.GetError();
}

inline Error& Error::operator=(const Error& rhs)
{
	if (this != &rhs)
	{
		this->SetError(rhs.GetError());
	}

	return *this;
}


}	// BambooDB

#endif	// BAMBOO_ERROR_H