#include "stdafx.h"

#include "Parser/ParserDriver.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "Parser/SQLLexer.h"
#include "Parser/SQLParser.h"
#include "Parser/SQLSemanticAnalyzer.h"

#include "Expr/BinaryExpr.h"
#include "Expr/BooleanExpr.h"
#include "Expr/ColumnExpr.h"
#include "Expr/ConstantExpr.h"

#include "Storage/IStorage.h"

#include "Utils/Common.h"
#include "Utils/Debug.h"
#include "Utils/Status.h"
#include "Utils/Types.h"

namespace PandaSQL
{


void PrintASTTree(pANTLR3_BASE_TREE parentTree, uint32_t level)
{
	for (uint32_t i = 0; i < level; i++)
	{
		std::cout<<"  ";
	}

	std::cout<<parentTree->toString(parentTree)->chars<<std::endl;

	pANTLR3_VECTOR children = parentTree->children;

	if (children)
	{
		ANTLR3_UINT32 n = parentTree->children->size(parentTree->children);
		
		for	(ANTLR3_UINT32 i = 0; i < n; i++)
		{
			pANTLR3_BASE_TREE child = (pANTLR3_BASE_TREE)parentTree->children->get(parentTree->children, i);

			PrintASTTree(child, level + 1);
		}
	}
}

/**************************************************
**ParserDriver**
***************************************************/

ParserDriver::ParserDriver(DBImpl *io_pDB)
:
mpDB(io_pDB)
,mpStmt(NULL)
,mLoadTable(false)
{
}

ParserDriver::~ParserDriver()
{
	delete mpStmt;
	mpStmt = NULL;
}

void ParserDriver::PushNewStatement(Statement::StatementType inType)
{
	//TODO: support subquery
	PDASSERT(!mpStmt);

	if (!mpStmt)
	{
		mpStmt = new Statement(inType, mpDB);
	}
}

Status ParserDriver::LoadFromFile(File *inFile)
{
	Status result;



	return result;
}

Status ParserDriver::ParseQuery(std::string inQueryString, Statement **io_statement)
{
	printf("\nstmt: %s\n", inQueryString.c_str());

	Status result;

		// Now we declare the ANTLR related local variables we need.
    // Note that unless you are convinced you will never need thread safe
    // versions for your project, then you should always create such things
    // as instance variables for each invocation.
    // -------------------

    // Name of the input file. Note that we always use the abstract type pANTLR3_UINT8
    // for ASCII/8 bit strings - the runtime library guarantees that this will be
    // good on all platforms. This is a general rule - always use the ANTLR3 supplied
    // typedefs for pointers/types/etc.
    //
    pANTLR3_UINT8	    fName;

    // The ANTLR3 character input stream, which abstracts the input source such that
    // it is easy to provide input from different sources such as files, or 
    // memory strings.
    //
	// For input from file:
	//		input = antlr3FileStreamNew(pANTLR3_UINT8 fileName, ANTLR3_UINT32 encoding)
	//
	// For input from memory string:
	//		input = antlr3StringStreamNew(pANTLR3_UINT8 data, ANTLR3_UINT32 encoding, ANTLR3_UINT32 size, pANTLR3_UINT8 name)
    //
    // Note that this is essentially a pointer to a structure containing pointers to functions.
    // You can create your own input stream type (copy one of the existing ones) and override any
    // individual function by installing your own pointer after you have created the standard 
    // version.
    //
    pANTLR3_INPUT_STREAM	    input;

    // The lexer is of course generated by ANTLR, and so the lexer type is not upper case.
    // The lexer is supplied with a pANTLR3_INPUT_STREAM from whence it consumes its
    // input and generates a token stream as output.
    //
    pSQLLexer			    lxr;

    // The token stream is produced by the ANTLR3 generated lexer. Again it is a structure based
    // API/Object, which you can customise and override methods of as you wish. a Token stream is
    // supplied to the generated parser, and you can write your own token stream and pass this in
    // if you wish.
    //
    pANTLR3_COMMON_TOKEN_STREAM	    tstream;

    // The Lang parser is also generated by ANTLR and accepts a token stream as explained
    // above. The token stream can be any source in fact, so long as it implements the 
    // ANTLR3_TOKEN_SOURCE interface. In this case the parser does not return anything
    // but it can of course specify any kind of return type from the rule you invoke
    // when calling it.
    //
    pSQLParser			    psr;

    // The parser produces an AST, which is returned as a member of the return type of
    // the starting rule (any rule can start first of course). This is a generated type
    // based upon the rule we start with.
    //
    SQLParser_stmt_return	    langAST;   


    // The tree nodes are managed by a tree adaptor, which doles
    // out the nodes upon request. You can make your own tree types and adaptors
    // and override the built in versions. See runtime source for details and
    // eventually the wiki entry for the C target.
    //
    pANTLR3_COMMON_TREE_NODE_STREAM	nodes;

    // Finally, when the parser runs, it will produce an AST that can be traversed by the 
    // the tree parser: c.f. LangDumpDecl.g3t
    pSQLSemanticAnalyzer		    treePsr;

	fName = (pANTLR3_UINT8)inQueryString.c_str();

	//Input from file
	//input = antlr3FileStreamNew(fName, ANTLR3_ENC_8BIT);	
	input = antlr3StringStreamNew(fName, ANTLR3_ENC_8BIT, (ANTLR3_UINT32)inQueryString.length(), (pANTLR3_UINT8)"Memory");

    // The input will be created successfully, providing that there is enough
    // memory and the file exists etc
    //
    if (input == NULL)
    {
		fprintf(stderr, "Unable to open file %s\n", (char *)fName);
		exit(ANTLR3_ERR_NOMEM);
    }

    // Our input stream is now open and all set to go, so we can create a new instance of our
    // lexer and set the lexer input to our input stream:
    //  (file | memory | ?) --> inputstream -> lexer --> tokenstream --> parser ( --> treeparser )?
    //
    lxr	    = SQLLexerNew(input);	    // CLexerNew is generated by ANTLR

    // Need to check for errors
    //
    if ( lxr == NULL )
    {
			fprintf(stderr, "Unable to create the lexer due to malloc() failure1\n");
			exit(ANTLR3_ERR_NOMEM);
    }

    // Our lexer is in place, so we can create the token stream from it
    // NB: Nothing happens yet other than the file has been read. We are just 
    // connecting all these things together and they will be invoked when we
    // call the parser rule. ANTLR3_SIZE_HINT can be left at the default usually
    // unless you have a very large token stream/input. Each generated lexer
    // provides a token source interface, which is the second argument to the
    // token stream creator.
    // Note tha even if you implement your own token structure, it will always
    // contain a standard common token within it and this is the pointer that
    // you pass around to everything else. A common token as a pointer within
    // it that should point to your own outer token structure.
    //
    tstream = antlr3CommonTokenStreamSourceNew(ANTLR3_SIZE_HINT, TOKENSOURCE(lxr));

    if (tstream == NULL)
    {
		fprintf(stderr, "Out of memory trying to allocate token stream\n");
		exit(ANTLR3_ERR_NOMEM);
    }

    // Finally, now that we have our lexer constructed, we can create the parser
    //
    psr	    = SQLParserNew(tstream);  // CParserNew is generated by ANTLR3

    if (psr == NULL)
    {
		fprintf(stderr, "Out of memory trying to allocate parser\n");
		exit(ANTLR3_ERR_NOMEM);
    }

    // We are all ready to go. Though that looked complicated at first glance,
    // I am sure, you will see that in fact most of the code above is dealing
    // with errors and there isn't really that much to do (isn't this always the
    // case in C? ;-).
    //
    // So, we now invoke the parser. All elements of ANTLR3 generated C components
    // as well as the ANTLR C runtime library itself are pseudo objects. This means
    // that they are represented as pointers to structures, which contain any
    // instance data they need, and a set of pointers to other interfaces or
    // 'methods'. Note that in general, these few pointers we have created here are
    // the only things you will ever explicitly free() as everything else is created
    // via factories, that allocate memory efficiently and free() everything they use
    // automatically when you close the parser/lexer/etc.
    //
    // Note that this means only that the methods are always called via the object
    // pointer and the first argument to any method, is a pointer to the structure itself.
    // It also has the side advantage, if you are using an IDE such as VS2005 that can do it
    // that when you type ->, you will see a list of all the methods the object supports.
    //
    langAST = psr->stmt(psr);

    // If the parser ran correctly, we will have a tree to parse. In general I recommend
    // keeping your own flags as part of the error trapping, but here is how you can
    // work out if there were errors if you are using the generic error messages
    //
	if (psr->pParser->rec->state->errorCount > 0)
	{
		fprintf(stderr, "The parser returned %d errors, tree walking aborted.\n", psr->pParser->rec->state->errorCount);

	}
	else
	{
		if (langAST.tree)
		{
			nodes	= antlr3CommonTreeNodeStreamNewTree(langAST.tree, ANTLR3_SIZE_HINT); // sIZE HINT WILL SOON BE DEPRECATED!!

			if (!this->IsLoadTable())
			{
				PrintASTTree(langAST.tree, 0);
				//printf("\nParser OK: Nodes: %s\n", langAST.tree->toStringTree(langAST.tree)->chars);
			}

			// Tree parsers are given a common tree node stream (or your override)
			//
			treePsr	= SQLSemanticAnalyzerNew(nodes);

			treePsr->stmt(treePsr, this);
			nodes   ->free  (nodes);	    nodes	= NULL;
			treePsr ->free  (treePsr);	    treePsr	= NULL;
		}
	}

	// We did not return anything from this parser rule, so we can finish. It only remains
	// to close down our open objects, in the reverse order we created them
	//
	psr	    ->free  (psr);		psr		= NULL;
	tstream ->free  (tstream);	tstream	= NULL;
	lxr	    ->free  (lxr);	    lxr		= NULL;
	input   ->close (input);	input	= NULL;

	if (result.OK())
	{
		result = this->GetRootStatement().Prepare();
	}

	if (result.OK())
	{
		*io_statement = &this->GetRootStatement();
	}
	else
	{
		*io_statement = NULL;
	}
	
	return result;
}

void ParserDriver::PrintCurrentState()
{
	printf("PrintCurrentState\n");
}

void ParserDriver::GetString(ANTLR3_BASE_TREE *tree, std::string *o_str)
{
	ANTLR3_COMMON_TOKEN *token = tree->getToken(tree);
	ANTLR3_STRING *antlrString = token->getText(token);

	*o_str = std::string((const char *)token->start
			, (const char *)token->stop - (const char *)token->start + 1);
}

void ParserDriver::GetNumber(ANTLR3_BASE_TREE *tree, int32_t *o_num)
{
	ANTLR3_COMMON_TOKEN *token = tree->getToken(tree);
	ANTLR3_STRING *antlrString = token->getText(token);

	std::string tmpStr = std::string((const char *)token->start
			, (const char *)token->stop - (const char *)token->start + 1);
	
	*o_num = atoi(tmpStr.c_str());
}

void ParserDriver::GetStringFromAntlrString(const ANTLR3_STRING &inOpString, std::string *io_string)
{
	*io_string = std::string((const char *)inOpString.chars, inOpString.len);
}

void ParserDriver::GetExprForText(ANTLR3_BASE_TREE *tree, Expr *o_expr)
{
	//o_expr->mType = kExprText;
	GetString(tree, &o_expr->mTextValue);
}

void ParserDriver::GetExprForNumber(ANTLR3_BASE_TREE *tree, Expr *o_expr)
{
	//o_expr->mType = kExprNumber;
	GetNumber(tree, &o_expr->mNumberValue);
}

Expr* ParserDriver::CreateExprForNumericLiteral(ANTLR3_BASE_TREE *numericTree)
{
	ConstantExpr *pNumericExpr = new ConstantExpr();

	int32_t number;

	ParserDriver::GetNumber(numericTree, &number);

	pNumericExpr->SetNumber(number);

	return pNumericExpr;
}

Expr* ParserDriver::CreateExprForBinaryOp(const ANTLR3_STRING &inOpString, Expr *io_leftOperand, Expr *io_rightOperand)
{
	BinaryExpr *pBinaryExpr = new BinaryExpr();

	BinaryExpr::BinaryOpType op;
	std::string opString;
	ParserDriver::GetStringFromAntlrString(inOpString, &opString);

	if (opString == "=")
	{
		op = BinaryExpr::kBinaryEqual;
	}
	else
	{

	}

	pBinaryExpr->SetOpType(op);
	pBinaryExpr->SetLeftOperand(io_leftOperand);
	pBinaryExpr->SetRightOperand(io_rightOperand);

	return pBinaryExpr;
}

Expr* ParserDriver::CreateExprForColumnReference(const ColumnQualifiedName &inColumnQualifiedName)
{
	ColumnExpr *pColumnExpr = new ColumnExpr();
	
	pColumnExpr->SetQualifiedColumnName(inColumnQualifiedName);

	return pColumnExpr;
}

BooleanExpr* ParserDriver::CreateExprForBooleanPrimary(Expr *io_subExpr)
{
	BooleanExpr *pBooleanExpr = new BooleanExpr();

	pBooleanExpr->AddExpr(io_subExpr);
	pBooleanExpr->SetType(BooleanExpr::kBooleanNormal);

	return pBooleanExpr;
}

BooleanExpr* ParserDriver::CreateExprForBooleanAndList()
{
	BooleanExpr *pBooleanExpr = new BooleanExpr();

	pBooleanExpr->SetType(BooleanExpr::kBooleanAndList);

	return pBooleanExpr;
}

BooleanExpr* ParserDriver::CreateExprForBooleanOrList()
{
	BooleanExpr *pBooleanExpr = new BooleanExpr();

	pBooleanExpr->SetType(BooleanExpr::kBooleanOrList);

	return pBooleanExpr;
}

}	// namespace PandaSQL
