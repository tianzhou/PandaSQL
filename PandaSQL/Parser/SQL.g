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
TOK_COLUMN_REF;
TOK_DELETE_STMT;
TOK_FROM_CLAUSE;
TOK_SELECT_CORE;
TOK_SELECT_LIST;
TOK_SELECT_STMT;
TOK_INSERT_STMT;
TOK_INSERT_VALUES;
TOK_UPDATE_CORE;
TOK_UPDATE_STMT;
TOK_UPDATE_SET_LIST;
TOK_UPDATE_SET;
TOK_TABLE_REF;
}

@parser::includes
{
}

@lexer::includes
{
}


//Starting rule
stmt
	:	dml_stmt^ (';' | EOF)!
	;
	
dml_stmt
	:	select_stmt
	|	update_stmt
	|	insert_stmt
	|	delete_stmt
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
	:	KW_FROM table_ref -> ^(TOK_FROM_CLAUSE table_ref)	
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
	:	KW_WHERE
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
	
KW_DELETE
	: ('D'|'d')('E'|'e')('L'|'l')('E'|'e')('T'|'t')('E'|'e')
	;
		
KW_DISTINCT
	: ('D'|'d')('I'|'i')('S'|'s')('T'|'t')('I'|'i')('C'|'c')('T'|'t')
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
	
KW_LIMIT
	: ('L'|'l')('I'|'i')('M'|'m')('I'|'i')('T'|'t')
	;
	
KW_GROUP
	: ('G'|'g')('R'|'r')('O'|'o')('U'|'u')('P'|'p')
	;
	
KW_ORDER
	: ('O'|'o')('R'|'r')('D'|'d')('E'|'e')('R'|'r')
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
	
KW_WHERE
	: ('W'|'w')('H'|'h')('E'|'e')('R'|'r')('E'|'e')
	;

IDENTIFIER
	:	(Letter | Digit)(Letter | Digit | '_')*
	;

COMMA : ',' ;
	
DOT : '.' ;
	
EQUAL : '=' ;

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
	