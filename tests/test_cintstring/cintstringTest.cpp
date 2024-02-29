// cintstringTest.cpp

//#include "StdAfx.h"
#include <UdmBase.h>
#include <cint_string.h>
#include "cintstringTest.h"
#include <cstring>

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( UdmTests::cintstringTest );

void 
UdmTests::cintstringTest::test()
{

	//Set Up
	cint_string a("hello");				//constructing from char *
	cint_string b = "hello";			//empty constructor and operator=(const char *)	
	cint_string c(b);					//constructing from an other cint_string
	cint_string d(5);					//constructing with length
	d = c;								//then assiging value
	cint_string e("hell");				
	e += 'o';							//operator += char
	cint_string f("hell");
	f += "o";							//operator += char *
	cint_string g("hell");				
	g += cint_string("o");				//operator += cints_string
	cint_string n;						//null string



	

	//check - these should be non-null
	CPPUNIT_ASSERT( (bool)a);
	CPPUNIT_ASSERT( (bool)b);
	CPPUNIT_ASSERT( (bool)c);
	CPPUNIT_ASSERT( (bool)d);
	CPPUNIT_ASSERT( (bool)e);
	CPPUNIT_ASSERT( (bool)f);
	CPPUNIT_ASSERT( (bool)g);

	//these should be equal all
	CPPUNIT_ASSERT( a == b);
	CPPUNIT_ASSERT( b == c);
	CPPUNIT_ASSERT( c == d);
	CPPUNIT_ASSERT( d == e);
	CPPUNIT_ASSERT( e == f);
	CPPUNIT_ASSERT( f == g);

	//it should be 'hello'
	CPPUNIT_ASSERT( strcmp(a.buffer(),"hello") == 0);

	//this should be null
	CPPUNIT_ASSERT( !(bool)n);
	CPPUNIT_ASSERT( !n);
}


