

/*
  03/28/05 - kalmar
        if (impl) delete[] impl, impl = 0; bug fixed

	04/27/04 -	endre
			added
			- copy constructor
			- += int, += char operators
*/

#include <UdmBase.h>
#include <cint_string.h>

#include <cstring>



UDM_DLL cint_string::cint_string()
{
	impl = NULL;
	length = 0;
};

UDM_DLL cint_string::cint_string(int length)
{
	impl = new char[length + 1];
	this->length = length;
	overflow = false;
};

UDM_DLL cint_string::cint_string(const cint_string& frm)
{
	impl = NULL;
	length = 0;

	*this = frm;
};

UDM_DLL cint_string::cint_string(const char * frm)
{
	if (frm)
	{
		length = strlen(frm);
		impl = new char[length + 1];
		strncpy(impl, frm, length + 1);
		
	} else 
	{
		impl = NULL;
		length = 0;
	}
	overflow = false;
};

UDM_DLL cint_string& cint_string::operator=(const cint_string& frm)
{
	if (impl) delete[] impl, impl = 0;

	if (frm.impl)
	{
//		length = strlen(frm.impl);
		length= frm.length;
		impl = new char[length + 1];
//		strncpy(impl, frm.impl, length + 1);
		strcpy( impl, frm.impl);
	}
	else 
	{
		length = 0;
		impl = NULL;
	}
	overflow = false;
	return *this;
};

UDM_DLL bool cint_string::operator ==(const cint_string& what)
{
	//if both are NULL, they are identical!
	if ((length == 0) && !what) return true;

	//if either is NULL, they are nmot identical
	if ((length == 0) || !what)	return false;

	//if both exists, they can be compared
	return (strcmp(impl, what.buffer()) == 0);
}

UDM_DLL cint_string& cint_string::operator +=(const char what)
{
	char t[2]; 
	t[0] = what;
	t[1] = '\0';
	cint_string cs(t);
	*this += cs;
	return *this;
};

UDM_DLL cint_string& cint_string::operator +=(const int  what)
{
	char t[20];
	sprintf(t, "%d",what);
	cint_string cs(t);
	*this += cs;
	return *this;

};

UDM_DLL cint_string& cint_string::operator +=(const cint_string& what)
{
	int what_length= strlen( what.buffer());
	int curr_length= strlen( impl);
	if ( what_length+ curr_length > length)
	{
		// not enough space -> realloc
		char* incr_impl= new char[ (what_length+ curr_length) * 2];
		strcpy( incr_impl, impl);
		delete[] impl, impl = 0;
		impl= incr_impl;
	}
	strcat( impl, what.buffer());
	/*
	//will hold the current value
	string temp_string(impl);
	
	
	//append the value
	temp_string += what.buffer();

	//create a new cint_string, then assign it
	//note that this will invoke cint_string constructor and
	//cint_string::operator=() another operator
	const cint_string temp_cint  = temp_string.c_str();

	*this = temp_cint;
	*/
	return *this;
};

UDM_DLL bool cint_string::operator !() const
{
	return length == 0;
};

UDM_DLL cint_string::operator bool() const
{
	return length != 0;
};
	
UDM_DLL cint_string::~cint_string()
{
	delete[] impl, impl = 0;
};

UDM_DLL int cint_string::comp(const char * compare_to) const
{
	return strcmp(compare_to, impl);
};

UDM_DLL char * cint_string::buffer() const
{
	return impl;
};

UDM_DLL bool cint_string::CopyFrom(const char * frm)
{
	//reset overflow flag
	overflow = false;

	if (impl && frm)
	{
		strncpy(impl, frm, length + 1);
		if (*(impl + length) != '\0')
		{
			overflow = true;
			*(impl + length) = '\0';
		}
	}
	return overflow;
};


UDM_DLL char cint_string::operator[]( int index) const
{
	if ( 0== impl)
		return 0;
	int len= strlen( impl);
	if (index >= len)
		return 0;
	return *(impl+ index);
}

UDM_DLL int cint_string::len() const
{
	if ( 0== impl)
		return 0;
	return strlen( impl);
}
