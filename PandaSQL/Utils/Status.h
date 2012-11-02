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
		kInternalError = 1,
		kEOF = 2,
		kMemoryError = 3,
		kIOError = 4,
		kTableMissing = 5,
		kColumnMissing = 6,
		kInvalidExpression = 7,
		kInvalidCommand = 8,
		kTableAlreadyExists = 9,
		kAmbiguousColumn = 10,
		kSQLSyntaxError = 11,
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