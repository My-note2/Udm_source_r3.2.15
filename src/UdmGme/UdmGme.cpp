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
*/
/*
CHANGELOG

  09/11/06	-	endre
			-	add support to get/set annotations

  03/22/06	-	endre
			-	change amapInitialize to use all subtypes of an association class, even if the meta contains an object named like the assoc class

  11/29/05	-	endre

			-	add support for references that are an abstract UML class

  01/12/05	-	endre

			-	More detailed error message in amapInitialize
  22/11/04	-	endre

			-	VC7.1 .net porting issues

  25/09/04	-	endre

			- struct UdmGme::assocmapitem now it's being defined here. In UdmGme.h it's being only forward declared in order to 
			keep the include file clean of MGA structs.

  24/09/04	-	endre

			- removed global variable amap and global function amapInitialize and placed as a member of GmeDataNetwork.
			Each GmeDataNetwork has it's own version of amap. This was required because GME wanted to have the meta objects released
			at the time of closing the project.
			the map is cleared when the datanetwork is closed.

  19/08/04	-	endre

			- fixed bug UDM-16 regarding inherited connections
  22/07/04	-	endre
	
			- minor bugfix regarding setting up the reference port helper connections
				(only navigable associationroles should be considered when setting registry values)
  13/07/04	-	endre

			-   addressed feature request UDM-6 regarding reference ports.

  21/06/04	-	endre

			- fixed amapInitialize to address bug UDM-10

  05/05/04	-	endre
	
			-	added support for abstract supertype connections 

  04/28/04	-	endre

			- folder objects could not be deleted, obvious fix in setParent() call
  04/26/04	-	endre

			-	createChild() bug. Scenario:
					
					- B is an abstract class
					- B is contained in A with empty child role
					- C is the only descendant of B
					- crateChild() tries to create C in A

  04/18/04	-	endre
			
				- when an object is detached, and the host datanetwork of the object is being part of a Udm project, 
				  the corresponding placeholder object (if any) should be detached as well.

  03/24/04	-	endre

				- Changing the syntax of createchild() as required by Udm::ObjectImpl
				- Added backendsemnatics to the OpenExisting function (BON version)

  11/04/02	-	endre

				- createChild() bug fixed. Still related to inherited childroles. 
				The newly inserted function GetMetaRole() should solve the problem.
				The problem was that MGA MetaRole it's composed like this:
				if ( child class has non-abstact derivates)
				{
					MetaRole = Kindname + Rolename;    // Rolename may be empty;
				}
				else 
				{
					 if (Rolename != "") MetaRole = Rolename
					 else MetaRole = KindName;
				}

  08/23/02	-	endre

				- fixed getParent() bug reported by Feng. Still related to 
				inherited childroles and parentroles.

  07/21/02	-	endre
				
				- fixed createChild(). Still problems with inherited childroles.

  06/18/02	-	endre

				-fixed createChild(role, kind) bug. 
					when both 'role' and  'kind' were provided, 
					and 'kind' was different from 'role.target()',
					(i.e. the type of the object(='kind') to be inserted 
					had only an inherited childrole), the new object was
					created with incosistent types: the target class of 
					the 'role' in the MGA layer, and with 'kind' in the 
					UDM layer. That could cause harm.
					
					  Scenario when this could happen:

						+---+a		+---+
						| A |-----<>| B |
						+-+-+		+---+
						  | 	
						 /_\
						  |
						+-+-+
						| C |
						+---+
					
						B Binst;

						If you wanted to create an object of type C in Binst, 
						you either did this:
						
						  C Cinst = C::Create(Binst); 
						
						or this:

						  C Cinst = C::Create(Binst, a)
						
						in both cases, Cinst would be created with it's type pointer
						set to C, but in MGA it would be an instance of type A.

					
	
				- fixed getChildren(role, kind) bug

					The desired behaviour of this function is that to return 
					all the children via role 'role' which are of type compatible
					with 'kind'. That is, either type 'kind' or a type derived from 'kind'

					However, this function returned only children with type 'kind' when
					kind was provided. 

					Scenario when this could happen:

						+---+a		+---+
						| A |-----<>| B |
						+-+-+		+---+
						  | 	
						 /_\
						  |
						+-+-+
						| C |
						+---+
					
						B Binst;						//we assume it's already created
						A Ainst = A::Create(Binst,a);	//childrole 'a' could be omitted 
						C Cinst = C::Create(Binst,a);	//childrole 'a' could be omitted  

					and then,

						Binst.__impl()->getChildren(a, A);

						is expected to return both Cinst and Ainst, but it returned
						only Ainst.

				
				Affected are mostly paradigm&backend indepent codes
				which use the createChild(role, kind) and getChildren(role, kind)
				functions with both params provided. 
				(UdmCopy, datanetwork assignment operator are such. PatternMatcher?)


  04/12/02	-	endre
				
				Fixed getParent(). Now it takes ParentRole() into account.
				It returns the parent only when tha parentrole matches.

  03/16/02  -   Fixed GmeDataNetwork::setChildren().  - Tihamer Levendovszky  
  03/16/02	-	BUG: parent()=NULL does not work. SetParent should be fixed
  03/10/02  -   Fixed GmeDataNetwork::OpenExisting(). - Tihamer Levendovszky
  03/06/02	-	Fixed GmeDataNetwork::SaveAs(). Now it works.	-	endre

*/
// Control SUPPORT for folders here!!!!!!!!!!!!!!!!!!




#pragma warning( disable : 4786 )	// this is the stupid warning with truncating identifiers to 255 chars
#define ASSERT assert
#include "CommonHeaders.h"
#include <map>
#include <iomanip>
#include <algorithm>

// Hack to detect VS2010 GME: VS2010 GME has ifdef guards in InterfaceVersion.h
#define INTERFACEVERSION_INCLUDED
#define cpp_quote(x) namespace { }
#include "InterfaceVersion.h"
#ifndef INTERFACE_VERSION
#define GME_VS10
#endif
#undef INTERFACEVERSION_INCLUDED
#undef INTERFACE_VERSION

// KMS: GME VS2010 renamed METALib to MGAMetaLib
// Need to declare this for GME VS2008
namespace MGAMetaLib { }
namespace METALib
{
	using namespace MGAMetaLib;
}
// UDM_IMGAMETAPROJECT is used in UdmGme.h
#define UDM_IMGAMETAPROJECT METALib::IMgaMetaProject
using namespace METALib;
using namespace MGALib;

#include "MgaDefs.h"


#define WRITE_OP_REFRESH_RATE 26		//this value was approximated by binary search, try&error
#include "UdmGme.h"
#include "GmeObject.h"
#include <UdmUtil.h>

#include <sstream>
#include <iterator>

// Hack to detect VS10 GME: VS10 GME has ifdef guards in InterfaceVersion.h
#define INTERFACEVERSION_INCLUDED
#define cpp_quote(x) namespace { }
#include "InterfaceVersion.h"
#ifndef INTERFACE_VERSION
#define GME_VS10
#endif
#undef INTERFACEVERSION_INCLUDED
#undef INTERFACE_VERSION


static _bstr_t BSTRFromUTF8(const std::string & utf8)
{
    if (utf8.empty())
        return _bstr_t();

    // Fail if an invalid input character is encountered
    const DWORD conversionFlags = MB_ERR_INVALID_CHARS;

    const int utf16Length = ::MultiByteToWideChar(CP_UTF8, conversionFlags, utf8.data(), utf8.length(), NULL, 0);
    if (utf16Length == 0)
    {
        DWORD error = ::GetLastError();

        throw udm_exception(
            (error == ERROR_NO_UNICODE_TRANSLATION) ? 
                "Invalid UTF-8 sequence found in input string." :
                "Can't get length of UTF-16 string (MultiByteToWideChar failed).");
	}

	BSTR utf16 = SysAllocStringByteLen(NULL, utf16Length*2);
	if (utf16 == NULL)
		throw std::bad_alloc();

	if (!::MultiByteToWideChar(CP_UTF8, 0, utf8.data(), utf8.length(), utf16, utf16Length))
    {
        DWORD error = ::GetLastError();
		SysFreeString(utf16);
        throw udm_exception("Can't convert string from UTF-8 to UTF-16 (MultiByteToWideChar failed).");
    }

    return _bstr_t(utf16, false);
}

static std::string UTF8FromBSTR(BSTR utf16)
{
    if (utf16 == NULL || *utf16 == L'\0')
        return std::string();


    // const int utf16Length = static_cast<int>(wcslen(utf16));
	const int utf16Length = SysStringLen(utf16);

    // Get length (in chars) of resulting UTF-8 string
    const int utf8Length = ::WideCharToMultiByte(CP_UTF8, 0, utf16, utf16Length, NULL, 0, NULL, NULL);

	if (utf8Length == 0)
    {
        DWORD error = ::GetLastError();
        throw udm_exception("Can't get length of UTF-8 string (WideCharToMultiByte failed).");
    }

    std::string utf8;
    utf8.resize(utf8Length);

	if (!::WideCharToMultiByte(CP_UTF8, 0, utf16, utf16Length, &utf8[0], utf8.length(), NULL, NULL))
    {
        DWORD error = ::GetLastError();
        throw udm_exception("Can't convert string from UTF-16 to UTF-8 (WideCharToMultiByte failed).");
    }

    return utf8;
}



namespace UdmGme 
{
	struct assocmapitem
	{
		
		::Uml::AssociationRole primary;
		IMgaMetaFCOPtr  metaobj;
		//if metaobj is null, there may be, depending on the name of the class to be created,
		//multiple metaobjs,
		//which will be computed "runtime"
		// this is because inheritence is not supported by MGA at all,
		// if in UML there is a supertype association class,
		// metaobjs will contain the connection objects which are inherited from the UML assocclass
		// and also can be instantiated in the MGA layer
		//
		// we also set metaobj to null and use metaobjs when this association represents
		// a reference and the reference is an abstract UML class; in this case metaobjs
		// will contain the non-abstract descendants of the class
		IMgaMetaFCOPtr* metaobjs;		
		int metaobjs_count;
		objtype_enum ot;

		//we need a flag showing that this is just a refport container helper connection
		//if this is true, primary will be the role with the name
		//and metaobj will be the MGA Meta Connection object
		bool rp_helper;

		// if this is a connection and there are rp_helper associations for it,
		// then these members are the corresponding association roles of the rp_helper associations;
		::Uml::AssociationRole src_rp_helper, dst_rp_helper;

		// the target of primary is an abstract UML class
		bool pt_abstract;

		assocmapitem() : metaobjs_count(0),  metaobjs(NULL), rp_helper(false), pt_abstract(false), src_rp_helper(&Udm::_null), dst_rp_helper(&Udm::_null) {};
	};

	SmartBSTR GetRoleFromMeta(IMgaMetaFCOPtr& meta, const wchar_t* role_type)
	{
		SmartBSTR role = meta->RegistryValue[role_type];
		if (role.length() == 0)
		{
			if (wcscmp(role_type, L"sName") == 0)
			{
				if (meta->ObjType == OBJTYPE_CONNECTION)
					return SmartBSTR(L"src") + meta->Name;
				else if (meta->ObjType == OBJTYPE_SET)
					return SmartBSTR(L"set") + meta->Name;
			}
			else if (wcscmp(role_type, L"dName") == 0)
			{
				if (meta->ObjType == OBJTYPE_CONNECTION)
					return SmartBSTR(L"dst") + meta->Name;
			}
			else if (wcscmp(role_type, L"mName") == 0)
			{
				if (meta->ObjType == OBJTYPE_SET)
					return SmartBSTR(L"members");
			}
			else if (wcscmp(role_type, L"rName") == 0)
			{
				if (meta->ObjType == OBJTYPE_REFERENCE)
					return SmartBSTR(L"ref");
			}
			else if (wcscmp(role_type, L"rrName") == 0)
			{
				if (meta->ObjType == OBJTYPE_REFERENCE)
					return SmartBSTR(L"referedby") + meta->Name;
			}
			// TODO dRefParent
		}
		return role;
	}

	
	struct privdata 
	{
		IMgaProjectPtr project;
		IMgaTerritoryPtr terr;
		std::map<Udm::Object::uniqueId_type, std::vector<Udm::Object::uniqueId_type> > derived_map;

		bool IsDerivedFrom(const Uml::Class& derived, const Uml::Class& kind);
	};

	void com_exception(HRESULT a, IErrorInfo* errorinfo) {
		std::stringstream error_message;
		SmartBSTR str;
		if (errorinfo != 0) {
			BSTR bbstr;
			if (SUCCEEDED(errorinfo->GetDescription(&bbstr))) {
				str.Attach(bbstr);
			}
		}
		if (!str) {
			LPTSTR errorText = NULL;
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL, a, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPTSTR)&errorText, 0, NULL);
			if (errorText != NULL) {
				str = errorText;
				LocalFree(errorText);
			}
		}
		error_message << "COM exception: ";
		if (!!str) {
			error_message << static_cast<const char*>(str);
			error_message << " (0x" << std::hex << setw(8) << (unsigned long)a << ")";
		} else {
			error_message << "0x" << std::hex << setw(8) << (unsigned long)a;
		}

		throw udm_exception(error_message.str());
	}

	void com_exception(HRESULT a, IUnknown *b, REFIID c) 
	{
		if (b) {
			ISupportErrorInfoPtr pp = b;
			if(pp) 
			{
				if(pp->InterfaceSupportsErrorInfo(c) == S_OK) 
				{
					IErrorInfoPtr ee;
					if (SUCCEEDED(GetErrorInfo(0, &ee))) {
						com_exception(a, ee);
					}
				}
			}
		}
		com_exception(a, NULL);
	}

	SmartBSTR createGMEconnstr(string sn);

	string getnameforassoc(const ::Uml::Association &ass, bool generate_name) 
	{
		::Uml::Class acl = ass.assocClass();
		string ret;
		if(acl) ret = getShortClassPath(acl);
		else 
		{
			ret = ass.name();
			if(generate_name && ret.empty()) 
			{
				set< ::Uml::AssociationRole> roles = ass.roles();
				set< ::Uml::AssociationRole>::iterator j = roles.begin(); 
				ret = "noname_";
				ret += getShortClassPath(*j->target());
				ret += "_to_";
				ret += getShortClassPath(*(++j)->target());
			}
		}
		return ret;
	}

	int CompareKindToClass(IMgaMetaFCOPtr kind, ::Uml::Class meta)
	{
		return getShortClassPath(meta).compare(static_cast<const char*>(kind->Name));
	}

	// convert a MGA rolename to the name generated by Uml::MakeRoleName:
	// - replace "::" with "_"
	// - use only the kindname, without namespace if the composition parent and role are in the same namespace
	string ToUdmRoleName(IMgaMetaRolePtr role, const ::Uml::CompositionChildRole &ccr)
	{
		string name = role->GetName();
		string result;
		if (Uml::IsCrossNSComposition( ccr.parent() )) {
			result = UdmUtil::replace_delimiter(name, "::", "_");
		} else {
			string::size_type i = name.find_last_of("::");
			if (i != string::npos)
				i++;
			else
				i = 0;
			result = name.substr(i);
		}
		return result;
	}

	

	typedef pair<bool, string> RpHelperID;	// pair<role_is_navigable, object_id>
	typedef vector<RpHelperID> RpHelperIDs;

	RpHelperIDs StringToList(const string &src)
	{
		RpHelperIDs ret;

		string::size_type begin, end;
		begin = src.find_first_not_of(' ');
		if (begin != string::npos)
		{
			end = src.find_first_of(' ', begin);
			string id = src.substr(begin, end - begin);
			while (id.length())
			{
				ret.push_back( RpHelperID((id.substr(0, 1) == "1"), id.substr(1)) );
				begin = src.find_first_not_of(' ', end);
				if (begin == string::npos) break;
				end = src.find_first_of(' ', begin);
				id = src.substr(begin, end - begin);
			}
		}

		return ret;
	}

	string ListToString(const RpHelperIDs &ids)
	{
		string ret;
		for (RpHelperIDs::const_iterator i = ids.begin(); i != ids.end(); i++)
		{
			if (ret.length()) ret += " ";
			ret += i->first ? "1" : "0";
			ret += i->second;
		}
		return ret;
	}

	// removes the ID of peer_toRemove from fco's registry
	// to clear peer_toRemove's registry too, call it again with arguments reversed and the other role name
	void RpHelperRemoveFromRegistry(const IMgaFCOPtr &fco, const IMgaFCOPtr &peer_toRemove, const string &role_name)
	{
		if (fco->Status != OBJECT_EXISTS)
			return;
		SmartBSTR co_ids = fco->RegistryValue[role_name.c_str()]; // FIXME: need to handle E_NOTFOUND or E_NAME_NOT_FOUND?
		if (!(!co_ids))
		{
			RpHelperIDs ids = StringToList(string(co_ids));
			for (RpHelperIDs::iterator i = ids.begin(); i != ids.end(); )
			{
				if (i->second.compare(peer_toRemove->GetID()) == 0)
					i = ids.erase(i);
				else
					i++;
			}
			if (ids.size())
				fco->PutRegistryValue(SmartBSTR(role_name.c_str()), SmartBSTR(ListToString(ids).c_str()));
			else
				fco->PutRegistryValue(SmartBSTR(role_name.c_str()), SmartBSTR());
		}
	}

	void RpHelperAddToRegistry(const IMgaFCOPtr &fco, const IMgaFCOPtr &peer_toAdd, const string &role_name, bool role_isNavigable)
	{
		SmartBSTR co_ids = fco->RegistryValue[role_name.c_str()];
		RpHelperIDs ids;
		if (!(!co_ids))
			ids = StringToList(string(co_ids));

		bool already_exists = false;
		for (RpHelperIDs::const_iterator i = ids.begin(); i != ids.end(); i++)
		{
			if (i->second.compare(peer_toAdd->GetID()) == 0)
			{
				already_exists = true;
				break;
			}
		}
		if (!already_exists)
		{
			ids.push_back( RpHelperID(role_isNavigable, string(peer_toAdd->GetID())) );
			fco->PutRegistryValue(SmartBSTR(role_name.c_str()), SmartBSTR(ListToString(ids).c_str()));
		}
	}

	IMgaFCOsPtr RpHelperFindPeerFCOsFromRegistry(const IMgaFCOPtr &self, const string &role_name, bool only_ifNavigable, const GmeDataNetwork *dn)
	{
		IMgaFCOsPtr ret;
		ret.CreateInstance(__uuidof(MgaFCOs));

		SmartBSTR co_ids = self->RegistryValue[role_name.c_str()];
		if (!(!co_ids))
		{
			RpHelperIDs ids = StringToList(string(co_ids));
			for (RpHelperIDs::const_iterator i = ids.begin(); i != ids.end(); i++)
			{
				if (!only_ifNavigable || i->first)
				{
					//extra check if the obtained value is an ID
					string co_id_str = i->second;
					if ((co_id_str.length() > 3) && ( co_id_str.substr(0, 3).compare("id-") == 0))
					{
						IMgaFCOPtr fco = dn->priv.project->GetFCOByID(co_id_str.c_str());
						COMTHROW(ret->Append(fco));
					}
				}
			}
		};

		return ret;
	}

	void RpHelperFindPeerFCOsFromModel(const IMgaFCOPtr &self,
		const string &role_name, bool role_isNavigable,
		const string &other_role_name, bool other_role_isNavigable,
		const GmeDataNetwork *dn, bool only_ifNavigable, IMgaFCOsPtr& ret, IMgaReferencePtr& reference)
	{
		if (self->GetObjType() == OBJTYPE_CONNECTION)
		{
			// we are the connection, find the reference (the first reference from
			// the chain of references linking the connection to the source or
			// destination
			IMgaSimpleConnectionPtr conn(self);
			IMgaFCOsPtr references;

			SmartBSTR src_role_name = conn->RegistryValue[L"sRefParent"];
			HRESULT (__stdcall IMgaSimpleConnection::*fn)(IMgaFCOs**) = NULL;

			if (!!src_role_name && role_name.compare(src_role_name) == 0) {
				fn = &IMgaSimpleConnection::get_SrcReferences;
			}
			if (!fn) {
				SmartBSTR dst_role_name = conn->RegistryValue[L"dRefParent"];
				if (!!dst_role_name && role_name.compare(dst_role_name) == 0)
				{
					fn = &IMgaSimpleConnection::get_DstReferences;
				}
			}
			if (!fn) {
				return;
			}
			{
				struct IMgaFCOs * _result = 0;
				IMgaSimpleConnection& iconn = *(static_cast<IMgaSimpleConnection*>(conn));
				HRESULT _hr = (iconn.*fn)(&_result);
				if (_hr != E_MGA_NAME_NOT_FOUND)
				{
					if (FAILED(_hr))
						_com_issue_errorex(_hr, conn, __uuidof(conn));
					references = IMgaFCOsPtr(_result, false);
				}
			}

			if (references != NULL && references->GetCount() > 0)
			{
				IMgaFCOPtr fco = references->GetItem(1);

				if (!only_ifNavigable || (only_ifNavigable && role_isNavigable))
					if (reference == NULL)
						COMTHROW(ret->Append(fco));
					else if (reference->GetIsEqual(fco))
						COMTHROW(ret->Append(self));
			}
		}
		else if (self->GetObjType() == OBJTYPE_REFERENCE)
		{
			// we are the reference, find the connection (the first connection to
			// this reference for which RpHelperFindPeerFCOs()
			// returns the same reference)
			IMgaReferencePtr reference(self);
			IMgaConnPointsPtr cp = reference->GetUsedByConns();
			MGACOLL_ITERATE(IMgaConnPoint, cp)
			{
				IMgaSimpleConnectionPtr conn = MGACOLL_ITER->Owner;
				RpHelperFindPeerFCOsFromModel(conn, other_role_name, other_role_isNavigable, role_name, role_isNavigable, dn, true, ret, reference);
			}
			MGACOLL_ITERATE_END;
		}
	}

	IMgaFCOsPtr RpHelperFindPeerFCOsFromModel(const IMgaFCOPtr &self,
		const string &role_name, bool role_isNavigable,
		const string &other_role_name, bool other_role_isNavigable,
		const GmeDataNetwork *dn, bool only_ifNavigable)
	{
		IMgaFCOsPtr ret;
		ret.CreateInstance(__uuidof(MgaFCOs));

		RpHelperFindPeerFCOsFromModel(self, role_name, role_isNavigable, other_role_name, other_role_isNavigable, dn, only_ifNavigable, ret, IMgaReferencePtr());

		return ret;
	}


#if 0
	// Before connecting self to peer, check that the result would be valid.
	// The result is invalid if self and/or peer is already connected to a different fco.
	bool RpHelperNotValidConn(const IMgaFCOPtr &self, const IMgaFCOPtr &peer, const string &role_name, const string &other_role_name)
	{

		if (self->GetObjType() == OBJTYPE_CONNECTION)
		{
			IMgaSimpleConnectionPtr conn(self);
			SmartBSTR src_role_name = conn->RegistryValue["sRefParent"];

			try
			{
				IMgaFCOsPtr references = role_name.compare(src_role_name) == 0 ? conn->GetSrcReferences() : conn->GetDstReferences();
				if (references->GetCount() > 0)
				{
					IMgaFCOPtr fco = references->GetItem(1);
					if (fco->GetIsEqual(peer) != VARIANT_TRUE)
						return true;
				}
			}
			catch (udm_exception &)
			{
			}
		}
		else if (self->GetObjType() == OBJTYPE_REFERENCE)
		{
			IMgaSimpleConnectionPtr conn(peer);
			SmartBSTR src_role_name = conn->RegistryValue["sRefParent"];

			try
			{
				IMgaFCOsPtr references = other_role_name.compare(src_role_name) == 0 ? conn->GetSrcReferences() : conn->GetDstReferences();
				if (references->GetCount() > 0)
				{
					IMgaFCOPtr fco = references->GetItem(1);
					if (fco->GetIsEqual(self) != VARIANT_TRUE)
						return true;
				}
			}
			catch (udm_exception &)
			{
			}
		}

		return false;
	}
#endif

	IMgaFCOPtr getPrefferedSrcRef(const IMgaSimpleConnectionPtr& conn, const GmeDataNetwork * dn, SmartBSTR& regName=SmartBSTR())
	{
		SmartBSTR regrolename = conn->RegistryValue["sRefParent"];
		IMgaFCOPtr ret;
		if (!(!regrolename))
		{
			regName = regrolename;
			IMgaFCOsPtr references = RpHelperFindPeerFCOsFromRegistry(conn, string(regrolename), true, dn);
			if (references->GetCount() == 1)
					ret = references->GetItem(1);
		};
		return ret;
	};

	IMgaFCOPtr getPrefferedDstRef(const IMgaSimpleConnectionPtr& conn, const GmeDataNetwork * dn, SmartBSTR& regName=SmartBSTR())
	{
		SmartBSTR regrolename = conn->RegistryValue["dRefParent"];
		IMgaFCOPtr ret;
		if (!(!regrolename))
		{
			regName = regrolename;
			IMgaFCOsPtr references = RpHelperFindPeerFCOsFromRegistry(conn, string(regrolename), true, dn);
			if (references->GetCount() == 1)
					ret = references->GetItem(1);
		};
		return ret;

	};

	IMgaFCOsPtr FindReferencesToFCOInParent(const IMgaModelPtr& connection_par,const IMgaFCOPtr& peer,const IMgaFCOPtr& preffered_ref = NULL) 
	{

		IMgaModelPtr peer_parent;

		peer->get_ParentModel(&peer_parent);//only models have ports, so this is also safe.

		if ((connection_par != NULL) && (peer_parent != NULL))
		{
			IMgaFCOsPtr cp_children;
			connection_par->get_ChildFCOs(&cp_children);

			IMgaFCOsPtr references;
				
			bool found = false;			
			MGACOLL_ITERATE(IMgaFCO, cp_children)
			{

				IMgaFCOPtr cp_child = MGACOLL_ITER;
				if (cp_child->GetObjType() != OBJTYPE_REFERENCE) continue;
				if ((preffered_ref != NULL) && (preffered_ref->GetIsEqual(cp_child) != VARIANT_TRUE)) continue;
							
				IMgaReferencePtr cp_ref_child(cp_child);
							
				IMgaFCOPtr referred = cp_ref_child->Referred;//immediate referred
				if (referred==NULL) continue;//null reference

				IMgaFCOPtr final_referred(referred);//final referred
			
				references.CreateInstance(__uuidof(MgaFCOs));
							
				COMTHROW(references->Append(cp_ref_child));

				while((referred!=NULL) && referred->GetObjType() == OBJTYPE_REFERENCE) 
				{
					//loop through references reffering references...
								
					IMgaReferencePtr new_reference(referred);
					referred = new_reference->Referred;
					if (referred) 
					{
						final_referred = referred;
						COMTHROW(references->Append(new_reference));
					}
				}

								
				if (final_referred->GetIsEqual(peer_parent) == VARIANT_TRUE)
				{	
					found = true;
					break;//ok, don't scan the other objects, we accomplished our objectives.
				}	
				
				references.Release();


			}
			MGACOLL_ITERATE_END

			if (found)
				return references;
		}



		string debug;

		debug.append("Parent model of the connection: ");
		debug.append( (char *)connection_par->GetAbsPath());
		debug.append(", name of peer: ");
		debug.append((char*) peer->GetAbsPath());
		


		throw udm_exception(string("Could not reach connecting port FCO through references to its parent, starting from the connection object. Debug info:") + debug);
		return NULL;
	}


	IMgaFCOsPtr CompleteReferencesChainToFCO(const IMgaSimpleConnectionPtr& connection, const IMgaFCOPtr& peer, const IMgaFCOsPtr& start_refs) 
	{

		IMgaModelPtr connection_parent, peer_parent;
		connection->get_ParentModel(&connection_parent);
		peer->get_ParentModel(&peer_parent);

		if (connection_parent != NULL && peer_parent != NULL)
		{
			IMgaFCOsPtr references;
			references.CreateInstance(__uuidof(MgaFCOs));

			MGACOLL_ITERATE(IMgaFCO, start_refs)
			{
				COMTHROW(references->Append(MGACOLL_ITER));
			}
			MGACOLL_ITERATE_END;

			IMgaReferencePtr ref(start_refs->GetItem(start_refs->Count));
							
			IMgaFCOPtr referred = ref->Referred;//immediate referred

			IMgaFCOPtr final_referred(referred);//final referred

			while (referred != NULL && referred->GetObjType() == OBJTYPE_REFERENCE) 
			{
				//loop through references reffering references...
				IMgaReferencePtr new_reference(referred);
				referred = new_reference->Referred;
				if (referred)
				{
					final_referred = referred;
					COMTHROW(references->Append(new_reference));
				}
			}

			if (final_referred != NULL && final_referred->GetIsEqual(peer_parent) == VARIANT_TRUE)
				return references;

			references.Release();
		}

		string debug;

		debug.append("Parent model of the connection: ");
		debug.append( (char *)connection_parent->GetAbsPath());
		debug.append(", name of peer: ");
		debug.append((char*) peer->GetAbsPath());
		


		throw udm_exception(string("Could not reach connecting port FCO through references to its parent, starting from the given reference chain. Debug info:") + debug);
		return NULL;
	}


	IMgaFCOsPtr GmeObject::FindReferencesToFCO(const IMgaFCOPtr& peer, const IMgaFCOPtr& preffered_ref ) const
	{
		if (self->GetObjType() != OBJTYPE_CONNECTION)
			throw udm_exception("Method valid only for connections!");

		IMgaModelPtr self_parent;
		self->get_ParentModel(&self_parent);
		return FindReferencesToFCOInParent(self_parent, peer, preffered_ref);
	}
	
	
	void GmeObject::setAssociation(const ::Uml::AssociationRole &meta, const vector<ObjectImpl*> &nvect, int mode, const bool direct)	
	{
		((GmeDataNetwork*)mydn)->CountWriteOps();

		::Uml::Association assoc = meta.parent();
		string rname = meta.name();

//		cout << "MGA BACKEND DEBUG: Self in setAssociation() is:" << (char *) self->GetName() << endl;



		assocmapitem *nn = ((UdmGme::GmeDataNetwork*) mydn)->amap.find(assoc.uniqueId())->second;

		bool reverse = (nn->primary == meta);

		if(nn->ot == OBJTYPE_MODEL) throw udm_exception("Setting Archetype/Derived relations is not supported");


		vector<ObjectImpl*> kvect = nvect;						 // things to add
		vector<ObjectImpl*> pvect = getAssociation(meta, mode);  // things to delete

		//cout << "MGA BACKEND DEBUG: size of kvect  is: " << kvect.size() << endl;
		//cout << "MGA BACKEND DEBUG: size of pvect  is: " << pvect.size() << endl;

		vector<ObjectImpl*>::iterator i;
		for(i = kvect.begin(); i != kvect.end(); ) 
		{
			//cout <<"MGA BACKEND DEBUG: Object present in original kvect: " <<  (char *)(static_cast<GmeObject *>(*i)->self->GetName()) << ", id:" << (char*)(static_cast<GmeObject *>(*i)->self->GetID()) <<  endl;
			for(vector<ObjectImpl*>::iterator j = pvect.begin(); j != pvect.end(); j++) 
			{
				
				/*
				Argh! COM pointers to the same object ain't the same!
				if(static_cast<GmeObject *>(*i)->self == static_cast<GmeObject *>(*j)->self) 
				*/
				// FIXME: this will crash if i or j are from the wrong DataNetwork and are not GmeObjects
				IMgaFCOPtr n21 = static_cast<GmeObject *>(*i)->self;
				IMgaFCOPtr n22 = static_cast<GmeObject *>(*j)->self;
				VARIANT_BOOL n = n21->GetIsEqual(n22);
				
				if (n == VARIANT_TRUE)
				{
					(*j)->release();	//pvect is obtained by getAssociation()!!
					pvect.erase(j);
					i = kvect.erase(i);
					goto bbreak;
				}
			}
			i++;
bbreak:			;
		}

		//cout << "MGA BACKEND DEBUG: size of kvect  NOW is: " << kvect.size() << endl;
		//cout << "MGA BACKEND DEBUG: size of pvect  NOW is: " << pvect.size() << endl;

		


		
		if(nn->ot == OBJTYPE_CONNECTION) 
		{
			// if reverse, get src 
			if(mode == Udm::TARGETFROMCLASS) 
			{
				if(nvect.size() != 1) throw udm_exception("Connection must have exactly one src/dst");
				IMgaFCOPtr peer = static_cast<GmeObject *>(*nvect.begin())->self;
				if(reverse) 
				{
					SmartBSTR regName;
					IMgaFCOPtr pref_ref = getPrefferedSrcRef(IMgaSimpleConnectionPtr(self), (GmeDataNetwork*)mydn, regName);
					if (pref_ref)
					{
						IMgaFCOsPtr references = FindReferencesToFCO(peer, pref_ref);
						COMTHROW(IMgaSimpleConnectionPtr(self)->SetSrc(references, peer));
						self->RegistryNode[regName]->Clear();
					}
					else
					{
						try
						{
							IMgaSimpleConnectionPtr(self)->SetSrc(NULL, peer);
						}
						catch(udm_exception&) 
						{
							// Warning: an arbitrary reference is chosen here
							IMgaFCOsPtr references = FindReferencesToFCO(peer);
							COMTHROW(IMgaSimpleConnectionPtr(self)->SetSrc(references, peer));
						}
						
					}

				}//if(reverse) 
				else 
				{
					SmartBSTR regName;
					IMgaFCOPtr pref_ref = getPrefferedDstRef(IMgaSimpleConnectionPtr(self), (GmeDataNetwork*)mydn, regName);
					if (pref_ref)
					{
						IMgaFCOsPtr references = FindReferencesToFCO(peer, pref_ref);
						COMTHROW(IMgaSimpleConnectionPtr(self)->SetDst(references, peer));
						self->RegistryNode[regName]->Clear();
					}
					else
					{
						try 
						{

							COMTHROW(IMgaSimpleConnectionPtr(self)->SetDst(NULL, peer));
						}
						catch(udm_exception&) 
						{
							// Warning: an arbitrary reference is chosen here
							IMgaFCOsPtr references = FindReferencesToFCO(peer);
							COMTHROW(IMgaSimpleConnectionPtr(self)->SetDst(references, peer));
						}
					}
				}//else if(reverse) 
			}//	if(mode == Udm::TARGETFROMCLASS) 
			else if(mode == Udm::CLASSFROMTARGET) 
			{
/*
				IMgaConnPointsPtr conns = self->PartOfConns;
				MGACOLL_ITERATE(IMgaConnPoint, conns) {
					IMgaSimpleConnectionPtr conn = MGACOLL_ITER->Owner;
					if(MGACOLL_ITER->ConnRole == SmartBSTR(reverse ? "dst" : "src") &&
						conn->Meta == nn->metaobj) {
						conn->DestroyObject();
					}									
				}
				MGACOLL_ITERATE_END;
*/
				for(i = pvect.begin(); i != pvect.end(); i++) 
				{
					IMgaSimpleConnectionPtr conn = static_cast<GmeObject *>(*i)->self;
					
					//we need to check if there is an associated helper connection
					//GmeObject go(conn, mydn);
					//go.RemoveHelperConnections();
					// FIXME: remove refport_parent regnode?
					if (reverse)
					{
						conn->SetDst(NULL, NULL);
					}
					else
					{
						conn->SetSrc(NULL, NULL);
					}
				}//for i pvect
				for(i = kvect.begin(); i != kvect.end(); i++) 
				{
					IMgaFCOPtr new_peer = static_cast<GmeObject *>(*i)->self;
					
					IMgaSimpleConnectionPtr conn = static_cast<GmeObject *>(*i)->self;
					if(reverse)
					{
						SmartBSTR regName;
						IMgaFCOPtr pref_ref = getPrefferedDstRef(conn, (GmeDataNetwork*)mydn, regName);
						if (pref_ref)
						{
							IMgaFCOsPtr references = static_cast<GmeObject *>(*i)->FindReferencesToFCO(self, pref_ref);
							COMTHROW(conn->SetDst(references, self));
							self->RegistryNode[regName]->Clear();
						}
						else
						{
							try
							{
								conn->SetDst(NULL, self);
							}
							catch (udm_exception&)
							{
								// Warning: an arbitrary reference is chosen here
								IMgaFCOsPtr references = static_cast<GmeObject *>(*i)->FindReferencesToFCO(self);
								conn->SetDst(references, self);
							}
						}
					}//if(reverse)
					else 
					{
						SmartBSTR regName;
						IMgaFCOPtr pref_ref = getPrefferedSrcRef(conn, (GmeDataNetwork*)mydn, regName);
						if (pref_ref)
						{
							IMgaFCOsPtr references = static_cast<GmeObject *>(*i)->FindReferencesToFCO(self, pref_ref);
							COMTHROW(conn->SetSrc(references, self));
							self->RegistryNode[regName]->Clear();
						}
						else
						{
							try
							{
								conn->SetSrc(NULL, self);
							}
							catch(udm_exception&)
							{
							//	cout <<"\t\t SetSrc() failed. " << endl;
							//	cout <<"\t\t Source: " << (char *)(self->GetName())<<endl;
							//	cout <<"\t\t connection: " << (char*)(conn->GetName()) << endl;

								// Warning: an arbitrary reference is chosen here
								IMgaFCOsPtr references = static_cast<GmeObject *>(*i)->FindReferencesToFCO(self);
								conn->SetSrc(references, self);

							}
						}
					}//else if(reverse)
				}//for i kvect
			}//if(mode == Udm::CLASSFROMTARGET) 
			else 
			{
				ASSERT(mode == Udm::TARGETFROMPEER);
				if (nn->rp_helper )
				{
					

					if (kvect.size() > 1 && self->GetObjType() != OBJTYPE_REFERENCE)
						throw udm_exception("In case of np_helper connection there should be maximum one connecting object!");

					::Uml::AssociationRole orole = ::Uml::theOther(meta);
					string oname = orole.name();
					bool oIsNavigable = orole.isNavigable();

					bool isNavigable = meta.isNavigable();

					if (kvect.size())
					{
						for (vector<ObjectImpl*>::const_iterator i = kvect.begin(); i != kvect.end(); i++)
						{
							IMgaFCOPtr connecting_object = static_cast<GmeObject *>(*i)->self;
#if 0
							if (RpHelperNotValidConn(self, connecting_object, rname, oname))
								throw udm_exception("Connection to reference port already exists, delete it first before changing the reference!");
#endif
							if (self->ObjType == OBJTYPE_CONNECTION)
							{
								RpHelperAddToRegistry(self, connecting_object, rname, isNavigable);
							}
							else
							{
								RpHelperAddToRegistry(connecting_object, self, oname, oIsNavigable);
							}
						}
					}
					if (pvect.size())
					{
						for (vector<ObjectImpl*>::const_iterator i = pvect.begin(); i != pvect.end(); i++)
						{
                            IMgaFCOPtr connecting_object = static_cast<GmeObject *>(*i)->self;
							if (self->ObjType == OBJTYPE_CONNECTION)
							{
								RpHelperRemoveFromRegistry(self, connecting_object, rname);
							}
							else
							{
								RpHelperRemoveFromRegistry(connecting_object, self, oname);
							}
						}
					};
				}//if (nn->rp_helper)
				else
				{
					IMgaConnPointsPtr conns = self->PartOfConns;
					
					MGACOLL_ITERATE(IMgaConnPoint, conns) 
					{
						IMgaSimpleConnectionPtr conn = MGACOLL_ITER->Owner;
						if(conn->Meta == nn->metaobj) 
						{
							IMgaFCOPtr peer = reverse ? conn->src : conn->dst;
							for(i = pvect.begin(); i != pvect.end(); i++) 
							{
								if(peer == static_cast<GmeObject *>(*i)->self) 
								{
									conn->DestroyObject();
									break;
								}
							}		
						}									
					}
					MGACOLL_ITERATE_END;

					IMgaModelPtr parent = self->ParentModel, gparent;
					IMgaMetaRolesPtr roles, groles;
					if(parent) 
					{
						roles = IMgaMetaModelPtr(parent->Meta)->LegalRoles(nn->metaobj);

						gparent = parent->ParentModel;

						if(gparent) 
						{
							groles = IMgaMetaModelPtr(gparent->Meta)->LegalRoles(nn->metaobj);
						}
					}

					for(i = kvect.begin(); i != kvect.end(); i++) 
					{
						IMgaFCOPtr o;
						IMgaFCOPtr oself = static_cast<GmeObject *>(*i)->self;
						HRESULT hs = S_OK+1;
						IMgaModelPtr op = oself->ParentModel, ogp;
						if(op) ogp = op->ParentModel;
						if(parent != NULL && ((parent->GetIsEqual(op)==VARIANT_TRUE) || (parent->GetIsEqual(ogp)==VARIANT_TRUE)) && roles->Count) 
						{
							try
							{
								COMTHROW(parent->raw_CreateSimpleConn(roles->Item[1], reverse ? oself : self, reverse ? self : oself,NULL,NULL, &o));
								hs = S_OK;
							}
							catch(udm_exception&)
							{
								//this is getting complicated,
								//because both connecting end may be through references
								IMgaFCOsPtr src_references, dst_references;
								try
								{
									src_references = FindReferencesToFCOInParent(parent,reverse ? oself : self);
								}
								catch(udm_exception&)
								{
									src_references = NULL;
								}

								try
								{
									dst_references = FindReferencesToFCOInParent(parent,reverse ? self : oself);
								}
								catch(udm_exception&)
								{
									dst_references = NULL;
								}

								hs = parent->raw_CreateSimpleConn(roles->Item[1], reverse ? oself : self, reverse ? self : oself,src_references,dst_references, &o);
							}

						}
						if(hs != S_OK && gparent != NULL && ((parent->GetIsEqual(op)==VARIANT_TRUE) || (parent->GetIsEqual(ogp)==VARIANT_TRUE)) && groles->Count) 
						{
							try
							{
								COMTHROW(gparent->raw_CreateSimpleConn(groles->Item[1], reverse ? oself : self, reverse ? self : oself,NULL,NULL, &o));
								hs = S_OK;
							}
							catch(udm_exception&)
							{
								IMgaFCOsPtr src_references, dst_references;
								try
								{
									src_references = FindReferencesToFCOInParent(gparent,reverse ? oself : self);
								}
								catch(udm_exception&)
								{
									src_references = NULL;
								}

								try
								{
									dst_references = FindReferencesToFCOInParent(gparent,reverse ? self : oself);
								}
								catch(udm_exception&)
								{
									dst_references = NULL;
								}

								hs = gparent->raw_CreateSimpleConn(groles->Item[1], reverse ? oself : self, reverse ? self : oself,src_references,dst_references, &o);
							}
						}

						if(hs != S_OK) throw udm_exception("Cannot create conn association " + getnameforassoc(assoc, true));
					}//for i kvect
				}//else if (nn->rp_helper)
			}//mode  TARGETFROMPEER		
		}//if(nn->ot == OBJTYPE_CONNECTION) 
		else 
		{
		  if(!reverse) 
		  {
			switch(nn->ot) 
			{
			case OBJTYPE_REFERENCE:
							if(nvect.size() > 1) throw udm_exception("Attempt to set multiple values for a reference");
							{
								IMgaFCOPtr tgt; 
								if(nvect.size())  tgt = static_cast<GmeObject *>(*nvect.begin())->self;
								IMgaReferencePtr(self)->Referred = tgt;
							}
							break;
			case OBJTYPE_SET:
							{
								vector<ObjectImpl*>::iterator i;
								for(i = pvect.begin(); i != pvect.end(); i++) {
									IMgaSetPtr(self)->RemoveMember(static_cast<GmeObject *>(*i)->self);										
								}									
								for(i = kvect.begin(); i != kvect.end(); i++) {
									IMgaSetPtr(self)->AddMember(static_cast<GmeObject *>(*i)->self);										
								}									
							}
							break;
		/*	case OBJTYPE_MODEL:
							{
								vector<ObjectImpl*>::iterator i;
								if ( ( pvect.size() > 1) || (nvect.size() > 1))
									throw udm_exception("More than one archetype requested to add/remove!");

								for(i = pvect.begin(); i != pvect.end(); i++) 
								{
									IMgaFCOPtr(self)->DetachFromArcheType();
								}									
								for(i = kvect.begin(); i != kvect.end(); i++) 
								{
									IMgaFCOPtr(self)->AttachToArcheType(static_cast<GmeObject *>(*i)->self, true);										
								}
							}
							break;
		*/	}
		  }
		  else {
			switch(nn->ot) {
			case OBJTYPE_REFERENCE:
							{
								vector<ObjectImpl*>::iterator i;
								for(i = pvect.begin(); i != pvect.end(); i++) {
									IMgaReferencePtr(static_cast<GmeObject *>(*i)->self)->Referred = NULL;										
								}									
								for(i = kvect.begin(); i != kvect.end(); i++) {
									objtype_enum oot = static_cast<GmeObject *>(*i)->self->ObjType;
									objtype_enum oot2 = self->ObjType;
									IMgaReferencePtr(static_cast<GmeObject *>(*i)->self)->Referred = self;										
								}									
							}
							break;
			case OBJTYPE_SET:
							{
								vector<ObjectImpl*>::iterator i;
								for(i = pvect.begin(); i != pvect.end(); i++) {
									IMgaSetPtr(static_cast<GmeObject *>(*i)->self)->RemoveMember(self);										
								}									
								for(i = kvect.begin(); i != kvect.end(); i++) {
									IMgaSetPtr(static_cast<GmeObject *>(*i)->self)->AddMember(self);										
								}									
							}
							break;
			}
		  }
		}  
		
		//free the ObjectImpls in pvect
		vector<ObjectImpl*>::iterator ii;
		for(ii = pvect.begin(); ii != pvect.end(); ii++) 
			(*ii)->release();
							
	};

	// Given this association role, find the assocmapitem for the corresponding rp_helper association, if any
	assocmapitem *getRPAssocMapItem(const ::Uml::AssociationRole &meta, const GmeDataNetwork *dn)
	{
		::Uml::Association assoc = meta.parent();
		assocmapitem *nn = dn->amap.find(assoc.uniqueId())->second;

		bool reverse = (nn->primary != meta);

		::Uml::AssociationRole rp_helper_role = reverse ? nn->dst_rp_helper : nn->src_rp_helper;
		if (!rp_helper_role)
			return NULL;

		assocmapitem *nn_rp = dn->amap.find( ((::Uml::Association) rp_helper_role.parent()).uniqueId() )->second;
		return nn_rp;
	}

	IMgaFCOsPtr GmeObject::FindReferencesChain(const IMgaFCOPtr& peer, bool reverse, const IMgaFCOsPtr& refs ) const
	{
		IMgaFCOsPtr ret;

		if (refs->Count)
			ret = CompleteReferencesChainToFCO(IMgaSimpleConnectionPtr(self), peer, refs);
		else
		{
			IMgaFCOPtr pref_ref;
			if (reverse)
				pref_ref = getPrefferedDstRef(IMgaSimpleConnectionPtr(self), (GmeDataNetwork*)mydn);
			else
				pref_ref = getPrefferedSrcRef(IMgaSimpleConnectionPtr(self), (GmeDataNetwork*)mydn);

			if (pref_ref)
				ret = FindReferencesToFCO(peer, pref_ref);
			else
				ret = FindReferencesToFCO(peer);
		}

		return ret;
	}

	void GmeObject::connectTo(const ::Uml::AssociationRole &meta, ObjectImpl* target, const vector<ObjectImpl*> &refs)
	{
		::Uml::Association assoc = meta.parent();
		string rname = meta.name();

		assocmapitem *nn = ((UdmGme::GmeDataNetwork*) mydn)->amap.find(assoc.uniqueId())->second;
		if (nn->ot != OBJTYPE_CONNECTION)
			throw udm_exception("Only connection types are supported");

		bool reverse = (nn->primary != meta);

		((GmeDataNetwork*)mydn)->CountWriteOps();

		GmeObject *conn_go, *target_go;
		if (self->GetObjType() == OBJTYPE_CONNECTION)
		{
			conn_go = this;
			target_go = (GmeObject *) static_cast<const GmeObject *>(target);
		}
		else if (self->GetObjType() == OBJTYPE_ATOM)
		{
			throw udm_exception("Only setting the connection src/dst is supported");
			conn_go = (GmeObject *) static_cast<const GmeObject *>(target);
			target_go = this;
		}

		IMgaSimpleConnectionPtr conn(conn_go->self);

		IMgaFCOsPtr pref_refs;
		pref_refs.CreateInstance(__uuidof(MgaFCOs));
		for (vector<Udm::ObjectImpl*>::const_iterator i = refs.begin(); i != refs.end(); i++)
			COMTHROW(pref_refs->Append(static_cast<GmeObject *>(*i)->self));

		IMgaFCOsPtr references = conn_go->FindReferencesChain(target_go->self, reverse, pref_refs);

		if (reverse)
			COMTHROW(conn->SetDst(references, target_go->self));
		else
			COMTHROW(conn->SetSrc(references, target_go->self));
	}

	void GmeObject::disconnectFrom(const ::Uml::AssociationRole &meta, ObjectImpl* peer)
	{
		::Uml::Association assoc = meta.parent();
		assocmapitem *nn = ((UdmGme::GmeDataNetwork*) mydn)->amap.find(assoc.uniqueId())->second;

		if (nn->ot != OBJTYPE_CONNECTION)
			throw udm_exception("Only connection types are supported");

		bool reverse = (nn->primary != meta);

		((GmeDataNetwork*)mydn)->CountWriteOps();

		// TODO: check if peer is really connected to us?

		IMgaSimpleConnectionPtr conn;
		if (self->GetObjType() == OBJTYPE_CONNECTION)
			conn = self;
		else if (self->GetObjType() == OBJTYPE_ATOM)
			conn = static_cast<const GmeObject *>(peer)->self;

		if (conn != NULL)
			if (reverse)
				conn->SetDst(NULL, NULL);
			else
				conn->SetSrc(NULL, NULL);
	}

	// chain of references to source or destination of the given connection
	IMgaFCOsPtr RefsChain(const IMgaSimpleConnectionPtr &conn, const string &role_name)
	{
		IMgaFCOsPtr ret;
		ret.CreateInstance(__uuidof(MgaFCOs));

		IMgaFCOsPtr references;

		SmartBSTR src_role_name = conn->RegistryValue[L"sRefParent"];
		if (!src_role_name) {
			SmartBSTR dst_role_name = conn->RegistryValue[L"dRefParent"];
			try {
				references = role_name.compare(dst_role_name) == 0 ? conn->GetDstReferences() : conn->GetSrcReferences();
			} catch (udm_exception&) {}
		} else {
			try {
				references = role_name.compare(src_role_name) == 0 ? conn->GetSrcReferences() : conn->GetDstReferences();
			} catch (udm_exception&) {}
		}

		if (references != NULL)
		{
			MGACOLL_ITERATE(IMgaFCO, references)
			{
				COMTHROW(ret->Append(MGACOLL_ITER));
			}
			MGACOLL_ITERATE_END;
		}

		return ret;
	}

	vector<ObjectImpl*> GmeObject::getConnectingChain(const ::Uml::AssociationRole &meta, const ObjectImpl* peer) const
	{
		vector<ObjectImpl*> ret;

		assocmapitem *nn_rp = getRPAssocMapItem(meta, (GmeDataNetwork*) mydn);
		if (!nn_rp)
			return ret;

		string rname = Uml::MakeRoleName(nn_rp->primary);

		if (nn_rp->ot == OBJTYPE_CONNECTION && nn_rp->rp_helper)
		{
			if (self->GetObjType() == OBJTYPE_CONNECTION)
			{
				IMgaFCOsPtr references = RefsChain(self, rname);
				MGACOLL_ITERATE(IMgaFCO, references)
				{
					ret.push_back(new GmeObject(MGACOLL_ITER, mydn));
				}
				MGACOLL_ITERATE_END;
			}
			else if (self->GetObjType() == OBJTYPE_ATOM)
			{
				IMgaFCOsPtr references = RefsChain(static_cast<const GmeObject *>(peer)->self, rname);
				MGACOLL_ITERATE(IMgaFCO, references)
				{
					ret.push_back(new GmeObject(MGACOLL_ITER, mydn));
				}
				MGACOLL_ITERATE_END;

				std::reverse(ret.begin(), ret.end());
			}
		}

		return ret;
	}


	//GmeObject member functions
	vector<ObjectImpl*> GmeObject::getAssociation(const ::Uml::AssociationRole &meta, int mode) const 
	{
		vector<ObjectImpl*> ret;

		::Uml::Association assoc = meta.parent();
		
			string rname = meta.name();

			assocmapitem *nn = ((GmeDataNetwork*)mydn)->amap.find(assoc.uniqueId())->second;

			bool reverse = (nn->primary == meta);
							
			if(nn->ot == OBJTYPE_CONNECTION) 
			{
				if (nn->rp_helper)
				{
					::Uml::AssociationRole orole = ::Uml::theOther(meta);
					IMgaFCOsPtr fcos = RpHelperFindPeerFCOsFromModel(self,
						rname, meta.isNavigable(),
						orole.name(), orole.isNavigable(),
						(GmeDataNetwork *)mydn, true);
					MGACOLL_ITERATE(IMgaFCO, fcos)
					{
						ret.push_back(new GmeObject(MGACOLL_ITER, mydn));
					}
					MGACOLL_ITERATE_END;

					// if nothing in the model, make visible the registry setting
					if (!ret.size()) {
						fcos = RpHelperFindPeerFCOsFromRegistry(self, rname, meta.isNavigable(), (GmeDataNetwork *)mydn);
						MGACOLL_ITERATE(IMgaFCO, fcos)
						{
							ret.push_back(new GmeObject(MGACOLL_ITER, mydn));
						}
						MGACOLL_ITERATE_END;
					}
				}
				else if(mode == Udm::TARGETFROMCLASS) 
				{
					IMgaSimpleConnectionPtr conn(self);
					IMgaFCOPtr peer;
					if(reverse) conn->get_src(&peer);
					else conn->get_dst(&peer);
					if(peer) ret.push_back(new GmeObject( peer, mydn));
				}
				else 
				{
					IMgaConnPointsPtr conns = self->PartOfConns;
					MGACOLL_ITERATE(IMgaConnPoint, conns) 
					{
						IMgaSimpleConnectionPtr conn = MGACOLL_ITER->Owner;
						/*
						BSTR debug_1 = MGACOLL_ITER->ConnRole;

						char * debug_1_ch = (char *)debug_1;
						bool debug_1_b = (MGACOLL_ITER->ConnRole == SmartBSTR(reverse ? "dst" : "src"));
						bool debug_2_b = (conn->Meta->GetMetaRef() == nn->metaobj->GetMetaRef());
						BSTR connection_meta = conn->Meta->GetName();
						BSTR ami_meta = nn->metaobj->GetName();
						*/

						if (nn->metaobj)
						{
							if(MGACOLL_ITER->ConnRole == SmartBSTR(reverse ? "dst" : "src") && 	conn->Meta->GetMetaRef() == nn->metaobj->GetMetaRef()) 
							{
								if(mode == Udm::CLASSFROMTARGET) 
								{
									//ret.push_back(new GmeObject(assoc.assocClass(), conn, mydn));
									ret.push_back(new GmeObject( conn, mydn));
								}
								else if(mode == Udm::TARGETFROMPEER) 
								{
									IMgaFCOPtr peer = reverse ? conn->src : conn->dst;
									ret.push_back(new GmeObject( peer, mydn));
								}
								else throw udm_exception("Invalid access mode");
							};									
						}
						else 
						{
							if (!nn->metaobjs_count) throw udm_exception("Expected >0 metaobjs_count!");
							for  (int i = 0; i < nn->metaobjs_count; i++)
							{
								
								IMgaMetaFCOPtr p = nn->metaobjs[i];
	/*
								BSTR debug_1 = MGACOLL_ITER->ConnRole;
								char * debug_1_ch = (char *)debug_1;
								bool debug_1_b = (MGACOLL_ITER->ConnRole == SmartBSTR(reverse ? "dst" : "src"));
								bool debug_2_b = (conn->Meta->GetMetaRef() == p->GetMetaRef());
								BSTR connection_meta = conn->Meta->GetName();
								BSTR ami_meta = p->GetName();
	*/
								
								if(MGACOLL_ITER->ConnRole == SmartBSTR(reverse ? "dst" : "src") && 	conn->Meta->GetMetaRef() == p->GetMetaRef()) 
								{
									if(mode == Udm::CLASSFROMTARGET) 
									{
										//ret.push_back(new GmeObject(assoc.assocClass(), conn, mydn));
										ret.push_back(new GmeObject( conn, mydn));
									}
									else if(mode == Udm::TARGETFROMPEER) 
									{
										IMgaFCOPtr peer = reverse ? conn->src : conn->dst;
										ret.push_back(new GmeObject( peer, mydn));
									}
									else throw udm_exception("Invalid access mode");
								};									
							};
						};
					}
					MGACOLL_ITERATE_END;
				}
			}
			else 
			{
				if(!reverse)
				{
					switch(nn->ot) 
					{
					case OBJTYPE_REFERENCE:
					{
						IMgaFCOPtr peer = IMgaReferencePtr(self)->Referred;
						if(peer) ret.push_back(new GmeObject( peer, mydn));
					}
					break;
					
					case OBJTYPE_SET:
					{
						IMgaFCOsPtr peers = IMgaSetPtr(self)->Members;
						MGACOLL_ITERATE(IMgaFCO, peers) 
						{
							ret.push_back(new GmeObject( MGACOLL_ITER, mydn));
						}
						MGACOLL_ITERATE_END;
					}
					break;
					case OBJTYPE_MODEL:
					{
						IMgaFCOsPtr peers = self->DerivedObjects;
						MGACOLL_ITERATE(IMgaFCO, peers) 
						{
							ret.push_back(new GmeObject( MGACOLL_ITER, mydn));
						}
						MGACOLL_ITERATE_END;
					}
					break;
					}
				}
				else 
				{
					switch(nn->ot) 
					{
					case OBJTYPE_REFERENCE:
									{
										IMgaFCOsPtr peers = self->ReferencedBy;
										MGACOLL_ITERATE(IMgaFCO, peers) 
										{
											if (!nn->pt_abstract)
											{
												if(MGACOLL_ITER->Meta->GetMetaRef() != nn->metaobj->GetMetaRef()) continue;
												ret.push_back(new GmeObject( MGACOLL_ITER, mydn));
											}
											else
											{
												// return all instances of the descendants that are references to the target
												for (int i = 0; i < nn->metaobjs_count; i++)
												{
													if (MGACOLL_ITER->Meta->GetMetaRef() != nn->metaobjs[i]->GetMetaRef()) continue;
													ret.push_back(new GmeObject( MGACOLL_ITER, mydn ));
												}
											}
										}
										MGACOLL_ITERATE_END;
									}
									break;
					case OBJTYPE_SET:
									{
										IMgaFCOsPtr peers = self->MemberOfSets;
										MGACOLL_ITERATE(IMgaFCO, peers) 
										{
											if(MGACOLL_ITER->Meta->GetMetaRef() != nn->metaobj->GetMetaRef()) continue;
											ret.push_back(new GmeObject( MGACOLL_ITER, mydn));
										}
										MGACOLL_ITERATE_END;
									}
									break;
					case OBJTYPE_MODEL:
									{
										IMgaFCOPtr peer = self->ArcheType;
										if(peer) ret.push_back(new GmeObject( peer, mydn));
									}
									break;
					}
				}
			};  
		
		return ret;
	}


	ObjectImpl * GmeObject::createChild(
		const ::Uml::CompositionChildRole &role,	
		const ::Uml::Class &kind, 
		const Udm::ObjectImpl* archetype, 
		const bool subtype,
		const bool real_archetype,
		const bool need_safetype )		//this is not used, since this call
										//does not recurse when a a derived/instace
										//object is created with an archetype that has children
										//(this is done by the underlaying MGA layer
	{
		((GmeDataNetwork*)mydn)->CountWriteOps();

		IMgaFCOPtr  nfco;
		if(folderself) 
		{
			if(role) 
			{
				::Uml::Composition comp = Uml::matchChildToParent(kind ? kind : role.target(), m_type);
				if(!comp || comp != role.parent()) 
				{
					throw udm_exception("Ambigious role specification for folder parents");
				}
			}
			IMgaMetaFolderPtr mf;
			IMgaMetaFCOPtr mfco;
			HRESULT hr1 = (folderself->MetaFolder->get_LegalRootObjectByName(SmartBSTR(PATHGET(kind)), &mfco));
			HRESULT hr2 = (folderself->MetaFolder->get_LegalChildFolderByName(SmartBSTR(PATHGET(kind)), &mf));
			
			if(mf) 
			{
				if(mfco)
					throw udm_exception("Multiple child kinds match");
				IMgaFolderPtr nf;
				nf = folderself->CreateFolder(mf);
				GmeObject * retval = new GmeObject( nf, mydn);
				retval->setDefaultAttributes(false);
				return retval;
			}
			else if(mfco) 
			{
				IMgaFCOPtr nfco;
				if (!archetype)
				{
					nfco = folderself->CreateRootObject(mfco);
					GmeObject * retval =   new GmeObject( nfco, mydn);
					retval->setDefaultAttributes(false);
					return retval;
				}
				else
				{
					nfco = folderself->DeriveRootObject(((GmeObject *)archetype)->self, subtype ? VARIANT_FALSE : VARIANT_TRUE);
					
					GmeObject * retval =   new GmeObject( nfco, mydn);
					return retval;
				}
			}

			else 
				throw udm_exception("Parent cannot have children of this kind");
		}

		IMgaModelPtr m = self;
		IMgaMetaModelPtr pmeta = Meta();
		if(m == NULL || pmeta == NULL) 
			throw udm_exception("Not a model");
		IMgaMetaRolePtr rr;
		if(role) 
		{
			
			if(kind) 
			{
				if ((*role.target()) != kind)
				{
					if (!static_cast<GmeDataNetwork*>(const_cast<GmeObject*>(this)->__getdn())->priv.IsDerivedFrom(kind, role.target()))
						throw udm_exception("GmeObject::CreateChild(): Role-Kindname mismatch");
				}
				
				rr = GetMetaRole(kind, role, pmeta);

			}
			else rr = GetMetaRoleForChildRole(role, pmeta);
		}

		else 
		{
			if(!kind) 
				throw udm_exception("Neither role nor kind is given!!!");
			IMgaMetaRolesPtr rrs = pmeta->Roles;
			MGACOLL_ITERATE(IMgaMetaRole, rrs) 
			{
				if(CompareKindToClass(MGACOLL_ITER->kind, kind) == 0) 
				{
					if(rr) 
						throw udm_exception("Multiple childroles match, role must be specified");
					rr = MGACOLL_ITER;
				}
			} MGACOLL_ITERATE_END;
			if(rr == NULL) 
				throw udm_exception("Parent of type " + UdmUtil::ExtractName(this->type()) +
					" cannot have children of kind '" + UdmUtil::ExtractName(kind) + "'");
		}	

		if (!archetype)
		{
			nfco = m->CreateChildObject(rr);
			
			GmeObject * retval =  new GmeObject( nfco, mydn);
			retval->setDefaultAttributes(false);
			return retval;
		}
		else
		{
			
			nfco = m->DeriveChildObject( ((GmeObject *)archetype)->self,  rr, subtype ? VARIANT_FALSE : VARIANT_TRUE );
				
			GmeObject * retval =  new GmeObject( nfco, mydn);
			return retval;


		}
		
	};




	void GmeObject::setChildren(const ::Uml::CompositionChildRole &role, const vector<ObjectImpl*> &a, const bool direct) 
	{
		((GmeDataNetwork*)mydn)->CountWriteOps();

		IMgaModelPtr parent = self;
		if(parent == NULL) 
			throw udm_exception("No parent or non-model parent specified");
		
		vector<ObjectImpl*> aa = a;
		IMgaMetaRolePtr rr;

		// FIX: Tihamer Levendovszky - March 2002 
		if(role) rr=GetMetaRoleForChildRole(role,Meta());
		
		IMgaFCOsPtr chds = parent->ChildFCOs;

		MGACOLL_ITERATE(IMgaFCO, chds) 
		{
// keep children with other roles
			if(rr != NULL && rr != MGACOLL_ITER->MetaRole) continue;
// keep children if they are about to be moved here
			vector<ObjectImpl *>::iterator i;
			for(i = aa.begin(); i != aa.end(); i++) 
			{
				if(*i && MGACOLL_ITER == static_cast<GmeObject *const>(*i)->self) 
				{
						(*i) = NULL;
						break;
				}
			}
			if(i != aa.end()) continue;

			COMTHROW(MGACOLL_ITER->DestroyObject());
		} MGACOLL_ITERATE_END;

		int cnt = 0;
		IMgaMetaRolesPtr roles;
		IMgaFCOsPtr fcos;
		fcos.CreateInstance(__uuidof(MgaFCOs));
		roles.CreateInstance("Mga.MgaMetaRoles");
		for(vector<ObjectImpl *>::const_iterator i = aa.begin(); i != aa.end(); i++) 
		{
			if(*i == NULL) continue;
			COMTHROW(fcos->Append(static_cast<GmeObject *const>(*i)->self));
			COMTHROW(roles->Append(rr));
			cnt++;
		};
		
		if(cnt) 
		{
			COMTHROW(parent->MoveFCOs(fcos,roles,NULL));
		}
	};

	bool privdata::IsDerivedFrom(const Uml::Class& derived, const Uml::Class& kind) {
		std::map<Udm::Object::uniqueId_type, std::vector<Udm::Object::uniqueId_type> >::iterator cache = derived_map.find(kind);
		if (kind && cache == derived_map.end())
		{
			std::vector<Udm::Object::uniqueId_type> cacheEntries;

			std::deque<Uml::Class> q;
			q.push_back(kind);
			while (!q.empty())
			{
				Uml::Class kind = q.front();
				q.pop_front();
				cacheEntries.push_back(kind.uniqueId());
				std::set<Uml::Class> baseTypes = kind.subTypes();
				std::copy(baseTypes.begin(), baseTypes.end(), std::back_inserter(q));
			}
			std::sort(cacheEntries.begin(), cacheEntries.end());
			cacheEntries.erase(std::unique(cacheEntries.begin(), cacheEntries.end()), cacheEntries.end());
			derived_map.insert(std::pair<Udm::Object::uniqueId_type, std::vector<Udm::Object::uniqueId_type> >(kind.uniqueId(), cacheEntries));
			cache = derived_map.find(kind.uniqueId());
		}
		bool ret = std::binary_search(cache->second.begin(), cache->second.end(), derived.uniqueId());
		ASSERT((Uml::IsDerivedFrom(derived, kind) == ret));
		return ret;
	}


	vector<ObjectImpl*> GmeObject::getChildren(const ::Uml::CompositionChildRole &role, const ::Uml::Class &kind) const 
	{
		vector<ObjectImpl*> ret;
		IMgaFCOsPtr chds;

		if(folderself) 
		{
			if(role) 
			{
				::Uml::Composition comp = Uml::matchChildToParent(kind ? kind : role.target(), m_type);
				if(!comp || comp != role.parent()) 
				{
					throw udm_exception("Ambigious role specification for folder parents");
				}
			}
			IMgaFoldersPtr fchds = folderself->ChildFolders;
			
			MGACOLL_ITERATE(IMgaFolder, fchds) 
			{
				GmeObject *p = new GmeObject( MGACOLL_ITER, mydn);
				if (!kind || static_cast<GmeDataNetwork*>(const_cast<GmeObject*>(this)->__getdn())->priv.IsDerivedFrom(p->m_type, kind))
					ret.push_back(p);
				else delete p;
			} MGACOLL_ITERATE_END;

			chds = folderself->ChildFCOs;
			MGACOLL_ITERATE(IMgaFCO, chds) 
			{
				GmeObject *p = new GmeObject( MGACOLL_ITER, mydn);
				if (!kind || static_cast<GmeDataNetwork*>(const_cast<GmeObject*>(this)->__getdn())->priv.IsDerivedFrom(p->m_type, kind))
					ret.push_back(p);
				else delete p;
			} MGACOLL_ITERATE_END;

			return ret;
		}

		IMgaModelPtr model = self;
		if(model == NULL)
			return ret;
		//IMgaMetaRolePtr rr;
		
		
		
		chds = model->ChildFCOs;
		MGACOLL_ITERATE(IMgaFCO, chds) 
		{
			/*
				MGA's MetaRole is completly different from UDM's CompositionChildRole:

						    _	CompostionRole if CompositionRole defined, and role.target() has no descendants
						   |	
				MetaRole = |_	kind+CompostionRole if CompositionRole defined and role.target() has descendants	
						   |
						   |_	the kind of the child, when CompostionRole is not defined

				This creates a lot of trouble when objects are contained via their
				inherited composition child roles. 
				 
				So, we create a set of strings, which will contain:

				1. role.name()
				2. all the names of role.target() descendants, including self
				3. for all 'name' in names of role.target()descendant, including self,
					the name + role.name()

				If we found the MGACOLL_ITER->MetaRole in this set of strings then it's OK,
				it should be returned

				the reason why we check against the first option right here and 
				avoid set operation is that this option will be the most common 
				case, when ever the target of the role has no descendant classes
				the name of the role will be the same as the MGA CompositionRole

				the set<string> containing the 2. and 3. options are obtained by
				GmeDataNetwork::getMetaRoleFilter(role), function which maintains
				a map<role, set<string>> and creates the sets only once.
			*/

			if (role)
			{
				//the CompositionRole of the contained FCO
				string MetaRole(static_cast<const char *>(MGACOLL_ITER->MetaRole->Name));
				string roleWithNamespaces = static_cast<std::string>(role.name());
				if (roleWithNamespaces.length() != 0)
				{
					Uml::Namespace ns = static_cast<Uml::Class>(role.target()).parent_ns();
					while (ns)
					{
						roleWithNamespaces = static_cast<std::string>(ns.name()) + "::" + roleWithNamespaces;
						ns = ns.parent_ns();
					}
				}


				if (MetaRole != roleWithNamespaces)//most common case
				{
					//get the possible MetaRoles for containment via this role
					set<string> MetaRoleFilter = ((GmeDataNetwork*)mydn)->GetMetaRoleFilter(role);
					if (MetaRoleFilter.find(MetaRole) == MetaRoleFilter.end()) 
						continue;//this means that MGACOLL_ITER is not contained via 'role' 
					//else is OK!
				}//else is OK
			}
			try 
			{
				GmeObject *p = new GmeObject( MGACOLL_ITER, mydn);
				if (!kind || static_cast<GmeDataNetwork*>(const_cast<GmeObject*>(this)->__getdn())->priv.IsDerivedFrom(p->m_type, kind))
					ret.push_back(p);
				else delete p;
			} catch(udm_exception &) 
			{ 
				if(MGACOLL_ITER->GetObjType() != OBJTYPE_CONNECTION) throw;
			} 
		} MGACOLL_ITERATE_END;
		return ret;
	};

	void GmeObject::detach() 
	{
	//	COMTHROW(("Cannot detach",1));
		setParent(NULL, NULL);
	};

	void GmeObject::setParent(ObjectImpl *a, const ::Uml::CompositionParentRole &role, const bool direct) 
	{
		((GmeDataNetwork*)mydn)->CountWriteOps();

		IMgaModelPtr parent;
		if (!a || a == &Udm::_null)
		{	
			long oStatus;
			COMTHROW(objself->get_Status(&oStatus));
			if (oStatus == OBJECT_EXISTS)
			{
				COMTHROW(objself->DestroyObject());	
			}
				
			
			//if this datanetwork is part of a project,
			//I might have a placeholder object,
			//which also should be deleted
			Udm::UdmProject * pr = NULL;
			if (__getdn()) pr = __getdn()->GetProject();
			if (pr)
			{
				Object po = pr->GetPlaceHolder(clone(), false);
				if (po) po.DeleteObject();
			}

			return;
		}
		parent = static_cast<GmeObject *>(a)->self;
		if(parent == NULL)
			throw udm_exception("No parent or non-model parent specified");

		IMgaMetaRolePtr rr;
		if(role)  
		{
			rr = GetMetaRoleForChildRole(Uml::theOther(role), parent->Meta);
			if(CompareKindToClass(rr->kind, m_type))
				throw udm_exception("Role-Kindname mismatch");
		}
#ifndef DEDUCE_NULLROLES
		else 
		{

			IMgaMetaFCOPtr childmfco = Meta();
			
			// FIX:Tihamer Levendovszky
			IMgaMetaModelPtr pParentMeta=static_cast<GmeObject *>(a)->Meta();

			IMgaMetaRolesPtr mrs;
			COMTHROW(pParentMeta->get_Roles(&mrs));
			// end of FIX  

			MGACOLL_ITERATE(IMgaMetaRole, mrs) 
			{
				if(MGACOLL_ITER->kind == childmfco) 
				{
					rr = MGACOLL_ITER;
					break;
				}
			} 
			MGACOLL_ITERATE_END;
			if(rr == NULL)
				throw udm_exception("Parent cannot have this child");
		}
#endif
		IMgaFCOsPtr fcos;
		IMgaMetaRolesPtr roles;

		fcos.CreateInstance(__uuidof(MgaFCOs));
		roles.CreateInstance("Mga.MgaMetaRoles");
		COMTHROW(fcos->Append(self));
		COMTHROW(roles->Append(rr));
		COMTHROW(parent->MoveFCOs(fcos,roles,NULL));
	};


	ObjectImpl *GmeObject::getParent(const ::Uml::CompositionParentRole &role) const 
	{
		IMgaMetaRolePtr rr;

		if (role)
		{
			
			

			IMgaMetaRolePtr real_role;
			COMTHROW (self->get_MetaRole(&real_role));
			if (real_role == NULL)
			{
				return &Udm::_null;
				//throw udm_exception("Object " + this->getPath() + " has a Folder parent, and thus no composition role");
			}
			::Uml::CompositionChildRole ccr = Uml::theOther(role);
			
			//in most cases, the role name matches
			if (real_role->Name != SmartBSTR(((string)ccr.name()).c_str()))
			{
				//however, if it doesn't it still may be an inerited childrole
				//and in such cases the MGA RoleName is not the same as the name
				//of the CompositionChildRole

				bool found = false;
				set< ::Uml::Class> descendants = Uml::DescendantClasses(ccr.target());
				set< ::Uml::Class>::iterator d_i = descendants.begin();

				while (!found && d_i != descendants.end())
				{
					::Uml::Class descendant = *d_i++;
					found = (real_role->Name == SmartBSTR(PATHGET(descendant)));
				}

				//and it also may be one of inherited multiple chilroles,
				//then the MGA RoleName is a concatenated string
				//kind + role 
				while (!found && d_i != descendants.end())
				{
					::Uml::Class descendant = *d_i++;
					found = (real_role->Name == SmartBSTR( (getShortClassPath(descendant) + (string)ccr.name()).c_str() ));
				}

				//if still not found, than the this is not contained via the requested role
				if (!found)
					return &Udm::_null;		
			}
		}

		
		IMgaObjectPtr po;
		objtype_enum objtype;
		COMTHROW(objself->GetParent(&po, &objtype));
		IMgaModelPtr m(po);

		if(m) 
			return new GmeObject( m, mydn);
		
		IMgaFolderPtr f(po);
		
		if(f) 
			return new GmeObject( f, mydn);

		return &Udm::_null;
	}

	IMgaMetaRolePtr GmeObject::GetMetaRole(::Uml::Class kind, ::Uml::CompositionChildRole ccr, IMgaMetaModelPtr mmodel)
	{
		IMgaMetaRolePtr rr;
		if(mmodel == NULL) 
			throw udm_exception("Metamodel not found");
	
		string mn;
		if (Uml::HasChildRoleMultipleTargets(ccr))
		{
			//metarole = kindname + rolename

			//string shr = Uml::MakeShortRoleName(ccr);
			//string kn = kind.name();

			//auto dbg1 = (string)ccr.name();
			//auto dbg2 = (string)((::Uml::Class)ccr.target()).name();
			//auto dbg3 = PATHGET(kind);

			string target = ccr.target()->name();
			if (Uml::IsCrossNSComposition(ccr.parent()))
			{
				Uml::Namespace ns = static_cast<Uml::Class>(ccr.target()).parent_ns();
				while (ns)
				{
					target = static_cast<std::string>(ns.name()) + "_" + target;
					ns = ns.parent_ns();
				}
			}

			if ( ((string)ccr.name()).compare(target) == 0)
			{
				//the rolename might be created with ccr.target().name()
				//by the CPP code generator. Hence, the MGA rolename may be
				//empty or may be Uml::MakeShortRoleName, no one can tell.
				
				//first we try without rolename appended, this happens more often
				mn = getShortClassPath(kind);
				mmodel->get_RoleByName(SmartBSTR(mn.c_str()), &rr);
				if (rr == NULL)
				{
					mn = getShortClassPath(kind) + (string)ccr.name();
					rr = mmodel->RoleByName[SmartBSTR(mn.c_str())];
				}
			}
			else
			{
				mn = getShortClassPath(kind) + (string)ccr.name();
				rr = mmodel->RoleByName[SmartBSTR(mn.c_str())];
			}
		}
		else
		{
			//metarole = rolename ? rolename : kindname
			//this is exactly the same thing as MakeShortRolename

			
			//MakeShortRolename is not good - when ccr has no name, and it points towards an abstract base class,
			//the returned string will be the name of the abstract base class, but it should be the inherited class

			string roleWithNamespaces = ccr.name();
			if (roleWithNamespaces.empty())
			{
				string kn = getShortClassPath(kind);
				rr = mmodel->RoleByName[SmartBSTR(kn.c_str())];
			}
			else
			{
				Uml::Namespace ns = static_cast<Uml::Class>(ccr.target()).parent_ns();
				while (ns)
				{
					roleWithNamespaces = static_cast<std::string>(ns.name()) + "::" + roleWithNamespaces;
					ns = ns.parent_ns();
				}

				mmodel->get_RoleByName(_bstr_t(roleWithNamespaces.c_str()), &rr);
			}
			
			//mn = Uml::MakeShortRoleName(ccr);
			

		}

		if(rr == NULL)
		{
			throw udm_exception("Role not found in model: " + mn);
		}

		return rr;

	};

	IMgaMetaRolePtr GmeObject::GetMetaRoleForChildRole(::Uml::CompositionChildRole meta, IMgaMetaModelPtr mmodel) 
	{ 
		IMgaMetaRolePtr rr;
		if(mmodel == NULL) 
			throw udm_exception("Metamodel not found");
		string mn = Uml::MakeShortRoleName(meta);
		if(mn.empty()) {
			mn = getShortClassPath(*meta.target());
			rr = mmodel->RoleByName[SmartBSTR(mn.c_str())];
		} else {
			IMgaMetaRolesPtr rrs;
			rrs = mmodel->Roles;
			MGACOLL_ITERATE(IMgaMetaRole, rrs) 
			{
				if (mn == ToUdmRoleName(MGACOLL_ITER, meta)) {
					rr = MGACOLL_ITER;
					break;
				}
			}
			MGACOLL_ITERATE_END;
		}

		if(rr == NULL) 
			throw udm_exception("Role not found in model: " + mn);
		return rr;
	};		
/*
	IMgaMetaRolePtr GmeObject::GetMetaRoleForKind(::Uml::Class kind, IMgaMetaModelPtr mmodel)
	{
		IMgaMetaRolePtr rr;
		if(mmodel == NULL) 
			throw udm_exception("Metamodel not found");
		string mn = kind.name();
		rr = mmodel->RoleByName[SmartBSTR(mn.c_str())];
	
		if(rr == NULL) 
			throw udm_exception("Role for kind not found in model: " + mn);
		return rr;
	};
*/

	void GmeObject::getChildRole(Udm::ObjectImpl *c, ::Uml::CompositionChildRole &ret) const
	{
		ret = NULL;
		if (c == NULL || c == &Udm::_null)
			return;

		GmeObject *cc = static_cast<GmeObject *>(c);

		// check that c is our child
		IMgaMetaRolePtr rr;
		bool is_child = false;
		if (folderself)
		{
			if (cc->folderself)
			{
				IMgaFoldersPtr fchds = folderself->ChildFolders;
				MGACOLL_ITERATE(IMgaFolder, fchds)
				{
					if (MGACOLL_ITER == cc->folderself)
					{
						is_child = true;
						break;
					}
				}
				MGACOLL_ITERATE_END;
			}
			else
			{
				IMgaFCOsPtr chds = folderself->ChildFCOs;
				MGACOLL_ITERATE(IMgaFCO, chds) 
				{
					if (MGACOLL_ITER == cc->self)
					{
						is_child = true;
						rr = MGACOLL_ITER->MetaRole;
						break;
					}
				}
				MGACOLL_ITERATE_END;
			}
		}
		else
		{
			IMgaModelPtr model = self;
			if (model == NULL)
				return;

			IMgaFCOsPtr chds = model->ChildFCOs;
			MGACOLL_ITERATE(IMgaFCO, chds) 
			{
				if (MGACOLL_ITER == cc->self)
				{
					is_child = true;
					rr = MGACOLL_ITER->MetaRole;
					break;
				}
			}
			MGACOLL_ITERATE_END;
		}

		if (!is_child)
			return;

		// if only one composition role permitted, then return it
		::Uml::Composition comp = ::Uml::matchChildToParent(cc->m_type, m_type);
		if (comp)
		{
			ret = comp.childRole();
			return;
		}

		ASSERT(rr != NULL);
		string MetaRole((const char *) rr->Name);

		set< ::Uml::Class> pancs = ::Uml::AncestorClasses(m_type);
		set< ::Uml::Class> cancs = ::Uml::AncestorClasses(cc->m_type);
		for (set< ::Uml::Class>::const_iterator j = cancs.begin(); j != cancs.end(); j++) {
			set< ::Uml::CompositionChildRole> cr = j->childRoles();
			for (set< ::Uml::CompositionChildRole>::const_iterator i = cr.begin(); i != cr.end(); i++)
				if (pancs.find(::Uml::theOther(*i).target()) != pancs.end())
				{
					if (MetaRole == static_cast<std::string>(i->name()))
					{
						ret = *i;
						return;
					}
					set<string> MetaRoleFilter = ((GmeDataNetwork*) mydn)->GetMetaRoleFilter(*i);
					if (MetaRoleFilter.find(MetaRole) != MetaRoleFilter.end())
					{
						ret = *i;
						return;
					}
				}
		}
	}


	IMgaAttributePtr GmeObject::getAttribute(BSTR name)
	{
		IMgaMetaAttributePtr attr_meta;
		COMTHROW(MetaFCO()->get_AttributeByName(name, &attr_meta));
		return testself->GetAttribute(attr_meta);
	}

	string GmeObject::getStringAttr(const ::Uml::Attribute &meta) const
	{
		SmartBSTR val;
		string ret;
		string rname = meta.name();

		if (meta.registry())
		{
			//to be written as a registry value
			IMgaRegNodePtr reg_node;
			BSTR reg_node_name = SmartBSTR(rname.c_str());
		
			if (folderself)
				COMTHROW(folderself->get_RegistryNode(reg_node_name, &reg_node));
			else
				COMTHROW(self->get_RegistryNode(reg_node_name, &reg_node));
				
			val = reg_node->Value;
			return val.length()? UTF8FromBSTR(val) : "";


		}
		if(rname == "name") val = objself->Name;
		else if(rname == "position") 
		{
			long x, y;
			IMgaPartsPtr parts; 
			if(testself->MetaRole != NULL && (parts = testself->Parts)->Count > 0) 
			{
				long count = testself->Parts->Count;
				if (count == 1)
				{
					IMgaPartPtr part = parts->Item[1];
					_bstr_t icon;
					COMTHROW(part->GetGmeAttrs(icon.GetAddress(), &x, &y));
					char buf[30];
					sprintf(buf,"(%d,%d)", x, y);
					return buf;
				}
				else
				{
					//it's defitively  > 1
					for (int i = 1; i <= count; i++)
					{
						IMgaPartPtr part = parts->Item[i];
						_bstr_t icon;
						COMTHROW(part->GetGmeAttrs(icon.GetAddress(), &x, &y));
						SmartBSTR aspect =  part->MetaAspect->Name;
						ret += (char *) aspect;

						char buf[20];
						sprintf(buf,"(%d,%d);", x, y);
						ret += buf;

					}

					return ret;
				}
			}
			else
				return "";
		}
		else val = testself->StrAttrByName[SmartBSTR(rname.c_str())];
		return UTF8FromBSTR(val);
	}

	typedef pair<const long, const long> pos_in_aspect;
	typedef map<const string, const pos_in_aspect> part_position;

	part_position getPartPosition(const string &input)
	{
		part_position ret;
		long x, y;
		if (*(input.begin()) == '(')
		{
			//one(main) aspect, only one position in the string
			if (sscanf(input.c_str(),"(%d,%d)", &x, &y) != 2)
				throw udm_exception("Invalid location string");

			ret.insert(part_position::value_type(string(), pos_in_aspect(x, y)));
			return ret;
		};
		
		string::size_type newpos, prevpos = 0;
		

		while ((newpos = input.find(';', prevpos)) != string::npos)
		{
			string posstring = input.substr(prevpos, newpos-prevpos);
			prevpos = newpos + 1;

			string::size_type asppos = posstring.find('(', 0);
			
			string asp = posstring.substr(0, asppos);//the name of the aspect
			string position = posstring.substr(asppos);//the two coordinates
			
			if (sscanf(position.c_str(),"(%d,%d)", &x, &y) != 2)
				throw udm_exception("Invalid location string");

			ret.insert(part_position::value_type(asp, pos_in_aspect(x, y)));
			
		}
		return ret;
	};

	void GmeObject::setStringAttr(const ::Uml::Attribute &meta, const string &a, const bool direct)
	{
		((GmeDataNetwork*)mydn)->CountWriteOps();
		string rname = meta.name();

		if (meta.registry())
		{
			//to be written as a registry value
			IMgaRegNodePtr reg_node;
			_bstr_t reg_node_name = BSTRFromUTF8(rname);
		
			if (folderself)
				COMTHROW(folderself->get_RegistryNode(reg_node_name, &reg_node));
			else
				COMTHROW(self->get_RegistryNode(reg_node_name, &reg_node));

			COMTHROW(reg_node->put_Value(BSTRFromUTF8(a)));


		}
		else if(rname == "name")
		{
			objself->Name = BSTRFromUTF8(a); 
			if (folderself && folderself->Project->RootFolder == folderself) {
				folderself->Project->Name = BSTRFromUTF8(a);
			}
#ifdef _DEBUG
			name = a; 
#endif
		}
		else if(rname == "position") 
		{
			if (!a.size())
				return;		//empty strings should be allowed and nothing should happen in such cases.

			if(testself->MetaRole == NULL)
				return;
			IMgaPartsPtr parts = testself->Parts;
			if(parts->Count == 0)
				return;

			const part_position poss = getPartPosition(a);

			if(poss.size()==1)
			{
				//position in main aspect will be set
				IMgaPartPtr part = parts->Item[1];
				COMTHROW(part->SetGmeAttrs(_bstr_t(), poss.begin()->second.first,poss.begin()->second.second ));
			}
			else
			{
				//set the positions in all aspects which are present in the position string
			
				MGACOLL_ITERATE(IMgaPart, parts) 
				{
					const string aspect = (const char *) (MGACOLL_ITER->MetaAspect->Name);
					part_position::const_iterator poss_i = poss.find(aspect);

					if ( poss_i != poss.end())
						COMTHROW(MGACOLL_ITER->SetGmeAttrs(_bstr_t(), poss_i->second.first,poss_i->second.second));
					
					
				} MGACOLL_ITERATE_END;
			}

		}
		else testself->StrAttrByName[BSTRFromUTF8(rname)] = BSTRFromUTF8(a);
	};

	bool GmeObject::getBooleanAttr(const ::Uml::Attribute &meta) const	
	{
		return testself->BoolAttrByName[SmartBSTR(NAMEGET(meta))] == VARIANT_TRUE;
	}

	void GmeObject::setBooleanAttr(const ::Uml::Attribute &meta, bool a, const bool direct)
	{
		((GmeDataNetwork*)mydn)->CountWriteOps();
		testself->BoolAttrByName[SmartBSTR(NAMEGET(meta))] = a ? VARIANT_TRUE : VARIANT_FALSE;
	}

	__int64 GmeObject::getIntegerAttr(const ::Uml::Attribute &meta) const
	{
		return __int64(testself->IntAttrByName[SmartBSTR(NAMEGET(meta))]);
	}

	void GmeObject::setIntegerAttr(const ::Uml::Attribute &meta, __int64 a, const bool direct)
	{
		((GmeDataNetwork*)mydn)->CountWriteOps();
		testself->IntAttrByName[SmartBSTR(NAMEGET(meta))] = (long)a;
	}

	double GmeObject::getRealAttr(const ::Uml::Attribute &meta) const
	{
		return testself->FloatAttrByName[SmartBSTR(NAMEGET(meta))];
	}

	void GmeObject::setRealAttr(const ::Uml::Attribute &meta, double a, const bool direct)
	{
		((GmeDataNetwork*)mydn)->CountWriteOps();
		testself->FloatAttrByName[SmartBSTR(NAMEGET(meta))] = a;
	}


	// Walk a collection of registry nodes, part of annotations tree. Convert the node path and value
	// to a string and collect the string. Recurse on subnodes until finished.
	// Conversion to string: node path (without leading "annotations/") + '=' + node value
	//                       '=' and '\\' are escaped in node path and in node value
	void walkAnnoRegistry(const IMgaRegNodesPtr &registry, vector<string> &ret, const string &path_prefix = "")
	{
		MGACOLL_ITERATE(IMgaRegNode, registry) 
		{
			SmartBSTR name = MGACOLL_ITER->GetName();

			// path of this node
			string path(path_prefix);
			if (path.length())
				path.append("/");
			path.append(name);

			SmartBSTR val = MGACOLL_ITER->GetValue();

			vector<string> v;
			v.push_back(path);
			if (!(!val))
				v.push_back(static_cast<std::string>(val));
			else
				v.push_back("");

			ret.push_back(UdmUtil::vector_to_string(v, '='));


			IMgaRegNodesPtr reg_subnodes = MGACOLL_ITER->GetSubNodes(VARIANT_FALSE);
			if (reg_subnodes->Count)
				walkAnnoRegistry(reg_subnodes, ret, path);
		}
		MGACOLL_ITERATE_END;
	}

	vector<string> GmeObject::getStringAttrArr(const ::Uml::Attribute &meta) const
	{
		string rname = meta.name();
		if (meta.registry() && rname == "annotations")
		{
			IMgaRegNodePtr reg_node;
			BSTR reg_node_name = SmartBSTR(rname.c_str());
		
			if (folderself)
				COMTHROW(folderself->get_RegistryNode(reg_node_name, &reg_node));
			else
				COMTHROW(self->get_RegistryNode(reg_node_name, &reg_node));
				
			IMgaRegNodesPtr reg_subnodes = reg_node->GetSubNodes(VARIANT_FALSE);

			vector<string> ret;

			if (reg_subnodes->Count)
				walkAnnoRegistry(reg_subnodes, ret);

			return ret;
		}
		else
			return this->ObjectImpl::getStringAttrArr(meta);
	}

	void GmeObject::setStringAttrArr(const ::Uml::Attribute &meta, const vector<string> &a, const bool direct)
	{
		string rname = meta.name();
		if (meta.registry() && rname == "annotations")
		{
			((GmeDataNetwork*)mydn)->CountWriteOps();

			IMgaRegNodePtr reg_node;
			BSTR reg_node_name = SmartBSTR(rname.c_str());
		
			if (folderself)
				COMTHROW(folderself->get_RegistryNode(reg_node_name, &reg_node));
			else
				COMTHROW(self->get_RegistryNode(reg_node_name, &reg_node));

			//remove all existing annotations
			reg_node->RemoveTree();

			for (vector<string>::const_iterator i = a.begin(); i != a.end(); i++)
			{
				vector<string> v = UdmUtil::string_to_vector(*i, '=');

				if (v.size() == 1)
					v.push_back("");
				else if (v.size() == 0 || v.size() > 2)
					throw udm_exception("Annotation value can not be parsed: " + *i);

				string path("annotations/");
				path.append(v[0]);				//full path

				if (folderself)
					COMTHROW(folderself->put_RegistryValue(SmartBSTR(path.c_str()), SmartBSTR(v[1].c_str())));
				else
					COMTHROW(self->put_RegistryValue(SmartBSTR(path.c_str()), SmartBSTR(v[1].c_str())));
			}
		}
		else
			this->ObjectImpl::setStringAttrArr(meta, a, direct);
	}

	long GmeObject::getAttrStatus(const ::Uml::Attribute &meta) const
	{
		string rname = meta.name();
		if (rname == "name")
		{
			return ATTSTATUS_HERE;
		}
		else if (rname == "position")
		{
			// XXX for now
			return ATTSTATUS_HERE;
		}
		else if (meta.registry())
		{
			IMgaRegNodePtr reg_node;
			BSTR reg_node_name = SmartBSTR(rname.c_str());
		
			if (folderself)
				COMTHROW(folderself->get_RegistryNode(reg_node_name, &reg_node));
			else
				COMTHROW(self->get_RegistryNode(reg_node_name, &reg_node));

			long status;
			reg_node->GetStatus(&status);
			return status;
		}
		else
		{
			IMgaMetaAttributePtr attr_meta;
			COMTHROW(MetaFCO()->get_AttributeByName(SmartBSTR(rname.c_str()), &attr_meta));
			return testself->GetAttribute(attr_meta)->GetStatus();
		}
	}

	string UdmId2GmeId(uniqueId_type udmId) {
		char buffer[24];
		sprintf_s(buffer, "id-%04x-%08x", (unsigned int)(udmId / 100000000 + 100), (unsigned int)(udmId % 100000000));
		return string(buffer);
	}

	uniqueId_type GmeId2UdmId(const wchar_t* gmeId) {
		long p, c;
		if(swscanf(gmeId, OLESTR("id-%04lx-%08lx"), &c, &p) != 2 ||
			(c-=100) > OBJTYPE_FOLDER || c < OBJTYPE_MODEL || p > 100000000) 
		{
			throw udm_exception("OBJID too big or invalid");
		}
		return 100000000 * c + p;
	}

	uniqueId_type GmeId2UdmId(const char* gmeId) {
		long p, c;
		if(sscanf(gmeId, "id-%04lx-%08lx", &c, &p) != 2 ||
			(c-=100) > OBJTYPE_FOLDER || c < OBJTYPE_MODEL || p > 100000000) 
		{
			throw udm_exception("OBJID too big or invalid");
		}
		return 100000000 * c + p;
	}

	uniqueId_type GmeObject::uniqueId() const 
	{
		SmartBSTR nn;
		if (__uniqueId_set)
			return __uniqueId;

		if(folderself) 
			nn = folderself->ID;
		else 
			nn = self->ID;

		return __set_uniqueId(GmeId2UdmId(static_cast<const wchar_t*>(nn)));
	};


	uniqueId_type GmeObject::__set_uniqueId(uniqueId_type id) const
	{
		((GmeObject*)this)->__uniqueId = id;
		((GmeObject*)this)->__uniqueId_set = true;
		return id;
	};

	const ::Uml::Class & GmeObject::type() const 
	{ 
		return m_type;
	};

	IMgaMetaBasePtr GmeObject::Meta() const 
	{
		if(folderself) 
			return folderself->MetaBase;
		else 
			return self->MetaBase;
	};

	IMgaMetaFCOPtr GmeObject::MetaFCO() const 
	{
		return testself->Meta;
	};

	//::Uml::Class GmeObject::findClass(const set< ::Uml::Class> &classes) 


	
	::Uml::Class GmeObject::findClass() 
	{
		SmartBSTR name = Meta()->Name;
		string key(name);

		map<string, const ::Uml::Class>::iterator j = ((GmeDataNetwork*)mydn)->meta_class_cache.find(key);

		if (j == ((GmeDataNetwork*)mydn)->meta_class_cache.end())
			throw udm_exception("Cannot find Udm metaobject for GME kind: " + name);

		return j->second;	
	
	};


	inline GmeObject::GmeObject(const ::Uml::Class &meta, IMgaFCO *obj, const DataNetwork * dn) :  m_type(meta), self(obj) 
	{ 
		__uniqueId_set=false;
		mydn = dn;
#ifdef _DEBUG
		name = (const char*)obj->GetName();
		uniqueId();
#endif
	};

	inline GmeObject::GmeObject(IMgaFCO *obj, const DataNetwork * dn) : self(obj) 
	{
		//set< ::Uml::Class> classes = diagram.classes();
		__uniqueId_set=false;
		mydn = dn;
		m_type = findClass();
#ifdef _DEBUG
		name = (const char*)obj->GetName();
		uniqueId();
#endif
		if (!mydn) throw udm_exception("Data Network is NULL in constructor! GmeObject without a data network ?!");
	};

	inline GmeObject::GmeObject(const ::Uml::Class &meta, IMgaFolder *obj, const DataNetwork * dn) :  m_type(meta), folderself(obj) 
	{
		__uniqueId_set=false;
		mydn = dn;
#ifdef _DEBUG
		name = (const char*)obj->GetName();
		uniqueId();
#endif
		if (!mydn) throw udm_exception("Data Network is NULL in constructor! GmeObject without a data network ?!");
	};

	inline GmeObject::GmeObject( IMgaFolder *obj, const DataNetwork * dn) : folderself(obj) 
	{
		//set< ::Uml::Class> classes = diagram.classes();
		__uniqueId_set=false;
		mydn = dn;
		m_type = findClass();
#ifdef _DEBUG
		name = (const char*)obj->GetName();
		uniqueId();
#endif
		if (!mydn) throw udm_exception("Data Network is NULL in constructor! GmeObject without a data network ?!");
	};

	inline ObjectImpl * GmeObject::clone()
	{
		return self
			? new GmeObject(m_type, self, mydn)
			: new GmeObject(m_type, folderself, mydn);
	};

	inline void GmeObject::release()
	{
		delete this;
	};

	inline vector<ObjectImpl*> GmeObject::getDerived() const
	{
		vector<ObjectImpl*> ret;
		IMgaFCOsPtr peers = self->DerivedObjects;
		MGACOLL_ITERATE(IMgaFCO, peers) 
		{
			IMgaFCOPtr peer = MGACOLL_ITER;
			if (!peer->IsInstance)
				ret.push_back(new GmeObject( peer, mydn));
		}
		MGACOLL_ITERATE_END;
		return ret;
	};
	inline vector<ObjectImpl*> GmeObject::getInstances() const
	{
		vector<ObjectImpl*> ret;
		if (self)
		{
			IMgaFCOsPtr peers = self->DerivedObjects;
			MGACOLL_ITERATE(IMgaFCO, peers) 
			{
				IMgaFCOPtr peer = MGACOLL_ITER;
				if (peer->IsInstance)
					ret.push_back(new GmeObject( peer, mydn));
			}
			MGACOLL_ITERATE_END;
		}
		return ret;
	};
	inline ObjectImpl* GmeObject::getArchetype() const
	{
		ObjectImpl * ret = NULL;
		if (self)
		{
			IMgaFCOPtr peer = self->DerivedFrom;
			if(peer) ret = new GmeObject( peer, mydn);
		}
		return ret;
	};

	inline bool GmeObject::hasRealArchetype() const 
	{
		ObjectImpl * ret = getArchetype();
		if (!ret)
			throw udm_exception("hasRealArchetype() can be evaluated only on instantiated  or derived objects, or (inherited)children of derived objects.");
		bool retval = (self->IsPrimaryDerived == VARIANT_TRUE);
		delete ret;
		return retval;

	};

	bool GmeObject::isSubtype() const
	{
		if (self)
		{
			IMgaFCOPtr peer = self->DerivedFrom;
			if (peer)
				return (self->IsInstance == VARIANT_FALSE);
			else
				return false;
		
		}
		return false;
	};

	bool GmeObject::isInstance() const
	{
		if (self)
		{
			IMgaFCOPtr peer = self->DerivedFrom;
			if (peer)
				return (self->IsInstance == VARIANT_TRUE);
			else
				return false;
	
		}
		return false;
	};
	


	Udm::DataNetwork * GmeObject::__getdn()
	{ 
		if (mydn)
			return const_cast<Udm::DataNetwork*>(mydn);
		throw udm_exception("Data Network is NULL in constructor! GmeObject without a data network ?!");

		/*
		IMgaProjectPtr p = self->Project;
		for(gdnmap::iterator ff = GDNMap.begin(); ff != GDNMap.end(); ff++) 
		{
			Object root = (*ff)->GetRootObject();
			if(root && static_cast<GmeObject *>(root.__impl())->folderself->Project == p) 
				mydn = *ff;
				return *ff;
		}
		throw udm_exception("Error locating GME DN for object");
		*/

	};


	bool GmeObject::isLibObject() const
	{
		return (objself->IsLibObject == VARIANT_TRUE);
	}

	bool GmeObject::isLibRoot() const
	{
		if (folderself) {
			SmartBSTR lib_name = folderself->LibraryName;
			if (!(!lib_name)) {
				return true;
			}
		}

		return false;
	}

	bool GmeObject::getLibraryName(string &name) const
	{
		// only library roots have a library name
		if (!isLibRoot())
			return false;

		SmartBSTR lib_name = folderself->LibraryName;
		name = lib_name;
		if (name.substr(0, 4) == "MGA=")
			name.erase(0, 4);

		return true;
	}

	void GmeObject::setLibraryName(const char *name)
	{
		ASSERT(folderself != NULL);
		folderself->LibraryName = SmartBSTR(name);
	}

	ObjectImpl * GmeObject::LibRoot()
	{
		ASSERT(isLibObject());

		GmeObject *cur = this;
		while (true) {
			// the libroot of a library rootfolder is not the rootfolder itself
			// but the containing library rootfolder
			if (cur != this && cur->folderself != NULL) {
				SmartBSTR lib_name = cur->folderself->LibraryName;
				if (!(!lib_name))
					return cur->clone();
			}

			ObjectImpl *parent = cur->getParent(NULL);
			ASSERT(parent && parent != &Udm::_null);

			if (cur != this)
				cur->release();

			cur = (GmeObject *) parent;
		}
	}

	vector<ObjectImpl*> GmeObject::getLibRoots() const 
	{
		vector<ObjectImpl*> ret;

		if (folderself) 
		{
			IMgaFoldersPtr fchds = folderself->ChildFolders;
			
			MGACOLL_ITERATE(IMgaFolder, fchds) 
			{
				SmartBSTR lib_name = MGACOLL_ITER->LibraryName;
				if (!(!lib_name))
				  ret.push_back(new GmeObject( MGACOLL_ITER, mydn));
			} MGACOLL_ITERATE_END;
		}
		return ret;
	}

	void GmeObject::GetGuid(long *p1, long *p2, long *p3, long *p4) const
	{
		objself->GetGuid(p1, p2, p3, p4);
	}

	class BinGuid {
	protected:
		long v1, v2, v3, v4;
	public:
		BinGuid() : v1(0), v2(0), v3(0), v4(0) {}
		BinGuid(long p1, long p2, long p3, long p4) : v1(p1), v2(p2), v3(p3), v4(p4) {}

		bool operator == (const BinGuid& peer) const
		{
			if( v1 != peer.v1)
				return false; // speed-up

			return ( v1 == peer.v1
					&& v2 == peer.v2
					&& v3 == peer.v3
					&& v4 == peer.v4);
		}

		bool operator != ( const BinGuid& peer) const
		{
			return !( *this == peer);
		}

		bool operator < ( const BinGuid& peer) const
		{
			if( v1 < peer.v1)
				return true; // speed-up

			return v1 < peer.v1
					|| v1 == peer.v1 && v2 < peer.v2
					|| v1 == peer.v1 && v2 == peer.v2 && v3 < peer.v3
					|| v1 == peer.v1 && v2 == peer.v2 && v3 == peer.v3 && v4 < peer.v4;
		}

		friend ostream & operator << (ostream &o, const BinGuid &v)
		{
			o << hex << v.v1 << "-" << v.v2 << "-" << v.v3 << "-" << v.v4 << dec;
			return o;
		}
	};

	class UniqueId {
	protected:
		BinGuid objectId, libId;
	public:
		UniqueId() {}
		UniqueId(const BinGuid &po, const BinGuid &pl) : objectId(po), libId(pl) {}

		bool operator == (const UniqueId &peer) const
		{
			return ( libId == peer.libId
					&& objectId == peer.objectId);
		}

		bool operator< ( const UniqueId& peer) const
		{
			if( objectId < peer.objectId)
				return true;

			return objectId < peer.objectId
					|| objectId == peer.objectId && libId < peer.libId;
		}

		friend ostream & operator << (ostream &o, const UniqueId &v)
		{
			o << "[" << v.objectId << "]-[" << v.libId << "]";
			return o;
		}
	};

	typedef map<UniqueId, ObjectImpl *> t_uid_to_impl_map;
	void BuildUniqueIdMap(ObjectImpl *po, t_uid_to_impl_map &m, bool obj_itself = true)
	{
		GmeObject *o = static_cast<GmeObject *>(po);

		vector<ObjectImpl *> children = o->getChildren(NULL, NULL);
		for (vector<ObjectImpl *>::const_iterator i = children.begin(); i != children.end(); i++) {
			GmeObject *child = (GmeObject *) *i;
			BuildUniqueIdMap(child, m);
			child->release();
		}

		if (!obj_itself)
			return;

		long v1, v2, v3, v4;
		BinGuid objId, libId;

		o->GetGuid(&v1, &v2, &v3, &v4);
		objId = BinGuid(v1, v2, v3, v4);

		if (!o->isLibObject()) {
			Object root = o->__getdn()->GetRootObject();
			// for root we use BinGuid(0, 0, 0, 0)
			if (o->uniqueId() != root.uniqueId()) {
				static_cast<GmeObject *>(root.__impl())->GetGuid(&v1, &v2, &v3, &v4);
				libId = BinGuid(v1, v2, v3, v4);
			}
		}
		else {
			ObjectImpl *lib_root = o->LibRoot();
			ASSERT(lib_root && lib_root != &Udm::_null);
			static_cast<GmeObject *>(lib_root)->GetGuid(&v1, &v2, &v3, &v4);
			libId = BinGuid(v1, v2, v3, v4);
			lib_root->release();
		}

		m.insert( make_pair(UniqueId(objId, libId), o->clone()) );
	}

	void MapLibToCopy(ObjectImpl *lib_root, ObjectImpl *copy_root, Udm::t_lib_to_copy_impl_map &m)
	{
		m.clear();
		m.insert( make_pair(lib_root->clone(), copy_root->clone()) );

		t_uid_to_impl_map lib_map, copy_map;
		BuildUniqueIdMap(lib_root, lib_map, false);
		BuildUniqueIdMap(copy_root, copy_map, false);

#if 0
		for (t_uid_to_impl_map::const_iterator i = lib_map.begin(); i != lib_map.end(); i++) {
			UniqueId uid = i->first;
			ObjectImpl *o = i->second;
			cout << o->uniqueId() << ": " << "\t" << uid << endl;
		}

		for (t_uid_to_impl_map::const_iterator i = copy_map.begin(); i != copy_map.end(); i++) {
			UniqueId uid = i->first;
			ObjectImpl *o = i->second;
			cout << o->uniqueId() << ": " << "\t" << uid << endl;
		}
#endif

		for (t_uid_to_impl_map::const_iterator i_src = lib_map.begin(); i_src != lib_map.end(); i_src++) {
			t_uid_to_impl_map::const_iterator i_dst = copy_map.find(i_src->first);
			ASSERT(i_dst != copy_map.end());
			m.insert( make_pair(i_src->second->clone(), i_dst->second->clone()) );
		}
	}

	// TODO: copy the library from lib_src to a temporary file, attach from that file and set LibraryName to lib_name
	ObjectImpl* GmeObject::AttachLibrary(ObjectImpl *lib_src, const string &lib_name, Udm::t_lib_to_copy_impl_map *copy_map)
	{
		ASSERT(folderself != NULL);

		IMgaFolderPtr lib_rf;
		COMTHROW(folderself->AttachLibrary(createGMEconnstr(lib_name), &lib_rf));

		GmeObject *lib_root = new GmeObject(lib_rf, mydn);

		if (copy_map)
			MapLibToCopy(lib_src, lib_root, *copy_map);

		return lib_root;
	}





		




	

	

	UDM_DLL GmeDataNetwork::GmeDataNetwork(const Udm::UdmDiagram& dia, Udm::UdmProject* project) 
		: priv(*new privdata), DataNetwork(dia, project) 
	{
		writeOps=0;
		GDNMap.push_front(this);
		static bool first = true; 
		if(first) 
		{ 
			// Modification: Tihamer Levendovszky - for Udm Interpreter //////////////////
			HRESULT res=CoInitializeEx(NULL,COINIT_APARTMENTTHREADED );
			if(FAILED(res)) // If COM has been initialized we are happy with that
				com_exception(res, NULL);
			//////////////////////////////////////////////////////////////////////////////
			first = false; 
		}
		hasOpened=false;

		//create a cache for meta-classes
		::Uml::DiagramClasses meta_classes(*dia.dgr);
		for (::Uml::DiagramClasses::iterator mci = meta_classes.begin(); mci != meta_classes.end(); mci++)
		{
			string key = getShortClassPath(*mci);
			pair<string const, const ::Uml::Class> mcc_item(key, *mci);
			pair<map<string, const ::Uml::Class>::iterator, bool> ins_res = meta_class_cache.insert(mcc_item);
			if (!ins_res.second)
				throw udm_exception("Insert failed when creating meta classes by name map!");
		}
	}

	UDM_DLL GmeDataNetwork::~GmeDataNetwork() 
	{ 
		switch(semantics) {
			case Udm::CHANGES_PERSIST_ALWAYS:
			case Udm::CHANGES_PERSIST_DEFAULT:
				CloseWithUpdate();
				break;
			default:
				CloseNoUpdate();
		}
		delete &priv;
		gdnmap::iterator ff;
		for(ff = GDNMap.begin(); ff != GDNMap.end(); ff++) {
			if(*ff == this) break;
		}
		if(ff == GDNMap.end()) throw udm_exception("Corrupt GME DN map");
		GDNMap.erase(ff);

	}

	


// if the assoc has an aclass, or it has a name and that name does not match either of the end class names -> connection
//		we will identify the metaconnection where 
//			the name of the metaconnection matches the UML name
//			the names of the navigable directions match the arrow names
// otherwise -> set or reference
// 		at either end (from which navigation is available) we expect to find a
// 		set or a reference  with
//			relation names matching the end names
	IMgaMetaFCOPtr MetaObjLookup(IMgaMetaModel *metam, const SmartBSTR &name) {
		IMgaMetaFCOPtr p;
		if(metam->get_DefinedFCOByName(name, VARIANT_FALSE, &p) == S_OK)
			return p;
		if(p)
			return p;
		IMgaMetaFCOsPtr FCOs = metam->DefinedFCOs;
		MGACOLL_ITERATE(IMgaMetaFCO, FCOs) {
			if(MGACOLL_ITER->ObjType == OBJTYPE_MODEL) {
				p = MetaObjLookup(IMgaMetaModelPtr(MGACOLL_ITER),name);
				if(p)
					return p;
			}
		}
		MGACOLL_ITERATE_END;
		return NULL;
	}

	IMgaMetaFCOPtr MetaObjLookup(IMgaMetaFolder *metaf, const SmartBSTR &name) {
		IMgaMetaFCOPtr p;
		if(metaf->get_DefinedFCOByName(name, VARIANT_FALSE, &p) == S_OK)
			return p;

		IMgaMetaFoldersPtr folds = metaf->DefinedFolders;
		MGACOLL_ITERATE(IMgaMetaFolder, folds) {
			p = MetaObjLookup(MGACOLL_ITER,name);
			if(p)
				return p;
		}
		MGACOLL_ITERATE_END;
		IMgaMetaFCOsPtr FCOs = metaf->DefinedFCOs;
		MGACOLL_ITERATE(IMgaMetaFCO, FCOs) {
			if(MGACOLL_ITER->ObjType == OBJTYPE_MODEL) {
				p = MetaObjLookup(IMgaMetaModelPtr(MGACOLL_ITER),name);
				if(p)
					return p;
			}
		}
		MGACOLL_ITERATE_END;
		return NULL;
	}

	IMgaMetaFCOPtr MetaObjLookup(METALib::IMgaMetaProject *metap, const string& name) {
		return MetaObjLookup(metap->RootFolder, SmartBSTR(name.c_str()));
	}
	

// primary points to the set or reference object, to the source of the connection, or to the baseclass
// herename should be the name towards the primary

	void GmeDataNetwork::amapInitialize(const ::Uml::Diagram &dgr, IMgaMetaProject *metaproj) 
	{
		std::string errPrefix;
		{
		IMgaMetaProjectPtr pmetaproj = metaproj;
		errPrefix = "Could not initiliaze Udm Uml Diagram '";
		errPrefix += static_cast<std::string>(dgr.name()) + "' from GME paradigm: '";
		errPrefix += static_cast<const char*>(pmetaproj->Name);
		errPrefix += "': ";
		}

		::Uml::DiagramAssociations assocs(dgr);
		for(::Uml::DiagramAssociations::iterator i = assocs.begin(); i != assocs.end(); i++) 
		{
			//for each association in the meta 
			::Uml::Association assoc = *i;
			set< ::Uml::AssociationRole> roles = assoc.roles();
			assocmapitem *nn = new assocmapitem();
			objtype_enum expect = OBJTYPE_NULL;
			bool reservednamesinUML = false;
			string searchname = assoc.name();

			::Uml::Class aclass = assoc.assocClass();
			for(set< ::Uml::AssociationRole>::iterator j = roles.begin(); j != roles.end(); j++) 
			{
				//for each (two) role of the association
				if(j->isNavigable()) 
				{
					//if the role is navigable
					string rolename = Uml::MakeRoleName(*j);
					/*
					if(searchname.empty() && !aclass && (rolename == "archetype") && ((string)Uml::theOther(*j).name() == "derived")) 
					{
						nn->ot = OBJTYPE_MODEL;
						nn->primary = *j;
						goto archetype_ready;
					}
					*/
					::Uml::AssociationRole orole = Uml::theOther(*j);
					//check if the rolename is reference or set-member relationship
					if(!aclass && (rolename == "ref" || rolename == "members")) 
					{
						::Uml::Class otarget = orole.target();
						nn->metaobj = MetaObjLookup(metaproj, getShortClassPath(otarget));

						// the reference could be an abstract UML class
						if (nn->metaobj == NULL && rolename == "ref")
						{
							typedef map<string, IMgaMetaFCOPtr> role_to_MetaFCO_map;
							role_to_MetaFCO_map role_map;

							// get rolenames of all descendants that are a reference
							set< ::Uml::Class> other_descs = Uml::DescendantClasses(otarget);
							for (set< ::Uml::Class>::iterator desc_i = other_descs.begin(); desc_i != other_descs.end(); desc_i++)
							{	
								IMgaMetaFCOPtr p = MetaObjLookup(metaproj, getShortClassPath(*desc_i));
								if (p)
								{
									if (p->GetObjType() != OBJTYPE_REFERENCE) continue;
									SmartBSTR herename = GetRoleFromMeta(p, L"rrName");
									SmartBSTR therename = GetRoleFromMeta(p, L"rName");
									if (!(!therename) && !(!herename))
									{
										if (string((const char *) therename) != "ref") continue;
										role_map.insert(role_to_MetaFCO_map::value_type(string((const char *) herename), p));
									}
								}
							}

							role_to_MetaFCO_map::iterator i = role_map.find(::Uml::MakeRoleName(orole));
							if (i != role_map.end())
							{
								// the reference is a single non-abstract descendant because a descendant exists
								// with the searched for rolename
								nn->metaobj = i->second;
							}
							else
							{
								// the reference is abstract because there is no descendant with the
								// searched for rolename;
								// add to nn->metaobjs all descendants; getAssociation on a refered instance
								// will return all instances of nn->metaobjs that are a reference to that
								// refered instance
								nn->metaobjs_count = role_map.size();
								if (nn->metaobjs_count > 0)
								{
									nn->metaobjs = new IMgaMetaFCOPtr[nn->metaobjs_count];
									nn->pt_abstract = true;
									int j = 0;
									for (i = role_map.begin(); i != role_map.end(); i++)
									{
										nn->metaobjs[j] = i->second;
										j++;
									}
								}
							}
						}
						nn->primary = orole;
						expect = rolename == "ref" ? OBJTYPE_REFERENCE : OBJTYPE_SET;
						break;
					}
					//check if it is a connection and the rolenames directly identify the source and the destination
					if (rolename == "dst" && (!orole.isNavigable() || static_cast<std::string>(orole.name()) == "src"))
					{
						set< ::Uml::Class> deriveds = Uml::DescendantClasses(aclass);
						nn->primary = Uml::theOther(*j);
						expect = OBJTYPE_CONNECTION;
						
						if (deriveds.size() <= 1)
						{
							nn->metaobj = MetaObjLookup(metaproj, getnameforassoc(assoc, false));	
							break;
						}
						else
						{
							nn->metaobj = NULL;
							set< ::Uml::Class> descs = Uml::DescendantClasses(aclass);
							if (descs.size())
							{

								//first we compute the real size: the number of descendant association classes,
								//which can be mapped to an MGA connection object.
								//this means that they are not abstract
								int real_size = 0;
								for (set< ::Uml::Class>::iterator desc_i = descs.begin();desc_i != descs.end();desc_i++)
								{	
									IMgaMetaFCOPtr p = MetaObjLookup(metaproj, getShortClassPath(*desc_i));
									if (p) real_size++;
								};

								//fill in the nn structure
								nn->metaobjs_count = real_size;
								nn->metaobjs = new IMgaMetaFCOPtr[real_size];
								int j = 0;

								//copy the MgaMetaFCOPtrs to the allocated array in the nn structure
								for (set< ::Uml::Class>::iterator desc_ii = descs.begin();desc_ii != descs.end();desc_ii++)
								{	
									IMgaMetaFCOPtr p = MetaObjLookup(metaproj, getShortClassPath(*desc_ii));
									if (p)
									{
										nn->metaobjs[j] = p;
										j++;
									};
								};
								
							};
							break;
						};
						
						
					}

				}
			}
			if((nn->metaobj != NULL) || (nn->metaobjs != NULL) )
			{	
				//association already resolved - it's either a connection or a reference or a set-member relationship
				reservednamesinUML = true;
			}
			else 
			{
				//the association is not resolved yet,
				//by the rolenames, so we try to resolve it through special registry key/value pairs
				if(!aclass) 
				{
					if(searchname.empty()) 
					{  // the assoc name is empty, it must be a reference or set
						//or reference parent helper
						set< ::Uml::AssociationRole>::iterator j;
						for(j = roles.begin(); j != roles.end(); j++) 
						{
							::Uml::AssociationRole orole = Uml::theOther(*j);
							if(!orole.isNavigable()) continue;

							IMgaMetaFCOPtr bt = MetaObjLookup(metaproj, getShortClassPath(*j->target()));
							if(bt) 
							{   // not abstract, etc;
								SmartBSTR herename, therename, sRefParent, dRefParent;
								switch(bt->GetObjType()) 
								{
									case OBJTYPE_REFERENCE:
											herename = GetRoleFromMeta(bt, L"rrName");
											therename = GetRoleFromMeta(bt, L"rName");
											break;
									case OBJTYPE_SET:						
											herename = GetRoleFromMeta(bt, L"sName");
											therename = GetRoleFromMeta(bt, L"mName");
											break;
									case OBJTYPE_CONNECTION:
											sRefParent = GetRoleFromMeta(bt, L"sRefParent");
											dRefParent = GetRoleFromMeta(bt, L"dRefParent");
											nn->rp_helper = true;//it's an rp_helper connection
											break;
									default:
										continue;
								}//switch(bt->GetObjType()) 
								if (nn->rp_helper)
								{
									if ( (sRefParent == SmartBSTR(Uml::MakeRoleName(orole).c_str())) || (dRefParent == SmartBSTR(Uml::MakeRoleName(orole).c_str())) )
									{
										nn->primary = orole;
										nn->metaobj = bt;
										expect = bt->GetObjType();
									}
									break;

								};//	if (nn->rp_helper)
								if( orole.isNavigable() && therename == SmartBSTR(Uml::MakeRoleName(orole).c_str()) && 	(!j->isNavigable() || herename == SmartBSTR(Uml::MakeRoleName(*j).c_str())) ) 
								{
									nn->primary = *j;
									nn->metaobj = bt;
									expect = bt->GetObjType();
									break;
								}
							}//if(bt)
						}//for j = roles.begin

					}//if(searchname.empty()) 
					else 
					{
						// the assoc name is not empty; 
						// if it matches any of the end class names, it is a reference or a set
						set< ::Uml::AssociationRole>::iterator j;
						for(j = roles.begin(); j != roles.end(); j++) 
						{
							// XXX what namespace?
							if(searchname == NAMEGET(*j->target()) && Uml::theOther(*j).isNavigable())
							{
								nn->primary = *j;
								nn->metaobj = MetaObjLookup(metaproj, getShortClassPath(*j->target()));
								expect = nn->metaobj->GetObjType();
								break;
							}
						}
						if(j == roles.end()) 
						{  
							// otherwise it is a connection
							nn->metaobj = MetaObjLookup(metaproj, searchname); // XXX what namespace?
							expect = OBJTYPE_CONNECTION;
						}
					}//else if(searchname.empty()) 
				}//if !aclass
				else 
				{
					// the assoc has an aclass, it must be a connection
					expect = OBJTYPE_CONNECTION;

					// check if it has subtypes
					set< ::Uml::Class> descs = Uml::DescendantClasses(aclass);
					if (descs.size() <= 1)
					{
						nn->metaobj = MetaObjLookup(metaproj, getShortClassPath(aclass));
					}
					else
					{
						//we have to list all the subtypes and put in the metaobjs

						//first we compute the real size: the number of descendant association classes,
						//which can be mapped to an MGA connection object.
						//this means that they are not abstract
						int real_size = 0;
						for (set< ::Uml::Class>::iterator desc_i = descs.begin();desc_i != descs.end();desc_i++)
						{	
							IMgaMetaFCOPtr p = MetaObjLookup(metaproj, getShortClassPath(*desc_i));
							if (p) real_size++;
						};

						//fill in the nn structure
						nn->metaobjs_count = real_size;
						nn->metaobjs = new IMgaMetaFCOPtr[real_size];
						int j = 0;

						//copy the MgaMetaFCOPtrs to the allocated array in the nn structure
						for (set< ::Uml::Class>::iterator desc_ii = descs.begin();desc_ii != descs.end();desc_ii++)
						{	
							IMgaMetaFCOPtr p = MetaObjLookup(metaproj, getShortClassPath(*desc_ii));
							if (p)
							{
								nn->metaobjs[j] = p;
								j++;
							};
						};
							
					} //if (descs.size() <= 1)
				} //if (nn->metaobj == NULL)
			}//if (nn->metaobj == NULL)
			if(nn->metaobj == NULL && !nn->metaobjs_count) {
				string rolenames;
				std::set<Uml::AssociationRole> roles = assoc.AssociationRole_kind_children();
				std::set<Uml::AssociationRole>::iterator rolesIt = roles.begin();
				for (; rolesIt != roles.end(); rolesIt++) {
					rolenames += string("'") + string(rolesIt->name()) + "' ";
				}
				throw udm_exception(errPrefix + "Cannot resolve association '" + getnameforassoc(assoc, true) + 
					"' with rolenames " + rolenames + "\n");
			}


			{
				SmartBSTR herename, therename;
				if (nn->metaobj)
				{
					//normal cases, without an association class or the association class could be mapped
					//directly to a IMgaMetaFCOPtr
					nn->ot = nn->metaobj->ObjType;
					if(nn->ot != expect) goto typeerr;
					switch(nn->ot) 
					{
						case OBJTYPE_REFERENCE:
							herename = GetRoleFromMeta(nn->metaobj, L"rrName");
							therename = GetRoleFromMeta(nn->metaobj, L"rName");
							break;
						case OBJTYPE_SET:
							herename = GetRoleFromMeta(nn->metaobj, L"sName");
							therename = GetRoleFromMeta(nn->metaobj, L"mName");
							break;
						case OBJTYPE_CONNECTION:
							if (nn->rp_helper) break;
							
							herename = GetRoleFromMeta(nn->metaobj, L"sName");
							therename = GetRoleFromMeta(nn->metaobj, L"dName");
							{
								for(set< ::Uml::AssociationRole>::iterator j = roles.begin(); j != roles.end(); j++) 
								{
									if(therename == SmartBSTR(Uml::MakeRoleName(Uml::theOther(*j)).c_str()) && 	(!j->isNavigable() || herename == SmartBSTR(Uml::MakeRoleName(*j).c_str())) ) 
									{
										nn->primary = *j;
										break;
									}
								}
							}
							

							break;
						default:
		typeerr:
							throw udm_exception(errPrefix + "Association resolves to invalid object type: " +  static_cast<std::string>(nn->metaobj->Name));
					}
				}//if (nn->metaobj)
				else
				{
					//the association class is an abstract supertype 
					//so a corresponding GME connection meta object does not exist
					//it can be mapped to more GME connection metaobjects
					
					
					for (int i = 0; i < nn->metaobjs_count; i++)
					{
						IMgaMetaFCOPtr p = nn->metaobjs[i];

						//we expect that all the metaobjects are the same and equal to the one expected, OBJTYPE_CONNECTION
						nn->ot = p->ObjType;
						if (nn->ot != expect)
							throw udm_exception(errPrefix + "Association resolves to invalid object type: " + static_cast<std::string>(p->Name));

						//compute which one is the primary role name
						herename = GetRoleFromMeta(p, L"sName");
						therename = GetRoleFromMeta(p, L"dName");
						{
							for(set< ::Uml::AssociationRole>::iterator j = roles.begin(); j != roles.end(); j++) 
							{
								if(therename == SmartBSTR(Uml::MakeRoleName(Uml::theOther(*j)).c_str()) && 	(!j->isNavigable() || herename == SmartBSTR(Uml::MakeRoleName(*j).c_str())) ) 
								{
									if (!nn->primary) 
										nn->primary = *j;
									else
									{
										//it should be the same for all the metaobjects!
										if (nn->primary != *j)
											throw udm_exception(errPrefix + "amapInitialize: Different primary rolenames found in descendant connection meta objects of the same connection supertype!");
									}
									break;
								}
							}
						}
						//break;
					};
				};//else if (nn->metaobj)

				if(!nn->primary)
				{
					if (nn->metaobj)
						throw udm_exception(errPrefix + "Association resolves to invalid object type: " +  static_cast<std::string>(nn->metaobj->Name));
					else 
					{
						string descr = "amapInitialize: Incosistency between UML rolenames and sName/dName specifications for these related connections :";
						for (int i = 0; i < nn->metaobjs_count; i++)
						{
							if (i) descr += ",";
							IMgaMetaFCOPtr p = nn->metaobjs[i];
							descr += p->Name; 
						};
						throw udm_exception(errPrefix + descr);
					};
				}
				if((!nn->rp_helper) &&(!reservednamesinUML) && (therename != SmartBSTR(Uml::MakeRoleName(Uml::theOther(nn->primary)).c_str()) || (nn->primary.isNavigable() && herename != SmartBSTR(Uml::MakeRoleName(nn->primary).c_str()))) ) 
				{
					throw udm_exception(errPrefix + "Association end names mismatch: " +  static_cast<std::string>(nn->metaobj->Name));
				}
			}
//archetype_ready:
			amap.insert(assocmap::value_type(assoc.uniqueId(), nn));
		}

		// TODO: handle the case when metaobj is NULL and metaobjs_count > 0
		for (assocmap::const_iterator i = amap.begin(); i != amap.end(); i++) {

			assocmapitem *nn = i->second;
			if ( !nn->metaobj ) continue;

			SmartBSTR sRefParent = GetRoleFromMeta(nn->metaobj, L"sRefParent");
			SmartBSTR dRefParent = GetRoleFromMeta(nn->metaobj, L"dRefParent");
			SmartBSTR herename = GetRoleFromMeta(nn->metaobj, L"sName");
			SmartBSTR therename = GetRoleFromMeta(nn->metaobj, L"dName");

			if ( nn->ot != OBJTYPE_CONNECTION || nn->rp_helper || !sRefParent || !dRefParent ) continue;

			if ( ( !nn->primary.isNavigable() || herename == SmartBSTR(Uml::MakeRoleName(nn->primary).c_str()) )
				&& therename == SmartBSTR(Uml::MakeRoleName(Uml::theOther(nn->primary)).c_str()) ) {

				for (assocmap::const_iterator j = amap.begin(); j != amap.end(); j++) {

					assocmapitem *sec = j->second;
					if (!sec->rp_helper || sec->metaobj != nn->metaobj) continue;

					if ( sRefParent == SmartBSTR(Uml::MakeRoleName(sec->primary).c_str()) )
						nn->src_rp_helper = sec->primary;
					else
						nn->dst_rp_helper = sec->primary;
				}
			}
		}

	}

	void amapClear(assocmap &amap)
	{
		for (assocmap::const_iterator i = amap.begin(); i != amap.end(); i++)
			delete i->second;
		amap.clear();
	}

	UDM_DLL struct Udm::AssociationRoleInfo GmeDataNetwork::GetAssociationRoleInfo(const ::Uml::AssociationRole &role) const
	{
		struct Udm::AssociationRoleInfo ret;

		::Uml::Association assoc = role.parent();
		assocmapitem *nn = amap.find(assoc.uniqueId())->second;

		ret.has_direction = true;
		ret.is_src = nn->primary == role;

		return ret;
	}

	SmartBSTR createGMEconnstr(string sn) {
		if(!_strnicmp(sn.c_str(),"GME:", 4))	sn.erase(0,4);
		if(sn[3] != '=' && !_strnicmp(sn.c_str() + sn.size() - 4 , ".mga",4)) {
			sn.insert(0, "MGA=");
		}
		return SmartBSTR(sn.c_str());
	}

	// For BON: Tihamer Levendovszky

	void GmeDataNetwork::OpenExisting(LPUNKNOWN pUnknown, enum Udm::BackendSemantics sem) {
		OpenExisting(pUnknown, sem, false);
	}

	void GmeDataNetwork::OpenExisting(LPUNKNOWN pUnknown, enum Udm::BackendSemantics sem, bool customtransactions )
	{
		semantics = sem;
		hasOpened=true;
		IMgaProjectPtr  &project = priv.project;
		project = pUnknown;

		if (customtransactions) {
			priv.terr = 0;
		} else {
			project->CreateTerritory(NULL, &priv.terr, NULL);
		}

		// For write
		project->Preferences |= MGAPREF_IGNORECONNCHECKS | MGAPREF_FREEINSTANCEREFS;


		if (priv.terr)
			project->BeginTransaction(priv.terr, TRANSACTION_GENERAL);
		try {
			amapInitialize(GetRootMeta(), project->RootMeta);
// get rootfolder & get name

			rootobject = new GmeObject( project->RootFolder, this);
		}
		catch(udm_exception &s) { 
			amapClear(amap);
			if (priv.terr) {
				project->AbortTransaction();
				priv.terr->Destroy();
			}
			throw s;
		}

	}



	
	UDM_DLL void GmeDataNetwork::OpenExisting(const string &systemname, 
									const string &metalocator, 
									enum Udm::BackendSemantics sem) 
	{
		hasOpened=false;
		semantics = sem;
		IMgaProjectPtr  &project = priv.project;
		COMTHROW(project.CreateInstance(OLESTR("Mga.MgaProject"), 0, CLSCTX_INPROC));
		VARIANT_BOOL ro;
		try {
			COMTHROW(project->Open( createGMEconnstr(systemname), &ro));
		} catch (udm_exception e) {
			throw udm_exception(std::string("Cannot open ") + systemname + ": " + e.what());
		}
		COMTHROW(project->CreateTerritory(NULL, &priv.terr, NULL));

	// BUGFIX:
		project->Preferences |= MGAPREF_IGNORECONNCHECKS | MGAPREF_FREEINSTANCEREFS;


		COMTHROW(project->BeginTransaction(priv.terr, TRANSACTION_GENERAL));
		try {
			amapInitialize(GetRootMeta(), project->RootMeta);
// get rootfolder & get name
			rootobject = new GmeObject( project->RootFolder, this );
		}
		catch(udm_exception &s) { 
			amapClear(amap);
			COMTHROW(project->AbortTransaction());
			COMTHROW(priv.terr->Destroy());
			COMTHROW(project->Close(VARIANT_TRUE));
			throw s;
		}
	}

	UDM_DLL void GmeDataNetwork::CreateNew(const string &systemname, 
									const string &metalocator, const ::Uml::Class &rootclass, 
									enum Udm::BackendSemantics sem) 
	{
		hasOpened=false;
		semantics = sem;
		IMgaProjectPtr  &project = priv.project;
		COMTHROW(project.CreateInstance(OLESTR("MGA.MgaProject"), 0, CLSCTX_INPROC));
		try {
			COMTHROW(project->Create( createGMEconnstr(systemname), SmartBSTR(metalocator.c_str())));
		} catch (udm_exception e) {
			throw udm_exception(std::string("Cannot create ") + systemname + " with meta name " +
				metalocator + ": " + e.what());
		}
		COMTHROW(project->CreateTerritory(NULL, &priv.terr, NULL));

		project->Preferences |= MGAPREF_IGNORECONNCHECKS | MGAPREF_FREEINSTANCEREFS;
		COMTHROW(project->BeginTransaction(priv.terr, TRANSACTION_GENERAL));
		try {
// get rootfolder & get name
			amapInitialize(GetRootMeta(), project->RootMeta);
			GmeObject *bb =  new GmeObject( project->RootFolder, this);
			if(bb->m_type != rootclass) 
			{
				delete bb;
				throw udm_exception("Cannot create this object as root");
			}

			rootobject = bb;
		}
		catch(udm_exception &s) { 
			amapClear(amap);
			COMTHROW(project->AbortTransaction());
			COMTHROW(priv.terr->Destroy());
			COMTHROW(project->Close(VARIANT_TRUE));
			throw s;
		}
	}


	// For Udm-based interpreters - Tihamer Levendovszky
	UDM_DLL Object GmeDataNetwork::Gme2Udm(LPUNKNOWN pUnknown)
	{
		IMgaFolderPtr pFolder(pUnknown);
		if(pFolder)
		{
			return new GmeObject( IMgaFolderPtr(pUnknown), this);
		}
		else
		{
			return new GmeObject( IMgaFCOPtr(pUnknown), this);
		}
		

	}

	UDM_DLL LPUNKNOWN Udm2Gme(const Udm::Object& o)
	{
		GmeObject* gmeObject = dynamic_cast<GmeObject*>(o.__impl());
		if (gmeObject == NULL)
		{
			return NULL;
		}
		if (gmeObject->self) {
			gmeObject->self.AddRef();
			return gmeObject->self;
		} else {
			gmeObject->folderself.AddRef();
			return gmeObject->folderself;
		}
	}


	UDM_DLL Object GmeDataNetwork::ObjectById(Object::uniqueId_type id) 
	{ 
			char buf[100];
			int kindcode = id/100000000;
			if(kindcode < OBJTYPE_MODEL || kindcode  > OBJTYPE_FOLDER)
				return NULL;
			sprintf(buf, "id-%04lx-%08lx", kindcode + 100, (unsigned int)(id % 100000000));
			
			IMgaObjectPtr obj;
			/*
			HRESULT hr = priv.project->GetObjectByID(SmartBSTR(buf), &obj);
			*/
			obj = priv.project->GetObjectByID(SmartBSTR(buf));
			IMgaFolderPtr fold(obj);
			//if(hr != S_OK)
			//return NULL;
			//else
			if(fold)
				return new GmeObject( IMgaFolderPtr(obj), this);
			else
				return new GmeObject( IMgaFCOPtr(obj), this);
	}

	UDM_DLL void GmeDataNetwork::CloseWithUpdate() 
	{
		if(rootobject) {
			rootobject = NULL;

			// only if this is the lastdata network closed.
			//if (GDNMap.size() == 1 && *(GDNMap.begin()) == this)
			//since the last change, amap is local to the datanetwork, and has to be cleaned each time.
			amapClear(amap);
			
			if (priv.terr) {
				priv.project->CommitTransaction();
				priv.terr = NULL;
			}
			if(!hasOpened)
				priv.project->Close(VARIANT_FALSE);
		}
	}

	UDM_DLL void GmeDataNetwork::CloseNoUpdate()			
	{  
		// Use dynamic_cast here instead of UdmObject::operator bool since for .NET we may no longer be in a transaction (e.g. GC)
		if (dynamic_cast<GmeObject*>(rootobject.__impl())) {
			rootobject = NULL;
			
			// only if this is the lastdata network closed.
			//if (GDNMap.size() == 1 && *(GDNMap.begin()) == this)
			//since the last change, amap is local to the datanetwork, and has to be cleaned each time.
			amapClear(amap);
			
			if (priv.terr) {
				// Don't throw an exception here, since this may be called by the destructor under CHANGES_LOST_DEFAULT
				priv.project->raw_AbortTransaction();
				priv.terr = NULL;
			}
			if(!hasOpened)
				priv.project->raw_Close(VARIANT_TRUE);
		}
	}
	
	UDM_DLL void GmeDataNetwork::SaveAs(string systemname) 
	{  
		if(rootobject) {
			if (priv.terr)
				COMTHROW(priv.project->CommitTransaction());
			COMTHROW(priv.project->Save(createGMEconnstr(systemname), VARIANT_TRUE	));
			if (priv.terr)
				COMTHROW(priv.project->BeginTransaction(priv.terr, TRANSACTION_GENERAL));
		}
	}
	UDM_DLL void GmeDataNetwork::CloseAs(string systemname) 
	{  
		SaveAs(systemname); 
		CloseNoUpdate();
	}

	UDM_DLL void GmeDataNetwork::CommitEditSequence() 
	{
		if (priv.terr) {
			COMTHROW(priv.project->CommitTransaction());
			COMTHROW(priv.project->BeginTransaction(priv.terr, TRANSACTION_GENERAL));
		}
	}
	UDM_DLL void GmeDataNetwork::AbortEditSequence() 
	{
		if (priv.terr) {
			COMTHROW(priv.project->AbortTransaction());
			COMTHROW(priv.project->BeginTransaction(priv.terr, TRANSACTION_GENERAL));
		}
	}

	void GmeDataNetwork::CountWriteOps()
	{
		if(semantics== Udm::CHANGES_LOST_DEFAULT)
			return;

		if(writeOps%WRITE_OP_REFRESH_RATE==WRITE_OP_REFRESH_RATE-1)
		{
			writeOps=0;
			CommitEditSequence();
		}
		else
		{
			writeOps++;
		}
	}

	set<string> GmeDataNetwork::GetMetaRoleFilter(::Uml::CompositionChildRole role)
	{
		//search the cache 
		map< ::Uml::CompositionChildRole, string_set>::iterator cache_i = meta_role_filter_cache.find(role);
		
		//return if found
		if (cache_i != meta_role_filter_cache.end())
			return cache_i->second;

		//create the valid MGA CompositionRoles for this role
		string_set MetaRoleFilter;
		set< ::Uml::Class> des = Uml::DescendantClasses(role.target());
		for (set< ::Uml::Class>::iterator des_i = des.begin(); des_i != des.end(); des_i++)
		{
			MetaRoleFilter.insert(getShortClassPath(*des_i));
			MetaRoleFilter.insert(getShortClassPath(*des_i) + static_cast<std::string>(role.name()));
		}

		//insert the newly created set<string> in a cache
		pair< ::Uml::CompositionChildRole const, string_set> cache_item(role, MetaRoleFilter);
		pair<map< ::Uml::CompositionChildRole, string_set>::iterator, bool> ins_res;
		ins_res = meta_role_filter_cache.insert(cache_item);
		if (!ins_res.second)
			throw udm_exception("MetaRoleFilter cache insert operation failed!");
		
		//return the set of string
		return MetaRoleFilter;
	}

	__declspec(noreturn) static void __stdcall _udmgme_com_raise_error(HRESULT hr, IErrorInfo* errorinfo) {
		com_exception(hr, errorinfo);
	}
	static class reg {
		public:
		reg() {
			GmeDataNetwork::RegisterBackend("GME", "mga", &GmeDataNetwork::factory);
			_set_com_error_handler(_udmgme_com_raise_error);
		}	
		~reg() {
			GmeDataNetwork::UnRegisterBackends();
			_set_com_error_handler(_com_raise_error);
		}
	} _reg_unused;

	UDM_DLL int DummyFL;   // for linker
}

