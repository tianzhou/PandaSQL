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
	:	ddl_stmt
	|	dml_stmt
	;
	
ddl_stmt
	:	create_table_stmt
		{
			PandaSQL::ParserDriver *pDriver = $stmt::pDriver;
			pDriver->GetStatement().SetStatementType(PandaSQL::Statement::kStmtCreateTable);
		}
	|	create_index_stmt
		{
			PandaSQL::ParserDriver *pDriver = $stmt::pDriver;
			pDriver->GetStatement().SetStatementType(PandaSQL::Statement::kStmtCreateIndex);
		}
	|	drop_table_stmt
		{
			PandaSQL::ParserDriver *pDriver = $stmt::pDriver;
			pDriver->GetStatement().SetStatementType(PandaSQL::Statement::kStmtDropTable);
		}
	|	drop_index_stmt
		{
			PandaSQL::ParserDriver *pDriver = $stmt::pDriver;
			pDriver->GetStatement().SetStatementType(PandaSQL::Statement::kStmtDropIndex);
		}
	;
	
create_table_stmt
@init
{
	std::string tableRef;
}
	:	^(TOK_CREATE_TABLE_STMT table_ref[tableRef] column_def_list)
		{
			PandaSQL::ParserDriver *pDriver = $stmt::pDriver;
			pDriver->GetStatement().AddTableRef(tableRef);
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
	std::string columnRef;
}
	:	^(TOK_COLUMN_DEF column_ref[columnRef] type=type_name constraint=column_constraint)
		{
			columnDef.columnName = columnRef;
			columnDef.dataType = type;
			columnDef.constraintType = constraint; 
			
			PandaSQL::ParserDriver *pDriver = $stmt::pDriver;
			pDriver->GetStatement().AddColumnDef(columnDef);
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
	std::string columnRef;
}
	:	^(TOK_CREATE_INDEX_STMT index_ref[indexRef] table_ref[tableRef] column_ref[columnRef])
		{
			PandaSQL::ParserDriver *pDriver = $stmt::pDriver;
			pDriver->GetStatement().SetIndexRef(indexRef);
			pDriver->GetStatement().AddTableRef(tableRef);
			pDriver->GetStatement().AddColumnRef(columnRef);
		}
	;
	
drop_table_stmt
@init
{
	std::string tableRef;
}
	:	^(TOK_DROP_TABLE table_ref[tableRef])
		{
			PandaSQL::ParserDriver *pDriver = $stmt::pDriver;
			pDriver->GetStatement().AddTableRef(tableRef);
		}
	;
	
drop_index_stmt
@init
{
	std::string indexRef;
}
	:	^(TOK_DROP_INDEX index_ref[indexRef])
		{
			PandaSQL::ParserDriver *pDriver = $stmt::pDriver;
			pDriver->GetStatement().SetIndexRef(indexRef);
		}
	;
	
dml_stmt
	:	select_stmt
		{
			PandaSQL::ParserDriver *pDriver = $stmt::pDriver;
			pDriver->GetStatement().SetStatementType(PandaSQL::Statement::kStmtSelect);
		}
	|	update_stmt
		{
			PandaSQL::ParserDriver *pDriver = $stmt::pDriver;
			pDriver->GetStatement().SetStatementType(PandaSQL::Statement::kStmtUpdate);
		}
	|	insert_stmt
		{
			PandaSQL::ParserDriver *pDriver = $stmt::pDriver;
			pDriver->GetStatement().SetStatementType(PandaSQL::Statement::kStmtInsert);
		}
	|	delete_stmt
		{
			PandaSQL::ParserDriver *pDriver = $stmt::pDriver;
			pDriver->GetStatement().SetStatementType(PandaSQL::Statement::kStmtDelete);
		}
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
		}
	;
	
select_core
	:	^(TOK_SELECT_CORE set_qualifier select_list from_clause? where_clause? group_by_clause?)
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
	std::string columnRef;
}
	:	^(TOK_SELECT_LIST TOK_ALL_COLUMNS)
		{
			printf("selectList: *\n");
		}
	|	^(TOK_SELECT_LIST 
			(column_ref[columnRef]
			{
				PandaSQL::ParserDriver *pDriver = $stmt::pDriver;
				pDriver->GetStatement().AddColumnRef(columnRef);
			}
			)+
		 )
		{
			printf("selectColumnRef: *\n");
		}
	;
	
from_clause
@init
{
	std::string tableRef;
}
	:	^(TOK_FROM_CLAUSE table_ref[tableRef])
		{
			PandaSQL::ParserDriver *pDriver = $stmt::pDriver;
			pDriver->GetStatement().AddTableRef(tableRef);
		} 	
	;
	
where_clause
	:	KW_WHERE
		{
		}
	;
	
update_stmt
@init
{
	printf("***update_statement begin***\n");
}
@after
{
	printf("***update_statement end***\n");
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
	:	^(TOK_UPDATE_CORE table_ref[tableRef] set_clause_list where_clause?)
		{
			PandaSQL::ParserDriver *pDriver = $stmt::pDriver;
			pDriver->GetStatement().AddTableRef(tableRef);
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
	std::string columnRef;
	PandaSQL::Expr theExpr;
}
	:	^(TOK_UPDATE_SET column_ref[columnRef] expr[theExpr])
		{
			PandaSQL::ParserDriver *pDriver = $stmt::pDriver;
			pDriver->GetStatement().AddColumnRef(columnRef);
			pDriver->GetStatement().AddExprRef(theExpr);
		}
	;
	
insert_stmt
@init
{
	std::string tableRef;
	std::string columnRef;
	PandaSQL::Expr theExpr;
}
	:	^(TOK_INSERT_STMT table_ref[tableRef]
			(column_ref[columnRef]
			{
				PandaSQL::ParserDriver *pDriver = $stmt::pDriver;
				pDriver->GetStatement().AddColumnRef(columnRef);
			})+
			TOK_INSERT_VALUES
			(expr[theExpr]
			{
				PandaSQL::ParserDriver *pDriver = $stmt::pDriver;
				pDriver->GetStatement().AddExprRef(theExpr);
			})+)
		{
			PandaSQL::ParserDriver *pDriver = $stmt::pDriver;
			pDriver->GetStatement().AddTableRef(tableRef);
		}
	;
	
delete_stmt
@init
{
	std::string tableRef;
}
	:	^(TOK_DELETE_STMT table_ref[tableRef] where_clause?)
		{
			PandaSQL::ParserDriver *pDriver = $stmt::pDriver;
			pDriver->GetStatement().AddTableRef(tableRef);
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
	
column_ref[std::string &o_columnRef]
@init
{
	std::string columnRef;
	std::string tableRef = PandaSQL::Statement::kNoTable;
	bool hasTableRef = false;
}
	:	^(TOK_COLUMN_REF identifier[columnRef] identifier[tableRef]?)
		{
			o_columnRef = PandaSQL::ParserDriver::GetColumnRef(tableRef, columnRef);
		}
	;
	
table_ref[std::string &o_tableRef]
@init
{
}
	:	^(TOK_TABLE_REF identifier[o_tableRef])
		{
		}
	;
	
index_ref[std::string &o_tableRef]
	:	^(TOK_INDEX_REF identifier[o_tableRef])
		{
			
		}
	;
	
identifier[std::string &o_str]
	:	id=IDENTIFIER
		{
			PandaSQL::ParserDriver::GetIdentifier($id, o_str);
		}
	;
	
expr[PandaSQL::Expr &o_expr]
@init
{
	std::string columnRef;
}
	:	num=NUMBER_LITERAL
		{
			o_expr.type = PandaSQL::kExprNumber;
			PandaSQL::ParserDriver::GetIdentifier($num, o_expr.text);
		}
	|	str=STRING_LITERAL
		{
			o_expr.type = PandaSQL::kExprText;
			PandaSQL::ParserDriver::GetIdentifier($str, o_expr.text);
		}
	|	column_ref[columnRef]
		{
			o_expr.type = PandaSQL::kExprColumnRef;
			o_expr.text = columnRef;
		}
	;
