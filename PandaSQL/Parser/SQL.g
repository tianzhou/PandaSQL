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
TOK_BOOLEAN_VALUE_EXPRESSION;
TOK_BOOLEAN_TERM;
TOK_BOOLEAN_FACTOR;
TOK_BOOLEAN_TEST;
TOK_BOOLEAN_PREDICAND;
TOK_BOOLEAN_PRIMARY;
TOK_COLUMN_DEF;
TOK_COLUMN_DEF_LIST;
TOK_COLUMN_REF;
TOK_COMMON_VALUE_EXPRESSION;
TOK_COMPARISON_PREDICATE;
TOK_CREATE_TABLE_STMT;
TOK_CREATE_INDEX_STMT;
TOK_DELETE_STMT;
TOK_DROP_TABLE;
TOK_DROP_INDEX;
TOK_FROM_CLAUSE;
TOK_INDEX_REF;
TOK_INSERT_STMT;
TOK_INSERT_VALUES;
TOK_NONPARENTHESIZED_VALUE_EXPRESSION_EXPRESSION;
TOK_PREDICATE;
TOK_PREDICATE_OR_LIST;
TOK_PREDICATE_AND_LIST;
TOK_ROW_VALUE_CONSTRUCTOR_PREDICAND;
TOK_ROW_VALUE_PREDICAND;
TOK_SEARCH_CONDITION;
TOK_SELECT_CORE;
TOK_SELECT_LIST;
TOK_SELECT_STMT;
TOK_STRING_VALUE_EXPRESSION;
TOK_TABLE_REF;
TOK_UNSIGNED_VALUE_SPECIFICATION;
TOK_UPDATE_CORE;
TOK_UPDATE_STMT;
TOK_UPDATE_SET_LIST;
TOK_UPDATE_SET;
TOK_VALUE_EXPRESSION_EXPRESSION;
TOK_WHERE;
}

@parser::includes
{

#include "stdafx.h"

}

@lexer::includes
{
}


//Starting rule
stmt
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
	:	column_reference type_name column_constraint -> ^(TOK_COLUMN_DEF column_reference type_name column_constraint)
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
	:	KW_CREATE KW_INDEX index_ref KW_ON table_ref LPAREN column_reference RPAREN
		-> ^(TOK_CREATE_INDEX_STMT index_ref table_ref column_reference)
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
	:	KW_SELECT set_qualifier select_list from_clause where_clause? group_by_clause? -> ^(TOK_SELECT_CORE set_qualifier from_clause select_list where_clause? group_by_clause?)
	;
	
set_qualifier
	:	KW_ALL
	|	KW_DISTINCT
	|	/* empty means all */ -> KW_ALL
	;
	
select_list
	:	STAR -> ^(TOK_SELECT_LIST TOK_ALL_COLUMNS)
	|	column_reference (COMMA column_reference)* -> ^(TOK_SELECT_LIST column_reference+)
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
	:	column_reference EQUAL expr -> ^(TOK_UPDATE_SET column_reference expr)
	;
	
insert_stmt
	:	KW_INSERT KW_INTO table_ref LPAREN column_reference (COMMA column_reference)* RPAREN KW_VALUES LPAREN expr (COMMA expr)* RPAREN
		-> ^(TOK_INSERT_STMT table_ref column_reference+ TOK_INSERT_VALUES expr+)
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
	
//----------5 Lexical elements BEGIN----------


//5.3 <literal>
unsigned_literal
	:	unsigned_numeric_literal
	|	general_literal
	;
	
general_literal
	:	character_string_literal
	|	boolean_literal
	;
	
character_string_literal
	:	QUOTE (Letter | Digit | '_')* QUOTE
	;
	
unsigned_numeric_literal
	:	exact_numeric_literal
	;
	
exact_numeric_literal
	:	unsigned_integer
	;
	
unsigned_integer
	:	UNSIGNED_INTEGER
	;
	
boolean_literal
	:	KW_TRUE
	|	KW_FALSE
	;

//----------Lexical elements END----------
	
//----------6 Scalar expressions BEGIN----------
//6.3 <value expression primary>
value_expression_primary
	:	nonparenthesized_value_expression_primary 
	|	LPAREN! nonparenthesized_value_expression_primary RPAREN!
	;

nonparenthesized_value_expression_primary
	:	unsigned_value_specification -> ^(TOK_VALUE_EXPRESSION_EXPRESSION unsigned_value_specification)
	|	column_reference -> ^(TOK_VALUE_EXPRESSION_EXPRESSION column_reference)
	;
	
//6.4 <value specification> and <target specification>
unsigned_value_specification
	:	unsigned_literal -> ^(TOK_UNSIGNED_VALUE_SPECIFICATION unsigned_literal)
	;
	
//6.7 <column reference>
column_reference
	:	(tableName=IDENTIFIER DOT)? columnName=IDENTIFIER -> ^(TOK_COLUMN_REF $columnName $tableName?)
	;

//6.26 <value expression>
common_value_expression
	:	string_value_expression -> ^(TOK_COMMON_VALUE_EXPRESSION string_value_expression)
	;
	
//6.29 <string value expression>
string_value_expression
	:	value_expression_primary -> ^(TOK_STRING_VALUE_EXPRESSION value_expression_primary)
	;
	
//6.35 <boolean value expression>
bool_value_expression
	:	boolean_term (KW_OR boolean_term)* -> ^(TOK_BOOLEAN_VALUE_EXPRESSION KW_OR? boolean_term+)
	;
	
boolean_term
	:	boolean_factor (KW_AND boolean_factor)* -> ^(TOK_BOOLEAN_TERM KW_AND? boolean_factor+)
	;
	
boolean_factor
	:	KW_NOT? boolean_test -> ^(TOK_BOOLEAN_FACTOR KW_NOT? boolean_test)
	;
	
boolean_test
	:	boolean_primary (KW_IS KW_NOT? boolean_literal)? -> ^(TOK_BOOLEAN_TEST boolean_primary (KW_IS KW_NOT? boolean_literal)?)
	;
	
boolean_primary
	:	predicate -> ^(TOK_BOOLEAN_PRIMARY predicate)
	|	boolean_predicand -> ^(TOK_BOOLEAN_PRIMARY boolean_predicand)
	;
	
boolean_predicand
	:	LPAREN! bool_value_expression RPAREN!
	|	nonparenthesized_value_expression_primary
	;

//----------Scalar expressions END----------
	
//----------7 Query expressions BEGIN----------

//7.1 <row value constructor>
row_value_constructor_predicand
	:	common_value_expression -> ^(TOK_ROW_VALUE_CONSTRUCTOR_PREDICAND common_value_expression)
	;
	
//7.2 <row value expression>
row_value_predicand
	:	row_value_constructor_predicand -> ^(TOK_ROW_VALUE_PREDICAND row_value_constructor_predicand) 
	;
	
where_clause
	:	KW_WHERE search_condition -> ^(TOK_WHERE search_condition)
	;

//----------Query expressions END----------

//----------8 Predicates BEGIN----------
predicate
	:	comparison_predicate -> ^(TOK_PREDICATE comparison_predicate)
	;
	
comparison_predicate
	:	lvalue=row_value_predicand comparison_op rvalue=row_value_predicand -> ^(TOK_COMPARISON_PREDICATE comparison_op $lvalue $rvalue)
	;
	
comparison_op
	:	EQUAL
	|	NEQ
	|	GREATER
	|	GEQ
	|	LESS
	|	LEQ
	;
	
search_condition
	:	bool_value_expression -> ^(TOK_SEARCH_CONDITION bool_value_expression)
	;
	
//----------Predicates END----------

group_by_clause
	:	KW_GROUP KW_BY
	;
	
table_ref
	:	IDENTIFIER -> ^(TOK_TABLE_REF IDENTIFIER)
	;
	
expr
	:	UNSIGNED_INTEGER
	|	NUMBER_LITERAL
	|	STRING_LITERAL
	|	column_reference
	;
	
UNSIGNED_INTEGER
	:	Digit+
	;	
	
NUMBER_LITERAL
	:	Digit+ (DOT (Digit)+)+
	;
	
STRING_LITERAL
    :	'\'' ( ~('\''|'\\') | ('\\' .) )* '\''
		| '\"' ( ~('\"'|'\\') | ('\\' .) )* '\"'
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
	
KW_FALSE
	: ('F'|'f')('A'|'a')('L'|'l')('S'|'s')('E'|'e')
	;
	
KW_FROM
	: ('F'|'f')('R'|'r')('O'|'o')('M'|'m')
	;
	
KW_INDEX
	: ('I'|'i')('N'|'n')('D'|'d')('E'|'e')('X'|'x')
	;
	
KW_INSERT
	: ('I'|'i')('N'|'n')('S'|'s')('E'|'e')('R'|'r')('T'|'t')
	;

KW_INTO
	: ('I'|'i')('N'|'n')('T'|'t')('O'|'o')
	;
	
KW_IS
	: ('I'|'i')('S'|'s')
	;
	
KW_KEY
	: ('K'|'k')('E'|'e')('Y'|'y')
	;
	
KW_LIMIT
	: ('L'|'l')('I'|'i')('M'|'m')('I'|'i')('T'|'t')
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

KW_TRUE
	: ('T'|'t')('R'|'r')('U'|'u')('E'|'e')
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

QUOTE : '\'' ;

SEMI : ';' ;

STAR : '*' ;
	
LPAREN : '(' ;

RPAREN : ')' ;

LSQUARE : '[' ;

RSQUARE : ']' ;

LCURLY : '{' ;

RCURLY : '}' ;
			
WS : (' '|'\r'|'\t'|'\n') {$channel=HIDDEN;} ;

//5.2 <token> and <separator>
COMMENT
	: '--' (~('\n'|'\r'))*	{ $channel = HIDDEN; }
	;

fragment
Letter
	:	'a'..'z' | 'A'..'Z'
	;

fragment
Digit
	:	'0'..'9'
	;
	