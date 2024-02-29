
header
{
	//this goes in the source file
	#include <strstream>
	#include "CardinalityObject.h"
}

options 
{
	language="Cpp";
} 


class Pcardin extends Parser;



startRule:	 
		(multiplicity)? EOF
		;


multiplicity
		:  
		(
			star									
		|	(atom RANGE star)=> atom RANGE star
		|	atom (|RANGE atom) (COMMA (atom(|RANGE (atom|star))) )* 
		) 
		;

atom
		:n:NUM
		{CardinalityObject::GetInstance()->int_rcv(atoi(n->getText().c_str()));}

		;

star
		:n:STAR
		{CardinalityObject::GetInstance()->star_rcv();}
		;


class Lcardin extends Lexer;



WS_	:	(' '
	|	'\t'
	|	'\n'
	|	'\r')
		{ _ttype = ANTLR_USE_NAMESPACE(antlr)Token::SKIP; }
	;
 

STAR:	'*'
	;

RANGE
    :   ".."
    ;

COMMA
	:	','
	;



NUM	:	(DIGIT)+
	; 



protected
DIGIT
	:	'0'..'9'
	;

