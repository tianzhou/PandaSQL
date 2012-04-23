tree grammar SQLSemanticAnalyzer;

options 
{
    tokenVocab	    = SQL;
    language	    = C;
    ASTLabelType	= pANTLR3_BASE_TREE;
}

@treeparser::header
{


#include "Parser/SQLParserDriver.h"


}

//Starting rule
statement[PandaSQL::SQLParserDriver *io_pDriver]
scope
{
	PandaSQL::SQLParserDriver *pDriver;
}
@init
{
	$statement::pDriver = io_pDriver;
}
	:	dml_statement
	;
	
dml_statement
	:	select_statement
	;
	
select_statement
@init
{
	printf("***select_statement begin***\n");
}
@after
{
	printf("***select_statement end***\n");
}
	:	^(KW_SELECT select_list from_clause)
	{
		PandaSQL::SQLParserDriver *pDriver = $statement::pDriver;
		pDriver->PrintCurrentState();
	}
	;
	
select_list
	:	^(TOK_COLUMN_LIST TOK_ALL_COLUMNS)
	{
		printf("selectList: *\n");
	}
	;
	
from_clause
	:	^(TOK_FROM tableName=table_name)
	{
		printf("tableName: \%s\n", $tableName.text->chars);
	} 	
	;
	
table_name
	:	^(TOK_TABLE_NAME IDENTIFIER)
	{
		
	}
	;
