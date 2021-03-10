/** \file
 *  This C header file was generated by $ANTLR version 3.5.2
 *
 *     -  From the grammar source file : SimpleC.g
 *     -                            On : 2021-03-10 18:07:43
 *     -                for the parser : SimpleCParserParser
 *
 * Editing it, at least manually, is not wise.
 *
 * C language generator and runtime by Jim Idle, jimi|hereisanat|idle|dotgoeshere|ws.
 *
 *
 * The parser 
SimpleCParser

has the callable functions (rules) shown below,
 * which will invoke the code for the associated rule in the source grammar
 * assuming that the input stream is pointing to a token/text stream that could begin
 * this rule.
 *
 * For instance if you call the first (topmost) rule in a parser grammar, you will
 * get the results of a full parse, but calling a rule half way through the grammar will
 * allow you to pass part of a full token stream to the parser, such as for syntax checking
 * in editors and so on.
 *
 * The parser entry points are called indirectly (by function pointer to function) via
 * a parser context typedef pSimpleCParser, which is returned from a call to SimpleCParserNew().
 *
 * The methods in pSimpleCParser are  as follows:
 *
 *  - 
 SimpleCParser_program_return
      pSimpleCParser->program(pSimpleCParser)
 *  - 
 SimpleCParser_declaration_return
      pSimpleCParser->declaration(pSimpleCParser)
 *  - 
 SimpleCParser_variable_return
      pSimpleCParser->variable(pSimpleCParser)
 *  - 
 SimpleCParser_declarator_return
      pSimpleCParser->declarator(pSimpleCParser)
 *  - 
 SimpleCParser_functionHeader_return
      pSimpleCParser->functionHeader(pSimpleCParser)
 *  - 
 SimpleCParser_functionCall_return
      pSimpleCParser->functionCall(pSimpleCParser)
 *  - 
 SimpleCParser_formalParameter_return
      pSimpleCParser->formalParameter(pSimpleCParser)
 *  - 
 SimpleCParser_type_return
      pSimpleCParser->type(pSimpleCParser)
 *  - 
 SimpleCParser_typepointer_return
      pSimpleCParser->typepointer(pSimpleCParser)
 *  - 
 SimpleCParser_block_return
      pSimpleCParser->block(pSimpleCParser)
 *  - 
 SimpleCParser_returnStatement_return
      pSimpleCParser->returnStatement(pSimpleCParser)
 *  - 
 SimpleCParser_cstat_return
      pSimpleCParser->cstat(pSimpleCParser)
 *  - 
 SimpleCParser_whileStatement_return
      pSimpleCParser->whileStatement(pSimpleCParser)
 *  - 
 SimpleCParser_ifStatement_return
      pSimpleCParser->ifStatement(pSimpleCParser)
 *  - 
 SimpleCParser_ifStat_return
      pSimpleCParser->ifStat(pSimpleCParser)
 *  - 
 SimpleCParser_elseStat_return
      pSimpleCParser->elseStat(pSimpleCParser)
 *  - 
 SimpleCParser_forStat_return
      pSimpleCParser->forStat(pSimpleCParser)
 *  - 
 SimpleCParser_assignStat_return
      pSimpleCParser->assignStat(pSimpleCParser)
 *  - 
 SimpleCParser_expr_return
      pSimpleCParser->expr(pSimpleCParser)
 *  - 
 SimpleCParser_condExpr_return
      pSimpleCParser->condExpr(pSimpleCParser)
 *  - 
 SimpleCParser_aexpr_return
      pSimpleCParser->aexpr(pSimpleCParser)
 *  - 
 SimpleCParser_atom_return
      pSimpleCParser->atom(pSimpleCParser)
 *  - 
 SimpleCParser_unaryID_return
      pSimpleCParser->unaryID(pSimpleCParser)
 *  - 
 SimpleCParser_unaryOperator_return
      pSimpleCParser->unaryOperator(pSimpleCParser)
 * 
 * 
 * 
 * 
 * 
 *
 * The return type for any particular rule is of course determined by the source
 * grammar file.
 */
// [The "BSD license"]
// Copyright (c) 2005-2009 Jim Idle, Temporal Wave LLC
// http://www.temporal-wave.com
// http://www.linkedin.com/in/jimidle
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. The name of the author may not be used to endorse or promote products
//    derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
// NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef	_SimpleCParser_H
#define _SimpleCParser_H
/* =============================================================================
 * Standard antlr3 C runtime definitions
 */
#include    <antlr3.h>

/* End of standard antlr 3 runtime definitions
 * =============================================================================
 */

#ifdef __cplusplus
extern "C" {
#endif

// Forward declare the context typedef so that we can use it before it is
// properly defined. Delegators and delegates (from import statements) are
// interdependent and their context structures contain pointers to each other
// C only allows such things to be declared if you pre-declare the typedef.
//
typedef struct SimpleCParser_Ctx_struct SimpleCParser, * pSimpleCParser;



#define _empty NULL


#ifdef	ANTLR3_WINDOWS
// Disable: Unreferenced parameter,							- Rules with parameters that are not used
//          constant conditional,							- ANTLR realizes that a prediction is always true (synpred usually)
//          initialized but unused variable					- tree rewrite variables declared but not needed
//          Unreferenced local variable						- lexer rule declares but does not always use _type
//          potentially unitialized variable used			- retval always returned from a rule
//			unreferenced local function has been removed	- susually getTokenNames or freeScope, they can go without warnigns
//
// These are only really displayed at warning level /W4 but that is the code ideal I am aiming at
// and the codegen must generate some of these warnings by necessity, apart from 4100, which is
// usually generated when a parser rule is given a parameter that it does not use. Mostly though
// this is a matter of orthogonality hence I disable that one.
//
#pragma warning( disable : 4100 )
#pragma warning( disable : 4101 )
#pragma warning( disable : 4127 )
#pragma warning( disable : 4189 )
#pragma warning( disable : 4505 )
#pragma warning( disable : 4701 )
#endif

/* ========================
 * BACKTRACKING IS ENABLED
 * ========================
 */

typedef struct SimpleCParser_program_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    SimpleCParser_program_return;



typedef struct SimpleCParser_declaration_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    SimpleCParser_declaration_return;



typedef struct SimpleCParser_variable_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    SimpleCParser_variable_return;



typedef struct SimpleCParser_declarator_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    SimpleCParser_declarator_return;



typedef struct SimpleCParser_functionHeader_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    SimpleCParser_functionHeader_return;



typedef struct SimpleCParser_functionCall_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    SimpleCParser_functionCall_return;



typedef struct SimpleCParser_formalParameter_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    SimpleCParser_formalParameter_return;



typedef struct SimpleCParser_type_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    SimpleCParser_type_return;



typedef struct SimpleCParser_typepointer_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    SimpleCParser_typepointer_return;



typedef struct SimpleCParser_block_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    SimpleCParser_block_return;



typedef struct SimpleCParser_returnStatement_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    SimpleCParser_returnStatement_return;



typedef struct SimpleCParser_cstat_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    SimpleCParser_cstat_return;



typedef struct SimpleCParser_whileStatement_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    SimpleCParser_whileStatement_return;



typedef struct SimpleCParser_ifStatement_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    SimpleCParser_ifStatement_return;



typedef struct SimpleCParser_ifStat_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    SimpleCParser_ifStat_return;



typedef struct SimpleCParser_elseStat_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    SimpleCParser_elseStat_return;



typedef struct SimpleCParser_forStat_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    SimpleCParser_forStat_return;



typedef struct SimpleCParser_assignStat_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    SimpleCParser_assignStat_return;



typedef struct SimpleCParser_expr_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    SimpleCParser_expr_return;



typedef struct SimpleCParser_condExpr_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    SimpleCParser_condExpr_return;



typedef struct SimpleCParser_aexpr_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    SimpleCParser_aexpr_return;



typedef struct SimpleCParser_atom_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    SimpleCParser_atom_return;



typedef struct SimpleCParser_unaryID_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    SimpleCParser_unaryID_return;



typedef struct SimpleCParser_unaryOperator_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    SimpleCParser_unaryOperator_return;




/** Context tracking structure for 
SimpleCParser

 */
struct SimpleCParser_Ctx_struct
{
    /** Built in ANTLR3 context tracker contains all the generic elements
     *  required for context tracking.
     */
    pANTLR3_PARSER   pParser;

     SimpleCParser_program_return
     (*program)	(struct SimpleCParser_Ctx_struct * ctx);

     SimpleCParser_declaration_return
     (*declaration)	(struct SimpleCParser_Ctx_struct * ctx);

     SimpleCParser_variable_return
     (*variable)	(struct SimpleCParser_Ctx_struct * ctx);

     SimpleCParser_declarator_return
     (*declarator)	(struct SimpleCParser_Ctx_struct * ctx);

     SimpleCParser_functionHeader_return
     (*functionHeader)	(struct SimpleCParser_Ctx_struct * ctx);

     SimpleCParser_functionCall_return
     (*functionCall)	(struct SimpleCParser_Ctx_struct * ctx);

     SimpleCParser_formalParameter_return
     (*formalParameter)	(struct SimpleCParser_Ctx_struct * ctx);

     SimpleCParser_type_return
     (*type)	(struct SimpleCParser_Ctx_struct * ctx);

     SimpleCParser_typepointer_return
     (*typepointer)	(struct SimpleCParser_Ctx_struct * ctx);

     SimpleCParser_block_return
     (*block)	(struct SimpleCParser_Ctx_struct * ctx);

     SimpleCParser_returnStatement_return
     (*returnStatement)	(struct SimpleCParser_Ctx_struct * ctx);

     SimpleCParser_cstat_return
     (*cstat)	(struct SimpleCParser_Ctx_struct * ctx);

     SimpleCParser_whileStatement_return
     (*whileStatement)	(struct SimpleCParser_Ctx_struct * ctx);

     SimpleCParser_ifStatement_return
     (*ifStatement)	(struct SimpleCParser_Ctx_struct * ctx);

     SimpleCParser_ifStat_return
     (*ifStat)	(struct SimpleCParser_Ctx_struct * ctx);

     SimpleCParser_elseStat_return
     (*elseStat)	(struct SimpleCParser_Ctx_struct * ctx);

     SimpleCParser_forStat_return
     (*forStat)	(struct SimpleCParser_Ctx_struct * ctx);

     SimpleCParser_assignStat_return
     (*assignStat)	(struct SimpleCParser_Ctx_struct * ctx);

     SimpleCParser_expr_return
     (*expr)	(struct SimpleCParser_Ctx_struct * ctx);

     SimpleCParser_condExpr_return
     (*condExpr)	(struct SimpleCParser_Ctx_struct * ctx);

     SimpleCParser_aexpr_return
     (*aexpr)	(struct SimpleCParser_Ctx_struct * ctx);

     SimpleCParser_atom_return
     (*atom)	(struct SimpleCParser_Ctx_struct * ctx);

     SimpleCParser_unaryID_return
     (*unaryID)	(struct SimpleCParser_Ctx_struct * ctx);

     SimpleCParser_unaryOperator_return
     (*unaryOperator)	(struct SimpleCParser_Ctx_struct * ctx);

     ANTLR3_BOOLEAN
     (*synpred2_SimpleC)	(struct SimpleCParser_Ctx_struct * ctx);

     ANTLR3_BOOLEAN
     (*synpred3_SimpleC)	(struct SimpleCParser_Ctx_struct * ctx);

     ANTLR3_BOOLEAN
     (*synpred4_SimpleC)	(struct SimpleCParser_Ctx_struct * ctx);

     ANTLR3_BOOLEAN
     (*synpred5_SimpleC)	(struct SimpleCParser_Ctx_struct * ctx);

     ANTLR3_BOOLEAN
     (*synpred17_SimpleC)	(struct SimpleCParser_Ctx_struct * ctx);
    // Delegated rules

    const char * (*getGrammarFileName)();
    void            (*reset)  (struct SimpleCParser_Ctx_struct * ctx);
    void	    (*free)   (struct SimpleCParser_Ctx_struct * ctx);
/* @headerFile.members() */
pANTLR3_BASE_TREE_ADAPTOR	adaptor;
pANTLR3_VECTOR_FACTORY		vectors;
/* End @headerFile.members() */
};

// Function protoypes for the constructor functions that external translation units
// such as delegators and delegates may wish to call.
//
ANTLR3_API pSimpleCParser SimpleCParserNew         (
pANTLR3_COMMON_TOKEN_STREAM
 instream);
ANTLR3_API pSimpleCParser SimpleCParserNewSSD      (
pANTLR3_COMMON_TOKEN_STREAM
 instream, pANTLR3_RECOGNIZER_SHARED_STATE state);

/** Symbolic definitions of all the tokens that the 
parser
 will work with.
 * \{
 *
 * Antlr will define EOF, but we can't use that as it it is too common in
 * in C header files and that would be confusing. There is no way to filter this out at the moment
 * so we just undef it here for now. That isn't the value we get back from C recognizers
 * anyway. We are looking for ANTLR3_TOKEN_EOF.
 */
#ifdef	EOF
#undef	EOF
#endif
#ifdef	Tokens
#undef	Tokens
#endif
#define EOF      -1
#define T__39      39
#define T__40      40
#define T__41      41
#define T__42      42
#define T__43      43
#define T__44      44
#define T__45      45
#define T__46      46
#define T__47      47
#define T__48      48
#define T__49      49
#define T__50      50
#define T__51      51
#define ARG_DEF      4
#define BLOCK      5
#define CHARACTER_LITERAL      6
#define COND      7
#define DIVI      8
#define ELSE_STAT      9
#define EQ      10
#define EQEQ      11
#define EscapeSequence      12
#define FOR      13
#define FUNC_CALL      14
#define FUNC_DECL      15
#define FUNC_DEF      16
#define FUNC_HDR      17
#define GTEQ      18
#define ID      19
#define IF_STAT      20
#define INT      21
#define LTEQ      22
#define MINUS      23
#define MULT      24
#define NEQ      25
#define OPGT      26
#define OPLT      27
#define PLUS      28
#define POI_DEF      29
#define POI_TYPE      30
#define REST      31
#define RET      32
#define STRING_LITERAL      33
#define UN_OP      34
#define VAR_DEF      35
#define VOID_TYPE      36
#define WHI_STAT      37
#define WS      38
#ifdef	EOF
#undef	EOF
#define	EOF	ANTLR3_TOKEN_EOF
#endif

#ifndef TOKENSOURCE
#define TOKENSOURCE(lxr) lxr->pLexer->rec->state->tokSource
#endif

/* End of token definitions for SimpleCParser
 * =============================================================================
 */
/** } */

#ifdef __cplusplus
}
#endif

#endif

/* END - Note:Keep extra line feed to satisfy UNIX systems */
