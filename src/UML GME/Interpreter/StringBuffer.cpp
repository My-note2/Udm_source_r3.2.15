#include "StdAfx.h"
#include "antlr/CommonToken.hpp"
#include "antlr/InputBuffer.hpp"
#include "antlr/BitSet.hpp"



#include "StringBuffer.h"

StringBuffer::StringBuffer(const std::string str) : 
ANTLR_USE_NAMESPACE(antlr)InputBuffer(), buf(str), index(0){}
	
int StringBuffer::getChar()
{
	if (index == buf.size()) return EOF;
	return (int) buf[index++];
}
