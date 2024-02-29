

/*

  CHANGELOG:
  
  12/31/05	-	endre
		- Removed the output to a stream operator

  29/03/04	-	endre

		Fixed bug #54, which was a side effect of the fix ofr bug #51
		multiple cardinality '0..*'. In the output xml file for the
		AssociationRole elements, attribute 'max' is set to "0", instead of "-1".

  22/03/04	-	endre
	
	  Fixed bug #51
	  if the cardinality in the model is 'n', the generated XML file should have 
		<AssociationRole name = " ", min = "n", max = "n">

		Instead, we have:
		<AssociationRole name = " ", min = "n", max = "1">

		which is of course not correct.
	
	  Fixed by changing getmax() to return the max(max,min).


  */
#include "StdAfx.h"

#include "CardinalityObject.h"

//parser
#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable:4996) // Disable POSIX name deprecated warning
#endif
#include "Lcardin.hpp"
#include "Pcardin.hpp"
#ifdef _WIN32
#pragma warning(pop)
#endif

#include "StringBuffer.h"

CardinalityObject* CardinalityObject::theInstance  = NULL;

CardinalityObject::CardinalityObject(const std::string str)
: min_set(false), star_rcvd(false),  UML_multiplicity(str)
{
	//warning C4003: not enough actual parameters for macro 'min'
	this->min = 1;
	this->max = 1;
	//allow parser to call back
	theInstance = this;
	
	StringBuffer strbuf(str);

	Lcardin lexer(strbuf);
	lexer.setFilename(std::string("buffer:") + str);
	Pcardin parser(lexer);
	parser.setFilename(std::string("buffer:") + str);

	try 
	{
		parser.startRule();
	}
	catch (...)
	{
		throw CardinalityObjectException(std::string("Unknown Parse error occurred while parsing:") + str);

	}

	theInstance = NULL;
};



CardinalityObject::CardinalityObject()
: min_set(false), star_rcvd(false)
{
	//warning C4003: not enough actual parameters for macro 'min'
	this->min = 1;
	this->max = 1;
	theInstance = this;
};

void CardinalityObject::ResetInstance()
{
	theInstance = NULL;
};


CardinalityObject * CardinalityObject ::GetInstance()
{
	if (theInstance == NULL)
		throw CardinalityObjectException("Error, instance is NULL!");
	return theInstance;
};

void CardinalityObject::int_rcv(const int i)
{
	if (i == 1) 
	{
		min_set = true;
		return;//this is the default behaviour
	}
	if (star_rcvd) 
		throw CardinalityObjectException("'*' should be the last character in the multiplicity specifier");

	if (!min_set)
	{
		min = i;
		min_set = true;
	}
	else
	{
		if ((min >= i) || (max >= i) )
			throw CardinalityObjectException("Intervals should be monotonically increasing!");
		
		max = i;
	}

};


void CardinalityObject::star_rcv()
{
	if (star_rcvd) throw CardinalityObjectException("'*' should be the last character in the multiplicity specifier");
	star_rcvd = true;
	max = -1;
	if (!min_set) 
	{
		min_set = true;
		//min = -1;
		min = 0;
	}
};

int CardinalityObject::getmin() const
{
	return min;
};

int CardinalityObject::getmax() const
{
	return max < 0 ? max : (max < min ? min : max );
};

std::string CardinalityObject::text()
{
	return UML_multiplicity;
};
