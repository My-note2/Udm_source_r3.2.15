// int_exception.h: interface for the int_exception class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INT_EXCEPTION_H__63768823_219C_47FD_810F_3A188864AD1F__INCLUDED_)
#define AFX_INT_EXCEPTION_H__63768823_219C_47FD_810F_3A188864AD1F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>

class int_exception  
{
protected:
	std::string emsg;
public:
	int_exception(char *EMSG) : emsg(EMSG) {}
	int_exception(const char *EMSG) : emsg(EMSG) {}
	int_exception(std::string EMSG) : emsg(EMSG) {}

	std::string what() const { return emsg; }

	virtual ~int_exception();

};

#endif // !defined(AFX_INT_EXCEPTION_H__63768823_219C_47FD_810F_3A188864AD1F__INCLUDED_)
