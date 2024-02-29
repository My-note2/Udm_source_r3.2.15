
header
{
	//this goes in the source file
	#include <strstream>
	#include "CardinalityObject.h"
	#include "AttributeObject.h"



}
options 
{
	language="Cpp";
} 


class Pattrib extends Parser;

startRule:	 
		(
			(STRING STRING STRING COLON STRING) =>
			nonpersistent visibility name COLON type
			| 
			(STRING STRING COLON STRING) =>
			visib_np name COLON type
			| 
			(VISIB_CHAR STRING COLON STRING) =>
			visib_char name COLON type
			|
			name COLON type
		) 
		(
			LBRACKET
			(
				(multiplicity STRING) => multiplicity order
				|multiplicity 
				|order
			)
			RBRACKET
		)?
		(
			EQUAL
			(
				defvalnum (COMMA defvalnum)*
			| 
				defvalstr (COMMA defvalstr)*
			|
				defvalbool (COMMA defvalbool)* 
			)
			
		)? EOF
		;


visib_char
		:n:VISIB_CHAR
		{AttributeObject::GetInstance()->setVisib(*(n->getText().begin()));}
		;

visib_np
		:n:STRING
		{isVisibNpSpec(n->getText())}?
		{AttributeObject::GetInstance()->setVisibNp(n->getText());}
		;
visibility
		:n:STRING
		{isVisibSpec(n->getText())}?
		{AttributeObject::GetInstance()->setVisib(n->getText());}
		;

nonpersistent
		:n:STRING
		{isVolatileSpec(n->getText())}?
		{AttributeObject::GetInstance()->setVolReg(n->getText());}
		;

type
		:n:STRING	
		{isTypeSpec(n->getText())}?
		{AttributeObject::GetInstance()->setType(n->getText());}
		;

name
		:n:STRING
		{AttributeObject::GetInstance()->setName(n->getText());}
		;

order
		:n:STRING
		{isOrderSpec(n->getText())}?
		{AttributeObject::GetInstance()->setOrder(n->getText());}
		;

defvalnum
		:
		(
			(defvalint POINT defvalint) => defvalreal POINT defvalreal
		|	defvalint
		)
		;


defvalint
		:n:NUM
		{AttributeObject::GetInstance()->CheckNum(n->getText())}?
		{AttributeObject::GetInstance()->addDefNum(n->getText());}
		;

defvalreal
		:n:NUM
		{AttributeObject::GetInstance()->CheckNum(n->getText())}?
		{AttributeObject::GetInstance()->addDefReal(n->getText());}
		;

defvalstr
		:n:STRING_LITERAL
		{AttributeObject::GetInstance()->addDefStr(n->getText());}
		;

defvalbool
		:n:STRING
		{isBoolSpec(n->getText())}?
		{AttributeObject::GetInstance()->addDefBool(n->getText());}
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


class Lattrib extends Lexer;
options { k=2; }

WS_	:	(' '
	|	'\t'
	|	'\n'
	|	'\r')
		{ _ttype = ANTLR_USE_NAMESPACE(antlr)Token::SKIP; }
	;
 
VISIB_CHAR:
	('#' | '+' | '-' | '~') WS_
	;

STRING:		('a'..'z'|'A'..'Z'|'_') ('a'..'z'|'A'..'Z'|'_'|'0'..'9'|'.')* 
	;


COLON:	':'
	;

EQUAL:	'='
	;

LBRACKET:	'['
	;

RBRACKET:	']'
	;

LPARANT:	'('
	;

RPARANT:	')'
	;

STAR:	'*'
	;

RANGE
    :   ".."
    ;

COMMA
	:	','
	;

POINT
	:	'.'
	;


NUM	:	('-'|'+')? (DIGIT)+ 
	; 


STRING_LITERAL
	:	'"' (~('"'|';'|'\\'))* '"'
	;
 
//ESC	:	'\\' '\\' 
//	;

protected
DIGIT
	:	'0'..'9'
	;

