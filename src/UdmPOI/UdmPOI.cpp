
/*
Copyright (c) Vanderbilt University, 2000-2001
ALL RIGHTS RESERVED
Vanderbilt University disclaims all warranties with regard to this
software, including all implied warranties of merchantability
and fitness.  In no event shall Vanderbilt University be liable for
any special, indirect or consequential damages or any damages
whatsoever resulting from loss of use, data or profits, whether
in an action of contract, negligence or other tortious action,
arising out of or in connection with the use or performance of
this software.

  FILE:		UdmInt.cpp
  SYNOPSIS:	Udm Interface to different interpretter engines
  CREATED:	08/10/02
  AUTHOR:	Endre Magyari(endre.magyari@vanderbilt.edu)

  CHANGELOG:
  05/12/05 - kalmar - clean up, get rid of cint
  01/28/05 - Himanshu - Adding a equals operator for UdmPseudoObject
  01/27/05 - Himanshu - Defining separate header files for CINT (UdmCint.h) and SWIG (UdmCintSwig.h)
				and adding functions for array attribute getters and setters
  01/11/05 - Himanshu - Added count() functions for functions returning a container of elements
  27/12/02	-	extended the UdmPseudo API with UdmPseudoDataNetwork class, and other initialization classes
  08/10/02	- Added Support for EiC C interpreter. See details 
				at EiC_Interpreter_iface_section



*/

#include <UdmBase.h>
#include <cint_string.h>
#include <UdmCintSwig.h>
#include <Uml.h>
#include <UmlExt.h>
#include <UdmDom.h>
#ifndef NO_ANTLR
#include <UdmOcl.h>
#endif
#include <cstring>

#if (_MSC_VER >= 1600) /* VS2010 */
#include "../Uml/Uml_xsd.h"
#endif

#define METADEPOSITORY_KEY(key) (string("__UdmPOI__") + string(key))

using namespace Udm;
using namespace Uml;

namespace Uml_xsd {
	const std::string& getString();
}

static string last_error;			//static string to store the last error
static bool was_error = false;		//static bool to indicate an error after the last

//structure to identify an object
struct _upi_o_id {
	unsigned long dnid;
	UdmPseudoObject::ob_id_type oid;
	_upi_o_id(const unsigned long d, const UdmPseudoObject::ob_id_type o)
	{
		dnid = d;
		oid = o;
	}
};

//get a UDM Object based on it's Data Network ID and ObjectID
Udm::DataNetwork * GetUdmDataNetwork(unsigned long dn_id)
{
	Udm::DataNetwork * dn = Udm::DataNetwork::GetDnById(dn_id);
	if (dn) return dn;
	throw udm_exception("DataNetwork not found!");
};
Udm::Object GetUdmObject(_upi_o_id id)
{

	Udm::DataNetwork * dn = GetUdmDataNetwork(id.dnid);
	//no check needed - if problems exception is thrown and caught by macro
	Udm::Object o =  dn->ObjectById(id.oid);
	if (o) return o;
	throw udm_exception("Object in DataNetwork not found!");
};


//set an error condition
//because of an exception thrown by UDM::Object API calls
void set_last_error(const char * what)
{
	was_error = true;
	last_error = what;
};


UdmPseudoObjectS::UdmPseudoObjectS(int length)
{
	if (length)
	{
		impl = new UdmPseudoObject[length];
	}
	else impl = NULL;

	this->length = length;
	
};

UdmPseudoObjectS& UdmPseudoObjectS::operator=(const UdmPseudoObjectS &frm)
{
	delete[] impl;
	length = frm.length;
	if (length) impl = new UdmPseudoObject[length];
	
	for (int i = 0; i < length; i++)
	{
		UdmPseudoObject * to = impl + i;
		*to = *(frm.impl + i);
	}
	return *this;
};

UdmPseudoObjectS::UdmPseudoObjectS(const UdmPseudoObjectS &frm)
{
	length = frm.length;
	if (length) impl = new UdmPseudoObject[length];
	
	for (int i = 0; i < length; i++)
	{
		UdmPseudoObject * to = impl + i;
		*to = *(frm.impl + i);
	}
};

UdmPseudoObject& UdmPseudoObjectS::operator[](const int index) const
{
	int index_ok = index;
	((UdmPseudoObjectS*)this)->overflow = false;		//hack because of const-ness
	if (index_ok > length - 1) 
	{
		index_ok = length - 1;
		((UdmPseudoObjectS*)this)->overflow = true;		//hack because of const-ness
	}

	return *(impl + index_ok);
};
	
void UdmPseudoObjectS::SetAt(const int index, const UdmPseudoObject& item)
{
	int index_ok = index;
	overflow = false;
	if (index_ok > length - 1)
	{
		index_ok = length - 1;
		overflow = true;
	}
	*(impl + index_ok) = item;

};

int UdmPseudoObjectS::GetLength() const
{
	return length;
};


UdmPseudoObjectS::~UdmPseudoObjectS()
{
	if (impl) delete[] impl;
	impl = NULL;
};

bool UdmPseudoObjectS::operator !() const { return length == 0; }
UdmPseudoObjectS::operator bool()  const { return  length != 0; }


AssociationInfo::AssociationInfo(const char *src, const char *dst, const char *assoc)
{
	this->SrcRolename = src;
	this->DstRolename = dst;
	this->assoc_class = assoc;
};

AssociationInfo::AssociationInfo(cint_string src, cint_string dst, cint_string assoc)
{
	SrcRolename = src;
	DstRolename = dst;
	assoc_class = assoc;
};

CompositionInfo::CompositionInfo(const char *pr, const char *cr)
{
	parentRole = pr;
	childRole = cr;
};

CompositionInfo::CompositionInfo(cint_string pr, cint_string cr)
{
	parentRole = pr;
	childRole = cr;
};



/*
class UdmPseudoObject
{
	unsigned long ob_id;
	unsigned long dn_id;
}*/	

#define CINT_TRY(x, y) try {Udm::Object upi_o = GetUdmObject(_upi_o_id(x, y))
#define CINT_TRY2(x1, y1, x2, y2) try {Udm::Object upi_o1 = GetUdmObject(_upi_o_id(x1, y1)); Udm::Object upi_o2 = GetUdmObject(_upi_o_id(x2, y2));
#define CINT_CATCH } catch (const udm_exception& e) {set_last_error(e.what()); return false;} catch(...) {set_last_error("Unexpected exception caught."); return false;}
#define CINT_DN_TRY(x) try {Udm::DataNetwork * dn = GetUdmDataNetwork(x)
#define CINT_DN_CATCH } catch (const udm_exception& e) {set_last_error(e.what()); return false;} catch(...) {set_last_error("Unexpected exception caught."); return false;}


UdmPseudoObject::UdmPseudoObject()
{
	this->dn_id = 0;
	this->ob_id = 0;
};


UdmPseudoObject::UdmPseudoObject(unsigned long dn_id, ob_id_type ob_id)
{
	this->dn_id = dn_id;
	this->ob_id = ob_id;
/*
	try
	{
		Udm::Object upi_o = GetUdmObject(_upi_o_id(dn_id, ob_id));
		upi_o.__impl()->clone();
	}
	catch (udm_exception &e)
	{
		set_last_error(e.what());
	};
	*/
};

UdmPseudoObject& UdmPseudoObject::operator=(const UdmPseudoObject& frm)
{
	this->dn_id = frm.dn_id;
	this->ob_id = frm.ob_id;
/*
	try
	{
		Udm::Object upi_o = GetUdmObject(_upi_o_id(dn_id, ob_id));
		upi_o.__impl()->clone();
	}
	catch (udm_exception &e)
	{
		set_last_error(e.what());
	};

*/
	return *this;
};

UdmPseudoObject::ob_id_type UdmPseudoObject::_ob_id() const
{
	return ob_id;
};
unsigned long UdmPseudoObject::_dn_id() const
{
	return dn_id;
};


// Himanshu: Adding a equals operator for UdmPseudoObject
 bool UdmPseudoObject::operator ==(const UdmPseudoObject& to)
{
	CINT_TRY(dn_id, ob_id);
	CINT_TRY(to.dn_id, to.ob_id);
	return ((this->ob_id == to.ob_id) && (this->dn_id == to.dn_id));
	CINT_CATCH;
	CINT_CATCH;
}

//error handling
bool UdmPseudoObject::GetLastError(cint_string& buffer)
{
	if (was_error)
	{
		//buffer.CopyFrom(last_error.c_str());
		buffer = last_error.c_str();
		was_error = false;
		return true;
	} else return false;
};

//type
bool UdmPseudoObject::type(cint_string &value) const
{
	CINT_TRY(dn_id, ob_id);

	string str = upi_o.type().name();
	value = str.c_str();	//this will reallocate !!
	return true;

	CINT_CATCH;
};

bool UdmPseudoObject::stereotype(cint_string &value) const
{
	CINT_TRY(dn_id, ob_id);

	string str = upi_o.type().stereotype();
	value = str.c_str();	//this will reallocate !!
	
	return true;

	CINT_CATCH;
};

bool UdmPseudoObject::uniqueId(UdmLong &value) const
{
	CINT_TRY(dn_id, ob_id);
	value.longVal = upi_o.uniqueId();
	return true;
	CINT_CATCH;
}

//IMPL API
bool UdmPseudoObject::getParent(const char * prole, UdmPseudoObject &value) const
{
	CINT_TRY(dn_id, ob_id);
	//obtain parentrole from prole
	::Uml::CompositionParentRole cpr;
	if (prole)
	{
		set< ::Uml::CompositionParentRole> proles = AncestorCompositionPeerParentRoles(upi_o.type());
		set< ::Uml::CompositionParentRole>::iterator proles_i = proles.begin();
		while (proles_i != proles.end())
		{
			if (!strcmp(prole, ((string)proles_i->name()).c_str()))
					break;
			proles_i++;
		} 

		if (proles_i == proles.end()) throw udm_exception(string("No such parentrole: ") + prole);
		cpr = *proles_i;
	}
	//getparent does the cloning
	Udm::Object par = upi_o.__impl()->getParent(cpr);
	if (par)
	{
		UdmPseudoObject upo(par.__impl()->__getdn()->uniqueId(), par.__impl()->uniqueId());
		value = upo;
	}else value = UdmPseudoObject();

	return true;

	CINT_CATCH;
};

bool UdmPseudoObject::setParent(UdmPseudoObject &parent, const char * prole)
{
	CINT_TRY(dn_id, ob_id);
	
	//the new parent
	Udm::Object new_parent = GetUdmObject(_upi_o_id(parent.dn_id, parent.ob_id));

	::Uml::CompositionParentRole cpr;
	//obtain parentrole from prole
	if(prole)
	{
		set< ::Uml::CompositionParentRole> proles = AncestorCompositionPeerParentRoles(upi_o.type());
		set< ::Uml::CompositionParentRole>::iterator proles_i = proles.begin();
		while (proles_i != proles.end())
		{
			if (!strcmp(prole, ((string)proles_i->name()).c_str()))
				break;
			proles_i++;
		}

		if (proles_i == proles.end()) throw udm_exception(string("No such parentrole: ") + prole);
		cpr = *proles_i;
	}


	upi_o.__impl()->setParent(new_parent.__impl(), cpr);
	return true;
	
	CINT_CATCH;
};

bool UdmPseudoObject::detach()
{
	CINT_TRY(dn_id, ob_id);
	upi_o.__impl()->detach();
	return true;
	CINT_CATCH;
};
	
bool UdmPseudoObject::getChildrenCount(const char * crole, const char *k, const char * kind_ns_path,  UdmLong &value) const // HN1
{
	CINT_TRY(dn_id, ob_id);
	::Uml::CompositionChildRole ccr;//it will be NullObject by default
	::Uml::Class kind;

	if (crole)
	{
		set< ::Uml::CompositionChildRole> croles = ::Uml::AncestorCompositionPeerChildRoles(upi_o.type());
		set< ::Uml::CompositionChildRole>::iterator croles_i = croles.begin();
		while (croles_i != croles.end())
		{
			if (!strcmp(crole, ((string)croles_i->name()).c_str()))
				break;
			croles_i++;
		}

		if (croles_i == croles.end()) throw udm_exception(string("No such childrole: ") + crole);;
		ccr = *croles_i;
	}

	if (k)
	{
		kind =  classByPath(upi_o.__impl()->__getdn()->GetRootMeta(), string(kind_ns_path) + "::" + k) ;
		if (!kind) throw udm_exception(string("No such class: ") + kind_ns_path + "::" + k);;
	}

	vector<ObjectImpl*> children = upi_o.__impl()->getChildren(ccr, kind);

	//impls needs to be freed!
	vector<ObjectImpl*>::iterator children_i = children.begin();
	while (children_i != children.end())
		Udm::Object child(*children_i++);	//so, it will be freed
	
	
	value.longVal = children.size();

	return true;
	CINT_CATCH;
};

bool UdmPseudoObject::getChildren(const char * crole, const char *k, const char * kind_ns_path, UdmPseudoObjectS &value) const
{
	CINT_TRY(dn_id, ob_id);
	::Uml::CompositionChildRole ccr;//it will be NullObject by default
	::Uml::Class kind;

	if (crole)
	{
		set< ::Uml::CompositionChildRole> croles = AncestorCompositionPeerChildRoles(upi_o.type());
		set< ::Uml::CompositionChildRole>::iterator croles_i = croles.begin();
		while (croles_i != croles.end())
		{
			if (!strcmp(crole, ((string)croles_i->name()).c_str()))
				break;
			croles_i++;
		}

		if (croles_i == croles.end()) throw udm_exception(string("No such childrole: ") + crole);;
		ccr = *croles_i;
	}

	if (k)
	{
		kind =  classByPath(upi_o.__impl()->__getdn()->GetRootMeta(), string(kind_ns_path) + "::" + k) ;
		if (!kind) throw udm_exception(string("No such class: ") + kind_ns_path + "::" + k);;
	}

	vector<ObjectImpl*> children = upi_o.__impl()->getChildren(ccr, kind);
	vector<ObjectImpl*>::iterator children_i = children.begin();
	int i = 0;
	while (children_i != children.end())
	{
		Udm::Object child(*children_i++);	//so, it will be freed
		UdmPseudoObject upo(child.__impl()->__getdn()->uniqueId(), child.uniqueId());
		value.SetAt(i++ ,upo);
	}
	return true;
	CINT_CATCH;
};

bool UdmPseudoObject::setChildren(const char * crole, const UdmPseudoObjectS &children)
{
	CINT_TRY(dn_id, ob_id);

	vector<ObjectImpl*> children_vec;

	for (int i = 0; i < children.GetLength(); i++)
	{
		UdmPseudoObject child = children[i];
		Udm::Object new_child = GetUdmObject(_upi_o_id(child.dn_id, child.ob_id));
		children_vec.push_back(new_child.__impl());
	};

	::Uml::CompositionChildRole ccr;

	if (crole)
	{
		set< ::Uml::CompositionChildRole> croles = AncestorCompositionPeerChildRoles(upi_o.type());
		set< ::Uml::CompositionChildRole>::iterator croles_i = croles.begin();
		while (croles_i != croles.end())
		{
			if (!strcmp(crole, ((string)croles_i->name()).c_str()))
				break;
			croles_i++;
		}

		if (croles_i == croles.end()) throw udm_exception(string("No such childrole: ") + crole);;
		ccr = *croles_i;
	}

	upi_o.__impl()->setChildren(ccr, children_vec);
	return true;
	CINT_CATCH;
};




//associations
bool UdmPseudoObject::getAssociationCount(const char* ass_role, UdmLong& value, int mode) const // HN2
{
	CINT_TRY(dn_id, ob_id);
	
	if (!ass_role) return false;
	set< ::Uml::AssociationRole> aroles = AncestorAssociationTargetRoles(upi_o.type());
	
	::Uml::Association assoc = ::Uml::GetAncestorAssociation(upi_o.type());
	if (assoc)
	{
		set< ::Uml::AssociationRole> legs = assoc.roles();	
		aroles.insert(legs.begin(), legs.end());
	}


	set< ::Uml::AssociationRole>::iterator aroles_i = aroles.begin();
	while (aroles_i != aroles.end())
	{
		if (!strcmp(ass_role, ((string)aroles_i->name()).c_str()))
			break;
		aroles_i++;
	}
	if (aroles_i == aroles.end()) throw udm_exception(string("No such associationrole: ") + ass_role);;

	vector<ObjectImpl*> ass_impls = upi_o.__impl()->getAssociation(*aroles_i, mode);
	value.longVal = ass_impls.size();

	vector<ObjectImpl*>::iterator ass_impls_i = ass_impls.begin();
	while (ass_impls_i != ass_impls.end())
		Udm::Object ass_object(*ass_impls_i++);	//so, it will be freed


	return true;
	CINT_CATCH;
};

bool UdmPseudoObject::getAssociation(const char* ass_role, UdmPseudoObjectS &value, int mode) const
{
	CINT_TRY(dn_id, ob_id);
	
	if (!ass_role) return false;
	set< ::Uml::AssociationRole> aroles = AncestorAssociationTargetRoles(upi_o.type());

	::Uml::Association assoc = ::Uml::GetAncestorAssociation(upi_o.type());
	if (assoc)
	{
		set< ::Uml::AssociationRole> legs = assoc.roles();	
		aroles.insert(legs.begin(), legs.end());
	}

	set< ::Uml::AssociationRole>::iterator aroles_i = aroles.begin();
	while (aroles_i != aroles.end())
	{
		if (!strcmp(ass_role, ((string)aroles_i->name()).c_str()))
			break;
		aroles_i++;
	}
	if (aroles_i == aroles.end()) throw udm_exception(string("No such associationrole: ") + ass_role);;

	vector<ObjectImpl*> ass_impls = upi_o.__impl()->getAssociation(*aroles_i, mode);
	vector<ObjectImpl*>::iterator ass_impls_i = ass_impls.begin();
	int i = 0;
	while (ass_impls_i != ass_impls.end())
	{
		Udm::Object ass_object(*ass_impls_i++);	//so, it will be freed
		UdmPseudoObject upo(ass_object.__impl()->__getdn()->uniqueId(), ass_object.uniqueId());
		value.SetAt(i++, upo);
	}
	return true;

	CINT_CATCH;
};
bool UdmPseudoObject::setAssociation(const char * ass_role, const UdmPseudoObjectS &value, int mode)
{
	CINT_TRY(dn_id, ob_id);
	if (!ass_role) return false;
	::Uml::Class cl = upi_o.type();
	::Uml::Association cl_assoc = ::Uml::GetAncestorAssociation(cl);
	set< ::Uml::AssociationRole> aroles;

	if (cl_assoc)
		aroles = cl_assoc.roles();
	else
		aroles = AncestorAssociationTargetRoles(upi_o.type());

	set< ::Uml::AssociationRole>::iterator aroles_i = aroles.begin();
	while (aroles_i != aroles.end())
	{
		if (!strcmp(ass_role, ((string)aroles_i->name()).c_str()))
			break;
		aroles_i++;
	}
	if (aroles_i == aroles.end()) throw udm_exception(string("No such associationrole: ") + ass_role);

	vector<ObjectImpl*> new_ass_impls;
	for (int i = 0; i < value.GetLength(); i++)
	{
		UdmPseudoObject ass_obj = value[i];
		Udm::Object new_ass_obj = GetUdmObject(_upi_o_id(ass_obj.dn_id, ass_obj.ob_id));
		//TBD: this need to be released() somewhere ...
		new_ass_impls.push_back(new_ass_obj.__impl()->clone());
	};

	upi_o.__impl()->setAssociation(*aroles_i,new_ass_impls, mode);

	

	return true;


	CINT_CATCH;
};



//attribute setters
bool UdmPseudoObject::SetIntVal(const char * name, __int64 value)
{
	CINT_TRY(dn_id, ob_id);
	bool ret = upi_o.SetIntValue(name, value);
	if (!ret) throw udm_exception(string("No such attribute: ") + name);
	return true;

	CINT_CATCH;
};
bool UdmPseudoObject::SetRealVal(const char * name, double value)
{
	CINT_TRY(dn_id, ob_id);
	bool ret = upi_o.SetRealValue(name, value);
	if (!ret) throw udm_exception(string("No such attribute: ") + name);
	return true;
	CINT_CATCH;
};
bool UdmPseudoObject::SetStrVal(const char * name, const char *  value)
{
	CINT_TRY(dn_id, ob_id);
	bool ret = upi_o.SetStrValue(name, value);
	if (!ret) throw udm_exception(string("No such attribute: ") + name);
	return true;
	
	CINT_CATCH;
};
bool UdmPseudoObject::SetBoolVal(const char * name, bool value)
{
	CINT_TRY(dn_id, ob_id);
	bool ret = upi_o.SetBoolValue(name, value);
	if (!ret) throw udm_exception(string("No such attribute: ") + name);
	return true;
	
	CINT_CATCH;
};

//attribute getters
bool UdmPseudoObject::GetIntVal(const char * name, UdmLong &value)
{
	// Himanshu: Commenting out as now UdmLongs hold __int64 integers
	/*CINT_TRY(dn_id, ob_id);
	__int64 val64;
	bool res = upi_o.GetIntValue(name, val64);
	value.longVal = (long) val64;
	if (!res) throw udm_exception(string("No such attribute: ") + name);
	return true;
	CINT_CATCH;*/

	CINT_TRY(dn_id, ob_id);
	bool res = upi_o.GetIntValue(name, value.longVal);
	if (!res) throw udm_exception(string("No such attribute: ") + name);
	return true;
	CINT_CATCH;
};
bool UdmPseudoObject::GetRealVal(const char * name, UdmDouble &value)
{
	CINT_TRY(dn_id, ob_id);
	bool res =  upi_o.GetRealValue(name, value.doubleVal);
	
	if (!res) throw udm_exception(string("No such attribute: ") + name);
	return true;
	
	CINT_CATCH;
};
bool UdmPseudoObject::GetStrVal(const char * name,  cint_string & value)
{
	CINT_TRY(dn_id, ob_id);
	string stl_value;
	if (upi_o.GetStrValue(name, stl_value))
	{
		//value.CopyFrom(stl_value.c_str());
		value = stl_value.c_str();
		return true;
	}
	throw udm_exception(string("No such attribute: ") + name);
	return false;
	CINT_CATCH;
};
bool UdmPseudoObject::GetBoolVal(const char * name, UdmBool &value)
{
	CINT_TRY(dn_id, ob_id);
	bool val;
	bool retval = upi_o.GetBoolValue(name, val);

	value.boolVal = val;
	if (!retval) throw udm_exception(string("No such attribute: ") + name);
	return true;
	CINT_CATCH;
};

// Himanshu: integer array attribute getter
bool UdmPseudoObject::GetIntValues(const char * name, vector<UdmLong>& values)
{
	// Himanshu: Commenting out as now UdmLongs hold __int64 integers
	/*CINT_TRY(dn_id, ob_id);
	std::vector<__int64> val64;
	bool res = upi_o.GetIntValues(name, val64);
	std::vector<UdmLong> val;
	std::vector<__int64>::const_iterator it = val64.begin();
	while(it != val64.end()) {
		UdmLong newVal((long) *it);
		val.push_back(newVal);
		it++;
	}
	values = val;
	if (!res) throw udm_exception(string("No such attribute: ") + name);
	return true;
	CINT_CATCH;*/

	CINT_TRY(dn_id, ob_id);
	std::vector<__int64> val64;
	bool res = upi_o.GetIntValues(name, val64);
	std::vector<UdmLong> val;
	std::vector<__int64>::const_iterator it = val64.begin();
	while(it != val64.end()) {
		UdmLong newVal(*it);
		val.push_back(newVal);
		it++;
	}
	values = val;
	if (!res) throw udm_exception(string("No such attribute: ") + name);
	return true;
	CINT_CATCH;
};

// Himanshu: double array attribute getter
bool UdmPseudoObject::GetRealValues(const char * name, vector<double>& values)
{
	CINT_TRY(dn_id, ob_id);
	bool res =  upi_o.GetRealValues(name, values);
	if (!res) throw udm_exception(string("No such attribute: ") + name);
	return true;
	CINT_CATCH;
};

// Himanshu: string array attribute getter
bool UdmPseudoObject::GetStrValues(const char * name, vector<cint_string>& values)
{
	CINT_TRY(dn_id, ob_id);
	vector<string> stl_values;
	if (upi_o.GetStrValues(name, stl_values))
	{
		vector<string>::const_iterator it = stl_values.begin();
		while(it != stl_values.end()) {
			values.push_back((*it).c_str());
			it++;
		}
		return true;
	}
	throw udm_exception(string("No such attribute: ") + name);
	return false;
	CINT_CATCH;
};

// Himanshu: bool array attribute getter
bool UdmPseudoObject::GetBoolValues(const char * name, vector<bool>& values)
{
	CINT_TRY(dn_id, ob_id);
	bool retval = upi_o.GetBoolValues(name, values);
	if (!retval) throw udm_exception(string("No such attribute: ") + name);
	return true;
	CINT_CATCH;
};

// Himanshu: integer array attribute setter
bool UdmPseudoObject::SetIntValues(const char * name, vector<UdmLong> values)
{
	// Himanshu: Commenting out as now UdmLongs hold __int64 integers
	/*CINT_TRY(dn_id, ob_id);
	std::vector<__int64> vals64;
	std::vector<UdmLong>::const_iterator it = values.begin();
	while(it != values.end()) {
		vals64.push_back((__int64) ((*it).longVal));
		it++;
	}
	bool ret = upi_o.SetIntValues(name, vals64);
	if (!ret) throw udm_exception(string("No such attribute: ") + name);
	return true;
	CINT_CATCH;*/


	CINT_TRY(dn_id, ob_id);
	std::vector<__int64> vals64;
	std::vector<UdmLong>::const_iterator it = values.begin();
	while(it != values.end()) {
		vals64.push_back((*it).longVal);
		it++;
	}
	bool ret = upi_o.SetIntValues(name, vals64);
	if (!ret) throw udm_exception(string("No such attribute: ") + name);
	return true;
	CINT_CATCH;
};

// Himanshu: double array attribute setter
bool UdmPseudoObject::SetRealValues(const char * name, vector<double> values)
{
	CINT_TRY(dn_id, ob_id);
	bool ret = upi_o.SetRealValues(name, values);
	if (!ret) throw udm_exception(string("No such attribute: ") + name);
	return true;
	CINT_CATCH;
};

// Himanshu: string array attribute setter
bool UdmPseudoObject::SetStrValues(const char * name, vector<cint_string> values)
{
	CINT_TRY(dn_id, ob_id);
	vector<string> stl_values;
	vector<cint_string>::const_iterator it = values.begin();
	while(it != values.end()) {
		stl_values.push_back((*it).buffer());
		it++;
	}
	bool ret = upi_o.SetStrValues(name, stl_values);
	if (!ret) throw udm_exception(string("No such attribute: ") + name);
	return true;
	CINT_CATCH;
};

// Himanshu: bool array attribute setter
bool UdmPseudoObject::SetBoolValues(const char * name, vector<bool> values)
{
	CINT_TRY(dn_id, ob_id);
	bool ret = upi_o.SetBoolValues(name, values);
	if (!ret) throw udm_exception(string("No such attribute: ") + name);
	return true;
	CINT_CATCH;
};

//TOMI -- to be done later on
bool UdmPseudoObject::GetAdjacentObjectsCount(UdmLong &value) // HN3
{
	CINT_TRY(dn_id, ob_id);
	multiset<Udm::Object> ret = upi_o.GetAdjacentObjects();
	value.longVal = ret.size();
	
	return true;
	CINT_CATCH;
};

bool UdmPseudoObject::GetAdjacentObjects(UdmPseudoObjectS &value)
{
	CINT_TRY(dn_id, ob_id);
	multiset<Udm::Object> ret = upi_o.GetAdjacentObjects();
	multiset<Udm::Object>::iterator ret_i = ret.begin();
	int i = 0;
	while (ret_i != ret.end())
		value.SetAt(i++, UdmPseudoObject(upi_o.__impl()->__getdn()->uniqueId(), upi_o.__impl()->uniqueId()));
	
	return true;
	CINT_CATCH;
};

bool UdmPseudoObject::GetAdjacentObjectsCount(const char * dst_type, const char * dst_type_ns_path,  UdmLong &value) // HN4
{
	CINT_TRY(dn_id, ob_id);
	
	::Uml::Class kind = ::Uml::classByPath(upi_o.__impl()->__getdn()->GetRootMeta(), string(dst_type_ns_path) + "::" + dst_type);
	if (!kind) throw udm_exception(string("No such kind: ") + dst_type_ns_path + "::" + dst_type);

	multiset<Udm::Object> ret = upi_o.GetAdjacentObjects(kind);
	value.longVal = ret.size();
	
	return true;
	CINT_CATCH;
};

bool UdmPseudoObject::GetAdjacentObjects(const char * dst_type, const char * dst_type_ns_path, UdmPseudoObjectS &value)
{
	CINT_TRY(dn_id, ob_id);
	
	::Uml::Class kind = ::Uml::classByPath(upi_o.__impl()->__getdn()->GetRootMeta(), string(dst_type_ns_path) + "::" + dst_type);
	if (!kind) throw udm_exception(string("No such kind: ") + dst_type_ns_path + "::" + dst_type);

	multiset<Udm::Object> ret = upi_o.GetAdjacentObjects(kind);
	multiset<Udm::Object>::iterator ret_i = ret.begin();
	int i = 0;
	while (ret_i != ret.end())
	{
		Udm::Object adj_ob = *ret_i;
		value.SetAt(i++, UdmPseudoObject(adj_ob.__impl()->__getdn()->uniqueId(), adj_ob.__impl()->uniqueId()));
	}
	
	return true;
	CINT_CATCH;
};

bool UdmPseudoObject::GetAdjacentObjectsCount(const char * dst_type, const char * dst_type_ns_path,  const AssociationInfo & ass, UdmLong &value) // HN5
{
	CINT_TRY(dn_id, ob_id);

	::Uml::Class kind = ::Uml::classByPath(upi_o.__impl()->__getdn()->GetRootMeta(), string(dst_type_ns_path) + "::" + dst_type);
	//kind can be null!

	::Uml::Class o_ass_info_ass_class;

	if (ass.assoc_class)
	{
		o_ass_info_ass_class = classByPath(upi_o.__impl()->__getdn()->GetRootMeta(), string(ass.assoc_class_ns.buffer()) + "::" + ass.assoc_class.buffer());
		if (!o_ass_info_ass_class) throw udm_exception(string("No such association class : ") + ass.assoc_class_ns.buffer() + "::" + ass.assoc_class.buffer());
	}
	Udm::Object::AssociationInfo o_ass_info(o_ass_info_ass_class);
	o_ass_info.strDstRoleName = ass.DstRolename.buffer();
	o_ass_info.strSrcRoleName = ass.SrcRolename.buffer();

	multiset<Udm::Object> ret = upi_o.GetAdjacentObjects(kind, o_ass_info);
	value.longVal = ret.size();
	
	return true;
	CINT_CATCH;
};

bool UdmPseudoObject::GetAdjacentObjects(const char * dst_type, const char * dst_type_ns_path, const AssociationInfo & ass, UdmPseudoObjectS & value)
{
	CINT_TRY(dn_id, ob_id);

	::Uml::Class kind = ::Uml::classByPath(upi_o.__impl()->__getdn()->GetRootMeta(), string(dst_type_ns_path) + "::" + dst_type);
	//kind can be null!

	::Uml::Class o_ass_info_ass_class;

	if (ass.assoc_class)
	{
		o_ass_info_ass_class = classByPath(upi_o.__impl()->__getdn()->GetRootMeta(), string(ass.assoc_class_ns.buffer()) + "::" + ass.assoc_class.buffer());
		if (!o_ass_info_ass_class) throw udm_exception(string("No such association class : ") + ass.assoc_class_ns.buffer() + "::" + ass.assoc_class.buffer());
	}
	Udm::Object::AssociationInfo o_ass_info(o_ass_info_ass_class);
	o_ass_info.strDstRoleName = ass.DstRolename.buffer();
	o_ass_info.strSrcRoleName = ass.SrcRolename.buffer();

	multiset<Udm::Object> ret = upi_o.GetAdjacentObjects(kind, o_ass_info);
	multiset<Udm::Object>::iterator ret_i = ret.begin();
	int i = 0;
	while (ret_i != ret.end())
	{
		Udm::Object adj_ob = *ret_i;
		value.SetAt(i++, UdmPseudoObject(adj_ob.__impl()->__getdn()->uniqueId(), adj_ob.__impl()->uniqueId()));
	}
	
	
	return true;
	CINT_CATCH;
};

bool UdmPseudoObject::GetParent(UdmPseudoObject &value)
{
	return getParent(NULL, value);
};

bool UdmPseudoObject::GetChildObjectsCount(const char * type, const char * type_ns_path, UdmLong &value) // HN6
{
	return getChildrenCount(NULL, type, type_ns_path, value);
};

bool UdmPseudoObject::GetChildObjects(const char * type, const char * type_ns_path, UdmPseudoObjectS &value)
{
	return getChildren(NULL, type, type_ns_path, value);
};

bool UdmPseudoObject::GetChildObjectsCount(const CompositionInfo &ci, const cint_string & type, const cint_string & type_ns_path, UdmLong &value) // HN7
{
	CINT_TRY(dn_id, ob_id);

	Udm::Object::CompositionInfo o_ci;
	o_ci.strChildRoleName = ci.childRole;
	o_ci.strParentRoleName = ci.parentRole;

	::Uml::Class kind = classByPath(upi_o.__impl()->__getdn()->GetRootMeta(), string(type_ns_path.buffer()) + "::" + type.buffer());

	set<Udm::Object> ret = upi_o.GetChildObjects(o_ci, kind);
	value.longVal = ret.size();
	
	return true;
	CINT_CATCH;
};

bool UdmPseudoObject::GetChildObjects(const CompositionInfo &ci, const cint_string & type, const cint_string & type_ns_path, UdmPseudoObjectS &value)
{
	CINT_TRY(dn_id, ob_id);

	Udm::Object::CompositionInfo o_ci;
	o_ci.strChildRoleName = ci.childRole;
	o_ci.strParentRoleName = ci.parentRole;

	::Uml::Class kind = classByPath(upi_o.__impl()->__getdn()->GetRootMeta(), string(type_ns_path.buffer()) + "::" + type.buffer());

	set<Udm::Object> ret = upi_o.GetChildObjects(o_ci, kind);

	set<Udm::Object>::iterator ret_i = ret.begin();
	int i = 0;
	while (ret_i != ret.end())
	{
		Udm::Object child_ob = *ret_i;
		value.SetAt(i++, UdmPseudoObject(child_ob.__impl()->__getdn()->uniqueId(), child_ob.__impl()->uniqueId()));
	}
	
	return true;
	
	CINT_CATCH;
};

bool UdmPseudoObject::GetAssociationClassObjectsCount(const UdmPseudoObject &dst, const AssociationInfo & ass, UdmLong &value) // HN8
{
	CINT_TRY(dn_id, ob_id);

	Udm::Object dstObject = GetUdmObject(_upi_o_id(dst.dn_id, dst.ob_id));
	
	::Uml::Class o_ass_info_ass_class;

	if (ass.assoc_class)
	{
		o_ass_info_ass_class = classByPath(upi_o.__impl()->__getdn()->GetRootMeta(), string(ass.assoc_class_ns.buffer()) + "::" + ass.assoc_class.buffer());
		if (!o_ass_info_ass_class) throw udm_exception(string("No such association class : ") + ass.assoc_class_ns.buffer() + "::" + ass.assoc_class.buffer());
	}
	
	Udm::Object::AssociationInfo o_ass_info(o_ass_info_ass_class);
	o_ass_info.strDstRoleName = ass.DstRolename.buffer();
	o_ass_info.strSrcRoleName = ass.SrcRolename.buffer();

	set<Udm::Object> ret = upi_o.GetAssociationClassObjects(dstObject, o_ass_info);
	value.longVal = ret.size();
	
	return true;
	CINT_CATCH;
};

bool UdmPseudoObject::GetAssociationClassObjects(const UdmPseudoObject &dst, const AssociationInfo & ass, UdmPseudoObjectS &value)
{
	CINT_TRY(dn_id, ob_id);

	Udm::Object dstObject = GetUdmObject(_upi_o_id(dst.dn_id, dst.ob_id));
	
	::Uml::Class o_ass_info_ass_class;

	if (ass.assoc_class)
	{
		o_ass_info_ass_class = classByPath(upi_o.__impl()->__getdn()->GetRootMeta(), string(ass.assoc_class_ns.buffer()) + "::" + ass.assoc_class.buffer());
		if (!o_ass_info_ass_class) throw udm_exception(string("No such association class : ") + ass.assoc_class_ns.buffer() + "::" + ass.assoc_class.buffer());
	}
	
	Udm::Object::AssociationInfo o_ass_info(o_ass_info_ass_class);
	o_ass_info.strDstRoleName = ass.DstRolename.buffer();
	o_ass_info.strSrcRoleName = ass.SrcRolename.buffer();

	set<Udm::Object> ret = upi_o.GetAssociationClassObjects(dstObject, o_ass_info);
	set<Udm::Object>::iterator ret_i = ret.begin();
	int i = 0;
	while (ret_i != ret.end())
	{
		Udm::Object ass_class_ob = *ret_i;
		value.SetAt(i++, UdmPseudoObject(ass_class_ob.__impl()->__getdn()->uniqueId(), ass_class_ob.__impl()->uniqueId()));
	}
	
	return true;
	CINT_CATCH;
};

bool UdmPseudoObject::GetPeersFromAssociationClassObjectCount(UdmLong &value) // HN9
{
	CINT_TRY(dn_id, ob_id);

	// FIXME: Himanshu: Line below may not be required (left it there just for validation)
	pair<Udm::Object, Udm::Object> ret = upi_o.GetPeersFromAssociationClassObject();


	value.longVal = 2; // It is always a pair; above code is for call validation
	return true;
	CINT_CATCH;
};

bool UdmPseudoObject::GetPeersFromAssociationClassObject(UdmPseudoObjectS &value)
{
	CINT_TRY(dn_id, ob_id);
	
	pair<Udm::Object, Udm::Object> ret = upi_o.GetPeersFromAssociationClassObject();
	int i = 0;
	
	Udm::Object peer_ob;

	peer_ob = ret.first;
	value.SetAt(i++, UdmPseudoObject(peer_ob.__impl()->__getdn()->uniqueId(), peer_ob.__impl()->uniqueId()));
	
	peer_ob = ret.second;
	value.SetAt(i++, UdmPseudoObject(peer_ob.__impl()->__getdn()->uniqueId(), peer_ob.__impl()->uniqueId()));
	
	
	return true;
	CINT_CATCH;
};

// Helper function
::Uml::CompositionChildRole GetCompositionChildRoleByName(const ::Uml::Class& kind, const string strRoleName)
{
	set< ::Uml::CompositionChildRole> ccrs = CompositionPeerChildRoles(kind);
	set< ::Uml::CompositionChildRole>::iterator it = ccrs.begin();
	
	while(it != ccrs.end()) {
		::Uml::CompositionChildRole aCCR = *it;
		if(string(aCCR.name()) == strRoleName) {
			return aCCR;
		}
		it++;
	}
	throw udm_exception(string("No such composition role found : ") + strRoleName);
}

// Creates a new child for 'this' object
bool UdmPseudoObject::CreateObject(const char * childType, const char * childType_ns_path, UdmPseudoObject &newChild, const char* compositionChildRole, const bool subtype)
{
	CINT_TRY(dn_id, ob_id);

	::Uml::Class childKind = ::Uml::classByPath(upi_o.__impl()->__getdn()->GetRootMeta(), string(childType_ns_path) + "::" + childType);
	if (!childKind) throw udm_exception(string("No such kind : ") + childType_ns_path + "::" + childType);

	Udm::Object new_o;

	if(compositionChildRole != 0) {
		::Uml::CompositionChildRole ccr = GetCompositionChildRoleByName(upi_o.type(), compositionChildRole);
		if (upi_o.__impl()->__getdn()->IsTypeSafe())
			new_o = upi_o.Create(childKind, upi_o, ccr, 0, subtype);
		else
			new_o = upi_o.Create(Uml::SafeTypeContainer::GetSafeType(childKind), upi_o, ccr, 0, subtype);
	} else {
		if (upi_o.__impl()->__getdn()->IsTypeSafe())
			new_o = upi_o.CreateObject(childKind);
		else
			new_o = upi_o.CreateObject(Uml::SafeTypeContainer::GetSafeType(childKind));
	}

	newChild.dn_id = new_o.__impl()->__getdn()->uniqueId();
	newChild.ob_id = new_o.__impl()->uniqueId();

	return true;
	CINT_CATCH;
};

bool UdmPseudoObject::CreateLink(UdmPseudoObject& dst, const AssociationInfo& ass)
{
	CINT_TRY(dn_id, ob_id);

	Udm::Object dstObject = GetUdmObject(_upi_o_id(dst.dn_id, dst.ob_id));
	
	::Uml::Class o_ass_info_ass_class;

	if (ass.assoc_class)
	{
		o_ass_info_ass_class = classByPath(upi_o.__impl()->__getdn()->GetRootMeta(), string(ass.assoc_class_ns.buffer()) + "::" + ass.assoc_class.buffer());
		if (!o_ass_info_ass_class) throw udm_exception(string("No such association class : ") + ass.assoc_class_ns.buffer() + "::" + ass.assoc_class.buffer() );;
	}
	
	Udm::Object::AssociationInfo o_ass_info(o_ass_info_ass_class);
	o_ass_info.strDstRoleName = ass.DstRolename.buffer();
	o_ass_info.strSrcRoleName = ass.SrcRolename.buffer();

	return upi_o.CreateLink(dstObject, o_ass_info);
	
	CINT_CATCH;
};

bool UdmPseudoObject::DeleteObject()
{
	return detach();
};



UdmPseudoDataNetwork::UdmPseudoDataNetwork(const char * mn, UdmBool& meta_found)
{
	dn_id = -1;		//not opened yet
	try
	{
		//just to check if exists
		const Udm::UdmDiagram& dgr = Udm::MetaDepository::LocateDiagram(METADEPOSITORY_KEY(mn));
		dgr.init();
		meta_name = mn;
		meta_found.boolVal = true;

	}
	catch (udm_exception &e)
	{
		set_last_error(e.what());
		meta_found.boolVal = false;
	};
	//we have to make sure that diagram is a Uml::Diagram
};
bool UdmPseudoDataNetwork::CreateNew(const char * sys_name, const char * meta_locator, const UdmPseudoObject& rootclass, const int backendsemantics)
{
	try
	{

		//at this point we don't have a data network ID
		if (dn_id != -1) throw udm_exception("UdmPseudoDataNetwork already opened!");
		//make sure root class is a Uml::Class and it is valid

		const Udm::UdmDiagram &dgr = Udm::MetaDepository::LocateDiagram(METADEPOSITORY_KEY(meta_name.buffer()));

			
		::Uml::Class rc;
		CINT_TRY(rootclass._dn_id(), rootclass._ob_id());
		rc = ::Uml::Class::Cast(upi_o);
		CINT_CATCH;
		
		
		//init dgr!
		UdmDom::DomDataNetwork * ddn = new UdmDom::DomDataNetwork(dgr);
		Udm::BackendSemantics ubs;
		switch (backendsemantics)
		{
		case UPDN_CHANGES_LOST_DEFAULT:
			ubs = CHANGES_LOST_DEFAULT;
			break;
		case UPDN_CHANGES_PERSIST_ALWAYS:
			ubs = CHANGES_PERSIST_ALWAYS;
			break;
		case UPDN_CHANGES_PERSIST_DEFAULT:
			ubs = CHANGES_PERSIST_DEFAULT;
			break;
		}
		ddn->CreateNew(sys_name, meta_locator, rc, ubs);
		dn_id = ddn->uniqueId();
		
		Udm::Object root_o = ddn->GetRootObject();
		if (!root_o) throw udm_exception("DataNetwork Creation failed!");

		UdmPseudoObject r(dn_id, root_o.__impl()->uniqueId());
		root = r;
		/*root.ob_id = root_o.__impl()->uniqueId();
		root.dn_id = dn_id;*/
		return true;	
	}
	catch (udm_exception &e)
	{
		set_last_error(e.what());
		return false;
	};

}
//======================================================================
bool UdmPseudoDataNetwork::CreateNewToString(const char * meta_locator, const UdmPseudoObject& rootclass)
{
	try
	{

		//at this point we don't have a data network ID
		if (dn_id != -1) throw udm_exception("UdmPseudoDataNetwork already opened!");
		//make sure root class is a Uml::Class and it is valid

		const Udm::UdmDiagram &dgr = Udm::MetaDepository::LocateDiagram(METADEPOSITORY_KEY(meta_name.buffer()));

			
    ::Uml::Class rc;
		CINT_TRY(rootclass._dn_id(), rootclass._ob_id());
    rc = ::Uml::Class::Cast(upi_o);
		CINT_CATCH;
		
		
		//init dgr!
		UdmDom::DomDataNetwork * ddn = new UdmDom::DomDataNetwork(dgr);

		ddn->CreateNewToString(meta_locator, rc, Udm::CHANGES_PERSIST_ALWAYS);
		dn_id = ddn->uniqueId();
		
		Udm::Object root_o = ddn->GetRootObject();
		if (!root_o) throw udm_exception("DataNetwork Creation failed!");

		UdmPseudoObject r(dn_id, root_o.__impl()->uniqueId());
		root = r;
		/*root.ob_id = root_o.__impl()->uniqueId();
		root.dn_id = dn_id;*/
		return true;	
	}
	catch (udm_exception &e)
	{
		set_last_error(e.what());
		return false;
	};

}


bool UdmPseudoDataNetwork::OpenExisting(const char * sys_name, const char * meta_locator, const int backendsemantics)
{
	try
	{

		//at this point we don't have a data network ID
		if (dn_id != -1) throw udm_exception("UdmPseudoDataNetwork already opened!");
		//make sure root class is a Uml::Class and it is valid

		const Udm::UdmDiagram &dgr = Udm::MetaDepository::LocateDiagram(METADEPOSITORY_KEY(meta_name.buffer()));

		//init dgr!
		UdmDom::DomDataNetwork * ddn = new UdmDom::DomDataNetwork(dgr);
		Udm::BackendSemantics ubs;
		switch (backendsemantics)
		{
		case UPDN_CHANGES_LOST_DEFAULT:
			ubs = CHANGES_LOST_DEFAULT;
			break;
		case UPDN_CHANGES_PERSIST_ALWAYS:
			ubs = CHANGES_PERSIST_ALWAYS;
			break;
		case UPDN_CHANGES_PERSIST_DEFAULT:
			ubs = CHANGES_PERSIST_DEFAULT;
			break;
		}
		ddn->OpenExisting(sys_name, meta_locator, ubs);
		dn_id = ddn->uniqueId();
		
		Udm::Object root_o = ddn->GetRootObject();
		if (!root_o) throw udm_exception("DataNetwork Creation failed!");

		/*root.ob_id = root_o.__impl()->uniqueId();
		root.dn_id = dn_id;
		*/

		UdmPseudoObject r(dn_id, root_o.__impl()->uniqueId());
		root = r;

		return true;	
	}
	catch (udm_exception &e)
	{
		set_last_error(e.what());
		return false;
	};

}
//===============================================================
bool UdmPseudoDataNetwork::OpenExistingFromString(const char * sys_name, const char * meta_locator)
{
	try
	{

		//at this point we don't have a data network ID
		if (dn_id != -1) throw udm_exception("UdmPseudoDataNetwork already opened!");
		//make sure root class is a Uml::Class and it is valid

		const Udm::UdmDiagram &dgr = Udm::MetaDepository::LocateDiagram(METADEPOSITORY_KEY(meta_name.buffer()));

		//init dgr!
		UdmDom::DomDataNetwork * ddn = new UdmDom::DomDataNetwork(dgr);

    string  a(sys_name);
    string  b(meta_locator);
	ddn->OpenExistingFromString(a, b, Udm::CHANGES_PERSIST_ALWAYS);
		dn_id = ddn->uniqueId();
		
		Udm::Object root_o = ddn->GetRootObject();
		if (!root_o) throw udm_exception("DataNetwork Creation failed!");

		/*root.ob_id = root_o.__impl()->uniqueId();
		root.dn_id = dn_id;
		*/

		UdmPseudoObject r(dn_id, root_o.__impl()->uniqueId());
		root = r;

		return true;	
	}
	catch (udm_exception &e)
	{
		set_last_error(e.what());
		return false;
	};

}
//================================================================
bool UdmPseudoDataNetwork::GetRootObject(UdmPseudoObject & value)
{
	value = root;
	return true;	
}
bool UdmPseudoDataNetwork::GetRootMeta(UdmPseudoObject &value)			//a Uml::Diagram should be returned
{
	CINT_DN_TRY(dn_id);
	const ::Uml::Diagram& dgr = dn->GetRootMeta();
	
	/*value.dn_id = dgr.__impl()->__getdn()->uniqueId();
	value.ob_id = dgr.__impl()->uniqueId();
	*/

	UdmPseudoObject r(dgr.__impl()->__getdn()->uniqueId(), dgr.__impl()->uniqueId());
	value = r;

	return true;	
	CINT_DN_CATCH
};
bool UdmPseudoDataNetwork::CloseWithUpdate()
{
	CINT_DN_TRY(dn_id);
	dn->CloseWithUpdate();
	return true;	
	CINT_DN_CATCH
};
bool UdmPseudoDataNetwork::CloseNoUpdate()
{
	CINT_DN_TRY(dn_id);
	dn->CloseNoUpdate();
	return true;	
	CINT_DN_CATCH
}
bool UdmPseudoDataNetwork::SaveAsString(cint_string& buffer)
{
	CINT_DN_TRY(dn_id);

  cint_string tmp(dn->Str().c_str());
  buffer = tmp;
	return true;	
	CINT_DN_CATCH
}
bool UdmPseudoDataNetwork::SaveAs(const char *systemname)
{
	CINT_DN_TRY(dn_id);
	dn->SaveAs(systemname);
	return true;	
	CINT_DN_CATCH
};
bool UdmPseudoDataNetwork::CloseAs(const char * systemname)
{
	CINT_DN_TRY(dn_id);
	dn->CloseAs(systemname);
	return true;	
	CINT_DN_CATCH
};
bool UdmPseudoDataNetwork::isOpen(UdmBool &value)
{
	CINT_DN_TRY(dn_id);
	value.boolVal = dn->isOpen();
	return true;	
	CINT_DN_CATCH
};
bool UdmPseudoDataNetwork::uniqueId(UdmLong &value)
{
	CINT_DN_TRY(dn_id);
	value.longVal = dn->uniqueId();
	return true;	
	CINT_DN_CATCH
};
UdmPseudoDataNetwork::~UdmPseudoDataNetwork()
{
	DataNetwork* dn = Udm::DataNetwork::GetDnById(dn_id);
	if (dn) delete dn;
	
};


#ifndef NO_ANTLR
bool UdmPseudoDataNetwork::OCL_Evaluate(cint_string& res)
{
  CINT_DN_TRY(dn_id);
     
  const Udm::Object& root = dn->GetRootObject();
  const Udm::UdmDiagram& ud = Udm::MetaDepository::LocateDiagram(METADEPOSITORY_KEY(meta_name.buffer()));
  const ::Uml::Diagram& metaDiagram = *(ud.dgr);


  Ocl::SErrorNotification en(false, Ocl::ENT_ALL) ;
  Ocl::Initialize( metaDiagram, en );

  Ocl::Evaluator eval(root);
  Ocl::SEvaluationOptions options;
  options.sErrorNotification = en;
  string out;
  try
  {

    Ocl::EEvaluationResult eResult = eval.Check(options);
  }
  catch ( const udm_exception& ex )
  {
    out =string(ex.what());
  }

  res = cint_string(out.c_str()); 

  Ocl::UnInitialize( metaDiagram );

  return true;
  CINT_DN_CATCH
}
#else
bool UdmPseudoDataNetwork::OCL_Evaluate(cint_string& res)
{
  // TODO: print a warning that constraints are disabled
	return true;
}
#endif /* NO_ANTLR */
	

bool UPO_SetDiagram(UdmPseudoObject &diagram_what, const char* name)
{
	try
	{
		const Udm::UdmDiagram & dgr = Udm::MetaDepository::LocateDiagram(METADEPOSITORY_KEY(name));
		/*diagram_what.dn_id = dgr.dgr->__impl()->__getdn()->uniqueId();
		diagram_what.ob_id = dgr.dgr->uniqueId();*/

		UdmPseudoObject r(dgr.dgr->__impl()->__getdn()->uniqueId(), dgr.dgr->uniqueId());
		diagram_what = r;
		return true;
	}
	catch (udm_exception &e)
	{
		set_last_error(e.what());
		return false;
	};
};

bool  UPO_SetNamespace(UdmPseudoObject &ns_what, UdmPseudoObject &parent, const char * target_name)
{
	CINT_TRY(parent._dn_id(), parent._ob_id());
	cint_string parent_type;
	if (!parent.type(parent_type)) throw udm_exception("Couldn't determine the parent type!");

	::Uml::Namespace what;

	if (parent_type.comp("Namespace") == 0) {
		::Uml::Namespace ns = ::Uml::Namespace::Cast(upi_o);
		if (!ns) throw udm_exception("The provided namespace is NULL!");
		SetNamespace(what, ns, target_name);
	} else if (parent_type.comp("Diagram") == 0) {
		::Uml::Diagram dgr = ::Uml::Diagram::Cast(upi_o);
		if (!dgr) throw udm_exception("The provided diagram is NULL!");
		SetNamespace(what, dgr, target_name);
	} else {
		throw udm_exception("Unknown parent provided to UPO_SetClass!");
	}

	if (!what) throw udm_exception("SetNamespace() failed!");

	UdmPseudoObject r(what.__impl()->__getdn()->uniqueId(), what.__impl()->uniqueId());
	ns_what = r;
	return true;
	CINT_CATCH;
}

bool UPO_SetNamespaceByPath(UdmPseudoObject &ns_what, UdmPseudoObject &diagram, const char * path)
{
	CINT_TRY(diagram._dn_id(), diagram._ob_id());

	::Uml::Diagram dgr = ::Uml::Diagram::Cast(upi_o);
	if (!dgr) throw udm_exception("The provided diagram is NULL!");

	::Uml::Namespace what = namespaceByPath(dgr, path);	// delim = "::"
	if (!what) throw udm_exception("Namespace not found by path!");

	UdmPseudoObject r(what.__impl()->__getdn()->uniqueId(), what.__impl()->uniqueId());
	ns_what = r;
	return true;
	CINT_CATCH;
}

bool UPO_SetClass(UdmPseudoObject &class_what, UdmPseudoObject &parent, const char* target_name)
{
//	CINT_TRY(dn_id, ob_id);

	CINT_TRY(parent._dn_id(), parent._ob_id());
	cint_string parent_type;
	if (!parent.type(parent_type)) throw udm_exception("Couldn't determine the parent type!");

	::Uml::Class what;

	if (parent_type.comp("Namespace") == 0) {
		::Uml::Namespace ns = ::Uml::Namespace::Cast(upi_o);
		if (!ns) throw udm_exception("The provided namespace is NULL!");
		SetClass(what, ns, target_name);
	} else if (parent_type.comp("Diagram") == 0) {
		::Uml::Diagram dgr = ::Uml::Diagram::Cast(upi_o);
		if (!dgr) throw udm_exception("The provided diagram is NULL!");
		SetClass(what, dgr, target_name);
	} else {
		throw udm_exception("Unknown parent provided to UPO_SetClass!");
	}

	if (!what) throw udm_exception("SetClass() failed!");
/*	class_what.dn_id = what.__impl()->__getdn()->uniqueId();
	class_what.ob_id = what.__impl()->uniqueId();
	*/
	UdmPseudoObject r(what.__impl()->__getdn()->uniqueId(), what.__impl()->uniqueId());
	class_what = r;
	return true;
	CINT_CATCH;

};

bool UPO_SetAttribute(UdmPseudoObject &attr_what, UdmPseudoObject &class_what_class,  const char* target_name)
{
	CINT_TRY(class_what_class._dn_id(), class_what_class._ob_id());
	::Uml::Class what_class = ::Uml::Class::Cast(upi_o);
	if (!what_class) throw udm_exception("The provided class is NULL!");

	::Uml::Attribute what;
	SetAttribute(what, what_class, target_name);
	if (!what) throw udm_exception("SetAttribute failed!");
/*	attr_what.dn_id = what.__impl()->__getdn()->uniqueId();
	attr_what.ob_id = what.__impl()->uniqueId();
	*/
	UdmPseudoObject r(what.__impl()->__getdn()->uniqueId(), what.__impl()->uniqueId());
	attr_what = r;
	return true;
	CINT_CATCH;
};

bool UPO_SetChildRole(UdmPseudoObject &ccr_what, UdmPseudoObject &what_target_class, UdmPseudoObject &what_theo_target_class, const char * role_name, const char * orole_name)
{
	CINT_TRY2(what_target_class._dn_id(), what_target_class._ob_id(), what_theo_target_class._dn_id(), what_theo_target_class._ob_id());
	::Uml::Class what_target = ::Uml::Class::Cast(upi_o1);
	if (!what_target) throw udm_exception("The provided class is NULL!");

	::Uml::Class what_theo_target = ::Uml::Class::Cast(upi_o2);
	if (!what_theo_target) throw udm_exception("The provided class is NULL!");

	::Uml::CompositionChildRole what;
	SetChildRole(what,what_target, what_theo_target, role_name, orole_name);

	if (!what) throw udm_exception("SetChildRole failed!");
	/*
	ccr_what.dn_id = what.__impl()->__getdn()->uniqueId();
	ccr_what.ob_id = what.__impl()->uniqueId();
	*/

	UdmPseudoObject r(what.__impl()->__getdn()->uniqueId(), what.__impl()->uniqueId());
	ccr_what = r;
	
	return true;
	CINT_CATCH;
};
bool UPO_SetParentRole(UdmPseudoObject &cpr_what, UdmPseudoObject &what_target_class, UdmPseudoObject &what_theo_target_class, const char * role_name, const char* orole_name)
{
	CINT_TRY2(what_target_class._dn_id(), what_target_class._ob_id(), what_theo_target_class._dn_id(), what_theo_target_class._ob_id());
	::Uml::Class what_target = ::Uml::Class::Cast(upi_o1);
	if (!what_target) throw udm_exception("The provided class is NULL!");

	::Uml::Class what_theo_target = ::Uml::Class::Cast(upi_o2);
	if (!what_theo_target) throw udm_exception("The provided class is NULL!");

	::Uml::CompositionParentRole what;
	SetParentRole(what,what_target, what_theo_target, role_name, orole_name);

	if (!what) throw udm_exception("SetChildRole failed!");
	/*
	cpr_what.dn_id = what.__impl()->__getdn()->uniqueId();
	cpr_what.ob_id = what.__impl()->uniqueId();
	*/
	UdmPseudoObject r(what.__impl()->__getdn()->uniqueId(), what.__impl()->uniqueId());
	cpr_what = r;
	
	return true;
	CINT_CATCH;
};
bool UPO_SetAssocRole(UdmPseudoObject &ar_what, UdmPseudoObject &what_target_class, UdmPseudoObject &what_theo_target_class, const char* target_name)
{
	CINT_TRY2(what_target_class._dn_id(), what_target_class._ob_id(), what_theo_target_class._dn_id(), what_theo_target_class._ob_id());
	::Uml::Class what_target = ::Uml::Class::Cast(upi_o1);
	if (!what_target) throw udm_exception("The provided class is NULL!");

	::Uml::Class what_theo_target = ::Uml::Class::Cast(upi_o2);
	if (!what_theo_target) throw udm_exception("The provided class is NULL!");

	::Uml::AssociationRole what;
	SetAssocRole(what,what_target, what_theo_target, target_name);

	if (!what) throw udm_exception("SetChildRole failed!");
	/*
	ar_what.dn_id = what.__impl()->__getdn()->uniqueId();
	ar_what.ob_id = what.__impl()->uniqueId();
	*/
	UdmPseudoObject r(what.__impl()->__getdn()->uniqueId(), what.__impl()->uniqueId());
	ar_what = r;
	return true;
	CINT_CATCH;
};
void UPO_dummy(){};

class UPO_metainfo
{
	Udm::DataNetwork * dn;
	::Uml::Diagram * udgr;
	Udm::UdmDiagram * dgr;
public:
	UPO_metainfo(Udm::DataNetwork *_dn, ::Uml::Diagram * _udgr, Udm::UdmDiagram * _dgr) : dn(_dn), udgr(_udgr), dgr(_dgr) {} ;
	~UPO_metainfo()
	{
		Udm::MetaDepository::RemoveDiagram(METADEPOSITORY_KEY(udgr->name()));

		delete udgr;
		delete dgr;
		delete dn;
	};
	static map<string,const UPO_metainfo *> upo_metamap;

	static bool reg_metainfo(const string & str, const UPO_metainfo * mi)
	{
		map<string, const UPO_metainfo *>::value_type item(str, mi); 
		pair<map<string, const UPO_metainfo *>::iterator, bool> res = upo_metamap.insert(item);
		return res.second;
	};
	static const UPO_metainfo * get_metainfo(const string &str)
	{
		map<string, const UPO_metainfo *>::iterator i = upo_metamap.find(str);
		if (i != upo_metamap.end()) return i->second;
		else return NULL;
	};

	static void unreg_metainfo(const string &str)
	{
		upo_metamap.erase(str);
	};
	
};

map<string,const UPO_metainfo *> UPO_metainfo::upo_metamap;
//===========================
void StoreUmlXsd()
{
  std::string nn("Uml.xsd");
  UdmDom::str_xsd_storage::str_str_map::const_iterator it  = UdmDom::str_xsd_storage::static_xsd_container.find(nn);
  if (it == UdmDom::str_xsd_storage::static_xsd_container.end())
    UdmDom::str_xsd_storage::StoreXsd(nn,Uml_xsd::getString().c_str());

}

bool  UPO_LoadDiagram(const char * xml_meta_file, UdmPseudoObject & diagram)
{
	try
	{
    StoreUmlXsd();
		//we get an UML XML file 
		//we have to load it and register it with MetaDepository
		//somewhere we have to release these objects
		UdmDom::DomDataNetwork * ddn = new UdmDom::DomDataNetwork(::Uml::diagram);
		::Uml::Diagram * loaded_dgr = new ::Uml::Diagram();
		Udm::UdmDiagram * dgr = new Udm::UdmDiagram();	//this will be fried by RemoveDiagram();

		ddn->OpenExisting(xml_meta_file, "Uml.xsd", Udm::CHANGES_LOST_DEFAULT);
		*loaded_dgr = ::Uml::Diagram::Cast(ddn->GetRootObject());

		if (!loaded_dgr) throw udm_exception("UML Diagram could not be loaded!");
		
		const UPO_metainfo * umi = new UPO_metainfo(ddn, loaded_dgr, dgr);
		if (!UPO_metainfo::reg_metainfo(xml_meta_file, umi))
			throw udm_exception("MetaInfo registration failed!");


		dgr->init = UPO_dummy;
		dgr->dgr = loaded_dgr;
		
		Udm::MetaDepository::StoreDiagram(METADEPOSITORY_KEY(loaded_dgr->name()), *dgr);

		//return value set-up
		/*
		diagram.dn_id = ddn->uniqueId();
		diagram.ob_id = loaded_dgr->__impl()->uniqueId();
		*/
		UdmPseudoObject r(ddn->uniqueId(), loaded_dgr->__impl()->uniqueId());
		diagram = r;
		return true;
	}
	catch (udm_exception &e)
	{
		set_last_error(e.what());
		return false;
	};
}
void replaceAll( string &s, const char *from, const char *to )
{
	size_t pos = 0, len = strlen( from );

    while ((pos = s.find( from, pos )) != s.npos)
    {
        s.replace( pos, len, to );
    }
}

bool  UPO_LoadDiagramFromString(const char * xml_meta_file, const char * xml_string, UdmPseudoObject & diagram)
{
	try
	{
		StoreUmlXsd();
		//we get an UML XML file 
		//we have to load it and register it with MetaDepository
		//somewhere we have to release these objects
		UdmDom::DomDataNetwork * ddn = new UdmDom::DomDataNetwork(::Uml::diagram);
		::Uml::Diagram * loaded_dgr = new ::Uml::Diagram();
		Udm::UdmDiagram * dgr = new Udm::UdmDiagram();	//this will be fried by RemoveDiagram();

		string xml(xml_string);
		replaceAll(xml,"\\&quot;","&quot;");
		replaceAll(xml,"\\n","&#10;");

		ddn->OpenExistingFromString(xml, "Uml.xsd", Udm::CHANGES_PERSIST_ALWAYS);
		*loaded_dgr = ::Uml::Diagram::Cast(ddn->GetRootObject());

		if (!loaded_dgr) throw udm_exception("UML Diagram could not be loaded!");
		
		const UPO_metainfo * umi = new UPO_metainfo(ddn, loaded_dgr, dgr);
		if (!UPO_metainfo::reg_metainfo(xml_meta_file, umi))
			throw udm_exception("MetaInfo registration failed!");


		dgr->init = UPO_dummy;
		dgr->dgr = loaded_dgr;
		
		Udm::MetaDepository::StoreDiagram(METADEPOSITORY_KEY(loaded_dgr->name()), *dgr);

		//return value set-up
		/*
		diagram.dn_id = ddn->uniqueId();
		diagram.ob_id = loaded_dgr->__impl()->uniqueId();
		*/
		UdmPseudoObject r(ddn->uniqueId(), loaded_dgr->__impl()->uniqueId());
		diagram = r;
		return true;
	}
	catch (udm_exception &e)
	{
		set_last_error(e.what());
		return false;
	};
};


bool  UPO_UnLoadDiagram(const char * xml_meta_file)
{
	const UPO_metainfo * umi = UPO_metainfo::get_metainfo(xml_meta_file);

	if (!umi) return false;
		
	UPO_metainfo::unreg_metainfo(xml_meta_file);
	delete umi;
	
	return true;

}
//===========================
bool  StoreXsd(const cint_string& key, const cint_string& xsd_str)
{
  try
  {
    UdmDom::str_xsd_storage::StoreXsd((const std::string&)key.buffer(), (const std::string&)xsd_str.buffer());
  }
  catch (const udm_exception& e) 
  {
    set_last_error(e.what()); return false;
  } 
  catch(...) 
  {
    set_last_error("Unexpected exception caught."); 
    return false;
  }
  return true;
}
//===========================
		
bool  RemoveXsd(const cint_string& key)
{
  try
  {

    UdmDom::str_xsd_storage::RemoveXsd((const std::string)key.buffer());
  }
  catch (const udm_exception& e) 
  {
    set_last_error(e.what()); return false;
  } 
  catch(...) 
  {
    set_last_error("Unexpected exception caught."); 
    return false;
  }

  return true;
}
bool  ClearXsdStorage()
{
  try
  {

    UdmDom::str_xsd_storage::ClearXsdStorage();
  }
  catch (const udm_exception& e) 
  {
    set_last_error(e.what()); return false;
  } 
  catch(...) 
  {
    set_last_error("Unexpected exception caught."); 
    return false;
  }

  return true;
}
//===========================
bool  AddURIToUMLNamespaceMapping(const cint_string& namespaceURI, 
                                  const cint_string& namespaceUML,
                                  const cint_string& xsdName)
{
  try
  {
    UdmDom::AddURIToUMLNamespaceMapping((const std::string)namespaceURI.buffer(), 
        (const string)namespaceUML.buffer(),
        (const string)xsdName.buffer());
  }
  catch (const udm_exception& e) 
  {
    set_last_error(e.what()); 
    return false;
  } 
  catch(...) 
  {
    set_last_error("Unexpected exception caught."); 
    return false;
  }
  return true;


}
//===========================
bool  ClearURIToUMLNamespaceMappings()
{

  try
  {
    UdmDom::ClearURIToUMLNamespaceMappings();
  }
  catch (const udm_exception& e) 
  {
    set_last_error(e.what()); 
    return false;
  } 
  catch(...) 
  {
    set_last_error("Unexpected exception caught."); 
    return false;
  }
  return true;

}



