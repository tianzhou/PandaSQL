grammar SQL;
options 
{
    // Note that in the C implementation, all implementations of trees and
    // adaptors pass around pANTLR3_BASE_TREE, which contains a super pointer
    // to your own implementation of a tree node and tree and so on. Hence
    // the node type is ALWAYS pANTLR3_BASE_TREE and there is no need to define
    // the type (the definition is silently ignored if you leave it there)
    //
	output	    = AST;
	language    = C;
    ASTLabelType	= pANTLR3_BASE_TREE;
}

tokens
{
TOK_ALL_COLUMNS;
TOK_COLUMN_LIST;
TOK_FROM;
TOK_TABLE_NAME;
}

@parser::includes
{
}

@lexer::includes
{
}


//Starting rule
statement
	:	dml_statement^ (';' | EOF)!
	;
	
dml_statement
	:	select_statement
	;
	
select_statement
	:	KW_SELECT select_list from_clause -> ^(KW_SELECT select_list from_clause)
	;
	
select_list
	:	'*'	-> ^(TOK_COLUMN_LIST TOK_ALL_COLUMNS)
	;
	
from_clause
	:	KW_FROM table_name -> ^(TOK_FROM table_name)	
	;
	
table_name
	:	IDENTIFIER -> ^(TOK_TABLE_NAME IDENTIFIER)
	;


KW_FROM
	: ('F'|'f')('R'|'r')('O'|'o')('M'|'m')
	;
KW_SELECT
	: ('S'|'s')('E'|'e')('L'|'l')('E'|'e')('C'|'c')('T'|'t')
	;
KW_TABLE
	: ('T'|'t')('A'|'a')('B'|'b')('L'|'l')('E'|'e')
	;

IDENTIFIER
	:	(Letter | Digit)(Letter | Digit | '_')*
	;
	
WS
	:  (' '|'\r'|'\t'|'\n') {$channel=HIDDEN;}
	;

fragment
Letter
	:	'a'..'z' | 'A'..'Z'
	;

fragment
Digit
	:	'0'..'9'
	;