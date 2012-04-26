tree grammar SQLSemanticAnalyzer;

options 
{
    tokenVocab	    = SQL;
    language	    = C;
    ASTLabelType	= pANTLR3_BASE_TREE;
}

@treeparser::header
{

#include "Parser/ParserDriver.h"

}

//Starting rule
stmt[PandaSQL::ParserDriver *io_pDriver]
scope
{
	PandaSQL::ParserDriver *pDriver;
}
@init
{
	$stmt::pDriver = io_pDriver;
}
	:	dml_stmt
	;
	
dml_stmt
	:	select_stmt
	;
	
select_stmt
@init
{
	printf("***select_statement begin***\n");
}
@after
{
	printf("***select_statement end***\n");
}
	:	^(TOK_SELECT_STMT select_core order_by_clause? limit_clause?)
		{
			PandaSQL::ParserDriver *pDriver = $stmt::pDriver;
			pDriver->PrintCurrentState();
		}
	;
	
select_core
	:	^(TOK_SELECT_CORE set_qualifier select_list from_clause? where_clause? group_by_clause?)
	;
	
order_by_clause
	:	KW_ORDER KW_BY
		{
		}
	;
	
limit_clause
	:	KW_LIMIT
		{
		}
	;
	
set_qualifier
	:	KW_ALL
		{
		}
	|	KW_DISTINCT
		{
		}
	;
	
select_list
	:	^(TOK_SELECT_LIST TOK_ALL_COLUMNS)
		{
			printf("selectList: *\n");
		}
	|	^(TOK_SELECT_LIST column_ref+)
		{
			printf("selectColumnRef: *\n");
		}
	;
	
from_clause
@init
{
}
	:	^(TOK_FROM_CLAUSE table_ref)
		{
		} 	
	;
	
where_clause
	:	KW_WHERE
		{
		}
	;
	
group_by_clause
	:	KW_GROUP KW_BY
		{
		}
	;
	
column_ref
@init
{
	std::string columnRef;
	std::string tableRef = PandaSQL::Statement::kNoTable;
	bool hasTableRef = false;
}
	:	^(TOK_COLUMN_REF identifier[columnRef] identifier[tableRef]?)
		{
			PandaSQL::ParserDriver *pDriver = $stmt::pDriver;
			pDriver->GetStatement().AddColumnRef(tableRef, columnRef);
		}
	;
	
table_ref
@init
{
	std::string tableRef;
}
	:	^(TOK_TABLE_REF identifier[tableRef])
		{
			PandaSQL::ParserDriver *pDriver = $stmt::pDriver;
			pDriver->GetStatement().AddTableRef(tableRef);
		}
	;
	
identifier[std::string &o_str]
	:	id=IDENTIFIER
		{
			PandaSQL::ParserDriver::GetIdentifier($id, o_str);
		}
	;
