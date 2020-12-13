grammar SimpleC;
options 
{
    backtrack=true;
    output			= AST;
    language		= C;
	ASTLabelType	= pANTLR3_BASE_TREE;
}

tokens 
{
    VAR_DEF;
    ARG_DEF;
    POI_DEF;
    FUNC_HDR;
    FUNC_DECL;
    FUNC_DEF;
    BLOCK;
    IF_STAT;
    COND;
    ELSE_STAT;
    WHI_STAT;
    FUNC_CALL;
    RET;
}

@header{
#define _empty NULL
}

@lexer::header{
#define _empty NULL
}

program
    :   declaration+
    ;

declaration
    :   variable
    |   functionHeader ';'	-> ^(FUNC_DECL functionHeader)
    |   functionHeader block	-> ^(FUNC_DEF  functionHeader block)
    |   functionCall -> ^(functionCall)
    |   cstat
    ;

variable
    :   type declarator lc=';'	-> ^(VAR_DEF[lc, "VAR_DEF"] type declarator)
    |   type declarator EQ expr ';'-> ^(EQ type declarator expr)
    |   typepointer declarator lc=';' -> ^(POI_DEF[lc, "POI_DEF"] typepointer declarator)
    ;

declarator
    :   ID
    ;

functionHeader
    :   type ID '(' ( formalParameter ( ',' formalParameter )* )? ')'
        -> ^(FUNC_HDR type ID formalParameter*)
    ;       

functionCall
    :   declarator lc='(' ( (ID | INT) ( ',' (ID | INT) )* )? ')' ';' ->^(FUNC_CALL[lc, "FUNC_CALL"] declarator ID*  INT* )
    ;

formalParameter
    :   type declarator 
				-> ^(ARG_DEF type declarator)
    ;

type
    :   'int'   
    |   'char'  
    |   'void'
    ;

typepointer
    : type'*' 
    ;

block
    :   lc='{'
            variable*
            cstat*
            returnStatement?
        '}'
        -> ^(BLOCK[$lc, "BLOCK"] variable* cstat* returnStatement?)
    ;

cstat: forStat
    | expr ';'!
    | block
    | assignStat
    | ifStatement
    | whileStatement
    | ';'!
    ;

whileStatement
    : 'while' '(' condExpr ')' block -> ^(WHI_STAT condExpr block)
    ;

ifStatement
  :  ifStat elseStat? -> ^(IF_STAT ifStat elseStat?)
  ;

ifStat
  :  'if' '('condExpr ')' block -> ^(COND condExpr block)
  ;

elseStat
  :  'else' block -> ^(ELSE_STAT block)
  ;

forStat
    :   'for' '(' start=assignStat ';' e=expr ';' next=assignStat ')' block
        -> ^('for' $start $e $next block)
    ;

assignStat
    :   ID EQ expr -> ^(EQ ID expr)
    |   ID EQ functionCall -> ^(EQ ID functionCall)
    ;

expr:   condExpr
    ;

returnStatement
    :
    | 'return' ';' -> ^(RET)
    | 'return' expr ';' -> ^(RET expr)
    ;

condExpr
    :   aexpr ( ('=='^ | '<'^ |'>'^ |'<='^ |'>='^ | '!='^ ) aexpr )?
    ;

aexpr
    :   atom ( ('+'^ | '-'^ | '%'^ | '*'^ | '/'^) atom )*
    ;


atom
    : ID      
    | INT
    | CHARACTER_LITERAL
    | STRING_LITERAL
    | '(' expr ')' -> expr
    ; 

FOR : 'for' ;

VOID_TYPE: 'void';



ID  :   ('a'..'z'|'A'..'Z'|'_') ('a'..'z'|'A'..'Z'|'0'..'9'|'_')*
    ;

INT :	('0' | '1'..'9' '0'..'9'*)
    ;

CHARACTER_LITERAL
    :   '\'' ( EscapeSequence | ~('\''|'\\') ) '\''
    ;
STRING_LITERAL
    :  '"' ( EscapeSequence | ~('\\'|'"') )* '"'
    ;

fragment
EscapeSequence
    :   '\\' ('b'|'t'|'n'|'f'|'r'|'\"'|'\''|'\\')
    ;

EQ   : '=' ;
EQEQ : '==' ;
NEQ : '!=' ;
OPLT   : '<' ;
OPGT : '>' ;
PLUS : '+' ;
MINUS : '-' ;
DIVI : '/' ;
REST : '%' ;
MULT : '*' ;


WS  :   (   ' '
        |   '\t'
        |   '\r'
        |   '\n'
        )+
        { $channel=HIDDEN; }
    ;    
