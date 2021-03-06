/** \file
 *  This C header file was generated by $ANTLR version 3.5.2
 *
 *     -  From the grammar source file : SimpleC.g
 *     -                            On : 2021-03-10 19:42:25
 *     -                 for the lexer : SimpleCLexerLexer
 *
 * Editing it, at least manually, is not wise.
 *
 * C language generator and runtime by Jim Idle, jimi|hereisanat|idle|dotgoeshere|ws.
 *
 *
 * The lexer 
SimpleCLexer

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
 * a parser context typedef pSimpleCLexer, which is returned from a call to SimpleCLexerNew().
 *
 * As this is a generated lexer, it is unlikely you will call it 'manually'. However
 * the methods are provided anyway.
 *
 * The methods in pSimpleCLexer are  as follows:
 *
 *  - 
 void
      pSimpleCLexer->T__39(pSimpleCLexer)
 *  - 
 void
      pSimpleCLexer->T__40(pSimpleCLexer)
 *  - 
 void
      pSimpleCLexer->T__41(pSimpleCLexer)
 *  - 
 void
      pSimpleCLexer->T__42(pSimpleCLexer)
 *  - 
 void
      pSimpleCLexer->T__43(pSimpleCLexer)
 *  - 
 void
      pSimpleCLexer->T__44(pSimpleCLexer)
 *  - 
 void
      pSimpleCLexer->T__45(pSimpleCLexer)
 *  - 
 void
      pSimpleCLexer->T__46(pSimpleCLexer)
 *  - 
 void
      pSimpleCLexer->T__47(pSimpleCLexer)
 *  - 
 void
      pSimpleCLexer->T__48(pSimpleCLexer)
 *  - 
 void
      pSimpleCLexer->T__49(pSimpleCLexer)
 *  - 
 void
      pSimpleCLexer->T__50(pSimpleCLexer)
 *  - 
 void
      pSimpleCLexer->T__51(pSimpleCLexer)
 *  - 
 void
      pSimpleCLexer->FOR(pSimpleCLexer)
 *  - 
 void
      pSimpleCLexer->VOID_TYPE(pSimpleCLexer)
 *  - 
 void
      pSimpleCLexer->ID(pSimpleCLexer)
 *  - 
 void
      pSimpleCLexer->INT(pSimpleCLexer)
 *  - 
 void
      pSimpleCLexer->CHARACTER_LITERAL(pSimpleCLexer)
 *  - 
 void
      pSimpleCLexer->STRING_LITERAL(pSimpleCLexer)
 *  - 
 void
      pSimpleCLexer->EscapeSequence(pSimpleCLexer)
 *  - 
 void
      pSimpleCLexer->EQ(pSimpleCLexer)
 *  - 
 void
      pSimpleCLexer->EQEQ(pSimpleCLexer)
 *  - 
 void
      pSimpleCLexer->NEQ(pSimpleCLexer)
 *  - 
 void
      pSimpleCLexer->OPLT(pSimpleCLexer)
 *  - 
 void
      pSimpleCLexer->LTEQ(pSimpleCLexer)
 *  - 
 void
      pSimpleCLexer->OPGT(pSimpleCLexer)
 *  - 
 void
      pSimpleCLexer->GTEQ(pSimpleCLexer)
 *  - 
 void
      pSimpleCLexer->PLUS(pSimpleCLexer)
 *  - 
 void
      pSimpleCLexer->MINUS(pSimpleCLexer)
 *  - 
 void
      pSimpleCLexer->DIVI(pSimpleCLexer)
 *  - 
 void
      pSimpleCLexer->REST(pSimpleCLexer)
 *  - 
 void
      pSimpleCLexer->MULT(pSimpleCLexer)
 *  - 
 void
      pSimpleCLexer->WS(pSimpleCLexer)
 *  - 
 void
      pSimpleCLexer->Tokens(pSimpleCLexer)
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

#ifndef	_SimpleCLexer_H
#define _SimpleCLexer_H
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
typedef struct SimpleCLexer_Ctx_struct SimpleCLexer, * pSimpleCLexer;



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

/** Context tracking structure for 
SimpleCLexer

 */
struct SimpleCLexer_Ctx_struct
{
    /** Built in ANTLR3 context tracker contains all the generic elements
     *  required for context tracking.
     */
    pANTLR3_LEXER    pLexer;

     void
     (*mT__39)	(struct SimpleCLexer_Ctx_struct * ctx);

     void
     (*mT__40)	(struct SimpleCLexer_Ctx_struct * ctx);

     void
     (*mT__41)	(struct SimpleCLexer_Ctx_struct * ctx);

     void
     (*mT__42)	(struct SimpleCLexer_Ctx_struct * ctx);

     void
     (*mT__43)	(struct SimpleCLexer_Ctx_struct * ctx);

     void
     (*mT__44)	(struct SimpleCLexer_Ctx_struct * ctx);

     void
     (*mT__45)	(struct SimpleCLexer_Ctx_struct * ctx);

     void
     (*mT__46)	(struct SimpleCLexer_Ctx_struct * ctx);

     void
     (*mT__47)	(struct SimpleCLexer_Ctx_struct * ctx);

     void
     (*mT__48)	(struct SimpleCLexer_Ctx_struct * ctx);

     void
     (*mT__49)	(struct SimpleCLexer_Ctx_struct * ctx);

     void
     (*mT__50)	(struct SimpleCLexer_Ctx_struct * ctx);

     void
     (*mT__51)	(struct SimpleCLexer_Ctx_struct * ctx);

     void
     (*mFOR)	(struct SimpleCLexer_Ctx_struct * ctx);

     void
     (*mVOID_TYPE)	(struct SimpleCLexer_Ctx_struct * ctx);

     void
     (*mID)	(struct SimpleCLexer_Ctx_struct * ctx);

     void
     (*mINT)	(struct SimpleCLexer_Ctx_struct * ctx);

     void
     (*mCHARACTER_LITERAL)	(struct SimpleCLexer_Ctx_struct * ctx);

     void
     (*mSTRING_LITERAL)	(struct SimpleCLexer_Ctx_struct * ctx);

     void
     (*mEscapeSequence)	(struct SimpleCLexer_Ctx_struct * ctx);

     void
     (*mEQ)	(struct SimpleCLexer_Ctx_struct * ctx);

     void
     (*mEQEQ)	(struct SimpleCLexer_Ctx_struct * ctx);

     void
     (*mNEQ)	(struct SimpleCLexer_Ctx_struct * ctx);

     void
     (*mOPLT)	(struct SimpleCLexer_Ctx_struct * ctx);

     void
     (*mLTEQ)	(struct SimpleCLexer_Ctx_struct * ctx);

     void
     (*mOPGT)	(struct SimpleCLexer_Ctx_struct * ctx);

     void
     (*mGTEQ)	(struct SimpleCLexer_Ctx_struct * ctx);

     void
     (*mPLUS)	(struct SimpleCLexer_Ctx_struct * ctx);

     void
     (*mMINUS)	(struct SimpleCLexer_Ctx_struct * ctx);

     void
     (*mDIVI)	(struct SimpleCLexer_Ctx_struct * ctx);

     void
     (*mREST)	(struct SimpleCLexer_Ctx_struct * ctx);

     void
     (*mMULT)	(struct SimpleCLexer_Ctx_struct * ctx);

     void
     (*mWS)	(struct SimpleCLexer_Ctx_struct * ctx);

     void
     (*mTokens)	(struct SimpleCLexer_Ctx_struct * ctx);
    const char * (*getGrammarFileName)();
    void            (*reset)  (struct SimpleCLexer_Ctx_struct * ctx);
    void	    (*free)   (struct SimpleCLexer_Ctx_struct * ctx);
};

// Function protoypes for the constructor functions that external translation units
// such as delegators and delegates may wish to call.
//
ANTLR3_API pSimpleCLexer SimpleCLexerNew         (
pANTLR3_INPUT_STREAM
 instream);
ANTLR3_API pSimpleCLexer SimpleCLexerNewSSD      (
pANTLR3_INPUT_STREAM
 instream, pANTLR3_RECOGNIZER_SHARED_STATE state);

/** Symbolic definitions of all the tokens that the 
lexer
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

/* End of token definitions for SimpleCLexer
 * =============================================================================
 */
/** } */

#ifdef __cplusplus
}
#endif

#endif

/* END - Note:Keep extra line feed to satisfy UNIX systems */
