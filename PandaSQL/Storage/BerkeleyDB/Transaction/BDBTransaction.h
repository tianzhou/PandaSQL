#ifndef PANDASQL_BDB_TRANSACTION_H
#define PANDASQL_BDB_TRANSACTION_H

#include <db_cxx.h>

namespace PandaSQL
{

int TransactionBegin(DB_ENV *env, DB_TXN *parent, DB_TXN **tid);
int TransactionCommit(DB_TXN *tid);
int TransactionAbort(DB_TXN *tid);

}	// PandaSQL

#endif	// PANDASQL_BDB_TRANSACTION_H