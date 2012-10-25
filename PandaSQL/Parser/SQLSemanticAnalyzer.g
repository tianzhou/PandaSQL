tree grammar SQLSemanticAnalyzer;

options 
{
    tokenVocab	    = SQL;
    language	    = C;
    ASTLabelType	= pANTLR3_BASE_TREE;
}

@treeparser::header
{

#include "stdafx.h"
#include "Parser/ParserDriver.h"
#include "Catalog/Table.h"

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
}
	:	ddl_stmt
	|	dml_stmt
	;
	
ddl_stmt
	:	create_table_stmt
	|	create_index_stmt
	|	drop_table_stmt
	|	drop_index_stmt
	;
	
create_table_stmt
@init
{
	std::string tableRef;
	
	PandaSQL::ParserDriver *pDriver = $stmt::pDriver;
	pDriver->PushNewStatement(PandaSQL::Statement::kStmtCreateTable);
}
	:	^(TOK_CREATE_TABLE_STMT table_ref[&tableRef] column_def_list)
		{
			PandaSQL::ParserDriver *pDriver = $stmt::pDriver;
			pDriver->GetCurrentStatement().AddTableRef(tableRef);
		}
	;
	
column_def_list
	:	^(TOK_COLUMN_DEF_LIST column_def+)
		{
			
		}
	;
	
column_def
@init
{
	PandaSQL::ColumnDef columnDef;
	PandaSQL::ColumnQualifiedName qualifiedName;
}
	:	^(TOK_COLUMN_DEF column_reference[&qualifiedName] type=type_name constraint=column_constraint)
		{
			columnDef.qualifiedName = qualifiedName;
			columnDef.dataType = $type.dataType;
			columnDef.constraintType = $constraint.constraintType; 
			
			PandaSQL::ParserDriver *pDriver = $stmt::pDriver;
			pDriver->GetCurrentStatement().AddColumnDef(columnDef);
		}
	;
	
type_name returns [PandaSQL::DataType dataType]
	:	KW_INT
		{
			dataType = PandaSQL::kInt;
		}
	|	KW_TEXT
		{
			dataType = PandaSQL::kText;
		}
	;
	
column_constraint returns[PandaSQL::ConstraintType constraintType]
@init
{
	constraintType = PandaSQL::kConstraintNone;
}
	:	KW_PRIMARY KW_KEY
		{
			constraintType = PandaSQL::kConstraintPrimaryKey;
		}
	|	KW_UNIQUE
		{
			constraintType = PandaSQL::kConstraintUnique;
		}
	|	KW_NOT KW_NULL
		{
			constraintType = PandaSQL::kConstraintNotNULL;
		}
	;
	
create_index_stmt
@init
{
	std::string indexRef;
	std::string tableRef;
	PandaSQL::ColumnQualifiedName qualifiedName;
	
	PandaSQL::ParserDriver *pDriver = $stmt::pDriver;
	pDriver->PushNewStatement(PandaSQL::Statement::kStmtCreateIndex);
}
	:	^(TOK_CREATE_INDEX_STMT index_ref[&indexRef] table_ref[&tableRef] column_reference[&qualifiedName])
		{
			PandaSQL::ParserDriver *pDriver = $stmt::pDriver;
			pDriver->GetCurrentStatement().SetIndexRef(indexRef);
			pDriver->GetCurrentStatement().AddTableRef(tableRef);
			pDriver->GetCurrentStatement().AddColumnWithQualifiedName(qualifiedName);
		}
	;
	
drop_table_stmt
@init
{
	std::string tableRef;
	
	PandaSQL::ParserDriver *pDriver = $stmt::pDriver;
	pDriver->PushNewStatement(PandaSQL::Statement::kStmtDropTable);
}
	:	^(TOK_DROP_TABLE table_ref[&tableRef])
		{
			PandaSQL::ParserDriver *pDriver = $stmt::pDriver;
			pDriver->GetCurrentStatement().AddTableRef(tableRef);
		}
	;
	
drop_index_stmt
@init
{
	std::string indexRef;
	
	PandaSQL::ParserDriver *pDriver = $stmt::pDriver;
	pDriver->PushNewStatement(PandaSQL::Statement::kStmtDropIndex);
}
	:	^(TOK_DROP_INDEX index_ref[&indexRef])
		{
			PandaSQL::ParserDriver *pDriver = $stmt::pDriver;
			pDriver->GetCurrentStatement().SetIndexRef(indexRef);
		}
	;
	
dml_stmt
	:	select_stmt
	|	update_stmt
	|	insert_stmt
	|	delete_stmt
	;
	
select_stmt
@init
{
	PandaSQL::ParserDriver *pDriver = $stmt::pDriver;
	pDriver->PushNewStatement(PandaSQL::Statement::kStmtSelect);
}
@after
{
	
}
	:	^(TOK_SELECT_STMT select_core order_by_clause? limit_clause?)
		{
		}
	;
	
select_core
	:	^(TOK_SELECT_CORE set_qualifier from_clause select_list where_clause? group_by_clause?)
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
@init
{
	PandaSQL::ColumnQualifiedName qualifiedName;
}
	:	^(TOK_SELECT_LIST TOK_ALL_COLUMNS)
		{
			PandaSQL::ParserDriver *pDriver = $stmt::pDriver;
			pDriver->GetCurrentStatement().AddAllColumns();
		}
	|	^(TOK_SELECT_LIST 
			(column_reference[&qualifiedName]
			{
				PandaSQL::ParserDriver *pDriver = $stmt::pDriver;
				pDriver->GetCurrentStatement().AddColumnWithQualifiedName(qualifiedName);
			}
			)+
		 )
		{
		}
	;
	
from_clause
@init
{
	std::string tableRef;
}
	:	^(TOK_FROM_CLAUSE
			(table_ref[&tableRef]
			{
				PandaSQL::ParserDriver *pDriver = $stmt::pDriver;
				pDriver->GetCurrentStatement().AddTableRef(tableRef);
			}
			)+
		 )
		{

		} 	
	;
	
//----------5 Lexical elements BEGIN----------


//5.3 <literal>
unsigned_literal returns [PandaSQL::Expr *io_pExpr]
@init
{
	PandaSQL::ParserDriver *pDriver = $stmt::pDriver;
}
	:	unl=unsigned_numeric_literal
		{
			$io_pExpr = $unl.io_pExpr;
		}
	|	general_literal
	;
	
general_literal
	:	character_string_literal
	|	boolean_literal
	;
	
character_string_literal
	:	QUOTE (Letter | Digit | '_')* QUOTE
	;
	
unsigned_numeric_literal returns [PandaSQL::Expr *io_pExpr]
	:	enl=exact_numeric_literal
		{
			$io_pExpr = $enl.io_pExpr;
		}
	;
	
exact_numeric_literal returns [PandaSQL::Expr *io_pExpr]
	:	ui=unsigned_integer
		{
			$io_pExpr = $ui.io_pExpr;
		}
	;
	
unsigned_integer returns [PandaSQL::Expr *io_pExpr]
@init
{
	PandaSQL::ParserDriver *pDriver = $stmt::pDriver;
}
	:	UNSIGNED_INTEGER
		{
			$io_pExpr = pDriver->CreateExprForNumericLiteral($UNSIGNED_INTEGER);
		}
	;
	
boolean_literal
	:	KW_TRUE
	|	KW_FALSE
	;
	
//----------Lexical elements END----------
	
//----------6 Scalar expressions BEGIN----------

//6.3 <value expression primary>
value_expression_primary returns [PandaSQL::Expr *io_pExpr]
@init
{
	PandaSQL::ColumnQualifiedName qualifiedName;
}
	:	^(TOK_VALUE_EXPRESSION_EXPRESSION uvs=unsigned_value_specification)
		{
			$io_pExpr = $uvs.io_pExpr;
		}
	|	^(TOK_VALUE_EXPRESSION_EXPRESSION cr=column_reference_expr)
		{
			$io_pExpr = $cr.io_pExpr;
		}
	;
	
//6.4 <value specification> and <target specification>
unsigned_value_specification returns [PandaSQL::Expr *io_pExpr]
@init
{

}
	:	^(TOK_UNSIGNED_VALUE_SPECIFICATION ul=unsigned_literal)
		{
			$io_pExpr = $ul.io_pExpr;
		}
	;

//6.7 <column reference>	
column_reference[PandaSQL::ColumnQualifiedName *o_columnQualifiedName]
@init
{
	std::string columnRef;
	std::string tableRef;
}
	:	^(TOK_COLUMN_REF identifier[&columnRef] identifier[&tableRef]?)
		{
			o_columnQualifiedName->columnName = columnRef;
			o_columnQualifiedName->tableName = tableRef;
		}
	;
	
column_reference_expr returns [PandaSQL::Expr *io_pExpr]
@init
{
	PandaSQL::ColumnQualifiedName qualifiedName;
	PandaSQL::ParserDriver *pDriver = $stmt::pDriver;
}
	:	column_reference[&qualifiedName]
		{
			io_pExpr = pDriver->CreateExprForColumnReference(qualifiedName);
		}
	;

//6.26 <value expression>
common_value_expression returns [PandaSQL::Expr *io_pExpr]
	:	^(TOK_COMMON_VALUE_EXPRESSION sve=string_value_expression)
		{
			$io_pExpr = $sve.io_pExpr;
		}
	;
	
//6.29 <string value expression>
string_value_expression returns [PandaSQL::Expr *io_pExpr]
	:	^(TOK_STRING_VALUE_EXPRESSION vep=value_expression_primary)
		{
			$io_pExpr = $vep.io_pExpr;
		}
	;
	
//6.35 <boolean value expression>
bool_value_expression returns [PandaSQL::BooleanExpr *io_pBooleanExpr]
@init
{
	PandaSQL::ParserDriver *pDriver = $stmt::pDriver;
	bool isSingleItem = true;
	$io_pBooleanExpr = NULL;
}
	:	^(TOK_BOOLEAN_VALUE_EXPRESSION 
			(KW_OR { isSingleItem = false; })?
			(bt=boolean_term
			{
				if (isSingleItem)
				{
					$io_pBooleanExpr = $bt.io_pBooleanExpr;
				}
				else
				{
					//Or list
					if (!$io_pBooleanExpr)
					{
						$io_pBooleanExpr = pDriver->CreateExprForBooleanOrList();
					}
					$io_pBooleanExpr->AddExpr($bt.io_pBooleanExpr);
				}				
			})+)
	;
	
boolean_term returns [PandaSQL::BooleanExpr *io_pBooleanExpr]
@init
{
	PandaSQL::ParserDriver *pDriver = $stmt::pDriver;
	bool isSingleItem = true;
	$io_pBooleanExpr = NULL;
}
	:	^(TOK_BOOLEAN_TERM
			(KW_AND { isSingleItem = false; })?
			(bf=boolean_factor
			{
				if (isSingleItem)
				{
					$io_pBooleanExpr = $bf.io_pBooleanExpr;
				}
				else
				{
					//And list
					if (!$io_pBooleanExpr)
					{
						$io_pBooleanExpr = pDriver->CreateExprForBooleanAndList();
					
					}
					$io_pBooleanExpr->AddExpr($bf.io_pBooleanExpr);
				}
			}
			)+)
	;
	
boolean_factor returns [PandaSQL::BooleanExpr *io_pBooleanExpr]
@init
{
	
}
	:	^(TOK_BOOLEAN_FACTOR KW_NOT? bt=boolean_test)
		{
			$io_pBooleanExpr = $bt.io_pBooleanExpr;
		}
	;
	
boolean_test returns [PandaSQL::BooleanExpr *io_pBooleanExpr]
@init
{
	
}
	:	^(TOK_BOOLEAN_TEST bp=boolean_primary (KW_IS KW_NOT? boolean_literal)?)
		{
			$io_pBooleanExpr = $bp.io_pBooleanExpr;
		}
	;
	
boolean_primary returns [PandaSQL::BooleanExpr *io_pBooleanExpr]
@init
{
	PandaSQL::ParserDriver *pDriver = $stmt::pDriver;
}
	:	^(TOK_BOOLEAN_PRIMARY p=predicate)
		{
			$io_pBooleanExpr = pDriver->CreateExprForBooleanPrimary(*$p.io_pExpr);
		}
	|	^(TOK_BOOLEAN_PRIMARY bp=boolean_predicand)
		{
			$io_pBooleanExpr = pDriver->CreateExprForBooleanPrimary(*$bp.io_pExpr);
		}
	;
	
boolean_predicand returns [PandaSQL::Expr *io_pExpr]
	:	bve=bool_value_expression
		{
			$io_pExpr = $bve.io_pBooleanExpr;
		}
	|	vep=value_expression_primary
		{
			$io_pExpr = $vep.io_pExpr;
		}
	;
	
//----------Scalar expressions END----------
		
//----------7 Query expressions BEGIN----------

//7.1 <row value constructor>
row_value_constructor_predicand returns [PandaSQL::Expr *io_pExpr]
	:	^(TOK_ROW_VALUE_CONSTRUCTOR_PREDICAND cve=common_value_expression)
		{
			$io_pExpr = $cve.io_pExpr;
		}
	;
	
//7.2 <row value expression>
row_value_predicand returns [PandaSQL::Expr *io_pExpr]
	:	^(TOK_ROW_VALUE_PREDICAND rvcp=row_value_constructor_predicand)
		{
			$io_pExpr = $rvcp.io_pExpr;
		}
	;
	
where_clause
@init
{
	PandaSQL::Predicate predicate;
	PandaSQL::ParserDriver *pDriver = $stmt::pDriver;
}
	:	^(TOK_WHERE predicate_list[predicate])
		{
			pDriver->GetCurrentStatement().SetPredicate(predicate);
		}
	|	^(TOK_WHERE sc=search_condition)
		{
			pDriver->GetCurrentStatement().SetWhereClauseExpression($sc.io_pBooleanExpr);
		}
	;
	
//----------Query expressions END----------
	
//----------8 Predicates BEGIN----------

predicate returns [PandaSQL::Expr *io_pExpr]
	:	^(TOK_PREDICATE cp=comparison_predicate)
		{
			$io_pExpr = $cp.io_pExpr;
		}
	;
	
comparison_predicate returns [PandaSQL::Expr *io_pExpr]
@init
{
	PandaSQL::ParserDriver *pDriver = $stmt::pDriver;
}
	:	^(TOK_COMPARISON_PREDICATE co=comparison_op lv=row_value_predicand rv=row_value_predicand)
		{
			$io_pExpr = pDriver->CreateExprForBinaryOp(*$co.text, *$lv.io_pExpr, *$rv.io_pExpr);
		}
	;
	
comparison_op
	:	EQUAL
	;

search_condition returns [PandaSQL::BooleanExpr *io_pBooleanExpr]
@init
{
}
	:	^(TOK_SEARCH_CONDITION ble=bool_value_expression)
		{
			$io_pBooleanExpr = $ble.io_pBooleanExpr;
		}
	;

//----------Predicates END----------	
	
predicate_list[PandaSQL::Predicate &o_Predicate]
@init
{
	std::vector<PandaSQL::Predicate> predicateList;
	PandaSQL::Predicate onePredicate;
}
	:	^(TOK_PREDICATE_OR_LIST
			(predicate_or[onePredicate]
			{
				predicateList.push_back(onePredicate);
				onePredicate.Reset();
			})+
		 )
		 {
			o_Predicate.SetOrPredicateWithSubpredicates(predicateList);
		 }
	;
	
predicate_or[PandaSQL::Predicate &o_Predicate]
@init
{
	std::vector<PandaSQL::Predicate> predicateList;
	PandaSQL::Predicate subPredicate;
}
	:	^(TOK_PREDICATE_AND_LIST
			(predicate_and[subPredicate]
			{
				predicateList.push_back(subPredicate);
			})+
		 )
		 {
			o_Predicate.SetAndPredicateWithSubpredicates(predicateList);
		 }
	;
	
predicate_and[PandaSQL::Predicate &o_predicate]
@init
{
	PandaSQL::Expr leftExpr;
	PandaSQL::Expr rightExpr;
	o_predicate = PandaSQL::Predicate();
}
	:	predicate_list[o_predicate]
	|	^(TOK_BINARY_OP op=binary_op expr[&leftExpr] expr[&rightExpr])
		{
			PandaSQL::PredicateItem predicateItem;
			predicateItem.SetFormat(leftExpr, rightExpr, op);
			o_predicate.SetSinglePredicateItem(predicateItem);
		}
	;
	
binary_op returns [PandaSQL::PredicateItem::PredicateComparisonType comparisonType]
	:	EQUAL
		{
			comparisonType =  PandaSQL::PredicateItem::kEqual;
		}
	|	NEQ
		{
			comparisonType =  PandaSQL::PredicateItem::kNotEqual;
		}
	|	GREATER
		{
			comparisonType =  PandaSQL::PredicateItem::kGreater;
		}
	|	GEQ
		{
			comparisonType =  PandaSQL::PredicateItem::kGreaterEqual;
		}
	|	LESS
		{
			comparisonType =  PandaSQL::PredicateItem::kLess;
		}
	|	LEQ
		{
			comparisonType =  PandaSQL::PredicateItem::kLessEqual;
		}
	;
	
update_stmt
@init
{
	PandaSQL::ParserDriver *pDriver = $stmt::pDriver;
	pDriver->PushNewStatement(PandaSQL::Statement::kStmtUpdate);
}
@after
{
}
	:	^(TOK_UPDATE_STMT update_core)
		{
		
		}
	;
	
update_core
@init
{
	std::string tableRef;
}
	:	^(TOK_UPDATE_CORE table_ref[&tableRef] set_clause_list where_clause?)
		{
			PandaSQL::ParserDriver *pDriver = $stmt::pDriver;
			pDriver->GetCurrentStatement().AddTableRef(tableRef);
		}
	;
	
set_clause_list
	:	^(TOK_UPDATE_SET_LIST set_clause+)
		{
		}
	;
	
set_clause
@init
{
	PandaSQL::ColumnQualifiedName qualifiedName;
	PandaSQL::Expr valueExpr;
}
	:	^(TOK_UPDATE_SET column_reference[&qualifiedName] expr[&valueExpr])
		{
			PandaSQL::ParserDriver *pDriver = $stmt::pDriver;
			pDriver->GetCurrentStatement().AddColumnWithQualifiedName(qualifiedName);
			pDriver->GetCurrentStatement().AddExprRef(valueExpr);
		}
	;
	
insert_stmt
@init
{
	std::string tableRef;
	PandaSQL::ColumnQualifiedName qualifiedName;
	PandaSQL::Expr valueExpr;
	
	PandaSQL::ParserDriver *pDriver = $stmt::pDriver;
	pDriver->PushNewStatement(PandaSQL::Statement::kStmtInsert);
}
	:	^(TOK_INSERT_STMT
			(table_ref[&tableRef]
			{
				PandaSQL::ParserDriver *pDriver = $stmt::pDriver;
				pDriver->GetCurrentStatement().AddTableRef(tableRef);
			})
			(column_reference[&qualifiedName]
			{
				PandaSQL::ParserDriver *pDriver = $stmt::pDriver;
				pDriver->GetCurrentStatement().AddColumnWithQualifiedName(qualifiedName);
			})+
			TOK_INSERT_VALUES
			(expr[&valueExpr]
			{
				PandaSQL::ParserDriver *pDriver = $stmt::pDriver;
				pDriver->GetCurrentStatement().AddExprRef(valueExpr);
			})+)
	;
	
delete_stmt
@init
{
	std::string tableRef;
	
	PandaSQL::ParserDriver *pDriver = $stmt::pDriver;
	pDriver->PushNewStatement(PandaSQL::Statement::kStmtDelete);
}
	:	^(TOK_DELETE_STMT table_ref[&tableRef] where_clause?)
		{
			PandaSQL::ParserDriver *pDriver = $stmt::pDriver;
			pDriver->GetCurrentStatement().AddTableRef(tableRef);
		}
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
	
group_by_clause
	:	KW_GROUP KW_BY
		{
		}
	;
	
table_ref[std::string *o_tableRef]
@init
{
}
	:	^(TOK_TABLE_REF identifier[o_tableRef])
		{
		}
	;
	
index_ref[std::string *o_tableRef]
	:	^(TOK_INDEX_REF identifier[o_tableRef])
		{
			
		}
	;
	
identifier[std::string *o_str]
	:	id=IDENTIFIER
		{
			PandaSQL::ParserDriver::GetString($id, o_str);
		}
	;
	
expr[PandaSQL::Expr *o_expr]
@init
{
	PandaSQL::ColumnQualifiedName qualifiedName;
}
	:	num=UNSIGNED_INTEGER
		{
			PandaSQL::ParserDriver::GetExprForNumber($num, o_expr);
		}
	|	num=NUMBER_LITERAL
		{
			PandaSQL::ParserDriver::GetExprForNumber($num, o_expr);
			
		}
	|	str=STRING_LITERAL
		{
			PandaSQL::ParserDriver::GetExprForText($str, o_expr);
		}
	|	column_reference[&qualifiedName]
		{
			PandaSQL::ParserDriver *pDriver = $stmt::pDriver;
			pDriver->GetCurrentStatement().AddColumnWithQualifiedName(qualifiedName);
		}
	;
