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
TOK_BINARY_OP;
TOK_COLUMN_REF;
TOK_CREATE_TABLE_STMT;
TOK_CREATE_INDEX_STMT;
TOK_COLUMN_DEF_LIST;
TOK_COLUMN_DEF;
TOK_DELETE_STMT;
TOK_DROP_TABLE;
TOK_DROP_INDEX;
TOK_FROM_CLAUSE;
TOK_PREDICATE_OR_LIST;
TOK_PREDICATE_AND_LIST;
TOK_SELECT_CORE;
TOK_SELECT_LIST;
TOK_SELECT_STMT;
TOK_INDEX_REF;
TOK_INSERT_STMT;
TOK_INSERT_VALUES;
TOK_UPDATE_CORE;
TOK_UPDATE_STMT;
TOK_UPDATE_SET_LIST;
TOK_UPDATE_SET;
TOK_TABLE_REF;
TOK_WHERE;
}

@parser::includes
{

#include "stdafx.h"
#include "Parser/ParserDriver.h"

}

@lexer::includes
{
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
@after
{
	if ($stmt.text)
	{
		std::string theStmt;
		theStmt.assign((const char *)$stmt.text->chars);
	
		PandaSQL::ParserDriver *pDriver = $stmt::pDriver;
		pDriver->GetStatement().SetOriginalStmtText(theStmt);
	}
}
	:	ddl_stmt^ SEMI!
	|	dml_stmt^ SEMI!
	|	EOF!
	;
	
ddl_stmt
	:	create_table_stmt
	|	create_index_stmt
	|	drop_table_stmt
	|	drop_index_stmt
	;
	
dml_stmt
	:	select_stmt
	|	update_stmt
	|	insert_stmt
	|	delete_stmt
	;
	
create_table_stmt
	:	KW_CREATE KW_TABLE table_ref LPAREN column_def_list RPAREN
		-> ^(TOK_CREATE_TABLE_STMT table_ref column_def_list)
	;
	
column_def_list
	:	column_def (COMMA column_def)* -> ^(TOK_COLUMN_DEF_LIST column_def+)
	;
	
column_def
	:	column_ref type_name column_constraint -> ^(TOK_COLUMN_DEF column_ref type_name column_constraint)
	;
	
type_name
	:	KW_INT
	|	KW_TEXT
	;
	
column_constraint
	:	KW_PRIMARY KW_KEY
	|	KW_UNIQUE
	|	KW_NOT KW_NULL
	;
	
create_index_stmt
	:	KW_CREATE KW_INDEX index_ref KW_ON table_ref LPAREN column_ref RPAREN
		-> ^(TOK_CREATE_INDEX_STMT index_ref table_ref column_ref)
	;
	
index_ref
	:	IDENTIFIER -> ^(TOK_INDEX_REF IDENTIFIER)
	;
	
drop_table_stmt
	:	KW_DROP KW_TABLE table_ref -> ^(TOK_DROP_TABLE table_ref)
	;
	
drop_index_stmt
	:	KW_DROP KW_INDEX index_ref -> ^(TOK_DROP_INDEX index_ref)
	;
	
select_stmt
	:	select_core order_by_clause? limit_clause? -> ^(TOK_SELECT_STMT select_core order_by_clause? limit_clause?)
	;
	
select_core
	:	KW_SELECT set_qualifier select_list from_clause? where_clause? group_by_clause? -> ^(TOK_SELECT_CORE set_qualifier select_list from_clause? where_clause? group_by_clause?)
	;
	
set_qualifier
	:	KW_ALL
	|	KW_DISTINCT
	|	/* empty means all */ -> KW_ALL
	;
	
select_list
	:	STAR -> ^(TOK_SELECT_LIST TOK_ALL_COLUMNS)
	|	column_ref (COMMA column_ref)* -> ^(TOK_SELECT_LIST column_ref+)
	;
	
from_clause
	:	KW_FROM table_ref (COMMA table_ref)* -> ^(TOK_FROM_CLAUSE table_ref+)	
	;

update_stmt
	:	update_core -> ^(TOK_UPDATE_STMT update_core)
	;
	
update_core
	:	KW_UPDATE table_ref KW_SET set_clause_list where_clause? -> ^(TOK_UPDATE_CORE table_ref set_clause_list where_clause?)
	;
	
set_clause_list
	:	set_clause (COMMA set_clause)* -> ^(TOK_UPDATE_SET_LIST set_clause+)
	;
	
set_clause
	:	column_ref EQUAL expr -> ^(TOK_UPDATE_SET column_ref expr)
	;
	
insert_stmt
	:	KW_INSERT KW_INTO table_ref LPAREN column_ref (COMMA column_ref)* RPAREN KW_VALUES LPAREN expr (COMMA expr)* RPAREN
		-> ^(TOK_INSERT_STMT table_ref column_ref+ TOK_INSERT_VALUES expr+)
	;
	
delete_stmt
	:	KW_DELETE KW_FROM table_ref where_clause? -> ^(TOK_DELETE_STMT table_ref where_clause?)
	;
	
order_by_clause
	:	KW_ORDER KW_BY
	;
	
limit_clause
	:	KW_LIMIT
	;
	
where_clause
	:	KW_WHERE predicate_list -> ^(TOK_WHERE predicate_list)
	;
	
predicate_list
	:	predicate_or (KW_OR predicate_or)* -> ^(TOK_PREDICATE_OR_LIST predicate_or+)
	;
	
predicate_or
	:	predicate_and (KW_AND predicate_and)* -> ^(TOK_PREDICATE_AND_LIST predicate_and+)
	;
	
predicate_and
	:	LPAREN! predicate_list RPAREN!
	|	lexpr=expr binary_op rexpr=expr	-> ^(TOK_BINARY_OP binary_op $lexpr $rexpr)
	;
	
binary_op
	:	EQUAL
	|	NEQ
	|	GREATER
	|	GEQ
	|	LESS
	|	LEQ
	;

group_by_clause
	:	KW_GROUP KW_BY
	;
	
column_ref
	:	(tableName=IDENTIFIER DOT)? columnName=IDENTIFIER -> ^(TOK_COLUMN_REF $columnName $tableName?)
	;
	
table_ref
	:	IDENTIFIER -> ^(TOK_TABLE_REF IDENTIFIER)
	;
	
expr
	:	NUMBER_LITERAL
	|	STRING_LITERAL
	|	column_ref
	;	
	
NUMBER_LITERAL
	:	Digit+ (DOT (Digit)+)?
	;
	
STRING_LITERAL
    :	( '\'' ( ~('\''|'\\') | ('\\' .) )* '\''
		| '\"' ( ~('\"'|'\\') | ('\\' .) )* '\"')+
    ;
	
//More specific rule comes first(e.g KW_FROM is before IDENTIFIER)
KW_ALL
	: ('A'|'a')('L'|'l')('L'|'l')
	;
	
KW_BY
	: ('B'|'b')('Y'|'y')
	;
	
KW_CREATE
	: ('C'|'c')('R'|'r')('E'|'e')('A'|'a')('T'|'t')('E'|'e')
	;
	
KW_DELETE
	: ('D'|'d')('E'|'e')('L'|'l')('E'|'e')('T'|'t')('E'|'e')
	;
		
KW_DISTINCT
	: ('D'|'d')('I'|'i')('S'|'s')('T'|'t')('I'|'i')('C'|'c')('T'|'t')
	;
	
KW_DROP
	: ('D'|'d')('R'|'r')('O'|'o')('P'|'p')
	;
	
KW_FROM
	: ('F'|'f')('R'|'r')('O'|'o')('M'|'m')
	;
	
KW_INSERT
	: ('I'|'i')('N'|'n')('S'|'s')('E'|'e')('R'|'r')('T'|'t')
	;

KW_INTO
	: ('I'|'i')('N'|'n')('T'|'t')('O'|'o')
	;
	
KW_KEY
	: ('K'|'k')('E'|'e')('Y'|'y')
	;
	
KW_LIMIT
	: ('L'|'l')('I'|'i')('M'|'m')('I'|'i')('T'|'t')
	;
	
KW_INDEX
	: ('I'|'i')('N'|'n')('D'|'d')('E'|'e')('X'|'x')
	;
	
KW_NOT
	: ('N'|'n')('O'|'o')('T'|'t')
	;
	
KW_NULL
	: ('N'|'n')('U'|'u')('L'|'l')('L'|'l')
	;
	
KW_GROUP
	: ('G'|'g')('R'|'r')('O'|'o')('U'|'u')('P'|'p')
	;
	
KW_ON
	: ('O'|'o')('N'|'n')
	;
	
KW_ORDER
	: ('O'|'o')('R'|'r')('D'|'d')('E'|'e')('R'|'r')
	;
	
KW_PRIMARY
	: ('P'|'p')('R'|'r')('I'|'i')('M'|'m')('A'|'a')('R'|'r')('Y'|'y')
	;
	
KW_UPDATE
	: ('U'|'u')('P'|'p')('D'|'d')('A'|'a')('T'|'t')('E'|'e')
	;
	
KW_SELECT
	: ('S'|'s')('E'|'e')('L'|'l')('E'|'e')('C'|'c')('T'|'t')
	;
	
KW_SET
	: ('S'|'s')('E'|'e')('T'|'t')
	;
		
KW_TABLE
	: ('T'|'t')('A'|'a')('B'|'b')('L'|'l')('E'|'e')
	;

KW_VALUES
	: ('V'|'v')('A'|'a')('L'|'l')('U'|'u')('E'|'e')('S'|'s')
	;
	
KW_UNIQUE
	: ('U'|'u')('N'|'n')('I'|'i')('Q'|'q')('U'|'u')('E'|'e')
	;
	
KW_WHERE
	: ('W'|'w')('H'|'h')('E'|'e')('R'|'r')('E'|'e')
	;
	
KW_ASC
	: ('A'|'a')('S'|'s')('C'|'c')
	;
	
KW_DESC
	: ('D'|'d')('E'|'e')('S'|'s')('C'|'c')
	;
	
KW_OR
	: ('O'|'o')('R'|'r')
	;
	
KW_AND
	: ('A'|'a')('N'|'n')('D'|'d')
	;
	
KW_INT
	: ('I'|'i')('N'|'n')('T'|'t')
	;
	
KW_TEXT
	: ('T'|'t')('E'|'e')('X'|'x')('T'|'t')
	;

IDENTIFIER
	:	(Letter | Digit)(Letter | Digit | '_')*
	;

COMMA : ',' ;
	
DOT : '.' ;
	
EQUAL : '=' ;

NEQ	  : '!=' ;

GREATER : '>' ;

GEQ   : '>=' ;

LESS  : '<' ;

LEQ   : '<=' ;

SEMI : ';' ;

STAR : '*' ;
	
LPAREN : '(' ;

RPAREN : ')' ;

LSQUARE : '[' ;

RSQUARE : ']' ;

LCURLY : '{' ;

RCURLY : '}' ;
			
WS : (' '|'\r'|'\t'|'\n') {$channel=HIDDEN;} ;

fragment
Letter
	:	'a'..'z' | 'A'..'Z'
	;

fragment
Digit
	:	'0'..'9'
	;
	