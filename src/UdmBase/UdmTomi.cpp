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
 
 *  FILE NAME:	UdmTomi.cpp 
 *	SYNOPSIS:	Paradigm Independent Udm Interface implementation
 *  AUTHOR:		Tihamér Levendovszky - tihamer.levendovszky@vanderbilt.edu
 *  CREATED:	06/10/02
 *  LAST UPDATE: Endre Magyari - Imported into UDM 08/01/02


	CHANGELOG
	12/06/04	-	endre
			Added new function

			UDM_DLL set<Object> Object::getChildrenByChildRoleChain(const ::Uml::Class& meta, vector< ::Uml::::Uml::CompositionChildRole> chain) const

			which recursively obtains objects of a certain type down in the hierarchi rooted at this.
			The function uses a CompositionChildRole chain which was previously obtained by Uml::GetChildRoleChain() function.



	23/11/04	-	endre

			Added getPath to Udm::Object.

	25/09/04	-	endre

		Changed GetChildObjects functions:
			1. Filtering of object types is not necesarry here, passing the required object type forward to
				ObjectImpl::getChildren() call will only return the objects of the specified (or derived) type.
			2. When evaluating CompositionInfo, parentrole is ignore if it is empty - because empty parentroles 
			can have generated names when using statically compiled-in metas and they remain empty when using
			dinamically loaded metas.

	19/09/04	-	endre
		Changed getAssociationEnd to also consider derived association classes.

	08/27/04	-	endre

		Changed DeleteObject(). After calling this member function, the Object will be a Null Object (Udm::_null)
	06/21/04	-	endre

		Fixed bug UDM-10 in GetAssociationClassObjects

	05/25/04	-	endre

		rollback of the previous change. Resulted in compiler error.

	05/24/04	-	endre

		Added 'using namespace Udm;' because typedef Object Udm::Object was removed from Uml namespace.

	04/16/04	-	endre

		Changed CreateLink, DeleteLink to allow temporary associations within the objects of the same datanetwork
	04/14/04	-	endre

		on Feng's request, changed the CreateObject functions to support creating of instantiated/derived objects.


	04/07/04	-	endre

		Added:

		- GetStrValue() to _int64 -  
		- SetStrValue() from _int64 -
		- GetIntValue() to _string - 
		- SetIntValue() from string -

	03/29/04	-	endre

		Upgraded the following functions to work with cross-package associations:
		
		GetAdjacentObjects 
		GetAssociationClassObjects
		GetPeersFromAssociationClassObject
		GetAssociationEnd
		DeleteLink
		CreateLink

	04/22/03    -   aditya

		Added new method 'Object CreateObject(const ::Uml::Class & clsType, const CompositionInfo& compType)'
	05/02/03	-	endre

		Added new method 'Object Object::GetAssociationEnd(string roleName)' provided by Feng.
	10/12/02	-	endre
		
		Bug in GetPeersFromAssociation() reported by Feng. Fixed

	08/30/02	-	endre

		Bug (crash) in CreateLink() reported by Feng.  Fixed

	08/26/02	-	endre
		
		Obvious bug in CreateObject() reported by Feng:
		"When create a child object, the parent object name will be changed into the child
		class type name." - fixed


	08/01/02	-	endre

		Imported and ported from Tihamer's code.
		Definitely needs some corrections later on...

*/
	

#include "UdmBase.h"
#include "Uml.h"
#include "UmlExt.h"
#include "UdmUtil.h"
#include <cint_string.h>

// -----
// --- Tihamer's TOMI interface
// -----


using namespace Udm;	//for Object

// UDM TOMI Paradigm Independent Interface
// Retrieves the adjacent objects of an object associated via simple association or association class. 
// The returned set can be empty. Composition relationships are not considered here.
UDM_DLL multiset<Object> Object::GetAdjacentObjects()
{
		
	multiset<Udm::Object> objAdjacentObs;
	const ::Uml::Class & srcClass= type();
	set< ::Uml::Class> ancestorClasses=::Uml::AncestorClasses(srcClass);

	for(set< ::Uml::Class>::iterator p_currClass=ancestorClasses.begin();p_currClass!=ancestorClasses.end(); p_currClass++)
	{
		// Getting the association roles and iterating through them
		set< ::Uml::AssociationRole> assocRoles=p_currClass->associationRoles();
		for(set< ::Uml::AssociationRole>::iterator p_currAssocRole=assocRoles.begin(); p_currAssocRole!=assocRoles.end();p_currAssocRole++)
		{
			vector<ObjectImpl*>dstPeers=impl->getAssociation(::Uml::theOther(*p_currAssocRole), Udm::TARGETFROMPEER);
			for(vector<ObjectImpl*>::iterator p_currDstPeer=dstPeers.begin(); p_currDstPeer!=dstPeers.end(); p_currDstPeer++)
				objAdjacentObs.insert(*p_currDstPeer);	
		}
	}

	//additive recursion for cross links
	if (__impl()->__getdn()->GetProject())
	{

		UdmProject * pr = __impl()->__getdn()->GetProject();
		Object src_o = pr->GetPlaceHolder(*this, false);
		if (src_o)
		{
			multiset<Object> ret = src_o.GetAdjacentObjects();
			for(multiset<Object>::iterator ret_i = ret.begin(); ret_i != ret.end(); ret_i++)
				objAdjacentObs.insert(pr->GetRealObject(*ret_i));
		}
	};

	return objAdjacentObs;

};

UDM_DLL set<Object> Object::GetAdjacentUniqueObjects() {
	std::multiset< Udm::Object> res= GetAdjacentObjects();
	return std::set< Udm::Object>( res.begin(), res.end());
}

// UDM TOMI Paradigm Independent Interface
// Retrieves the adjacent objects of an object. The adjacent objects 
// are of the type of clsType or derived from it. The returned set can 
// be empty. Composition relationships are not considered here.
UDM_DLL multiset<Object> Object::GetAdjacentObjects(const ::Uml::Class & clsDstType)
{
	multiset<Udm::Object> objAdjacentObs;
	::Uml::Class srcClass= type();
	set< ::Uml::Class> ancestorClasses=::Uml::AncestorClasses(srcClass);

	for(set< ::Uml::Class>::iterator p_currClass=ancestorClasses.begin();
					p_currClass!=ancestorClasses.end(); p_currClass++)
	{
		// Getting the association roles and iterating through them
		set< ::Uml::AssociationRole> assocRoles=p_currClass->associationRoles();
		for(set< ::Uml::AssociationRole>::iterator p_currAssocRole=assocRoles.begin();
							p_currAssocRole!=assocRoles.end();p_currAssocRole++)
		{

			vector<ObjectImpl*>dstPeers=impl->getAssociation(::Uml::theOther(*p_currAssocRole), Udm::TARGETFROMPEER);
			

			for(vector<ObjectImpl*>::iterator p_currDstPeer=dstPeers.begin();
								p_currDstPeer!=dstPeers.end(); p_currDstPeer++)
			{
				Udm::Object dstObject(*p_currDstPeer);
						
				if(Uml::IsDerivedFrom(dstObject.type(),clsDstType))
				{
					objAdjacentObs.insert(dstObject);
				}
		
				
			}			
		}
	}

	//additive recursion for cross links
	if (__impl()->__getdn()->GetProject())
	{

		UdmProject * pr = __impl()->__getdn()->GetProject();
		Object src_o = pr->GetPlaceHolder(*this, false);
		if (src_o)
		{
			if (pr->HasCrossMeta())
			{
				::Uml::Class cross_clsDstType = ::Uml::GetClassFromCrossDgr(*(pr->GetCrossMeta().dgr), clsDstType);
				multiset<Object> ret = src_o.GetAdjacentObjects(cross_clsDstType);
				for(multiset<Object>::iterator ret_i = ret.begin(); ret_i != ret.end(); ret_i++)
					objAdjacentObs.insert(pr->GetRealObject(*ret_i));
			}
		}
	};


	return objAdjacentObs;

};

UDM_DLL set<Object> Object::GetAdjacentUniqueObjects( const ::Uml::Class & clsDstType) {
	std::multiset< Udm::Object> res= GetAdjacentObjects( clsDstType);
	return std::set< Udm::Object>( res.begin(), res.end());
}

// UDM TOMI Paradigm Independent Interface
// Retrieves the adjacent objects of an object via link instance of ascType. 
// The adjacent objects are of the type of clsType or derived from it. 
// The returned set can be empty. Composition relationships are not considered here. 
// Parameter clsType can be null.
UDM_DLL multiset<Object> Object::GetAdjacentObjects(const ::Uml::Class & clsDstType, const AssociationInfo& ascType)
{
	multiset<Udm::Object> objAdjacentObs;
	::Uml::Class srcClass= type();
	set< ::Uml::Class> ancestorClasses=::Uml::AncestorClasses(srcClass);

	for(set< ::Uml::Class>::iterator p_currClass=ancestorClasses.begin();
					p_currClass!=ancestorClasses.end(); p_currClass++)
	{
		// Getting the association roles and iterating through them
		set< ::Uml::AssociationRole> assocRoles=p_currClass->associationRoles();
		for(set< ::Uml::AssociationRole>::iterator p_currAssocRole=assocRoles.begin();
							p_currAssocRole!=assocRoles.end();p_currAssocRole++)
		{

			// Checking role names
			string strSrcRoleName = p_currAssocRole->name();
			::Uml::AssociationRole oRole = ::Uml::theOther(*p_currAssocRole);
			string strDstRoleName = oRole.name();

			if ((strSrcRoleName != ascType.strSrcRoleName && !ascType.strSrcRoleName.empty()) || (strDstRoleName != ascType.strDstRoleName && !ascType.strDstRoleName.empty()))
				continue;

			// Check if there is an association class for this role
			::Uml::Association assoc = oRole.parent();
			::Uml::Class assoc_cls = assoc.assocClass();
			if (ascType.clsAssociation != ::Uml::Class(NULL) && assoc_cls == ::Uml::Class(NULL))
				continue;

			// If a simple association, get the matching peers and continue with the next role
			if (assoc_cls == ::Uml::Class(NULL)) {
				vector<ObjectImpl*> dstPeers = impl->getAssociation(oRole, Udm::TARGETFROMPEER);

				for(vector<ObjectImpl*>::iterator p_currDstPeer=dstPeers.begin(); p_currDstPeer != dstPeers.end(); p_currDstPeer++)
				{
					Udm::Object dstObject(*p_currDstPeer);

					// Checking class type
					if (clsDstType != ::Uml::Class(NULL) && !::Uml::IsDerivedFrom(dstObject.type(),clsDstType))
						continue;

					objAdjacentObs.insert(dstObject);
				}
				continue;
			}

			// Check all association with classes
			vector<ObjectImpl*> assocs = impl->getAssociation(oRole, Udm::CLASSFROMTARGET);

			for(vector<ObjectImpl*>::iterator p_currAssoc = assocs.begin(); p_currAssoc != assocs.end(); p_currAssoc++)
			{
				Udm::Object assocCls_obj(*p_currAssoc);
				::Uml::Class clsAssociation = assocCls_obj.type();

				// Checking association class type
				if (ascType.clsAssociation != ::Uml::Class(NULL) && ascType.clsAssociation != clsAssociation && !::Uml::IsDerivedFrom(clsAssociation, ascType.clsAssociation))
					continue;

				// Get and test the peers
				vector<ObjectImpl*> dstPeers = assocCls_obj.impl->getAssociation(oRole, Udm::TARGETFROMCLASS);
				for(vector<ObjectImpl*>::iterator p_currDstPeer = dstPeers.begin(); p_currDstPeer != dstPeers.end(); p_currDstPeer++)
				{
					Udm::Object dstObject(*p_currDstPeer);
						
					// Checking class type
					if(clsDstType!=::Uml::Class(NULL) && !::Uml::IsDerivedFrom(dstObject.type(),clsDstType))
						continue;		

					objAdjacentObs.insert(dstObject);

				}
			}
		}
	}

	//additive recursion for cross links
	if (__impl()->__getdn()->GetProject())
	{

		UdmProject * pr = __impl()->__getdn()->GetProject();
		Object src_o = pr->GetPlaceHolder(*this, false);
		if(src_o)
		{
			if (pr->HasCrossMeta())
			{
				//we have to translate ascType as well
				::Uml::Class newAscTypeCl;
				if (ascType.clsAssociation)
					newAscTypeCl = ::Uml::GetClassFromCrossDgr( *(pr->GetCrossMeta().dgr), ascType.clsAssociation );
				AssociationInfo newAscType(newAscTypeCl);

				// don't proceed if no cross association was found
				if ((ascType.clsAssociation != ::Uml::Class(NULL) && newAscType.clsAssociation != ::Uml::Class(NULL)) || ascType.clsAssociation == ::Uml::Class(NULL))
				{
					newAscType.strSrcRoleName = ascType.strSrcRoleName;
					newAscType.strDstRoleName = ascType.strDstRoleName;

					::Uml::Class cross_clsDstType = ::Uml::GetClassFromCrossDgr(*(pr->GetCrossMeta().dgr), clsDstType);
					multiset<Object> ret = src_o.GetAdjacentObjects(cross_clsDstType, newAscType);
					for(multiset<Object>::iterator ret_i = ret.begin(); ret_i != ret.end(); ret_i++)
						objAdjacentObs.insert(pr->GetRealObject(*ret_i));
				
				}
			}
		}
	};


	return objAdjacentObs;

};

UDM_DLL set<Object> Object::GetAdjacentUniqueObjects( const ::Uml::Class & clsDstType, const AssociationInfo& ascType) {
	std::multiset< Udm::Object> res= GetAdjacentObjects( clsDstType, ascType);
	return std::set< Udm::Object>( res.begin(), res.end());
}

// UDM TOMI Paradigm Independent Interface
// Retrieves the adjacent objects, together with the association class,
// of an object via link instance of ascType. 
// The adjacent objects are of the type of clsType or derived from it. 
// The returned set can be empty. Composition relationships are not considered here. Associations without an association class are ignored.
// Parameter clsType can be null.
UDM_DLL multiset< pair<Object, Object> > Object::GetAdjacentObjectsWithAssocClasses(const ::Uml::Class & clsDstType, const AssociationInfo& ascType)
{
	multiset< pair<Udm::Object, Udm::Object> > objAdjacentObs;
	::Uml::Class srcClass= type();
	set< ::Uml::Class> ancestorClasses=::Uml::AncestorClasses(srcClass);

	for(set< ::Uml::Class>::iterator p_currClass=ancestorClasses.begin();
					p_currClass!=ancestorClasses.end(); p_currClass++)
	{
		// Getting the association roles and iterating through them
		set< ::Uml::AssociationRole> assocRoles=p_currClass->associationRoles();
		for(set< ::Uml::AssociationRole>::iterator p_currAssocRole=assocRoles.begin();
							p_currAssocRole!=assocRoles.end();p_currAssocRole++)
		{

			// Checking role names
			string strSrcRoleName = p_currAssocRole->name();
			::Uml::AssociationRole oRole = ::Uml::theOther(*p_currAssocRole);
			string strDstRoleName = oRole.name();

			if ((strSrcRoleName != ascType.strSrcRoleName && !ascType.strSrcRoleName.empty()) || (strDstRoleName != ascType.strDstRoleName && !ascType.strDstRoleName.empty()))
				continue;

			// Check if there is an association class for this role
			::Uml::Association assoc = oRole.parent();
			::Uml::Class assoc_cls = assoc.assocClass();
			if (assoc_cls == ::Uml::Class(NULL))
				continue;

			vector<ObjectImpl*> assocs = impl->getAssociation(oRole, Udm::CLASSFROMTARGET);

			for(vector<ObjectImpl*>::iterator p_currAssoc = assocs.begin(); p_currAssoc != assocs.end(); p_currAssoc++)
			{
				Udm::Object assocCls_obj(*p_currAssoc);
				::Uml::Class clsAssociation = assocCls_obj.type();

				// Checking association class type
				if (ascType.clsAssociation != ::Uml::Class(NULL) && ascType.clsAssociation != clsAssociation && !::Uml::IsDerivedFrom(clsAssociation, ascType.clsAssociation))
					continue;

				// Get and test the peers
				vector<ObjectImpl*> dstPeers = assocCls_obj.impl->getAssociation(oRole, Udm::TARGETFROMCLASS);
				for(vector<ObjectImpl*>::iterator p_currDstPeer = dstPeers.begin(); p_currDstPeer != dstPeers.end(); p_currDstPeer++)
				{
					Udm::Object dstObject(*p_currDstPeer);
						
					// Checking class type
					if(clsDstType!=::Uml::Class(NULL) && !::Uml::IsDerivedFrom(dstObject.type(),clsDstType))
						continue;		

					objAdjacentObs.insert(make_pair(dstObject, assocCls_obj));

				}
			}
		}
	}

	//additive recursion for cross links
	if (__impl()->__getdn()->GetProject())
	{

		UdmProject * pr = __impl()->__getdn()->GetProject();
		Object src_o = pr->GetPlaceHolder(*this, false);
		if(src_o)
		{
			if (pr->HasCrossMeta())
			{
				//we have to translate ascType as well
				::Uml::Class newAscTypeCl;
				if (ascType.clsAssociation)
					newAscTypeCl = ::Uml::GetClassFromCrossDgr( *(pr->GetCrossMeta().dgr), ascType.clsAssociation );
				
				AssociationInfo newAscType(newAscTypeCl);

				// don't proceed if no cross association was found
				if ((ascType.clsAssociation != ::Uml::Class(NULL) && newAscType.clsAssociation != ::Uml::Class(NULL)) || ascType.clsAssociation == ::Uml::Class(NULL))
				{
					newAscType.strSrcRoleName = ascType.strSrcRoleName;
					newAscType.strDstRoleName = ascType.strDstRoleName;

					::Uml::Class cross_clsDstType = ::Uml::GetClassFromCrossDgr( *(pr->GetCrossMeta().dgr), clsDstType );
					multiset< pair<Object, Object> > ret = src_o.GetAdjacentObjectsWithAssocClasses(cross_clsDstType, newAscType);
					for(multiset<pair<Object, Object> >::iterator ret_i = ret.begin(); ret_i != ret.end(); ret_i++)
						objAdjacentObs.insert(make_pair(pr->GetRealObject(ret_i->first), pr->GetRealObject(ret_i->second)));
				
				}
			}
		}
	};


	return objAdjacentObs;

};

// UDM TOMI Paradigm Independent Interface
// Get attributes by name. These are INEFFICIENT functions using iteration.
// These functions return false if the attribute with the specified type
// and attribute name does not exist. Hence these parameters are specified
// in the metamodel, it can be serious error. If no problem they retrieve true.
UDM_DLL bool Object::GetIntValue(string strAttrName, __int64& value) const
{
	// Getting Attributes from meta
	::Uml::Class cls= type();
	set< ::Uml::Attribute> attrs=cls.attributes();	
	
	// Adding parent attributes
	set< ::Uml::Attribute> aattrs=::Uml::AncestorAttributes(cls);
	attrs.insert(aattrs.begin(),aattrs.end());

	
	for(set< ::Uml::Attribute>::iterator ai = attrs.begin();ai != attrs.end(); ai++) 
	{
		string strCurrAttrName=ai->name();
		
		if(string(ai->type())=="Integer"&& strCurrAttrName==strAttrName)
		{
			value=getIntegerAttr(*ai);
			return true;
		}
	}
	// Could not find the attribute.
	return false;
};

UDM_DLL bool Object::GetIntValue(string strAttrName, string& value) const
{
	__int64 val;
	if (GetIntValue(strAttrName, val))
	{
		char tmp[50]; //the maximu __i64 is about 20 characters long
#ifdef _WIN32
		sprintf(tmp, "%I64d", val);
#else
		sprintf(tmp, "%lld", val);
#endif
		value = tmp;
		return true;
	};
	return false;
};


// Himanshu: integer array attributes getter
UDM_DLL bool Object::GetIntValues(string strAttrName, vector<__int64>& values) const
{
	// Getting Attributes from meta
	::Uml::Class cls= type();
	set< ::Uml::Attribute> attrs=cls.attributes();	
	
	// Adding parent attributes
	set< ::Uml::Attribute> aattrs=Uml::AncestorAttributes(cls);
	attrs.insert(aattrs.begin(),aattrs.end());

	
	for(set< ::Uml::Attribute>::iterator ai = attrs.begin();ai != attrs.end(); ai++) 
	{
		string strCurrAttrName=ai->name();
		
		if(string(ai->type())=="Integer"&& strCurrAttrName==strAttrName)
		{
			values=getIntegerAttrArr(*ai);
			return true;
		}
	}
	// Could not find the attribute.
	return false;
};

// UDM TOMI Paradigm Independent Interface
// get the value of Attribute named strAttrName of type String
// return false if not found,true if ok
UDM_DLL bool Object::GetStrValue( string strAttrName, string& value) const
{
	// Getting Attributes from meta
	::Uml::Class cls= type();
	set< ::Uml::Attribute> attrs=cls.attributes();	
	
	// Adding parent attributes
	set< ::Uml::Attribute> aattrs=Uml::AncestorAttributes(cls);
	attrs.insert(aattrs.begin(),aattrs.end());

	
	for(set< ::Uml::Attribute>::iterator ai = attrs.begin();ai != attrs.end(); ai++) 
	{
		string strCurrAttrName=ai->name();
		
		if( ((string(ai->type())=="String") || (string(ai->type())=="Text") ) && strCurrAttrName==strAttrName)
		{
			value=getStringAttr(*ai);
			return true;
		}
	}
	// Could not find the attribute.
	return false;
};

// Himanshu: string array attribute getter
UDM_DLL bool Object::GetStrValues(string strAttrName, vector<string>& values) const
{
	// Getting Attributes from meta
	::Uml::Class cls= type();
	set< ::Uml::Attribute> attrs=cls.attributes();	
	
	// Adding parent attributes
	set< ::Uml::Attribute> aattrs=Uml::AncestorAttributes(cls);
	attrs.insert(aattrs.begin(),aattrs.end());

	
	for(set< ::Uml::Attribute>::iterator ai = attrs.begin();ai != attrs.end(); ai++) 
	{
		string strCurrAttrName=ai->name();
		
		if( ((string(ai->type())=="String") || (string(ai->type())=="Text") ) && strCurrAttrName==strAttrName)
		{
			values=getStringAttrArr(*ai);
			return true;
		}
	}
	// Could not find the attribute.
	return false;
};



UDM_DLL bool Object::GetStrValue( string strAttrName, __int64& value) const
{
	string strval;
	if (GetStrValue(strAttrName, strval))
	{
		
		int ret;
#ifdef _WIN32
		ret = sscanf(strval.c_str(), "%I64d",&value);
#else
		ret = sscanf(strval.c_str(), "%lld", &value);
#endif
		if(ret != 1) return false;

		return true;
	};
	return false;


};

// UDM TOMI Paradigm Independent Interface
// get the value of Attribute named strAttrName of type Real
// return false if not found,true if ok

UDM_DLL bool Object::GetRealValue( string strAttrName, double& value) const
{
	// Getting Attributes from meta
	::Uml::Class cls= type();
	set< ::Uml::Attribute> attrs=cls.attributes();	
	
	// Adding parent attributes
	set< ::Uml::Attribute> aattrs=Uml::AncestorAttributes(cls);
	attrs.insert(aattrs.begin(),aattrs.end());

	
	for(set< ::Uml::Attribute>::iterator ai = attrs.begin();ai != attrs.end(); ai++) 
	{
		string strCurrAttrName=ai->name();
		
		if(string(ai->type())=="Real"&& strCurrAttrName==strAttrName)
		{
			value=getRealAttr(*ai);
			return true;
		}
	}
	// Could not find the attribute.
	return false;
};

// Himanshu: double array attribute getter
UDM_DLL bool Object::GetRealValues(string strAttrName, vector<double>& values) const
{
	// Getting Attributes from meta
	::Uml::Class cls= type();
	set< ::Uml::Attribute> attrs=cls.attributes();	
	
	// Adding parent attributes
	set< ::Uml::Attribute> aattrs=::Uml::AncestorAttributes(cls);
	attrs.insert(aattrs.begin(),aattrs.end());

	
	for(set< ::Uml::Attribute>::iterator ai = attrs.begin();ai != attrs.end(); ai++) 
	{
		string strCurrAttrName=ai->name();
		
		if(string(ai->type())=="Real"&& strCurrAttrName==strAttrName)
		{
			values=getRealAttrArr(*ai);
			return true;
		}
	}
	// Could not find the attribute.
	return false;
};

// UDM TOMI Paradigm Independent Interface
// get the value of Attribute named strAttrName of type Bool
// return false if not found,true if ok
UDM_DLL bool Object::GetBoolValue( string strAttrName, bool& value) const
{
	// Getting Attributes from meta
	::Uml::Class cls= type();
	set< ::Uml::Attribute> attrs=cls.attributes();	
	
	// Adding parent attributes
	set< ::Uml::Attribute> aattrs=Uml::AncestorAttributes(cls);
	attrs.insert(aattrs.begin(),aattrs.end());

	
	for(set< ::Uml::Attribute>::iterator ai = attrs.begin();ai != attrs.end(); ai++) 
	{
		string strCurrAttrName=ai->name();
		
		if(string(ai->type())=="Boolean"&& strCurrAttrName==strAttrName)
		{
			value=getBooleanAttr(*ai);
			return true;
		}
	}
	// Could not find the attribute.
	return false;
};

// Himanshu: bool array attribute getter
UDM_DLL bool Object::GetBoolValues(string strAttrName, vector<bool>& values) const
{
	// Getting Attributes from meta
	::Uml::Class cls= type();
	set< ::Uml::Attribute> attrs=cls.attributes();	
	
	// Adding parent attributes
	set< ::Uml::Attribute> aattrs=Uml::AncestorAttributes(cls);
	attrs.insert(aattrs.begin(),aattrs.end());

	
	for(set< ::Uml::Attribute>::iterator ai = attrs.begin();ai != attrs.end(); ai++) 
	{
		string strCurrAttrName=ai->name();
		
		if(string(ai->type())=="Boolean"&& strCurrAttrName==strAttrName)
		{
			values=getBooleanAttrArr(*ai);
			return true;
		}
	}
	// Could not find the attribute.
	return false;
};


// UDM TOMI Paradigm Independent Interface
// get the value of Attribute named strAttrName of type Int
// return false if not found,true if ok
UDM_DLL bool Object::SetIntValue( string strAttrName, const __int64& value)
{
	// Getting Attributes from meta
	::Uml::Class cls= type();
	set< ::Uml::Attribute> attrs=cls.attributes();	
	
	// Adding parent attributes
	set< ::Uml::Attribute> aattrs=Uml::AncestorAttributes(cls);
	attrs.insert(aattrs.begin(),aattrs.end());

	
	for(set< ::Uml::Attribute>::iterator ai = attrs.begin();ai != attrs.end(); ai++) 
	{
		string strCurrAttrName=ai->name();
		
		if(string(ai->type())=="Integer"&& strCurrAttrName==strAttrName)
		{
			setIntegerAttr(*ai, value);
			return true;
		}
	}
	// Could not find the attribute.
	return false;
};

UDM_DLL bool Object::SetIntValue( string strAttrName, const string& value)
{
	__int64 intval;
	int ret;
#ifdef _WIN32
	ret = sscanf(value.c_str(), "%I64d",&intval);
#else
	ret = sscanf(value.c_str(), "%lld", &intval);
#endif
	if(ret != 1) throw udm_exception("SetIntValue(string): the input string could not be converted to an integer!");

	return SetIntValue(strAttrName, intval);
	
}
// Himanshu: integer array attribute setter
UDM_DLL bool Object::SetIntValues(string strAttrName, const vector<__int64>& values)
{
	// Getting Attributes from meta
  ::Uml::Class cls= type();
	set< ::Uml::Attribute> attrs=cls.attributes();	
	
	// Adding parent attributes
	set< ::Uml::Attribute> aattrs=Uml::AncestorAttributes(cls);
	attrs.insert(aattrs.begin(),aattrs.end());

	
	for(set< ::Uml::Attribute>::iterator ai = attrs.begin();ai != attrs.end(); ai++) 
	{
		string strCurrAttrName=ai->name();
		
		if(string(ai->type())=="Integer"&& strCurrAttrName==strAttrName)
		{
			setIntegerAttrArr(*ai, values);
			return true;
		}
	}
	// Could not find the attribute.
	return false;
}


// UDM TOMI Paradigm Independent Interface
// set the value of Attribute named strAttrName of type String
// return false if not found,true if ok
UDM_DLL bool Object::SetStrValue(string strAttrName, const string& value)
{
	// Getting Attributes from meta
	::Uml::Class cls= type();
	set< ::Uml::Attribute> attrs=cls.attributes();	
	
	// Adding parent attributes
	set< ::Uml::Attribute> aattrs=Uml::AncestorAttributes(cls);
	attrs.insert(aattrs.begin(),aattrs.end());

	
	for(set< ::Uml::Attribute>::iterator ai = attrs.begin();ai != attrs.end(); ai++) 
	{
		string strCurrAttrName=ai->name();
		
		if(( (string(ai->type())=="String") || (string(ai->type())=="Text") )&& strCurrAttrName==strAttrName)
		{
			setStringAttr(*ai, value);
			return true;
		}
	}
	// Could not find the attribute.
	return false;
};

UDM_DLL bool Object::SetStrValue(string strAttrName, const __int64& value)
{
	string strval;

	char tmp[50]; //the maximu __i64 is about 20 characters long
#ifdef _WIN32
	sprintf(tmp, "%I64d", value);
#else
	sprintf(tmp, "%lld", value);
#endif
	
	return SetStrValue(strAttrName, tmp);
}

// Himanshu: string array attribute setter
UDM_DLL bool Object::SetStrValues(string strAttrName, const vector<string>& values)
{
	// Getting Attributes from meta
  ::Uml::Class cls= type();
	set< ::Uml::Attribute> attrs=cls.attributes();	
	
	// Adding parent attributes
	set< ::Uml::Attribute> aattrs=Uml::AncestorAttributes(cls);
	attrs.insert(aattrs.begin(),aattrs.end());

	
	for(set< ::Uml::Attribute>::iterator ai = attrs.begin();ai != attrs.end(); ai++) 
	{
		string strCurrAttrName=ai->name();
		
		if(( (string(ai->type())=="String") || (string(ai->type())=="Text") )&& strCurrAttrName==strAttrName)
		{
			setStringAttrArr(*ai, values);
			return true;
		}
	}
	// Could not find the attribute.
	return false;
};



// UDM TOMI Paradigm Independent Interface
// set the value of Attribute named strAttrName of type Real
// return false if not found,true if ok
UDM_DLL bool Object::SetRealValue( string strAttrName, double value)
{
	// Getting Attributes from meta
	::Uml::Class cls= type();
	set< ::Uml::Attribute> attrs=cls.attributes();	
		
	// Adding parent attributes
	set< ::Uml::Attribute> aattrs=Uml::AncestorAttributes(cls);
	attrs.insert(aattrs.begin(),aattrs.end());

			
	for(set< ::Uml::Attribute>::iterator ai = attrs.begin();ai != attrs.end(); ai++) 
	{
		string strCurrAttrName=ai->name();
				
		if(string(ai->type())=="Real"&& strCurrAttrName==strAttrName)
		{
			setRealAttr(*ai, value);
			return true;
		}
	}
	// Could not find the attribute.
	return false;
};
// Himanshu: double array attribute setter
UDM_DLL bool Object::SetRealValues(string strAttrName, const vector<double>& values)
{
	// Getting Attributes from meta
  ::Uml::Class cls= type();
  set< ::Uml::Attribute> attrs=cls.attributes();	
		
	// Adding parent attributes
  set< ::Uml::Attribute> aattrs=Uml::AncestorAttributes(cls);
	attrs.insert(aattrs.begin(),aattrs.end());

			
  for(set< ::Uml::Attribute>::iterator ai = attrs.begin();ai != attrs.end(); ai++) 
	{
		string strCurrAttrName=ai->name();
				
		if(string(ai->type())=="Real"&& strCurrAttrName==strAttrName)
		{
			setRealAttrArr(*ai, values);
			return true;
		}
	}
	// Could not find the attribute.
	return false;
};

// UDM TOMI Paradigm Independent Interface
// set the value of Attribute named strAttrName of type Bool
// return false if not found,true if ok
UDM_DLL bool Object::SetBoolValue( string strAttrName, bool value)
{
	// Getting Attributes from meta
	::Uml::Class cls= type();
	set< ::Uml::Attribute> attrs=cls.attributes();	
	
	// Adding parent attributes
	set< ::Uml::Attribute> aattrs=Uml::AncestorAttributes(cls);
	attrs.insert(aattrs.begin(),aattrs.end());

	
	for(set< ::Uml::Attribute>::iterator ai = attrs.begin();ai != attrs.end(); ai++) 
	{
		string strCurrAttrName=ai->name();
		
		if(string(ai->type())=="Boolean"&& strCurrAttrName==strAttrName)
		{
			setBooleanAttr(*ai, value);
			return true;
		}
	}
	// Could not find the attribute.
	return false;
}

// Himanshu: bool array attribute setter
UDM_DLL bool Object::SetBoolValues(string strAttrName, const vector<bool>& values)
{
	// Getting Attributes from meta
  ::Uml::Class cls= type();
	set< ::Uml::Attribute> attrs=cls.attributes();	
	
	// Adding parent attributes
	set< ::Uml::Attribute> aattrs=Uml::AncestorAttributes(cls);
	attrs.insert(aattrs.begin(),aattrs.end());

	
	for(set< ::Uml::Attribute>::iterator ai = attrs.begin();ai != attrs.end(); ai++) 
	{
		string strCurrAttrName=ai->name();
		
		if(string(ai->type())=="Boolean"&& strCurrAttrName==strAttrName)
		{
			setBooleanAttrArr(*ai, values);
			return true;
		}
	}
	// Could not find the attribute.
	return false;
}




// UDM TOMI Paradigm Independent Interface
// Functions for composition
// Returns the parent object.
UDM_DLL Object Object::GetParent() const
{
	//object is this
	return impl->getParent(NULL);
	
};

// UDM TOMI Paradigm Independent Interface
// Retrieves all children not considering types and role names.
UDM_DLL set<Object> Object::GetChildObjects() const 
{
	//object is this
	set<Udm::Object> objChildren;
	vector<ObjectImpl*> objChildrenImpl=impl->getChildren(NULL,NULL);

	vector<ObjectImpl*>::iterator p_currChild=objChildrenImpl.begin();

	while(p_currChild!=objChildrenImpl.end())
	{
		objChildren.insert(*p_currChild);
		p_currChild++;
	}

	return objChildren;

};

// UDM TOMI Paradigm Independent Interface
// Retrieves all children considering child types but not role names.
UDM_DLL set<Object> Object::GetChildObjects(const ::Uml::Class &clsType) const
{
	//object is this
	//object is this
	set<Udm::Object> objChildren;
	vector<ObjectImpl*> objChildrenImpl=impl->getChildren(NULL,clsType);

	vector<ObjectImpl*>::iterator p_currChild=objChildrenImpl.begin();

	while(p_currChild!=objChildrenImpl.end())
	{
		objChildren.insert(*p_currChild);
		p_currChild++;
	}

	return objChildren;
};

// UDM TOMI Paradigm Independent Interface
// Retrieves all children considering role names and child types.
// To ignore child types set clsChildType to ClassType(NULL).
UDM_DLL set<Object> Object::GetChildObjects(const CompositionInfo& cmpType, const ::Uml::Class &clsChildType) const
{
	//object is this
	set<Udm::Object> objChildren;



	set< ::Uml::Class> ancestorClasses=::Uml::AncestorClasses(type());

	for(set< ::Uml::Class>::iterator p_currClass=ancestorClasses.begin();
					p_currClass!=ancestorClasses.end(); p_currClass++)
	{
		// Traversing the containment hierarchy
		set< ::Uml::CompositionParentRole> compParentRoles=p_currClass->parentRoles();
		for(set< ::Uml::CompositionParentRole>::iterator p_currRole=compParentRoles.begin();
			p_currRole!=compParentRoles.end(); p_currRole++)
		{
			
			string strParentRoleName=p_currRole->name();
			string strChildRoleName=::Uml::theOther(*p_currRole).name();

			// Checking the role name
			//ignore if supplied parentrole is empty
			//because it can be either empty or not empty, for the same diagram
			//when empty in the UML diagram, it will also be empty in the XML and thus
			//when working with dynamic meta, it will be empty
			//when working with static meta, the code generator will generate a rolename automatically (and also an access method)
			if ((cmpType.strParentRoleName != "") && (strParentRoleName!=cmpType.strParentRoleName)) continue;
			if(strChildRoleName!=cmpType.strChildRoleName) continue;

			
			// If everything is OK, get children
			// we just pass clsChildType to getChildren,
			// which will take care and will return only the matching children types(same or derived from)
			//
			vector<ObjectImpl*>children=impl->getChildren(::Uml::theOther(*p_currRole),clsChildType);
			
			for(vector<ObjectImpl*>::iterator p_currImpl=children.begin();
				p_currImpl!=children.end();p_currImpl++)
			{
				Udm::Object objChild(*p_currImpl);
				objChildren.insert(objChild);
			}
		}
	}			
	return objChildren;
};


// UDM TOMI Paradigm Independent Interface
// Get the composition child role the given child has in this parent. Sets the ret parameter to NULL if the given child is not actually a child.
UDM_DLL void Object::GetChildRole(const Object &child, ::Uml::CompositionChildRole &ret) const
{
	impl->getChildRole(child.__impl(), ret);
}


// UDM TOMI Paradigm Independent Interface
// Test if the object is in the tree rooted at where
UDM_DLL bool Object::IsNodeOfTree(const Object &where)
{
	//object is this
	Object parent = GetParent();

	while (parent) {
		if (parent == where)
			return true;
		parent = parent.GetParent();
	}

	return false;
};


// Functions for associations and links

// UDM TOMI Paradigm Independent Interface
// Gets the objects of the association class from between two objects. 
// ascType.clsAssociation  should be NULL to be ignored.
UDM_DLL set<Object> Object::GetAssociationClassObjects(Object dstObject, const AssociationInfo& ascType, bool tryCrosslink)
{
	//srcObject is this
	// Return value, initially NULL.
	set<Udm::Object> objsRet;

	::Uml::Class srcClass= type();
	set< ::Uml::Class> ancestorClasses=::Uml::AncestorClasses(srcClass);

	for(set< ::Uml::Class>::iterator p_currClass=ancestorClasses.begin();
				p_currClass!=ancestorClasses.end(); p_currClass++)
	{
		// Getting the association roles and iterating through them
		set< ::Uml::AssociationRole> assocRoles=p_currClass->associationRoles();
		for(set< ::Uml::AssociationRole>::iterator p_currAssocRole=assocRoles.begin();
					p_currAssocRole!=assocRoles.end();p_currAssocRole++)
		{
			::Uml::Class assocClass=::Uml::Association(p_currAssocRole->parent()).assocClass();

			bool	isAssocClass=assocClass?true:false;

			// We want to find assoc. class. If it does not exist...
			if(!isAssocClass) continue;

			// Checking the type given in ascType, ignoring if it was NULL
			if(ascType.clsAssociation!=::Uml::Class(NULL) && !Uml::IsDerivedFrom(ascType.clsAssociation,assocClass) ) continue;
	

			
			string strSrcRole=p_currAssocRole->name();
			::Uml::AssociationRole oRole = ::Uml::theOther(*p_currAssocRole);
			string strDstRole = oRole.name();

			if (ascType.clsAssociation)
			{
				if ((ascType.strSrcRoleName != "") && (strSrcRole !=ascType.strSrcRoleName))continue;
				if ((ascType.strDstRoleName != "") && (strDstRole !=ascType.strDstRoleName))continue;
			}

			//if((ascType.clsAssociation)&&(strSrcRole !=ascType.strSrcRoleName)) continue;


			//impls are cloned in getAssociation() call!
			vector<ObjectImpl*>dstPeers=impl->getAssociation(oRole,Udm::CLASSFROMTARGET);
	
			vector<ObjectImpl*>::iterator dstPeers_i = dstPeers.begin();
			
			while (dstPeers_i != dstPeers.end())
			{
				Udm::Object objRet = *dstPeers_i++;
				if (dstObject) 
				{
					pair<Object,Object> objPair = objRet.GetPeersFromAssociationClassObject();

					if(objPair.first==dstObject ||objPair.second==dstObject)
						objsRet.insert(objRet);
				}
				else
					objsRet.insert(objRet);
 
			}
		}
	}
	
	//additive recursion
	if (tryCrosslink && __impl()->__getdn()->GetProject() )
	{
		//this condition will block the recursion

		UdmProject * pr = __impl()->__getdn()->GetProject();
		
		//getting placeholder objects for the source and destination
		Object src_o = pr->GetPlaceHolder(*this, false);
		if (src_o && pr->HasCrossMeta())
		{

			Object dst_o;
			if (dstObject) dst_o = pr->GetPlaceHolder(dstObject, false);

			//it's ok for dst_o to be NULL
			//we have to translate ascType as well
			::Uml::Class newAscTypeCl;
			if (ascType.clsAssociation)
				newAscTypeCl = ::Uml::GetClassFromCrossDgr( *(pr->GetCrossMeta().dgr), ascType.clsAssociation );
			AssociationInfo newAscType(newAscTypeCl);

			newAscType.strSrcRoleName = ascType.strSrcRoleName;
			newAscType.strDstRoleName = ascType.strDstRoleName;


			set<Object> ret = src_o.GetAssociationClassObjects(dst_o,newAscType);
			for (set<Object>::iterator ret_i = ret.begin(); ret_i != ret.end(); ret_i++)
				objsRet.insert(pr->GetRealObject(*ret_i));
		
		}		
	}
	

	return objsRet;
};
		
// UDM TOMI Paradigm Independent Interface
// Gets the two peers from an object of association class type
UDM_DLL pair<Object,Object> Object::GetPeersFromAssociationClassObject()
{
	//objassocclass is this
	pair<Udm::Object,Udm::Object> objsRet;
	set< ::Uml::Class> ancestorClasses=::Uml::AncestorClasses(type());
	for(set< ::Uml::Class>::iterator p_currClass=ancestorClasses.begin();
				p_currClass!=ancestorClasses.end(); p_currClass++)
	{
		::Uml::Association Assoc =p_currClass->association();


		if(Assoc) 
		{
		
			set< ::Uml::AssociationRole> peerRoles=Assoc.roles();

			#ifdef _DEBUG
			assert(peerRoles.size()==2);
			#endif

			// Get first peer peer					
			vector<ObjectImpl*> firstPeers =impl->getAssociation(*peerRoles.begin(),Udm::TARGETFROMCLASS);

			#ifdef _DEBUG
			assert(firstPeers.size()==1);
			#endif

			Udm::Object objFirst=*firstPeers.begin();

			// Get first peer peer					
			vector<ObjectImpl*> secondPeers =impl->getAssociation(*peerRoles.rbegin(),Udm::TARGETFROMCLASS);

			#ifdef _DEBUG
			assert(secondPeers.size()==1);
			#endif

			Udm::Object objSecond=*secondPeers.begin();

			// Set up the pair and return
			objsRet.first=objFirst;
			objsRet.second=objSecond;

			//through the inheritence hierarchy, only one association class can exist
			continue;
		}
	}
	if ( !objsRet.first)
	{
		//try cross package
		if (__impl()->__getdn()->GetProject())
		{
			UdmProject * pr = __impl()->__getdn()->GetProject();
			Object src_o = pr->GetPlaceHolder(*this, false);
			if (src_o)
			{
				pair<Object, Object> ret = src_o.GetPeersFromAssociationClassObject();
				objsRet.first = pr->GetRealObject(ret.first);
				objsRet.second = pr->GetRealObject(ret.second);
			}
		};
	};


	return objsRet;
};


// UDM TOMI Paradigm Independent Interface
// For an association class instance, returns the source and, respectively, the
// destination, if the source role or destination role can be established (MGA backedn)
UDM_DLL	Object Object::getSrcObject()
{
	Object ret;

	set< ::Uml::Class> ancestorClasses = ::Uml::AncestorClasses(type());
	for (set< ::Uml::Class>::iterator p_currClass = ancestorClasses.begin();
				p_currClass != ancestorClasses.end(); p_currClass++)
	{
		::Uml::Association assoc = p_currClass->association();
		if (assoc) 
		{
			set< ::Uml::AssociationRole> peerRoles = assoc.roles();
			::Uml::AssociationRole role = *peerRoles.begin();
			
			struct AssociationRoleInfo roleInfo = __impl()->__getdn()->GetAssociationRoleInfo(role);
			if (roleInfo.has_direction) {
				vector<ObjectImpl*> srcPeers = roleInfo.is_src
					? impl->getAssociation(role, Udm::TARGETFROMCLASS)
					: impl->getAssociation(::Uml::theOther(role), Udm::TARGETFROMCLASS);
				ret = *srcPeers.begin();
			}

			//through the inheritence hierarchy, only one association class can exist
			continue;
		}
	}
	if ( ret == &Udm::_null )
	{
		//try cross package
		if (__impl()->__getdn()->GetProject())
		{
			UdmProject * pr = __impl()->__getdn()->GetProject();
			Object src_o = pr->GetPlaceHolder(*this, false);
			if (src_o)
			{
				Object ret_o = src_o.getSrcObject();
				ret = pr->GetRealObject(ret_o);
			}
		};
	}

	return ret;
}

UDM_DLL	Object Object::getDstObject()
{
	Object ret;

	set< ::Uml::Class> ancestorClasses = ::Uml::AncestorClasses(type());
	for (set< ::Uml::Class>::iterator p_currClass = ancestorClasses.begin();
				p_currClass != ancestorClasses.end(); p_currClass++)
	{
		::Uml::Association assoc = p_currClass->association();
		if (assoc) 
		{
			set< ::Uml::AssociationRole> peerRoles = assoc.roles();
			::Uml::AssociationRole role = *peerRoles.begin();
			
			struct AssociationRoleInfo roleInfo = __impl()->__getdn()->GetAssociationRoleInfo(role);
			if (roleInfo.has_direction) {
				vector<ObjectImpl*> dstPeers = !roleInfo.is_src
					? impl->getAssociation(role, Udm::TARGETFROMCLASS)
					: impl->getAssociation(::Uml::theOther(role), Udm::TARGETFROMCLASS);
				ret = *dstPeers.begin();
			}

			//through the inheritence hierarchy, only one association class can exist
			continue;
		}
	}
	if ( ret == &Udm::_null )
	{
		//try cross package
		if (__impl()->__getdn()->GetProject())
		{
			UdmProject * pr = __impl()->__getdn()->GetProject();
			Object src_o = pr->GetPlaceHolder(*this, false);
			if (src_o)
			{
				Object ret_o = src_o.getDstObject();
				ret = pr->GetRealObject(ret_o);
			}
		};
	}

	return ret;
}


// Functions for creating Udm entities

// UDM TOMI Paradigm Independent Interface
// Creates an object of clsType
/*
static Object Create(const ::Uml::Class &meta, const Object &parent, 
			const ::Uml::::Uml::CompositionChildRole &role, 
			const ObjectImpl * archetype = &_null,
			const bool subtype = false) 
		{ 
			return  __Create(meta, parent, role, archetype, subtype); 
		}
*/
//Object CreateObject(const ::Uml::Class & clsType, const Udm::Object& archetype = &Udm::_null, const bool subtype = false);
UDM_DLL Object Object::CreateObject(const ::Uml::Class & clsType, const Udm::Object& archetype, const bool subtype)
{
	Udm::Object objRet;
	//objectparent is this
	if (!impl->__getdn()->IsTypeSafe())
	{

		const ::Uml::Class & clsSafeType = Uml::SafeTypeContainer::GetSafeType(clsType);
		objRet=Udm::Object::Create(clsSafeType,*this,NULL, archetype.__impl(), subtype);
	}
	else
		objRet=Udm::Object::Create(clsType,*this,NULL,archetype.__impl(), subtype);

	if(objRet!=Udm::Object(NULL))
	{
		objRet.SetStrValue("name",clsType.name());	
	}
	return objRet;
};


UDM_DLL Object Object::CreateObject(const ::Uml::Class & clsType, const CompositionInfo& compType,const Udm::Object& archetype, const bool subtype)
{
	Udm::Object objRet;
	if( compType.strParentRoleName==""  && compType.strChildRoleName=="")
	{	return CreateObject(clsType);
	}
	//objectparent is this
	::Uml::Class srcClass= type();
	set< ::Uml::Class> ancestorClasses=::Uml::AncestorClasses(srcClass);

	for(set< ::Uml::Class>::iterator p_currClass=ancestorClasses.begin();
			p_currClass!=ancestorClasses.end(); p_currClass++)
	{
		// Getting the association roles and iterating through them
		set< ::Uml::CompositionParentRole> parentRoles=p_currClass->parentRoles();
		for(set< ::Uml::CompositionParentRole>::iterator p_currParentRole=parentRoles.begin();
							p_currParentRole!=parentRoles.end();p_currParentRole++)
		{
			::Uml::CompositionParentRole currParentRole = *p_currParentRole;
			::Uml::CompositionChildRole currChildRole =::Uml::theOther(currParentRole);
			string strParentRoleName = currParentRole.name();
			string strChildRoleName  = currChildRole.name();
			
			if( (compType.strParentRoleName=="" || strParentRoleName==compType.strParentRoleName) &&
			   (compType.strChildRoleName=="" || strChildRoleName==compType.strChildRoleName))
			{
				if (!impl->__getdn()->IsTypeSafe())
				{

					const ::Uml::Class & clsSafeType = Uml::SafeTypeContainer::GetSafeType(clsType);
					objRet=Udm::Object::Create(clsSafeType,*this, currChildRole,archetype.__impl(), subtype);
				} 
				else
					objRet=Udm::Object::Create(clsType,*this, currChildRole,archetype.__impl(), subtype);

				if(objRet!=Udm::Object(NULL))
				{
					objRet.SetStrValue("name",clsType.name());	
				}
				return objRet;
				
			}
							
		}
	}
	return objRet;
};

// UDM TOMI Paradigm Independent Interface
// Creates a link of a simple association or an association with association class. 
// If ascType.clsAssociation is not valid a simple association will be tried.
// On error results in false, true otherwise.
UDM_DLL bool Object::CreateLink(Object dstObject, const AssociationInfo& ascType, const bool direct)
{
	//srcObject is this
	bool bRetVal=false;

	// if no local associationroles are found, we will try cross-links
	bool tryCrossLink = true;

	::Uml::Class srcClass= type();
	set< ::Uml::Class> ancestorClasses=::Uml::AncestorClasses(srcClass);

	for(set< ::Uml::Class>::iterator p_currClass=ancestorClasses.begin();
			p_currClass!=ancestorClasses.end(); p_currClass++)
	{
		// Getting the association roles and iterating through them
		set< ::Uml::AssociationRole> assocRoles=p_currClass->associationRoles();
		for(set< ::Uml::AssociationRole>::iterator p_currAssocRole=assocRoles.begin();
							p_currAssocRole!=assocRoles.end();p_currAssocRole++)
		{
			string strSrcRoleName= p_currAssocRole->name();
			::Uml::AssociationRole oRole = ::Uml::theOther(*p_currAssocRole);
			string strDstRoleName = oRole.name();

			if(strSrcRoleName==ascType.strSrcRoleName &&
			   strDstRoleName==ascType.strDstRoleName)
			{

				tryCrossLink = false; //this works, no need to check the cross-link version
				::Uml::Class assocClass=::Uml::Association(p_currAssocRole->parent()).assocClass();

				
				vector<ObjectImpl*>srcPeers=impl->getAssociation(oRole, assocClass?Udm::CLASSFROMTARGET : Udm::TARGETFROMPEER);



				
				

				ObjectImpl * to_add = dstObject.__impl();
				srcPeers.push_back(to_add);		//setAssociation will clone it 

				//this will not work for association classes.
				//first, you create the assoc. class
				//then, you set the assoc. class association with at least two setAssociation calls
				//on at least two objects.

				impl->setAssociation(oRole,srcPeers, assocClass?Udm::CLASSFROMTARGET : Udm::TARGETFROMPEER,direct);

				//getassociation clone()-s it first

				vector<ObjectImpl*>::iterator srcPeers_i = srcPeers.begin();
				while (srcPeers_i != srcPeers.end()) 
				{
					ObjectImpl *oi = *srcPeers_i++;
					if (oi != to_add) oi->release();
				};
				
				bRetVal=true;
			}
		}
	}
	if (tryCrossLink)	//this will block the recursion
	{
		// we need to try cross-links as well

		//both object should be in the _same_ project

		if (__impl()->__getdn() && dstObject.__impl()->__getdn())
		{
			//this will also block the recursion
			//if (__impl()->__getdn() != dstObject.__impl()->__getdn())
			//{
				if (__impl()->__getdn()->GetProject() && dstObject.__impl()->__getdn()->GetProject() )
				{
					if (__impl()->__getdn()->GetProject() == dstObject.__impl()->__getdn()->GetProject() )
					{
						//both objects belong to the same project , different datanetworks between the same project
						//we have to get their placeholder objects,
						//and invoke CreateLink for them!

						UdmProject * pr = __impl()->__getdn()->GetProject();
						
						//getting placeholder objects for the source and destination
						Object src_o = pr->GetPlaceHolder(*this);
						Object dst_o = pr->GetPlaceHolder(dstObject);

						//as simple as that with a recursive step
						//because the names of the roles are not changed in the cross-meta
						return src_o.CreateLink(dst_o, ascType);

					}
				}
			//}
		}
		


	}
	return bRetVal;
};


UDM_DLL bool Object::DeleteLink(Object dstObject, const Object::AssociationInfo &ascType)
{	
	//srcObject is this
	bool bRetVal=false;

	// if no local associationroles are found, we will try cross-links
	bool tryCrossLink = true;

	::Uml::Class srcClass= type();
	set< ::Uml::Class> ancestorClasses=::Uml::AncestorClasses(srcClass);

	for(set< ::Uml::Class>::iterator p_currClass=ancestorClasses.begin();
			p_currClass!=ancestorClasses.end(); p_currClass++)
	{
		// Getting the association roles and iterating through them
		set< ::Uml::AssociationRole> assocRoles=p_currClass->associationRoles();
		for(set< ::Uml::AssociationRole>::iterator p_currAssocRole=assocRoles.begin();
							p_currAssocRole!=assocRoles.end();p_currAssocRole++)
		{
			string strSrcRoleName= p_currAssocRole->name();
			::Uml::AssociationRole oRole = ::Uml::theOther(*p_currAssocRole);
			string strDstRoleName = oRole.name();

			if(strSrcRoleName==ascType.strSrcRoleName &&
			   strDstRoleName==ascType.strDstRoleName)
			{
				tryCrossLink = false;

				::Uml::Class assocClass=::Uml::Association(p_currAssocRole->parent()).assocClass();

				
				vector<ObjectImpl*>srcPeers=impl->getAssociation(oRole, assocClass?Udm::CLASSFROMTARGET : Udm::TARGETFROMPEER);
				vector<ObjectImpl*> newsrcPeers;
				
				
				vector<ObjectImpl*>::iterator srcPeers_i = srcPeers.begin();

				while (srcPeers_i != srcPeers.end())
				{
					Object srcPeer_o(*srcPeers_i++);
					if (srcPeer_o != dstObject)
						newsrcPeers.push_back(srcPeer_o.__impl());
					else bRetVal = true;
				}

				impl->setAssociation(oRole,newsrcPeers, assocClass?Udm::CLASSFROMTARGET : Udm::TARGETFROMPEER);
				bRetVal=true;
			}
		}
	}

	if (tryCrossLink)	//this will block the recursion
	{
		// we need to try cross-links as well

		//both object should be in the _same_ project

		if (__impl()->__getdn() && dstObject.__impl()->__getdn())
		{
			//this will also block the recursion
			//if (__impl()->__getdn() != dstObject.__impl()->__getdn())
			//{
				if (__impl()->__getdn()->GetProject() && dstObject.__impl()->__getdn()->GetProject() )
				{
					if (__impl()->__getdn()->GetProject() == dstObject.__impl()->__getdn()->GetProject() )
					{
						//both objects belong to the same project , different datanetworks between the same project
						//we have to get their placeholder objects,
						//and invoke CreateLink for them!

						UdmProject * pr = __impl()->__getdn()->GetProject();
						
						//getting placeholder objects for the source and destination
						Object src_o = pr->GetPlaceHolder(*this, false);
						Object dst_o = pr->GetPlaceHolder(dstObject, false);

						//as simple as that with a recursive step
						//because the names of the roles are not changed in the cross-meta
						if (src_o && dst_o) return src_o.DeleteLink(dst_o, ascType);
						else return false;

					}
				}
			//}
		}
		
	}
	return bRetVal;
};

// Removes an object from the persistent storage.
UDM_DLL void Object::DeleteObject()
{
	impl->detach();
	impl->release();
	impl = &Udm::_null;
};

UDM_DLL Object Object::GetAssociationEnd(string roleName)
{
	//objassocclass is this
	Object objRet;

	bool tryCrossLink = true;

//	Association Assoc =type().association();

	::Uml::Association Assoc = ::Uml::GetAncestorAssociation(type());

	if(Assoc) 
	{
		set< ::Uml::AssociationRole> peerRoles=Assoc.roles();
		#ifdef _DEBUG
		assert(peerRoles.size()==2);
		#endif

		for(set< ::Uml::AssociationRole>::iterator i=peerRoles.begin();i!=peerRoles.end();i++)
		{
			::Uml::AssociationRole peerRole = *i;
			if((string)peerRole.name() == roleName)
			{
				tryCrossLink = false;//we found the association role

				vector<ObjectImpl*> assoPeers=impl->getAssociation(peerRole,Udm::TARGETFROMCLASS);
				#ifdef _DEBUG
				assert(assoPeers.size()==1);
				#endif
				objRet = *assoPeers.begin();
			}
		}
	}

	/* Recursive step for obtaining the same in the case of cross-links */
	if (tryCrossLink)//this will stop the recursion
	{	
		if (__impl()->__getdn()->GetProject())
		{
				UdmProject * pr = __impl()->__getdn()->GetProject();
				Object this_o = pr->GetPlaceHolder(*this, false);
				if (this_o) return pr->GetRealObject(this_o.GetAssociationEnd(roleName));
		}
	}
	return objRet;
};

UDM_DLL Object Object::FindCorrespondingObjectInSubtypes(const Object & where, const bool prim_derived) const
{	
	
	//set<Udm::Object> children = where.GetChildObjects();
	set<Udm::Object> deriveds = derived();

	set<Udm::Object>::iterator deriveds_i = deriveds.begin();
	while ( deriveds_i != deriveds.end())
	{
		if ( !prim_derived  || (prim_derived && deriveds_i->hasRealArchetype()))
		{
			
			//if ( (deriveds.find(*children_i) != deriveds.end())  && ( !prim_derived  || (prim_derived && children_i->hasRealArchetype())) )
			if (deriveds_i->GetParent() == where)
				return *deriveds_i;
		}
		deriveds_i++;

	}
	throw udm_exception("Could not find the corresponding subtype or instance in one of the parent's subtype/instance object.");
};

UDM_DLL Object Object::FindCorrespondingObjectInInstances(const Object & where, const bool prim_derived) const
{	
	
	
	//set<Udm::Object> children = where.GetChildObjects();
	set<Udm::Object> instances = this->instances();

	set<Udm::Object>::iterator instances_i = instances.begin();
	while ( instances_i != instances.end())
	{
		if ( !prim_derived  || (prim_derived && instances_i->hasRealArchetype()))
		{
			if (instances_i->GetParent() == where)
				return *instances_i;
		}

		//if ( (instances.find(*children_i) != instances.end())  && ( !prim_derived  || (prim_derived && children_i->hasRealArchetype())) )
		//	return *children_i;
		instances_i++;

	}
	throw udm_exception("Could not find the corresponding subtype or instance in one of the parent's subtype/instance object.");
};


UDM_DLL Object Object::FindCorrespondingObjectInInstancesTree(const Object& where, const bool prim_derived) const
{
	Udm::Object ret;
	
	set<Udm::Object> instances = this->instances();
	
	set<Udm::Object>::iterator inst_i = instances.begin();
	while (inst_i != instances.end())
	{
		if (!prim_derived  || (prim_derived && inst_i->hasRealArchetype()))
		{
			Udm::Object par = where;
			while (par)
			{
				if (par == *inst_i) return *inst_i;
				par = par.GetParent();
			};

		}
		inst_i++;
	};

	return ret;

}

UDM_DLL Object Object::FindCorrespondingObjectInSubtypesTree(const Object& where, const bool prim_derived) const
{
	Udm::Object ret;
	set<Udm::Object> deriveds = this->derived();
	set<Udm::Object>::iterator inst_i = deriveds.begin();
	while (inst_i != deriveds.end())
	{
		if (!prim_derived  || (prim_derived && inst_i->hasRealArchetype()))
		{
			Udm::Object par = where;
			while (par)
			{
				if (par == *inst_i) return *inst_i;
				par = par.GetParent();
			};

		}
		inst_i++;
	};

	return ret;

}


UDM_DLL set<Object> Object::GetMyParentsSubtypes(const bool only_primary_derived) const
{
	set<Object> ret;
	Object parent  = GetParent();
	if (parent && !only_primary_derived)  return parent.derived();

	if(parent)
	{
		set<Object> derived = parent.derived();
		set<Object>::iterator i = derived.begin();
		while ( i != derived.end())
		{
			if (i->hasRealArchetype()) ret.insert(*i);
			i++;
		}
	}
	return ret;
};

UDM_DLL set<Object> Object::GetMyParentsInstances(const bool only_primary_derived) const
{
	set<Object> ret;
	Object parent  = GetParent();
	if (parent && !only_primary_derived)  return parent.instances();

	if(parent)
	{
		set<Object> instances = parent.instances();
		set<Object>::iterator i = instances.begin();
		while ( i != instances.end())
		{
			if (i->hasRealArchetype()) ret.insert(*i);
			i++;
		}
	}
	return ret;
};
		

UDM_DLL bool Object::HasObjectSubtypes(const bool only_primary_derived ) const
{
	if (!only_primary_derived) return derived().size() > 0;
	set<Object> deriveds = derived();
	set<Object>::iterator derived_i = deriveds.begin();
	while(derived_i != deriveds.end())
	{
		if (derived_i->hasRealArchetype()) return true;
		derived_i++;
	}
	return false;
};

UDM_DLL bool Object::HasObjectInstances(const bool only_primary_derived ) const
{
	if (!only_primary_derived) return instances().size()  > 0;
	set<Object> my_instances = instances();
	set<Object>::iterator instances_i = my_instances.begin();
	while(instances_i != my_instances.end())
	{
		if (instances_i->hasRealArchetype()) return true;
		instances_i++;
	}
	return false;
};

UDM_DLL Object Object::FindClosestPrimarilyDerivedParent() const
{
	/*
		hasRealArchetype() will throw an exception if it is not inherited at all.
	*/
	if (hasRealArchetype()) return *this;
	
	Object ret = GetParent();

	while(ret)
	{
		if (ret.hasRealArchetype()) return ret;
		ret = ret.GetParent();
	}

	return ret;
};

//get a derived object of this in the tree rooted at where
UDM_DLL Object Object::getDerivedObjectInTree(const Udm::Object &where) const
{
	set<Object> children = where.GetChildObjects();
	set<Object> my_derived = derived();
	
	set<Object>::iterator c_i = children.begin();
	while (c_i != children.end())
	{
		Object o = *c_i++;
		set<Object>:: iterator search = my_derived.find(o);

		if (search != my_derived.end()) return *search;
 
	}

	c_i = children.begin();
	while (c_i != children.end())
	{
		Object o = *c_i++;
		Object ret = getDerivedObjectInTree(o);
		if (ret) return ret;
	}

	return &Udm::_null;
};

//check if which is within the same derived/instantiated block
UDM_DLL Object Object::getDerivedObjectInPrimarilyDerivedBlock(const Udm::Object & where) const
{
	//this will throw an exception if this is not derived&instantiated at all.
	// (expected behaviour, this should be either primarily derived, or inherited derived)

	Object derived_block =  where.FindClosestPrimarilyDerivedParent();
	set<Object> derived = this->derived();
	if (derived.find(derived_block) != derived.end()) return derived_block;
	return getDerivedObjectInTree(derived_block);
};


//finds an instantied object in of this in where
UDM_DLL Object Object::getInstantiatedObjectInTree(const Udm::Object &where) const
{
	set<Object> children = where.GetChildObjects();
	set<Object> my_instances = instances();
	
	set<Object>::iterator c_i = children.begin();
	while (c_i != children.end())
	{
		Object o = *c_i++;
		set<Object>:: iterator search = my_instances.find(o);

		if (search != my_instances.end()) return *search;
 
	}

	c_i = children.begin();
	while (c_i != children.end())
	{
		Object o = *c_i++;
		Object ret = getInstantiatedObjectInTree(o);
		if (ret) return ret;
	}

	return &Udm::_null;
};
//check if which is within the same derived/instantiated block
UDM_DLL Object Object::getInstantiatedObjectInPrimarilyDerivedBlock(const Udm::Object &where) const
{
		//this will throw an exception if this is not derived&instantiated at all.
	// (expected behaviour, this should be either primarily derived, or inherited derived)

	Object derived_block =  where.FindClosestPrimarilyDerivedParent();
	set<Object> instances = this->instances();
	if (instances.find(derived_block) != instances.end()) return derived_block;
	return getInstantiatedObjectInTree(derived_block);
};

//checks if the object is a reference object. 
//if so, and it's not an empty reference, returns the referred object
//Note: References in UDM does not really exist, it's merely an MGA concept.
//in UDM references are emulated with associations with rolename "ref" at the referenced object

UDM_DLL Object Object::getReferencedObject(const string refrole) const
{
	set< ::Uml::AssociationRole> aroles = Uml::AncestorAssociationTargetRoles(type());
	for (set< ::Uml::AssociationRole>::iterator aroles_i = aroles.begin(); aroles_i!= aroles.end(); aroles_i++)
	{
		::Uml::AssociationRole arole = *aroles_i;
		if ((string)arole.name() == refrole)
		{
			set<Object> referenced  = getAssociation(arole);
			if (referenced.size())
			{
				return *referenced.begin();
			}
		}		
	}


	return Udm::null;
};

// Getter functions based on cint_string
UDM_DLL bool Object::GetIntVal( const cint_string &cstrAttrName, long &value) const
{	__int64 val;
	bool ret = GetIntValue(cstrAttrName.buffer(), val);
	value = (long) val;
	return ret;
};
UDM_DLL bool Object::GetStrVal( const cint_string &cstrAttrName, cint_string &value) const
{	string val;
	bool ret = GetStrValue(cstrAttrName.buffer(), val);
	value.CopyFrom(val.c_str());
	return ret;
};
UDM_DLL bool Object::GetRealVal( const cint_string &cstrAttrName, double& value) const
{
	return GetRealValue(cstrAttrName.buffer(), value);

};
UDM_DLL bool Object::GetBoolVal( const cint_string &cstrAttrName, bool& value) const
{
	return GetBoolValue(cstrAttrName.buffer(), value);
};
UDM_DLL bool Object::GetBoolVal( const cint_string &cstrAttrName, long& value) const
{	bool val;
	bool ret = GetBoolValue(cstrAttrName.buffer(), val);
	value = (bool) val;
	return ret;
};

// Setter functions based on cint_string
UDM_DLL bool Object::SetIntVal( const cint_string &cstrAttrName, long value)
{
	return SetIntValue(cstrAttrName.buffer(), value);
};
UDM_DLL bool Object::SetStrVal( const cint_string &cstrAttrName, cint_string &value)
{	string val = value.buffer();
	return SetStrValue(cstrAttrName.buffer(), val);
};
UDM_DLL bool Object::SetStrVal( const cint_string &cstrAttrName, char *value)
{	string val = value;
	return SetStrValue(cstrAttrName.buffer(), val);
};
UDM_DLL bool Object::SetRealVal( const cint_string &cstrAttrName, double value)
{
	return SetRealValue(cstrAttrName.buffer(), value);

};
UDM_DLL bool Object::SetBoolVal( const cint_string &cstrAttrName, bool value)
{
	return SetBoolValue(cstrAttrName.buffer(), value);
};

/*UDM_DLL bool Object::CreateObject(const cint_string &type, Object &value )
{
	::Uml::Class kind = classByName(__impl()->__getdn()->GetRootMeta(), type.buffer());
	if (!kind) return false;

	value = CreateObject(kind);
	return true;
};

UDM_DLL bool Object::CreateLink(Object& dst, const AssociationInfo& ass)
{
	Class o_ass_info_ass_class;

	if (ass.assoc_class)
	{
		o_ass_info_ass_class = classByName(__impl()->__getdn()->GetRootMeta(), ass.assoc_class.buffer());
		if (!o_ass_info_ass_class) return false;
	}
	
	Udm::Object::AssociationInfo o_ass_info(o_ass_info_ass_class);
	o_ass_info.strDstRoleName = ass.DstRolename.buffer();
	o_ass_info.strSrcRoleName = ass.SrcRolename.buffer();

	return CreateLink(dstObject, o_ass_info);
};
*/

UDM_DLL string Object::getPath( const std::string& strDelimiter , bool bReverseOrder, bool bNeedRootFolder, const string &att_name, bool omit_lead_delim) const
{
		
	if (*this == impl->__getdn()->GetRootObject())
	{
		if (bNeedRootFolder)
		{	
			if (bReverseOrder)
				return UdmUtil::ExtractName(*this,att_name) + strDelimiter;
			else
			{
				if (omit_lead_delim)
					return UdmUtil::ExtractName(*this, att_name);
				else
					return strDelimiter + UdmUtil::ExtractName(*this, att_name);
			}
		}
		else 
		{
			if (omit_lead_delim) return "";
			else return strDelimiter; 
		}

	}
	else 
	{
		if (bReverseOrder)
			return UdmUtil::ExtractName(*this,att_name) + strDelimiter + GetParent().getPath(strDelimiter, bReverseOrder, bNeedRootFolder, att_name, omit_lead_delim);
		else
			return GetParent().getPath(strDelimiter, bReverseOrder, bNeedRootFolder, att_name, omit_lead_delim) + strDelimiter + UdmUtil::ExtractName(*this,att_name);

	}

};

UDM_DLL string Object::getPath2( const std::string& strDelimiter , bool bNeedRootFolder ) const
{
		
	if ((impl->__getdn() && *this == impl->__getdn()->GetRootObject()) || GetParent() == &Udm::_null)
	{
		if (bNeedRootFolder)
			return UdmUtil::ExtractName(*this, "name");
		else 
			return "";
	}
	else 
	{
		string path = GetParent().getPath2(strDelimiter, bNeedRootFolder);
		if (path.length())
			path += strDelimiter;	// parent could be the root folder, if bNeedRootFolder is
									// false then path to parent is empty and delimiter must not be added

		path += UdmUtil::ExtractName(*this, "name");
		return path;

	}

};

UDM_DLL int Object::depth_level() const
{
	if ((impl->__getdn() && *this == impl->__getdn()->GetRootObject()) || GetParent() == &Udm::_null)
		return 0;

	return 1 + GetParent().depth_level();
};

/*
	recursively obtains objects of a certain type down in the hierarchi rooted at this.
	The function uses a CompositionChildRole chain which was previously obtained by Uml::GetChildRoleChain() function.
*/
UDM_DLL set<Object> Object::getChildrenByChildRoleChain(const ::Uml::Class& meta, vector< ::Uml::CompositionChildRole> chain) const
{
	set<Object> ret;
	if (chain.size())
	{
		const ::Uml::CompositionChildRole ccr = *(chain.rbegin());
		chain.pop_back();

		vector<ObjectImpl*> children = impl->getChildren(ccr, &Udm::_null);
		for (vector<ObjectImpl*>::iterator i = children.begin(); i != children.end(); i++)
		{
			Object child = *i;
			set<Object> ret_set = child.getChildrenByChildRoleChain(meta, chain);
			if (ret_set.size()) ret.insert(ret_set.begin(), ret_set.end());
		}

		return ret;
	}
	else
	{
		if (Uml::IsDerivedFrom(type(), meta)) ret.insert(*this);
		
		return ret;
	}
};

