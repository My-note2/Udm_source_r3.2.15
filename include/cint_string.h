#ifndef MOBIES_CINT_STRING_H 
#define MOBIES_CINT_STRING_H
/*
04/27/04 -	endre
			added
			- copy constructor
			- += int, += char operators
*/

class  cint_string
{
	char * impl;
	int length;											// means capacity, i.e. the size of valid memory pointed by impl!
	
public:
	cint_string();										//for creating a NULL string
	cint_string(int length);							//for creating an empty string, length chars long

	cint_string(const char * frm);						//for creating a cint_string containing frm
	cint_string(const cint_string& frm);						
	cint_string& operator=(const cint_string& frm);		
	cint_string& operator+=(const cint_string& what);	//requested by Feng
	cint_string& operator+=(const char what);	//requested by vijan
	cint_string& operator+=(const int what);	//requested by vijan

	bool operator ==(const cint_string& what);			//also requested by Feng

	bool CopyFrom(const char *);
	bool operator !() const;
	operator bool() const;

	
	~cint_string();
	int comp(const char * compare_to) const;
	bool overflow;
	char * buffer() const;

	char operator[]( int index) const;	// returns 0 if index is out of range
	int len() const;
};

#endif //MOBIES_CINT_STRING_H

