
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

  FILE:		UdmCInt.h
  SYNOPSIS:	CInt interface classes - common classes shared by Udm and CInt
  CREATED:	08/10/02
  AUTHOR:	Endre Magyari(endre.magyari@vanderbilt.edu)

  CHANGELOG:
		01/28/05 - Himanshu
				Adding a equals operator for UdmPseudoObject
		01/27/05 - Himanshu
				Defining separate header files for CINT (UdmCint.h) and SWIG (UdmCintSwig.h)
				and adding functions for array attribute getters and setters
		01/20/05 - Himanshu - Defining separate header files for CINT (UdmCint.h) and SWIG (UdmCintSwig.h)
		01/11/05 - Himanshu - Added count() functions for functions returning a container of elements
		04/27/04 -	endre
			added
			- copy constructor
			- += int, += char operators
		
		10/12/02	operator == for cint_string added, requested by Feng
		09/30/02	operator += for cint_string added, requested by Feng


*/
#ifndef UDMCINTSWIG_H
#define UDMCINTSWIG_H
#include <vector>
#include "cint_string.h"

#ifdef _WIN64
#define NO_ANTLR
#endif

using namespace std;


struct UdmDouble 
{
	UdmDouble(double val) {doubleVal = val;};
	double doubleVal;
};

struct UdmBool 
{
	UdmBool(bool val) {boolVal = val;};
	bool boolVal;
};
#ifndef _WIN32
  typedef long long __int64; 
#endif

struct UdmLong {
	UdmLong() {longVal = 0;};
	UdmLong(__int64 val) {longVal = val;};
	__int64 longVal;
};


class UdmPseudoObject;

class  UdmPseudoObjectS

{
	UdmPseudoObject * impl;
	int length;
public:

	UdmPseudoObjectS(int length);
	UdmPseudoObjectS(const UdmPseudoObjectS &frm);
	UdmPseudoObjectS& operator=(const UdmPseudoObjectS &frm);
	bool operator !() const;
	operator bool()  const;
	bool overflow;

	
	//for getting object no index
	UdmPseudoObject& operator[](const int index) const;
	//for setting object no index
	void SetAt(const int index, const UdmPseudoObject& item);
	int GetLength() const;

	~UdmPseudoObjectS();
};




struct  AssociationInfo

{

	cint_string assoc_class;//can be NULL
	cint_string assoc_class_ns;	//path(delim = "::")
	cint_string SrcRolename;
	cint_string DstRolename;
	AssociationInfo(const char *src, const char *dst, const char *assoc);
	AssociationInfo(cint_string src, cint_string dst, cint_string assoc);
};

struct  CompositionInfo
{
	cint_string parentRole;
	cint_string childRole;
	CompositionInfo(const char *pr, const char *cr);
	CompositionInfo(cint_string pr, cint_string cr);
};




static const int TARGETFROMPEER = 0;
static const int TARGETFROMCLASS = 1;
static const int CLASSFROMTARGET = 2;

class  UdmPseudoObject

{
public:
#if defined(_M_AMD64)
	typedef __int64 ob_id_type;
#else
	typedef unsigned long ob_id_type;
#endif

private:
	ob_id_type ob_id;
	unsigned long dn_id;

public:
	
	ob_id_type _ob_id() const;
	unsigned long _dn_id() const;

	UdmPseudoObject();				//NULL object
	UdmPseudoObject(unsigned long dn_id, ob_id_type ob_id);
	UdmPseudoObject& operator=(const UdmPseudoObject& frm);
	// Himanshu: Adding a equals operator for UdmPseudoObject
	bool operator ==(const UdmPseudoObject& to);
	//error handling (mostly for errors resulting from operations 
	//being executed on invalid ids)
	static bool GetLastError(cint_string& buffer);
	
	//type
	bool type(cint_string &) const;
	bool stereotype(cint_string &value) const;
	bool uniqueId(UdmLong &) const;

	//IMPL API
	bool getParent(const char * prole, UdmPseudoObject &value) const;
	bool setParent(UdmPseudoObject &parent, const char * prole);
	bool detach();
	bool getChildrenCount(const char * crole, const char * kind, const char * kind_ns_path, UdmLong& count) const; // HN1
	bool getChildren(const char * crole, const char * kind, const char * kind_ns_path, UdmPseudoObjectS &value) const;
	bool setChildren(const char * crole, const UdmPseudoObjectS &value);



	
	//associations
	bool getAssociationCount(const char * ass_role, UdmLong& count, int mode = TARGETFROMPEER) const; // HN2
	bool getAssociation(const char * ass_role, UdmPseudoObjectS &value, int mode = TARGETFROMPEER) const;
	bool setAssociation(const char * ass_role, const UdmPseudoObjectS &value, int mode = TARGETFROMPEER);



	//attribute setters
	bool SetIntVal(const char * name, __int64 value);
	bool SetRealVal(const char * name, double value);
	bool SetStrVal(const char * name, const char * value);
	bool SetBoolVal(const char * name, bool value);

	//attribute getters
	bool GetIntVal(const char * name, UdmLong &value);
	bool GetRealVal(const char * name, UdmDouble &value);
	bool GetStrVal(const char * name, cint_string & value);
	bool GetBoolVal(const char * name, UdmBool &value);

	// Himanshu: array attribute getters
	bool GetIntValues(const char * name, vector<UdmLong>& values);
	bool GetRealValues(const char * name, vector<double>& values);
	bool GetStrValues(const char * name, vector<cint_string>& values);
	bool GetBoolValues(const char * name, vector<bool>& value);

	// Himanshu: array attribute setters
	bool SetIntValues(const char * name, vector<UdmLong> values);
	bool SetRealValues(const char * name, vector<double> values);
	bool SetStrValues(const char * name, vector<cint_string> values);
	bool SetBoolValues(const char * name, vector<bool> value);

	//TOMI
	bool GetAdjacentObjectsCount(UdmLong &count); // HN3
	bool GetAdjacentObjects(UdmPseudoObjectS &value);
	bool GetAdjacentObjectsCount(const char * dst_type, const char * dst_type_ns_path, UdmLong& count); // HN4
	bool GetAdjacentObjects(const char * dst_type, const char * dst_type_ns_path, UdmPseudoObjectS &value);
	bool GetAdjacentObjectsCount(const char * dst_type, const char * dst_type_ns_path, const AssociationInfo & ass, UdmLong& count); // HN5
	bool GetAdjacentObjects(const char * dst_type, const char * dst_type_ns_path, const AssociationInfo & ass, UdmPseudoObjectS & value);

	bool GetParent(UdmPseudoObject &value);

	bool GetChildObjectsCount(const char * type, const char * type_ns_path, UdmLong& count); // HN6
	bool GetChildObjects(const char * type, const char * type_ns_path, UdmPseudoObjectS &value);
	bool GetChildObjectsCount(const CompositionInfo &ci, const cint_string & type, const cint_string & type_ns_path, UdmLong& count); // HN7
	bool GetChildObjects(const CompositionInfo &ci, const cint_string & type, const cint_string & type_ns_path, UdmPseudoObjectS &value);
	
	bool GetAssociationClassObjectsCount(const UdmPseudoObject &dstObject, const AssociationInfo & ai, UdmLong& count); // HN8
	bool GetAssociationClassObjects(const UdmPseudoObject &dstObject, const AssociationInfo & ai, UdmPseudoObjectS &value);
	bool GetPeersFromAssociationClassObjectCount(UdmLong& count); // HN9
	bool GetPeersFromAssociationClassObject(UdmPseudoObjectS &value);


	bool CreateObject(const char * type, const char * type_ns,UdmPseudoObject &value, const char* compositionChildRole = 0, const bool subtype = false );
	bool CreateLink(UdmPseudoObject &dst, const AssociationInfo& ass_type);

	bool DeleteObject();
};

static const int UPDN_CHANGES_PERSIST_ALWAYS = 0;
static const int UPDN_CHANGES_PERSIST_DEFAULT = 1;
static const int UPDN_CHANGES_LOST_DEFAULT = 2;

class  UdmPseudoDataNetwork
{

	cint_string meta_name;
	UdmPseudoObject root;
	long dn_id;

public:
	UdmPseudoDataNetwork(const char * meta_name, UdmBool & retval);
	~UdmPseudoDataNetwork();
	bool OpenExisting(const char * sys_name, const char * meta_locator, const int backendsemantics = UPDN_CHANGES_PERSIST_ALWAYS);
  bool OpenExistingFromString(const char * sys_name, const char * meta_locator);
	bool CreateNew(const char * sys_name, const char * meta_locator, const UdmPseudoObject& rootclass, const int backendsemantics = UPDN_CHANGES_PERSIST_ALWAYS);
  bool CreateNewToString(const char * meta_locator, const UdmPseudoObject& rootclass); 
	bool GetRootObject(UdmPseudoObject & value);
	bool GetRootMeta(UdmPseudoObject &value);			//a Uml::Diagram should be returned
	
	bool CloseWithUpdate();
	bool CloseNoUpdate();
	bool SaveAs(const char * systemname);
  bool SaveAsString(cint_string& buffer);
	bool CloseAs(const char * systemname);
	bool isOpen(UdmBool &value);
	bool uniqueId(UdmLong &value);
  bool OCL_Evaluate(cint_string& res);
};

// ---------------------------- Dynamic Meta Initialization
// used to set the static API member variables to the already initialized meta objects 


bool  UPO_LoadDiagram(const char *  xml_meta_file, UdmPseudoObject & diagram);
bool  UPO_LoadDiagramFromString(const char * xml_meta_file, const char * xml_string, UdmPseudoObject & diagram);
bool  UPO_UnLoadDiagram(const char * xml_meta_file);
bool  UPO_SetDiagram(UdmPseudoObject &diagram_what, const char * name);
bool  UPO_SetNamespace(UdmPseudoObject &ns_what, UdmPseudoObject &parent, const char * target_name);
bool  UPO_SetNamespaceByPath(UdmPseudoObject &ns_what, UdmPseudoObject &diagram, const char * path);
bool  UPO_SetClass(UdmPseudoObject &class_what, UdmPseudoObject &parent, const char * target_name);
bool  UPO_SetAttribute(UdmPseudoObject &attr_what, UdmPseudoObject &class_what_class,  const char * target_name);
bool  UPO_SetChildRole(UdmPseudoObject &ccr_what, UdmPseudoObject &what_target_class, UdmPseudoObject &what_theo_target_class, const char * role_name, const char *  orole_name);
bool  UPO_SetParentRole(UdmPseudoObject &cpr_what, UdmPseudoObject &what_target_class, UdmPseudoObject &what_theo_target_class, const char * role_name, const char * orole_name);
bool  UPO_SetAssocRole(UdmPseudoObject &ar_what, UdmPseudoObject &what_target_class, UdmPseudoObject &what_theo_target_class, const char *target_name);
bool  StoreXsd(const cint_string& key, const cint_string& xsd_str);
bool  RemoveXsd(const cint_string& key);
bool  ClearXsdStorage();
bool  AddURIToUMLNamespaceMapping(const cint_string& namespaceURI, const cint_string& namespaceUML, const cint_string& xsdName);
bool  ClearURIToUMLNamespaceMappings();

#endif//UDMCINTSWIG_H



