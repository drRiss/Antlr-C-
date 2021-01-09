/** \file
 *  This C header file was generated by $ANTLR version 3.5.2
 *
 *     -  From the grammar source file : SimpleCWalker.g
 *     -                            On : 2020-12-27 16:50:21
 *     -           for the tree parser : SimpleCWalkerTreeParser
 *
 * Editing it, at least manually, is not wise.
 *
 * C language generator and runtime by Jim Idle, jimi|hereisanat|idle|dotgoeshere|ws.
 *
 *
 * The tree parser 
SimpleCWalker

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
 * a parser context typedef pSimpleCWalker, which is returned from a call to SimpleCWalkerNew().
 *
 * The methods in pSimpleCWalker are  as follows:
 *
 *  - 
 void
      pSimpleCWalker->program(pSimpleCWalker)
 *  - 
 void
      pSimpleCWalker->declaration(pSimpleCWalker)
 *  - 
 void
      pSimpleCWalker->variable(pSimpleCWalker)
 *  - 
 void
      pSimpleCWalker->declarator(pSimpleCWalker)
 *  - 
 void
      pSimpleCWalker->functionHeader(pSimpleCWalker)
 *  - 
 void
      pSimpleCWalker->formalParameter(pSimpleCWalker)
 *  - 
 void
      pSimpleCWalker->type(pSimpleCWalker)
 *  - 
 void
      pSimpleCWalker->block(pSimpleCWalker)
 *  - 
 void
      pSimpleCWalker->cstat(pSimpleCWalker)
 *  - 
 void
      pSimpleCWalker->forStat(pSimpleCWalker)
 *  - 
 void
      pSimpleCWalker->expr(pSimpleCWalker)
 *  - 
 void
      pSimpleCWalker->atom(pSimpleCWalker)
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

#ifndef	_SimpleCWalker_H
#define _SimpleCWalker_H
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
typedef struct SimpleCWalker_Ctx_struct SimpleCWalker, * pSimpleCWalker;



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

/** Context tracking structure for 
SimpleCWalker

 */
struct SimpleCWalker_Ctx_struct
{
    /** Built in ANTLR3 context tracker contains all the generic elements
     *  required for context tracking.
     */
    pANTLR3_TREE_PARSER	    pTreeParser;


     void
     (*program)	(struct SimpleCWalker_Ctx_struct * ctx);

     void
     (*declaration)	(struct SimpleCWalker_Ctx_struct * ctx);

     void
     (*variable)	(struct SimpleCWalker_Ctx_struct * ctx);

     void
     (*declarator)	(struct SimpleCWalker_Ctx_struct * ctx);

     void
     (*functionHeader)	(struct SimpleCWalker_Ctx_struct * ctx);

     void
     (*formalParameter)	(struct SimpleCWalker_Ctx_struct * ctx);

     void
     (*type)	(struct SimpleCWalker_Ctx_struct * ctx);

     void
     (*block)	(struct SimpleCWalker_Ctx_struct * ctx);

     void
     (*cstat)	(struct SimpleCWalker_Ctx_struct * ctx);

     void
     (*forStat)	(struct SimpleCWalker_Ctx_struct * ctx);

     void
     (*expr)	(struct SimpleCWalker_Ctx_struct * ctx);

     void
     (*atom)	(struct SimpleCWalker_Ctx_struct * ctx);
    // Delegated rules

    const char * (*getGrammarFileName)();
    void            (*reset)  (struct SimpleCWalker_Ctx_struct * ctx);
    void	    (*free)   (struct SimpleCWalker_Ctx_struct * ctx);
};

// Function protoypes for the constructor functions that external translation units
// such as delegators and delegates may wish to call.
//
ANTLR3_API pSimpleCWalker SimpleCWalkerNew         (
pANTLR3_COMMON_TREE_NODE_STREAM
 instream);
ANTLR3_API pSimpleCWalker SimpleCWalkerNewSSD      (
pANTLR3_COMMON_TREE_NODE_STREAM
 instream, pANTLR3_RECOGNIZER_SHARED_STATE state);

/** Symbolic definitions of all the tokens that the 
tree parser
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
#define T__37      37
#define T__38      38
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
#define ID      18
#define IF_STAT      19
#define INT      20
#define MINUS      21
#define MULT      22
#define NEQ      23
#define OPGT      24
#define OPLT      25
#define PLUS      26
#define POI_DEF      27
#define POI_TYPE      28
#define REST      29
#define RET      30
#define STRING_LITERAL      31
#define UN_OP      32
#define VAR_DEF      33
#define VOID_TYPE      34
#define WHI_STAT      35
#define WS      36
#ifdef	EOF
#undef	EOF
#define	EOF	ANTLR3_TOKEN_EOF
#endif

#ifndef TOKENSOURCE
#define TOKENSOURCE(lxr) lxr->pLexer->rec->state->tokSource
#endif

/* End of token definitions for SimpleCWalker
 * =============================================================================
 */
/** } */

#ifdef __cplusplus
}
#endif

#endif

/* END - Note:Keep extra line feed to satisfy UNIX systems */
