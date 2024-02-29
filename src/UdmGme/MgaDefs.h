
#ifndef MOBIES_MGA_DEFS_H
#define MOBIES_MGA_DEFS_H

#define SmartBSTR _bstr_t


//MGA macros
//GME 003 macros
#define MGACOLL_ITERATE(iftype, collifptr) \
{ \
	ASSERT( collifptr != NULL ); \
	long iter_count = 0; \
	COMTHROW( collifptr->get_Count(&iter_count) ); \
	ASSERT( iter_count >= 0 ); \
	iftype##Ptr *arrptr, *arrend, *array = new iftype##Ptr[iter_count]; \
	if(iter_count > 0) \
		COMTHROW( collifptr->GetAll(iter_count, &(*array)) ); \
	arrend = array+iter_count; \
	for(arrptr = array; arrptr != arrend; arrptr++)

#define MGACOLL_ITER (*arrptr)

#define MGACOLL_AT_END (arrptr == arrend)

#define MGACOLL_ITERATE_END \
	delete[] array; \
}




//so-called error handling 

void comthrow(HRESULT res, const char *e, int l);
#define COMTHROW(expr) comthrow(expr, #expr, __LINE__)

class gme_exc : public udm_exception 
{ 
	char excbuf[20];
public:
	gme_exc(long l) : udm_exception(std::string("GME error: ")+ _ltoa(l, excbuf, 16)) { ; }
};


static void comthrow(HRESULT res, const char *e, int l) 
{
	if (FAILED(res))
		UdmGme::com_exception(res, NULL);
}

//GmeObject class helper macros
#define objself (folderself? (IMgaObject *) folderself : (IMgaObject *)self)
#define testself (self ? self : (throw udm_exception("Invalid method on folders"), NULL))
#define NAMEGET(x) (((string)((x).name())).c_str())

static string getShortClassPath(const Uml::Class& c)
{
	if (!c)
		return "";
	std::string ret;
	Udm::Object o = c;
	while (o.GetParent())
	{
		const Uml::Class& type = o.type();
		if (type == Uml::Class::meta)
		{
			ret = static_cast<std::string>(Uml::Class::Cast(o).name()) + ret;
		} 
		else if (type == Uml::Namespace::meta)
		{
			ret = static_cast<std::string>(Uml::Namespace::Cast(o).name()) + std::string("::") + ret;
			return ret;
		} 
		else if (type == Uml::Diagram::meta)
		{
			ret = static_cast<std::string>(Uml::Diagram::Cast(o).name()) + std::string("::") + ret;
			return ret;
		} 
		o = o.GetParent();
	}
	// ASSERT(ret == c.getPath2("::", false));
    return ret;
}


#define PATHGET(x) (getShortClassPath(x).c_str())
#define foldiffold (folderself ? folderself : self)
#endif