

#ifndef MOBIES_STRINGBUFFER_H
#define MOBIES_STRINGBUFFER_H
/*
	An in-memory stream for ANTLR Lexer
*/
class StringBuffer : public ANTLR_USE_NAMESPACE(antlr)InputBuffer
{
private:
	std::string buf;
	unsigned int index;
public:
	StringBuffer(const std::string str);
	int getChar();
};


#endif