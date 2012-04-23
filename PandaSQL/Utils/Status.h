#ifndef PANDA_SQL_STATUS_H
#define PANDA_SQL_STATUS_H

namespace PandaSQL
{

class Status
{
public:
	enum Code
	{
		kOK = 0,
		kError = 1
	};

	Status() : mState(kOK) {}
	Status(Code inCode) : mState(inCode) {}
	~Status() {}

	Status(const Status& rhs);
	Status& operator=(const Status& rhs);

	bool IsOK() const { return mState == kOK; }

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

#endif	// PANDA_SQL_STATUS_H