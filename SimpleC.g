grammar SimpleC;
options 
{
    output			= AST;
    language		= C;
	ASTLabelType	= pANTLR3_BASE_TREE;
}

tokens 
{
    VAR_DEF;
    ARG_DEF;
    FUNC_HDR;
    FUNC_DECL;
    FUNC_DEF;
    BLOCK;
    IF_STAT;
    COND;
    ELSE_STAT;
    WHI_STAT;
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
    |   cstat
    ;

variable
    :   type declarator ';'	-> ^(VAR_DEF type declarator)
    |   type declarator EQ expr ';'-> ^(EQ type declarator expr)
    ;

declarator
    :   ID
    |   '*' ID
    |   '&' ID 
    ;

functionHeader
    :   type ID '(' ( formalParameter ( ',' formalParameter )* )? ')'

				-> ^(FUNC_HDR type ID formalParameter+)
    ;

formalParameter
    :   type declarator 
				-> ^(ARG_DEF type declarator)
    ;

type
    :   'int'   
    |   'char'  
    |   'void'
    |   ID        
    ;

block
    :   lc='{'
            variable*
            cstat*
        '}'
        -> ^(BLOCK[$lc, "BLOCK"] variable* cstat*)
    ;

cstat: forStat
    | expr ';'!
    | block
    | assignStat ';'!
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
    ;

expr:   condExpr
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
INT_TYPE : 'int' ;
CHAR_TYPE: 'char';
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
