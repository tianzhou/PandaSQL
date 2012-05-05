#ifndef PANDASQL_STATUS_H
#define PANDASQL_STATUS_H

namespace PandaSQL
{

class Status
{
public:
	enum Code
	{
		kOK = 0,
		kEOF = 1,
		kMemoryError = 2,
		kIOError = 3
	};

	Status() : mState(kOK) {}
	Status(Code inCode) : mState(inCode) {}
	~Status() {}

	Status(const Status& rhs);
	Status& operator=(const Status &rhs);

	bool OK() const { return mState == kOK; }
	bool IsEOF() const { return mState == kEOF; }

	Code GetCode() const { return mState; }
	void SetCode(Code inCode) { mState = inCode; }

private:
	Code mState;
};

inline Status::Status(const Status& rhs)
{
	mState = rhs.GetCode();
}

inline Status& Status::operator=(const Status& rhs)
{
	if (this != &rhs)
	{
		this->SetCode(rhs.GetCode());
	}

	return *this;
}

}	// namespace PandaSQL

#endif	// PANDASQL_STATUS_H