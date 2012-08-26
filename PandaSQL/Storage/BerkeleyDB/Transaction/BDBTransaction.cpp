#include "stdafx.h"

#include "BDBTransaction.h"

#include <iostream>

namespace PandaSQL
{

int TransactionBegin(DB_ENV *env, DB_TXN *parent, DB_TXN **tid)
{
	u_int32_t flags = DB_READ_COMMITTED | DB_TXN_SYNC | DB_TXN_WAIT;
	return env->txn_begin(env, parent, tid, flags);
}

int TransactionCommit(DB_TXN *tid)
{
	u_int32_t flags = DB_TXN_SYNC;
	return tid->commit(tid, flags);
}

int TransactionAbort(DB_TXN *tid)
{
	return tid->abort(tid);
}

}	// PandaSQL