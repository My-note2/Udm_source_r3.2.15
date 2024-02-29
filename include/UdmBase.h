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
10/17/06	-	endre
		Added same-class assignment operator to helper classes used to access attributes. Now assignments like a.name() = b.name() are accepted by g++.

12/12/04	-	endre

		-	G++ 3 porting issues.

12/07/04	-	endre

		fixed trivial bugs in SmartDataNetwork CreateNewFromStr and OpenExistingFromString function.

12/06/04	-	endre

		added 
			-	GetAllInstances() function to Udm::DataNetwork class
			-	getChildrenByChildRoleChain() function to Udm::Object class

			See the descriptions of the files in the apropiate .cpp (UdmTomi.cpp, UdmBase.cpp) files.


22/09/04	-	endre

		added the DOM string functions to DataNetwork and SmartDataNetwork, so they are
		available for those using the SmartDataNetwork class to handle DOM datanetworks.

06/04/04	-	endre
		
		  addedd class BaseVisitor, and an Udm::Object::Accept(cont BaseVisitor&);

05/31/04	-	endre
			porting issues to Linux/GCC

05/24/04	-	endre

		added UDM_DLL to forward declarations of classes fromthe Uml namespace
04/22/04	-	endre

		added sorter template parameters on template classes Childrenattr<CLASS>, AssocAttr<CLASS>, ...
		The default parameter is class Pred = less<CLASS>

04/14/04	-	endre

		on Feng's request, changed the Udm::Object::CreateObject functions to support creating of instantiated/derived objects.

04/07/04	-	endre

		Added the Tomi functions to Udm::Object
		
		- GetStrValue() to _int64 -  
		- SetStrValue() from _int64 -
		- GetIntValue() to _string - 
		- SetIntValue() from string -


03/29/04	-	endre

		Made UdmProject: GetRealObject public because of TOMI access requirement

03/24/04	-	endre

		-	changed the signature of createchild(), added the bool flag need_safetype.
			If this bool flag is true, (and the object to be created has an archetype)
			the backend should create a safe copy of the type for each object

03/15/04	-	endre

		- added constants with the names of the internal attributes used in the Udm Dom backend

03/03/04 - endre
		
		- added Close() to StaticUdmproject, the project must be Closed before Individual Datanetworks are closed
		- added a StaticDataNetwork specifier which also expect DataNetwork (and not just StaticDataNetwork)

02/10/04 - endre

		added the generic container() function to class Udm::Object  

01/26/04 - endre

		Mainly porting issues to g++ 2.96, specially the compress/decompress stuff.
  		void type had to be add to all the functions declared with no return type

01/07/04	-	endre

		Changed Udm::StaticDataNetworkSpecifier to accept only SmartDataNetworks,
		this was required because of the difference between a SmartDataNetwork address
		and the address of a DataNetwork inside a SmartDataNetwork. 
		This problem was reported by Feng on 01/07/04


12/12/03	-	endre

		Udm::UdmProject::cross_delimiter - delimiter string for
		string delimiting the class name from the diagram name in the
		cross-class name

08/01/02	-	endre

		TOMI Paradigm Independent API ported from Tihamer's
		graph rewriting engine. See documentation for API description.

		Headers of API calls are added as member functions to 
		class Udm::Object

07/31/02	-	endre


		ChildrenAttr:
				
					- added cast operator for vector<CLASS> to preserve 
						the order of the children. This leaves all previous
						set<> assignments intact, except this note:

					
					- +=, -= operators on children now return vectors instead
						of sets. This should not be a problem when using a syntax 
						like this:

							lamp.children() += new_bulb;

						because there is a ChildrenAttr assignment operator 
						which takes a vector of object as input. 

						However, if any of these syntax is used, 
						the code will not compile:

						1:
							[...]
							set<Bulb> bulbs = lamp.children() += new_bulb;
							[...]
						
						2:
							[...]
							set<Bulb> AddBulbFunc(Bulb newBulb, Lamp hostLamp)
							{
								return hostLamp.children() += new_bulb;
							}
							[...]

		

						the sets should be changed to vectors, when using 
						the return value of the operators

						The change would be:
						
						1:
							[...]
							lamp.children() += new_bulb;
							set<Bulb> bulbs = lamp.children();
							[...]
						
						2:

							[...]
							set<Bulb> AddBulbFunc(Bulb newBulb, Lamp hostLamp)
							{
								lamp.children() += new_bulb;
								return lamp.children();
							}
							[...]


													

					

06/19/02	-	endre
	
	  Attribute array support: 
	  
		Added Attribute Array classes for both persistent and non-persistent
		attributes.

		Added array attribute setter/getter declarations for ObjectImpl, for both
		persistent and non-persistent attributes

		However, the persistent array attribute setters/getters
		are not abstract functions, a basic functionality is 
		provided at ObjectImpl level, but it can be overriden with
		a more performant one in the backend specific ObjectImpl class.
		See comments in UdmBase.cpp

*/
#ifndef MOBIES_UDMBASE_H
#define MOBIES_UDMBASE_H

#ifdef _WIN32
#pragma once
//stupid win32 warnings
#pragma warning ( disable : 4786 )
#pragma warning( disable : 4290 ) //C++ Exception Specification Ignored warning
#pragma warning( disable : 4503 ) //Decorated name length exceeded
#pragma warning( disable : 4103 ) //used #pragma pack to change alignment

#endif

#ifndef TYPENAME
#define TYPENAME typename
#endif

#ifndef MOBIES_ERRHAND_H
#include "ErrHand.h"
#endif



#include <set>
#include <vector>
#include <list>
#include <string>
#include <map>
#include <memory>

namespace Uml
{

	class UDM_DLL Diagram;
	class UDM_DLL Namespace;
	class UDM_DLL Class;
	class UDM_DLL Attribute;
	class UDM_DLL Association;
	class UDM_DLL AssociationRole;
	class UDM_DLL Composition;
	class UDM_DLL CompositionParentRole;
	class UDM_DLL CompositionChildRole;
	class UDM_DLL Constraint;
	class UDM_DLL ConstraintDefinition;
	UDM_DLL void Initialize();
	class SafeTypeContainer;
};

namespace UdmStatic
{
	class StaticDataNetwork;
	class StaticObject;
};

class UDM_DLL cint_string;

namespace UDM_NAMESPACE
{
	using namespace std;
// --------------------------- ObjectImpl

	// NOTE: All returned ObjectImpl pointers are refCounted (if not NULL),
	// they must be released, or passed to the Object constructor.

	static const int TARGETFROMPEER = 0;
	static const int TARGETFROMCLASS = 1;
	static const int CLASSFROMTARGET = 2;

	// From MGA
	typedef enum attstatus_enum {
        ATTSTATUS_HERE          = 0,
        ATTSTATUS_METADEFAULT   = -1,
        ATTSTATUS_UNDEFINED     = -2,
//AND:
        ATTSTATUS_IN_ARCHETYPE1 = 1,
        ATTSTATUS_IN_ARCHETYPE2 = 2,
        ATTSTATUS_IN_ARCHETYPE3 = 3,
        ATTSTATUS_IN_ARCHETYPE4 = 4,
// etc.
	} attstatus_enum;

	// Only the MGA backend knows the direction
	struct AssociationRoleInfo
	{
		bool has_direction;
		bool is_src;
	};


	class DataNetwork;
	class UdmProject;
	class StaticUdmProject;
	class udmvariant;

	typedef map<string, udmvariant> tmap;
	bool UDM_DLL IsDerivedFrom(const ::Uml::Class &derived, const ::Uml::Class &base);

	class UDM_DLL ObjectImpl;
	class UDM_DLL Object;

	typedef map<ObjectImpl *, ObjectImpl *> t_lib_to_copy_impl_map;
	typedef map<Object, Object> t_lib_to_copy_map;
	
	class UDM_DLL ObjectImpl
	{
	// --- reference counting

	public:
		virtual ObjectImpl *clone() = 0;
		virtual void release() = 0;
		virtual DataNetwork *__getdn() = 0;
		//virtual ~ObjectImpl() {};

	// --- type

	public:
		virtual const ::Uml::Class &type() const = 0;

	// --- order

	public:
		// every object in one project must return a unique uniqueId
#if defined(_WIN32) && defined(_M_AMD64)
		typedef size_t uniqueId_type;
#else
		typedef long uniqueId_type;
#endif

		virtual uniqueId_type uniqueId() const = 0;

	// --- attributes

	public:
		//no array versions
		virtual string getStringAttr(const ::Uml::Attribute &meta) const = 0;
		virtual void setStringAttr(const ::Uml::Attribute &meta, const string &a, const bool direct = true) = 0;

		virtual bool getBooleanAttr(const ::Uml::Attribute &meta) const = 0;
		virtual void setBooleanAttr(const ::Uml::Attribute &meta, bool a, const bool direct = true) = 0;

		virtual __int64 getIntegerAttr(const ::Uml::Attribute &meta) const = 0;
		virtual void setIntegerAttr(const ::Uml::Attribute &meta, __int64 a, const bool direct = true) = 0;

		virtual double getRealAttr(const ::Uml::Attribute &meta) const = 0;
		virtual void setRealAttr(const ::Uml::Attribute &meta, double a, const bool direct = true) = 0;

		//array versions

		virtual vector<string> getStringAttrArr(const ::Uml::Attribute &meta) const;
		virtual void setStringAttrArr(const ::Uml::Attribute &meta, const vector<string> &a, const bool direct = true);

		virtual vector<bool> getBooleanAttrArr(const ::Uml::Attribute &meta) const;
		virtual void setBooleanAttrArr(const ::Uml::Attribute &meta, const vector<bool> &a, const bool direct = true);

		virtual vector<__int64> getIntegerAttrArr(const ::Uml::Attribute &meta) const;
		virtual void setIntegerAttrArr(const ::Uml::Attribute &meta, const vector<__int64> &a, const bool direct = true) ;

		virtual vector<double> getRealAttrArr(const ::Uml::Attribute &meta) const;
		virtual void setRealAttrArr(const ::Uml::Attribute &meta, const vector<double> &a, const bool direct = true);


		virtual long getAttrStatus(const ::Uml::Attribute &meta) const = 0;



	// --- containment

	public:

// get the parent object or null if object has no parent
// if role is not NULLROLE, return parent only if the role actually applies
		virtual ObjectImpl *getParent(const ::Uml::CompositionParentRole &role) const = 0;	// can return NULL

// role must be specified unless a is null, or child role in the new parent is unambigious
// if a was already the parent, existing other childroles are not deleted
// XML: if multiple roles are enabled, 
//		object will keep its existing role(s) as well if not moved to a new parent
//		if a = NULL and role != null, the role is removed from the containment relation (if it was there), 
//									   but the object will only be deleted, if the role specified was its only one
		virtual void setParent(ObjectImpl *a, const ::Uml::CompositionParentRole &role, const bool direct = true) = 0;

// set parent to null; equivalent to setParent(NULL, NULL);
		virtual void detach() = 0;

// if role is given, return only those children that have that role (ignore kind)
// else if kind is not null, return all children which are compatible with kind
// else if kind is null, return all children
		virtual vector<ObjectImpl*> getChildren(const ::Uml::CompositionChildRole &role, const ::Uml::Class &kind) const = 0;

// if role is non-null. 
//		All children omitted from the list lose this role and may also
//		become detached if this was their only role.
// if role is null
//		All existing children are removed and new children are added only if their containment role is unambigious
//	XML: if multiple roles are enabled, new children with this role may already have other roles as well.

// direct specifies if the call is coming from user code (true), or is a recursive call
// which was invoked because this operation was invoked on an archetype
		virtual void setChildren(const ::Uml::CompositionChildRole &meta, const vector<ObjectImpl*> &a, const bool direct = true) = 0;


// role must be specified unless child role in the new parent is unambigious

		/*
				if there is an archetype, the 3rd argument is not null
				if arg exists,
					if subtype is false, instance is created (default behaviour)
					if subtype is true, subtype/derived object is created

				if real_archetype is false, the child being created is created automatically,
				upon createChild() of the parent, because the parent is an instace/derived
				of an archetype which has children
		*/
		virtual ObjectImpl *createChild(const ::Uml::CompositionChildRole &role, 
			const ::Uml::Class &meta,
			const ObjectImpl* arch = NULL, 
			const bool subtype = false,
			const bool real_archetype = true,
			const bool need_safetype = false) = 0;

		/* 
			should be invoked when an object has an archetype
			returns false if the object is there because one of the parent inherited it
			returns true if the object was instantiated/derived directly, from an archetype
		*/
		virtual bool hasRealArchetype() const = 0;

		virtual bool isSubtype() const = 0;
		virtual bool isInstance() const = 0;
	// --- associations

	public:
// arole must be non-null. 
// Associations with names on both sides are navigable from both directions.
		virtual vector<ObjectImpl*> getAssociation(const ::Uml::AssociationRole &arole, int mode = TARGETFROMPEER) const = 0;
// arole must be non-null. 
// Existing associations omitted from the new list are 
// deleted, along with their association classes.
// Associations created from either side are considered equal


		virtual void setAssociation(
			const ::Uml::AssociationRole &arole, 
			const vector<ObjectImpl*> &a, 
			int mode = TARGETFROMPEER,
			const bool direct = true) = 0;	
		//if direct is false, the link to be created is an inherited link
		//that exists between the children of my parent's archetype
		//and needs to be copied here 

	//archetype/derived relationships
		virtual vector<ObjectImpl*> getDerived() const = 0;
		virtual vector<ObjectImpl*> getInstances() const = 0;
		virtual ObjectImpl* getArchetype() const = 0;
	
// temporary values
		tmap &locatemap(bool create) const;

		//single value attributes

		string getTempStringAttr(const ::Uml::Attribute &meta) const;
		void setTempStringAttr(const ::Uml::Attribute &meta, const string &a, const bool direct = true);
		__int64 getTempIntegerAttr(const ::Uml::Attribute &meta) const;
		void setTempIntegerAttr(const ::Uml::Attribute &meta, const __int64 &a, const bool direct = true);
		double getTempRealAttr(const ::Uml::Attribute &meta) const;
		void setTempRealAttr(const ::Uml::Attribute &meta, const double &a, const bool direct = true);
		bool getTempBooleanAttr(const ::Uml::Attribute &meta) const;
		void setTempBooleanAttr(const ::Uml::Attribute &meta, const bool &a, const bool direct = true);

		long getTempAttrStatus(const ::Uml::Attribute &meta) const;

		//multiple value attributes

		vector<string> getTempStringAttrArr(const ::Uml::Attribute &meta) const;
		void setTempStringAttrArr(const ::Uml::Attribute &meta, const vector<string> &a, const bool direct = true);
		vector<__int64> getTempIntegerAttrArr(const ::Uml::Attribute &meta) const;
		void setTempIntegerAttrArr(const ::Uml::Attribute &meta, const vector<__int64> &a, const bool direct = true);
		vector<double> getTempRealAttrArr(const ::Uml::Attribute &meta) const;
		void setTempRealAttrArr(const ::Uml::Attribute &meta, const vector<double> &a, const bool direct = true);
		vector<bool> getTempBooleanAttrArr(const ::Uml::Attribute &meta) const;
		void setTempBooleanAttrArr(const ::Uml::Attribute &meta, const vector<bool> &a, const bool direct = true);

		//for new Objects, set the default values for attributes
		//this will be invoked whenever creating a new ObjectImpl instance
		//in each backend
		//if all is false, only non-persistent attributes are set to default values
		//MEM backend has an optimized version for default values.
		void setDefaultAttributes(const bool all = true);

		//atribute copier functions
		void CopyAttributeFrom(const ::Uml::Attribute& which, const ObjectImpl* from, bool direct = true);
		void CopyAttributesFrom(const ObjectImpl*  from, bool direct = true);
		void CopyAttributeFromArchetype(const ::Uml::Attribute& which);
		void CopyAttributesFromArchetype();

		// libraries
		// as in GME, the root folder of the library
		// is not a library object (when the library is attached
		// to the host, the root folder of the library is replaced
		// by a child belonging to the host)
		virtual bool isLibObject() const = 0;
		virtual bool isLibRoot() const = 0;
		// if the object is a library root, then the method will
		// copy the library name into the parameter and will return
		// true, otherwise the method will return false and will
		// leave the parameter unchanged
		virtual bool getLibraryName(string &name) const = 0;
		virtual void setLibraryName(const char *name) = 0;
		virtual ObjectImpl* AttachLibrary(ObjectImpl *lib_src, const string &lib_name, t_lib_to_copy_impl_map *copy_map = NULL);
		virtual ObjectImpl *createLibRootChild(const ::Uml::Class &meta, const bool need_safetype = false) = 0;
		virtual vector<ObjectImpl*> getLibRoots() const = 0;

		
		// association with reference ports
		virtual void connectTo(
			const ::Uml::AssociationRole &meta,
			ObjectImpl* target,
			const vector<ObjectImpl*> &refs = vector<ObjectImpl*>());
		virtual void disconnectFrom(const ::Uml::AssociationRole &meta, ObjectImpl* peer);
		virtual vector<ObjectImpl*> getConnectingChain(const ::Uml::AssociationRole &meta, const ObjectImpl* peer) const;

		// utilities
		// root folder is added by default, no reverse order suport, delimiter is never added to the front
		virtual string getPath( const string &strDelimiter = "/", bool bNeedRootFolder = true ) const;
		virtual string toString() const;

		/*
			If c is a child of this object, then set ret to be the role taken by the child,
			otherwise set ret to null.
		 */
		virtual void getChildRole(ObjectImpl *c, ::Uml::CompositionChildRole &ret) const = 0;

	};


	typedef map<ObjectImpl::uniqueId_type, tmap> ttmap;
	
	//non-persistent, single-value attribute objects
	class UDM_DLL TempStringAttr
	{
	private:
		ObjectImpl *impl;
		const ::Uml::Attribute &meta;

	public:
		TempStringAttr(ObjectImpl *i, const ::Uml::Attribute &m) : impl(i), meta(m) { }

		operator string() const { return impl->getTempStringAttr(meta); }
		TempStringAttr &operator =(const string &a) { impl->setTempStringAttr(meta, a); return *this; }

		TempStringAttr &operator =(const TempStringAttr &a) { return operator =( string(a) ); }
	};



	class UDM_DLL TempIntegerAttr
	{
	private:
		ObjectImpl *impl;
		const ::Uml::Attribute &meta;

	public:
		TempIntegerAttr(ObjectImpl *i, const ::Uml::Attribute &m) : impl(i), meta(m) { }

		operator __int64() const { return impl->getTempIntegerAttr(meta); }
		TempIntegerAttr &operator =(const __int64 &a) { impl->setTempIntegerAttr(meta, a); return *this; }

		TempIntegerAttr &operator =(const TempIntegerAttr &a) { return operator =( (__int64) a ); }
	};


	class UDM_DLL TempRealAttr
	{
	private:
		ObjectImpl *impl;
		const ::Uml::Attribute &meta;

	public:
		TempRealAttr(ObjectImpl *i, const ::Uml::Attribute &m) : impl(i), meta(m) { }

		operator double() const { return impl->getTempRealAttr(meta); }

		TempRealAttr &operator =(const double &a) { impl->setTempRealAttr(meta, a); return *this; }

		TempRealAttr &operator =(const TempRealAttr &a) { return operator =( double(a) ); }
	};


	class UDM_DLL TempBooleanAttr
	{
	private:
		ObjectImpl *impl;
		const ::Uml::Attribute &meta;

	public:
		TempBooleanAttr(ObjectImpl *i, const ::Uml::Attribute &m) : impl(i), meta(m) { }

		operator bool() const { return impl->getTempBooleanAttr(meta); }
		TempBooleanAttr &operator =(const bool &a) { impl->setTempBooleanAttr(meta, a); return *this; }

		TempBooleanAttr &operator =(const TempBooleanAttr &a) { return operator =( bool(a) ); }
	};
	




// --------------------------- null;

	class Object;
	extern UDM_DLL Object null;

	class NullObject : public ObjectImpl
	{
	protected:
		static udm_exception e;

	public:
		inline NullObject();
		// in Object::Object(...) we assume this refererence counting behaviour
		virtual ObjectImpl *clone() { return this; }
		virtual void release() { } 
		virtual DataNetwork *__getdn() { throw e; }
		virtual string toString() const { return "NullObject"; }

		virtual const ::Uml::Class &type() const { throw e; }

		virtual string getStringAttr(const ::Uml::Attribute &meta) const { throw e; }
		virtual void setStringAttr(const ::Uml::Attribute &meta, const string &a, const bool direct = true) { throw e; }

		virtual bool getBooleanAttr(const ::Uml::Attribute &meta) const { throw e; }
		virtual void setBooleanAttr(const ::Uml::Attribute &meta, bool a, const bool direct = true) { throw e; }

		virtual __int64 getIntegerAttr(const ::Uml::Attribute &meta) const { throw e; }
		virtual void setIntegerAttr(const ::Uml::Attribute &meta, __int64 a, const bool direct = true) { throw e; }

		virtual double getRealAttr(const ::Uml::Attribute &meta) const { throw e; }
		virtual void setRealAttr(const ::Uml::Attribute &meta, double a, const bool direct = true) { throw e; }

		virtual vector<string> getStringAttrArr(const ::Uml::Attribute &meta) const {throw e;};
		virtual void setStringAttrArr(const ::Uml::Attribute &meta, const vector<string> &a, const bool direct = true) {throw e;};

		virtual vector<bool> getBooleanAttrArr(const ::Uml::Attribute &meta) const {throw e;};
		virtual void setBooleanAttrArr(const ::Uml::Attribute &meta, const vector<bool> &a, const bool direct = true) {throw e;};

		virtual vector<__int64> getIntegerAttrArr(const ::Uml::Attribute &meta) const {throw e;};
		virtual void setIntegerAttrArr(const ::Uml::Attribute &meta, const vector<__int64> &a, const bool direct = true) {throw e;};

		virtual vector<double> getRealAttrArr(const ::Uml::Attribute &meta) const {throw e;};
		virtual void setRealAttrArr(const ::Uml::Attribute &meta, const vector<double> &a, const bool direct = true) {throw e;};

		virtual long getAttrStatus(const ::Uml::Attribute &meta) const {throw e;};



		virtual ObjectImpl *getParent(const ::Uml::CompositionParentRole &role) const { throw e; }
		virtual void setParent(ObjectImpl *a, const ::Uml::CompositionParentRole &role, const bool direct = true) { throw e; }
		virtual void detach() { throw e; }

		virtual vector<ObjectImpl*> getChildren(const ::Uml::CompositionChildRole &meta, const ::Uml::Class &kind) const { throw e; }
		virtual void setChildren(const ::Uml::CompositionChildRole &meta, const vector<ObjectImpl*> &a, const bool direct = true) { throw e; }

		virtual ObjectImpl *createChild(const ::Uml::CompositionChildRole &role,
			const ::Uml::Class &meta,
			const ObjectImpl* archetype = NULL,
			const bool subtype = false,
			const bool real_archetype = true,
			const bool need_safetype = false) { throw e; }

		virtual void getChildRole(ObjectImpl *c, ::Uml::CompositionChildRole &ret) const { throw e; }

		virtual vector<ObjectImpl*> getAssociation(const ::Uml::AssociationRole &meta, int mode = TARGETFROMPEER) const { throw e; }
		virtual void setAssociation(
			const ::Uml::AssociationRole &meta, 
			const vector<ObjectImpl*> &a, 
			int mode = TARGETFROMPEER, 
			const bool direct = true) { throw e; }

		virtual void connectTo(
			const ::Uml::AssociationRole &meta,
			ObjectImpl* target,
			const vector<ObjectImpl*> &refs = vector<ObjectImpl*>()) { throw e; }
		virtual void disconnectFrom(const ::Uml::AssociationRole &meta, ObjectImpl* peer) { throw e; }
		virtual vector<ObjectImpl*> getConnectingChain(const ::Uml::AssociationRole &meta, const ObjectImpl* peer) const { throw e; }

		virtual uniqueId_type uniqueId() const { return 0; }
		
		virtual vector<ObjectImpl*> getDerived() const {throw e;};
		virtual vector<ObjectImpl*> getInstances() const {throw e;};
		virtual ObjectImpl* getArchetype() const {throw e;};
		virtual bool hasRealArchetype() const {throw e;};
		virtual bool isSubtype() const {throw e;};
		virtual bool isInstance() const {throw e;};
	
		virtual bool isLibObject() const {throw e;};
		virtual bool isLibRoot() const {throw e;};
		virtual bool getLibraryName(string &name) const {throw e;};
		virtual void setLibraryName(const char *name) {throw e;};
		virtual ObjectImpl* AttachLibrary(ObjectImpl *lib_src, const string &lib_name, t_lib_to_copy_impl_map *copy_map = NULL) {throw e;};
		virtual ObjectImpl *createLibRootChild(const ::Uml::Class &meta, const bool need_safetype = false) {throw e;};
		virtual vector<ObjectImpl*> getLibRoots() const {throw e;};

	};

	extern UDM_DLL NullObject _null;

	inline NullObject::NullObject() { UDM_ASSERT(this == &_null); }
  
	extern UDM_DLL const ::Uml::CompositionChildRole NULLCHILDROLE;
	extern UDM_DLL const ::Uml::CompositionParentRole NULLPARENTROLE;

// single-value(no-array) versions of Attribute objects

// --------------------------- StringAttr

	class UDM_DLL StringAttr
	{
	private:
		ObjectImpl *impl;
		const ::Uml::Attribute &meta;

	public:
		StringAttr(ObjectImpl *i, const ::Uml::Attribute &m) : impl(i), meta(m) { }

		operator string() const { return impl->getStringAttr(meta); }

		StringAttr &operator =(const string &a) { impl->setStringAttr(meta, a); return *this; }

		StringAttr &operator =(const StringAttr &a) { return operator =( string(a) ); }

		bool operator==(const StringAttr &a) const { return static_cast<string>(*this) == static_cast<string>(a); }

		bool operator==(const string &a) const { return static_cast<string>(*this) == a; }

		bool operator==(const char* const a) const { return static_cast<string>(*this) == a; }
	};

// --------------------------- BooleanAttr

	class UDM_DLL BooleanAttr
	{
	private:
		ObjectImpl *impl;
		const ::Uml::Attribute &meta;

	public:
		BooleanAttr(ObjectImpl *i, const ::Uml::Attribute &m) : impl(i), meta(m) { }

		operator bool() const { return impl->getBooleanAttr(meta); }

		BooleanAttr &operator =(bool a) { impl->setBooleanAttr(meta, a); return *this; }

		BooleanAttr &operator =(const BooleanAttr &a) { return operator =( bool(a) ); }
	};

// --------------------------- IntegerAttr

	class UDM_DLL IntegerAttr
	{
	private:
		ObjectImpl *impl;
		const ::Uml::Attribute &meta;

	public:
		IntegerAttr(ObjectImpl *i, const ::Uml::Attribute &m) : impl(i), meta(m) { }

		//operator long() const { return impl->getIntegerAttr(meta); }
		operator __int64() const { return impl->getIntegerAttr(meta); }

		//IntegerAttr operator =(long a) { impl->setIntegerAttr(meta, a); return *this; }
		IntegerAttr &operator =(__int64 a) { impl->setIntegerAttr(meta, a); return *this; }

		IntegerAttr &operator =(const IntegerAttr &a) { return operator =( (__int64) a ); }
	};

// --------------------------- RealAttr

	class UDM_DLL RealAttr
	{
	private:
		ObjectImpl *impl;
		const ::Uml::Attribute &meta;

	public:
		RealAttr(ObjectImpl *i, const ::Uml::Attribute &m) : impl(i), meta(m) { }

		operator double() const { return impl->getRealAttr(meta); }

		RealAttr &operator =(double a) { impl->setRealAttr(meta, a); return *this; }

		RealAttr &operator =(const RealAttr &a) { return operator =( double(a) ); }
	};

// multiple-value(array) versions of Attribute objects
/*
	Array element template class to be returned by [] op,
	implementing the basic cast and assignment operators
*/	

	bool UDM_DLL CheckAttributeOrdering(const ::Uml::Attribute & att);

	template <class CLASS>
	class ArrAttrItem
	{
		
	protected:
		
		const ::Uml::Attribute & meta;			//attribute to be altered
		vector<CLASS> array;		//vector with array elements
		ObjectImpl * object;	//object this array attribute belongs to
		unsigned int index;					//iterators can not be stored
									//however, most of the code is not duplicated
		virtual void set() = 0;
	
	public:
		
		ArrAttrItem(const vector<CLASS>& array_ref, const int  a_i,  ObjectImpl * oi, const ::Uml::Attribute & m):  meta(m), array(array_ref), index (a_i)
		{
			object = oi;
			if (!object)
				throw udm_exception("Object pointer is NULL");
		};
		operator CLASS() const 
		{
			if (array.size() > index) return array.at(index); 
			else return CLASS();
		};

		CLASS operator*(){return (CLASS)*this;};
		

		virtual ArrAttrItem& operator =(const CLASS &a)
		{
			//pad with CLASS() if array is smaller than index
			while (array.size() <=index)
				array.push_back(CLASS());

			array.at(index) = a;

			set();
			return *this;
		};

		virtual ArrAttrItem& operator =(const ArrAttrItem<CLASS> &a)
		{
			if (this != &a)
			{
				array = a.array;
				index = a.index;
				set();
			}
			return *this;
		};

		virtual bool operator==(const ArrAttrItem<CLASS>& a)
		{
			if (&array != &a.array)
				throw udm_exception("Comparision of two unrelated iterators!");
			
			return (index == a.index);
		};

		virtual bool operator!=(const ArrAttrItem<CLASS>& a)
		{
			
			return !(*this == a);
		};




		//iterator-like mechanism, inline postfix routine
		virtual ArrAttrItem& operator++(int){index++; return *this;};

		
		void CheckOrdering()
		{
			if (CheckAttributeOrdering(meta))
					throw udm_exception("Items at a specified position in ordered attributes can not be set directly!");
		}

		virtual ~ArrAttrItem(){};

	};

#if __GNUC__ >= 4
	// starting with gcc 4.0, a std::_Bit_reference can't be casted
	// to bool&, so we need this template specialization

	template <>
	class ArrAttrItem <bool>
	{
		
	protected:
		
		const ::Uml::Attribute & meta;			//attribute to be altered
		vector<bool> array;		//vector with array elements
		ObjectImpl * object;	//object this array attribute belongs to
		unsigned int index;					//iterators can not be stored
									//however, most of the code is not duplicated
		virtual void set() = 0;
	
	public:
		
		ArrAttrItem(const vector<bool>& array_ref, const int  a_i,  ObjectImpl * oi, const ::Uml::Attribute & m):  meta(m), array(array_ref), index (a_i)
		{
			object = oi;
			if (!object)
				throw udm_exception("Object pointer is NULL");
		};
		operator bool() const 
		{
			if (array.size() > index) return array.at(index); 
			else return bool();
		};

		bool operator*(){return (bool)*this;};
		

		virtual ArrAttrItem& operator =(const bool &a)
		{
			//pad with bool() if array is smaller than index
			while (array.size() <=index)
				array.push_back(bool());

			//std::_Bit_reference i = (std::_Bit_reference)array.at(index);
			//i = a;
			array.at(index)= a;
			set();
			return *this;
		};

		virtual ArrAttrItem& operator =(const ArrAttrItem<bool> &a)
		{
			if (this != &a)
			{
				array = a.array;
				index = a.index;
				set();
			}
			return *this;
		};

		virtual bool operator==(const ArrAttrItem<bool>& a)
		{
			if (&array != &a.array)
				throw udm_exception("Comparision of two unrelated iterators!");
			
			return (index == a.index);
		};

		virtual bool operator!=(const ArrAttrItem<bool>& a)
		{
			
			return !(*this == a);
		};




		//iterator-like mechanism, inline postfix routine
		virtual ArrAttrItem& operator++(int){index++; return *this;};

		
		void CheckOrdering()
		{
			if (CheckAttributeOrdering(meta))
					throw udm_exception("Items at a specified position in ordered attributes can not be set directly!");
		}

		virtual ~ArrAttrItem(){};

	};
#endif // __GNUC__ >= 4

	/*
		Array element template class which also implements the
		+= operator for long, string, real types
	*/
	template <class CLASS>
	class AdditiveArrAttrItem : public ArrAttrItem<CLASS>
	{
	public:
		AdditiveArrAttrItem(const vector<CLASS>& array_ref, const int  a_i,  ObjectImpl * oi, const ::Uml::Attribute & m)
			:ArrAttrItem<CLASS>(array_ref, a_i, oi, m){};
		
		virtual AdditiveArrAttrItem& operator +=(const CLASS &a)
		{
			//pad with CLASS() if array is smaller than index
			while (this->array.size() <= this->index)
				this->array.push_back(CLASS());

			CLASS & i = this->array.at(this->index);
			i += a;
			this->set();
			return *this;
		};

		virtual AdditiveArrAttrItem<CLASS>& operator =(const CLASS &a)
		{
			ArrAttrItem<CLASS>::operator =(a);
			return *this;
		}

		virtual AdditiveArrAttrItem<CLASS>& operator =(const AdditiveArrAttrItem<CLASS> &a)
		{
			ArrAttrItem<CLASS>::operator =(a);
			return *this;
		}

		virtual ~AdditiveArrAttrItem(){};
	};

	//persistent array attribute element(iterator) classes
	// string array element providing operations with const char *
	class StringArrAttrItem : public AdditiveArrAttrItem<string>
	{
	public:	
		StringArrAttrItem(const vector<string>& array_ref, const int  a_i,  ObjectImpl * oi, const ::Uml::Attribute & m)
			:AdditiveArrAttrItem<string>(array_ref, a_i, oi, m){};
		void set()
		{
			CheckOrdering();		//this will crash if array is ordered
			//ordered attributes are not supposed to be changed in a by-item fashion

			object->setStringAttrArr(meta, array);
		};
		
		//provide these operators for const char * as well
		StringArrAttrItem& operator =(const char * a)
		{
			AdditiveArrAttrItem<string>::operator=(string(a));
			return *this;
		};

		StringArrAttrItem& operator += (const char * a)
		{
			AdditiveArrAttrItem<string>::operator +=(string(a));
			return *this;
		};
		
	};

	class RealArrAttrItem : public AdditiveArrAttrItem<double>
	{
	public:	
		RealArrAttrItem(const vector<double>& array_ref, const int  a_i,  ObjectImpl * oi, const ::Uml::Attribute & m)
			:AdditiveArrAttrItem<double>(array_ref, a_i, oi, m){};
		void set()
		{
			CheckOrdering();		//this will crash if array is ordered
			//ordered attributes are not supposed to be changed in a by-item fashion

			object->setRealAttrArr(meta, array);
		};
		
		//provide these operators for long, int, float, char as well
		RealArrAttrItem& operator=  (const int a)	{AdditiveArrAttrItem<double>::operator =((double)a); return *this;};
		RealArrAttrItem& operator+= (const int a)	{AdditiveArrAttrItem<double>::operator +=((double)a);return *this;};
		RealArrAttrItem& operator=  (const long a)	{AdditiveArrAttrItem<double>::operator =((double)a);return *this;};
		RealArrAttrItem& operator+= (const long a)	{AdditiveArrAttrItem<double>::operator +=((double)a);return *this;};
		RealArrAttrItem& operator=  (const char a)	{AdditiveArrAttrItem<double>::operator =((double)a);return *this;};
		RealArrAttrItem& operator+= (const char a)	{AdditiveArrAttrItem<double>::operator +=((double)a);return *this;};
		RealArrAttrItem& operator=  (const float a)	{AdditiveArrAttrItem<double>::operator =((double)a);return *this;};
		RealArrAttrItem& operator+= (const float a) {AdditiveArrAttrItem<double>::operator +=((double)a);return *this;};
		
	};

	class IntegerArrAttrItem : public AdditiveArrAttrItem<__int64>
	{
	public:
		IntegerArrAttrItem(const vector<__int64>& array_ref, const int  a_i,  ObjectImpl * oi, const ::Uml::Attribute & m)
			:AdditiveArrAttrItem<__int64>(array_ref, a_i, oi, m){};
		void set()
		{
			CheckOrdering();		//this will crash if array is ordered
			//ordered attributes are not supposed to be changed in a by-item fashion

			object->setIntegerAttrArr(meta, array);
		};
		
		//provide these operators for long, int, float, char as well
		IntegerArrAttrItem& operator =(const double a)		{AdditiveArrAttrItem<__int64>::operator =((__int64)a); return *this;};
		IntegerArrAttrItem& operator += (const double  a)	{AdditiveArrAttrItem<__int64>::operator +=((__int64)a);return *this;};
		IntegerArrAttrItem& operator = (const int  a)		{AdditiveArrAttrItem<__int64>::operator =((__int64)a);return *this;};
		IntegerArrAttrItem& operator += (const int  a)		{AdditiveArrAttrItem<__int64>::operator +=((__int64)a);return *this;};
		IntegerArrAttrItem& operator = (const char  a)		{AdditiveArrAttrItem<__int64>::operator =((__int64)a);return *this;};
		IntegerArrAttrItem& operator += (const char  a)		{AdditiveArrAttrItem<__int64>::operator +=((__int64)a);return *this;};
		IntegerArrAttrItem& operator = (const float  a)		{AdditiveArrAttrItem<__int64>::operator =((__int64)a);return *this;};
		IntegerArrAttrItem& operator += (const float  a)	{AdditiveArrAttrItem<__int64>::operator +=((__int64)a);return *this;};
		
	};

	class BooleanArrAttrItem : public ArrAttrItem<bool>
	{
	public:
		BooleanArrAttrItem(const vector<bool>& array_ref, const int  a_i,  ObjectImpl * oi, const ::Uml::Attribute & m)
			:ArrAttrItem<bool>(array_ref, a_i, oi, m){};
		void set()
		{
			CheckOrdering();		//this will crash if array is ordered
			//ordered attributes are not supposed to be changed in a by-item fashion

			object->setBooleanAttrArr(meta, array);
		};
		
		//provide these operators for long, int, float, char as well
		BooleanArrAttrItem& operator =(const double a)	{ArrAttrItem<bool>::operator =(a != 0.0);return *this;};
		BooleanArrAttrItem& operator = (const long  a)	{ArrAttrItem<bool>::operator =(a != 0);return *this;};
		BooleanArrAttrItem& operator = (const char  a)	{ArrAttrItem<bool>::operator =(a != 0);return *this;};
		BooleanArrAttrItem& operator = (const float  a)	{ArrAttrItem<bool>::operator =(a != (float)0.0);return *this;};
		
	};

	


	//non-persistent, array attribute iterators

		//persistent array attribute element(iterator) classes
	// string array element providing operations with const char *
	class TempStringArrAttrItem : public AdditiveArrAttrItem<string>
	{
	public:	
		TempStringArrAttrItem(const vector<string>& array_ref, const int  a_i,  ObjectImpl * oi, const ::Uml::Attribute & m)
			:AdditiveArrAttrItem<string>(array_ref, a_i, oi, m){};
		void set()
		{
			CheckOrdering();		//this will crash if array is ordered
			//ordered attributes are not supposed to be changed in a by-item fashion

			object->setTempStringAttrArr(meta, array);
		};
		
		//provide these operators for const char * as well
		TempStringArrAttrItem& operator =(const char * a)
		{
			AdditiveArrAttrItem<string>::operator=(string(a));
			return *this;
		};

		TempStringArrAttrItem& operator += (const char * a)
		{
			AdditiveArrAttrItem<string>::operator +=(string(a));
			return *this;
		};
		
	};

	class TempRealArrAttrItem : public AdditiveArrAttrItem<double>
	{
	public:	
		TempRealArrAttrItem(const vector<double>& array_ref, const int  a_i,  ObjectImpl * oi, const ::Uml::Attribute & m)
			:AdditiveArrAttrItem<double>(array_ref, a_i, oi, m){};
		void set()
		{
			CheckOrdering();		//this will crash if array is ordered
			//ordered attributes are not supposed to be changed in a by-item fashion

			object->setTempRealAttrArr(meta, array);
		};
		
		//provide these operators for long, int, float, char as well
		TempRealArrAttrItem& operator =(const int a)	{AdditiveArrAttrItem<double>::operator =((double)a);return *this;};
		TempRealArrAttrItem& operator += (const int  a)	{AdditiveArrAttrItem<double>::operator +=((double)a);return *this;};
		TempRealArrAttrItem& operator = (const long  a)	{AdditiveArrAttrItem<double>::operator =((double)a);return *this;};
		TempRealArrAttrItem& operator += (const long  a){AdditiveArrAttrItem<double>::operator +=((double)a);return *this;};
		TempRealArrAttrItem& operator = (const char  a)	{AdditiveArrAttrItem<double>::operator =((double)a);return *this;};
		TempRealArrAttrItem& operator += (const char  a){AdditiveArrAttrItem<double>::operator +=((double)a);return *this;};
		TempRealArrAttrItem& operator = (const float a) {AdditiveArrAttrItem<double>::operator =((double)a);return *this;};
		TempRealArrAttrItem& operator += (const float a){AdditiveArrAttrItem<double>::operator +=((double)a);return *this;};
		
	};

	class TempIntegerArrAttrItem : public AdditiveArrAttrItem<__int64>
	{
	public:
		TempIntegerArrAttrItem(const vector<__int64>& array_ref, const int  a_i,  ObjectImpl * oi, const ::Uml::Attribute & m)
			:AdditiveArrAttrItem<__int64>(array_ref, a_i, oi, m){};
		void set()
		{
			CheckOrdering();		//this will crash if array is ordered
			//ordered attributes are not supposed to be changed in a by-item fashion

			object->setTempIntegerAttrArr(meta, array);
		};
		
		//provide these operators for long, int, float, char as well
		TempIntegerArrAttrItem& operator =(const double a)		{AdditiveArrAttrItem<__int64>::operator =((__int64)a);return *this;};
		TempIntegerArrAttrItem& operator += (const double  a)	{AdditiveArrAttrItem<__int64>::operator +=((__int64)a);return *this;};
		TempIntegerArrAttrItem& operator = (const int  a)	{AdditiveArrAttrItem<__int64>::operator =((__int64)a);return *this;};
		TempIntegerArrAttrItem& operator += (const int  a)	{AdditiveArrAttrItem<__int64>::operator +=((__int64)a);return *this;};
		TempIntegerArrAttrItem& operator = (const char  a)	{AdditiveArrAttrItem<__int64>::operator =((__int64)a);return *this;};
		TempIntegerArrAttrItem& operator += (const char  a)	{AdditiveArrAttrItem<__int64>::operator +=((__int64)a);return *this;};
		TempIntegerArrAttrItem& operator = (const float  a)	{AdditiveArrAttrItem<__int64>::operator =((__int64)a);return *this;};
		TempIntegerArrAttrItem& operator += (const float  a)	{AdditiveArrAttrItem<__int64>::operator +=((__int64)a);return *this;};
		
	};

	class TempBooleanArrAttrItem : public ArrAttrItem<bool>
	{
	public:
		TempBooleanArrAttrItem(const vector<bool>& array_ref, const int  a_i,  ObjectImpl * oi, const ::Uml::Attribute & m)
			:ArrAttrItem<bool>(array_ref, a_i, oi, m){};
		void set()
		{
			CheckOrdering();		//this will crash if array is ordered
			//ordered attributes are not supposed to be changed in a by-item fashion

			object->setTempBooleanAttrArr(meta, array);
		};
		
		//provide these operators for long, int, float, char as well
		TempBooleanArrAttrItem& operator =(const double a)	{ArrAttrItem<bool>::operator =(a != 0.0);return *this;};
		TempBooleanArrAttrItem& operator = (const long  a)	{ArrAttrItem<bool>::operator =(a != 0);return *this;};
		TempBooleanArrAttrItem& operator = (const char  a)	{ArrAttrItem<bool>::operator =(a != 0);return *this;};
		TempBooleanArrAttrItem& operator = (const float  a)	{ArrAttrItem<bool>::operator =(a != (float)0.0);return *this;};
		
	};


	//non-persistent, array attribute objects

	

	template <class CLASS>
	vector<CLASS> SortArrAttr(const vector<CLASS> &a)
	{
		multiset<CLASS> b;
		vector<CLASS> c;
		b.insert(a.begin(), a.end());
		TYPENAME multiset<CLASS>::iterator i = b.begin();
		while (i != b.end()) c.push_back(*i++);
		return c;
	};

	vector<string> UDM_DLL SortStrArrAttr(const vector<string> &a);

	/*
		These are needed because defvalue on Attribute is 
		always declared as a string array, but it's real type
		can be just about anything.
	*/

	vector<__int64> UDM_DLL StrToLongArrAttr(const vector<string> &a);
	vector<double> UDM_DLL StrToRealArrAttr(const vector<string> &a);
	vector<bool> UDM_DLL StrToBoolArrAttr(const vector<string> &a);
	


	class TempStringAttrArr
	{
	private:
		ObjectImpl *impl;
		const ::Uml::Attribute &meta;

	public:
		TempStringAttrArr(ObjectImpl *i, const ::Uml::Attribute &m) : impl(i), meta(m) { }

		operator vector<string>() const { return impl->getTempStringAttrArr(meta); }
		TempStringAttrArr &operator =(const vector<string> &a) 
		{ 
			if (CheckAttributeOrdering(meta))
				impl->setTempStringAttrArr(meta, SortStrArrAttr(a)); 
			else
				impl->setTempStringAttrArr(meta, a); 
			return *this; 
		}

		TempStringAttrArr &operator =(const TempStringAttrArr &a) 
		{
			vector<string> av = a;
			return operator =( av );
		}

		//on write-operations the array is ordered if requested
		const vector<string> operator +=(const vector<string> &a) 
		{
			vector<string> b = impl->getTempStringAttrArr(meta);
			
			vector<string>::const_iterator ci_a = a.begin();
			while (ci_a != a.end())
				b.push_back(*ci_a++);

			//impl->setTempStringAttrArr(meta,b);	
			*this = b;
			return b;
		}
		
		const vector<string> operator +=(const string &a) 
		{
			vector<string> b = impl->getTempStringAttrArr(meta);
			b.push_back(a);
			//impl->setTempStringAttrArr(meta,b);	
			*this = b;
			return b;
		}

		const vector<string> operator +=(const char * a) 
		{
			vector<string> b = impl->getTempStringAttrArr(meta);
			b.push_back(string(a));
			//impl->setTempStringAttrArr(meta,b);	
			*this = b;
			return b;
		}

		bool operator !() const 
		{ 
			vector<string> b = impl->getTempStringAttrArr(meta);
			return  (b.size() == 0);
		};
		operator bool()  const 
		{ 
			vector<string> b = impl->getTempStringAttrArr(meta);
			return  (b.size() != 0); 
		};


		TempStringArrAttrItem  operator[](const int index)
		{
			vector<string> a = impl->getTempStringAttrArr(meta);
			TempStringArrAttrItem array_indexer(a, index, impl, meta);
			return array_indexer;
		}

		TempStringArrAttrItem  begin()
		{
			vector<string> a = impl->getTempStringAttrArr(meta);
			TempStringArrAttrItem array_indexer(a, 0, impl, meta);
			return array_indexer;
		}

		TempStringArrAttrItem  end()
		{
			vector<string> a = impl->getTempStringAttrArr(meta);
			TempStringArrAttrItem array_indexer(a, (int)a.size(), impl, meta);
			return array_indexer;
		}


	};

	class TempIntegerAttrArr
	{
	private:
		ObjectImpl *impl;
		const ::Uml::Attribute &meta;

	public:
		TempIntegerAttrArr(ObjectImpl *i, const ::Uml::Attribute &m) : impl(i), meta(m) { }

		operator vector<__int64>() const { return impl->getTempIntegerAttrArr(meta); }
		TempIntegerAttrArr &operator =(const vector<__int64> &a) 
		{ 
			
			if (CheckAttributeOrdering(meta))
				impl->setTempIntegerAttrArr(meta, SortArrAttr<__int64>(a)); 
			else
				impl->setTempIntegerAttrArr(meta, a); 
			return *this; 
		}
		TempIntegerAttrArr &operator =(const TempIntegerAttrArr &a) 
		{
			vector<__int64> av = a;
			return operator =( av );
		}
		const vector<__int64> operator +=(const vector<__int64> &a) 
		{
			vector<__int64> b = impl->getTempIntegerAttrArr(meta);
			
			vector<__int64>::const_iterator ci_a = a.begin();
			while (ci_a != a.end())
				b.push_back(*ci_a++);
			//impl->setTempIntegerAttrArr(meta,b);	
			*this = b;
			return b;
		}


		const vector<__int64> operator +=(const long a) 
		{
			vector<__int64> b = impl->getTempIntegerAttrArr(meta);
			b.push_back(a);
			//impl->setTempIntegerAttrArr(meta,b);	
			*this = b;
			return b;
		}


		bool operator !() const 
		{ 
			vector<__int64> b = impl->getTempIntegerAttrArr(meta);
			return  (b.size() == 0);
		};
		operator bool()  const 
		{ 
			vector<__int64> b = impl->getTempIntegerAttrArr(meta);
			return  (b.size() != 0); 
		};

		TempIntegerArrAttrItem  operator[](const int index)
		{
			vector<__int64> a = impl->getTempIntegerAttrArr(meta);
			TempIntegerArrAttrItem array_indexer(a, index, impl, meta);
			return array_indexer;
		}

		TempIntegerArrAttrItem  begin()
		{
			vector<__int64> a = impl->getTempIntegerAttrArr(meta);
			TempIntegerArrAttrItem array_indexer(a, 0, impl, meta);
			return array_indexer;
		}
	
		TempIntegerArrAttrItem  end()
		{
			vector<__int64> a = impl->getTempIntegerAttrArr(meta);
			TempIntegerArrAttrItem array_indexer(a, (int)a.size(), impl, meta);
			return array_indexer;
		}



	};


	class TempRealAttrArr
	{
	private:
		ObjectImpl *impl;
		const ::Uml::Attribute &meta;

	public:
		TempRealAttrArr(ObjectImpl *i, const ::Uml::Attribute &m) : impl(i), meta(m) { }

		operator vector<double>() const { return impl->getTempRealAttrArr(meta); }
		TempRealAttrArr &operator =(const vector<double> &a) 
		{ 
			
			if (CheckAttributeOrdering(meta))
				impl->setTempRealAttrArr(meta, SortArrAttr<double>(a)); 
			else
				impl->setTempRealAttrArr(meta, a); 

			return *this; 
		}
		TempRealAttrArr &operator =(const TempRealAttrArr &a) 
		{
			vector<double> av = a;
			return operator =( av );
		}
		const vector<double> operator +=(const vector<double> &a) 
		{
			vector<double> b = impl->getTempRealAttrArr(meta);
			
			vector<double>::const_iterator ci_a = a.begin();
			while (ci_a != a.end())
				b.push_back(*ci_a++);

			*this = b;    //call the above operator
			return b;
		}


		const vector<double> operator +=(const double a) 
		{
			vector<double> b = impl->getTempRealAttrArr(meta);
			b.push_back(a);
			*this = b;    //call the above operator
			return b;
		}


		bool operator !() const 
		{ 
			vector<double> b = impl->getTempRealAttrArr(meta);
			return  (b.size() == 0);
		};
		operator bool()  const 
		{ 
			vector<double> b = impl->getTempRealAttrArr(meta);
			return  (b.size() != 0); 
		};


		TempRealArrAttrItem  operator[](const int index)
		{
			vector<double> a = impl->getTempRealAttrArr(meta);
			TempRealArrAttrItem array_indexer(a, index, impl, meta);
			return array_indexer;
		}

		TempRealArrAttrItem  begin()
		{
			vector<double> a = impl->getTempRealAttrArr(meta);
			TempRealArrAttrItem array_indexer(a, 0, impl, meta);
			return array_indexer;
		}

		TempRealArrAttrItem  end()
		{
			vector<double> a = impl->getTempRealAttrArr(meta);
			TempRealArrAttrItem array_indexer(a, (int)a.size(), impl, meta);
			return array_indexer;
		}

	};


	class TempBooleanAttrArr
	{
	private:
		ObjectImpl *impl;
		const ::Uml::Attribute &meta;

	public:
		TempBooleanAttrArr(ObjectImpl *i, const ::Uml::Attribute &m) : impl(i), meta(m) { }

		operator vector<bool>() const { return impl->getTempBooleanAttrArr(meta); }
		TempBooleanAttrArr &operator =(const vector<bool> &a) 
		{ 
			if (CheckAttributeOrdering(meta))
				impl->setTempBooleanAttrArr(meta, SortArrAttr(a));
			else
				impl->setTempBooleanAttrArr(meta, a); 
			return *this; 
		}

		TempBooleanAttrArr &operator =(const TempBooleanAttrArr &a) 
		{
			vector<bool> av = a;
			return operator =( av );
		}

		const vector<bool> operator +=(const vector<bool> &a) 
		{
			vector<bool> b = impl->getTempBooleanAttrArr(meta);
			
			vector<bool>::const_iterator ci_a = a.begin();
			while (ci_a != a.end())
				b.push_back(*ci_a++);
			//impl->setTempBooleanAttrArr(meta,b);	
			*this = b;
			return b;
		}


		const vector<bool> operator +=(const bool a) 
		{
			vector<bool> b = impl->getTempBooleanAttrArr(meta);
			b.push_back(a);
			//impl->setTempBooleanAttrArr(meta,b);	
			*this = b;
			return b;
		}


		bool operator !() const 
		{ 
			vector<bool> b = impl->getTempBooleanAttrArr(meta);
			return  (b.size() == 0);
		};
		operator bool()  const 
		{ 
			vector<bool> b = impl->getTempBooleanAttrArr(meta);
			return  (b.size() != 0); 
		};


		TempBooleanArrAttrItem  operator[](const int index)
		{
			vector<bool> a = impl->getTempBooleanAttrArr(meta);
			TempBooleanArrAttrItem array_indexer(a, index, impl, meta);
			return array_indexer;
		}

		TempBooleanArrAttrItem  begin()
		{
			vector<bool> a = impl->getTempBooleanAttrArr(meta);
			TempBooleanArrAttrItem array_indexer(a, 0, impl, meta);
			return array_indexer;
		}

		TempBooleanArrAttrItem  end()
		{
			vector<bool> a = impl->getTempBooleanAttrArr(meta);
			TempBooleanArrAttrItem array_indexer(a, (int)a.size(), impl, meta);
			return array_indexer;
		}

	};



// --------------------------- StringAttrArr

	class StringAttrArr
	{
	private:
		ObjectImpl *impl;
		const ::Uml::Attribute &meta;

	public:
		StringAttrArr(ObjectImpl *i, const ::Uml::Attribute &m) : impl(i), meta(m) { }

		operator vector<string>() const { return impl->getStringAttrArr(meta); }
		StringAttrArr &operator =(const vector<string> &a) 
		{ 
			if (CheckAttributeOrdering(meta))
				impl->setStringAttrArr(meta, SortStrArrAttr(a));
			else
				impl->setStringAttrArr(meta, a); 

			return *this; 

		}
		StringAttrArr &operator =(const StringAttrArr &a) 
		{
			vector<string> av = a;
			return operator =( av );
		}
		const vector<string> operator +=(const vector<string> &a) 
		{
			vector<string> b = impl->getStringAttrArr(meta);
			
			vector<string>::const_iterator ci_a = a.begin();
			while (ci_a != a.end())
				b.push_back(*ci_a++);
			//impl->setStringAttrArr(meta,b);	
			*this = b;
			return b;
		}
		
		const vector<string> operator +=(const string &a) 
		{
			vector<string> b = impl->getStringAttrArr(meta);
			b.push_back(a);
			//impl->setStringAttrArr(meta,b);	
			*this = b;
			return b;
		}

		const vector<string> operator +=(const char * a) 
		{
			vector<string> b = impl->getStringAttrArr(meta);
			b.push_back(string(a));
			//impl->setStringAttrArr(meta,b);	
			*this = b;
			return b;

		}

		bool operator !() const 
		{ 
			vector<string> b = impl->getStringAttrArr(meta);
			return  (b.size() == 0);
		};
		operator bool()  const 
		{ 
			vector<string> b = impl->getStringAttrArr(meta);
			return  (b.size() != 0); 
		};


		StringArrAttrItem  operator[](const int index)
		{
			vector<string> a = impl->getStringAttrArr(meta);
			StringArrAttrItem array_indexer(a, index, impl, meta);
			return array_indexer;
		}


		StringArrAttrItem  begin()
		{
			vector<string> a = impl->getStringAttrArr(meta);
			StringArrAttrItem array_indexer(a, 0, impl, meta);
			return array_indexer;
		}

		StringArrAttrItem  end()
		{
			vector<string> a = impl->getStringAttrArr(meta);
			StringArrAttrItem array_indexer(a, (int)a.size(), impl, meta);
			return array_indexer;
		}
	
		
		
	};

// --------------------------- BooleanAttrArr

	class BooleanAttrArr
	{
	private:
		ObjectImpl *impl;
		const ::Uml::Attribute &meta;

	public:
		BooleanAttrArr(ObjectImpl *i, const ::Uml::Attribute &m) : impl(i), meta(m) { }

		operator vector<bool>() const { return impl->getBooleanAttrArr(meta); }
		BooleanAttrArr &operator =(const vector<bool>& a) 
		{ 
			if (CheckAttributeOrdering(meta))
				impl->setBooleanAttrArr(meta, SortArrAttr<bool>(a));
			else
				impl->setBooleanAttrArr(meta, a); 
			return *this; 
		}

		BooleanAttrArr &operator =(const BooleanAttrArr &a) 
		{
			vector<bool> av = a;
			return operator =( av );
		}

		const vector<bool> operator +=(const vector<bool> &a) 
		{
			vector<bool> b = impl->getBooleanAttrArr(meta);
			
			vector<bool>::const_iterator ci_a = a.begin();
			while (ci_a != a.end())
				b.push_back(*ci_a++);
			//impl->setBooleanAttrArr(meta,b);	
			*this = b;
			return b;
		}


		const vector<bool> operator +=(const bool a) 
		{
			vector<bool> b = impl->getBooleanAttrArr(meta);
			b.push_back(a);
			//impl->setBooleanAttrArr(meta,b);	
			*this = b;
			return b;
		}


		bool operator !() const 
		{ 
			vector<bool> b = impl->getBooleanAttrArr(meta);
			return  (b.size() == 0);
		};
		operator bool()  const 
		{ 
			vector<bool> b = impl->getBooleanAttrArr(meta);
			return  (b.size() != 0); 
		};


		BooleanArrAttrItem  operator[](const int index)
		{
			vector<bool> a = impl->getBooleanAttrArr(meta);
			BooleanArrAttrItem array_indexer(a, index, impl, meta);
			return array_indexer;
		}

	};

// --------------------------- IntegerAttrArr

	class IntegerAttrArr
	{
	private:
		ObjectImpl *impl;
		const ::Uml::Attribute &meta;

	public:
		IntegerAttrArr(ObjectImpl *i, const ::Uml::Attribute &m) : impl(i), meta(m) { }

		operator vector<__int64>() const { return impl->getIntegerAttrArr(meta); }
		IntegerAttrArr &operator =(const vector<__int64>& a) 
		{
			if (CheckAttributeOrdering(meta))
				impl->setIntegerAttrArr(meta, SortArrAttr<__int64>(a)); 
			else
				impl->setIntegerAttrArr(meta, a); 

			return *this; 
		}
		IntegerAttrArr &operator =(const IntegerAttrArr &a) 
		{
			vector<__int64> av = a;
			return operator =( av );
		}

		const vector<__int64> operator +=(const vector<__int64> &a) 
		{
			vector<__int64> b = impl->getIntegerAttrArr(meta);
			
			vector<__int64>::const_iterator ci_a = a.begin();
			while (ci_a != a.end())
				b.push_back(*ci_a++);
			//impl->setIntegerAttrArr(meta,b);	
			*this = b;
			return b;
		}


		const vector<__int64> operator +=(const __int64 a) 
		{
			vector<__int64> b = impl->getIntegerAttrArr(meta);
			b.push_back(a);
			//impl->setIntegerAttrArr(meta,b);	
			*this = b;
			return b;
		}


		bool operator !() const 
		{ 
			vector<__int64> b = impl->getIntegerAttrArr(meta);
			return  (b.size() == 0);
		};
		operator bool()  const 
		{ 
			vector<__int64> b = impl->getIntegerAttrArr(meta);
			return  (b.size() != 0); 
		};

		IntegerArrAttrItem  operator[](const int index)
		{
			vector<__int64> a = impl->getIntegerAttrArr(meta);
			IntegerArrAttrItem array_indexer(a, index, impl, meta);
			return array_indexer;
		}

		IntegerArrAttrItem begin()
		{
			vector<__int64> a = impl->getIntegerAttrArr(meta);
			IntegerArrAttrItem array_indexer(a, 0, impl, meta);
			return array_indexer;
		}

		IntegerArrAttrItem end()
		{
			vector<__int64> a = impl->getIntegerAttrArr(meta);
			IntegerArrAttrItem array_indexer(a, (int)a.size(), impl, meta);
			return array_indexer;
		}

	};

// --------------------------- RealAttrArr

	class RealAttrArr
	{
	private:
		ObjectImpl *impl;
		const ::Uml::Attribute &meta;

	public:
		RealAttrArr(ObjectImpl *i, const ::Uml::Attribute &m) : impl(i), meta(m) { }

		operator vector<double>() const { return impl->getRealAttrArr(meta); }
		RealAttrArr &operator =(const vector<double> &a) 
		{
			if (CheckAttributeOrdering(meta))
				impl->setRealAttrArr(meta, SortArrAttr<double>(a)); 
			else
				impl->setRealAttrArr(meta, a); 
			return *this; 
		}

		RealAttrArr &operator =(const RealAttrArr &a) 
		{
			vector<double> av = a;
			return operator =( av );
		}

		const vector<double> operator +=(const vector<double> &a) 
		{
			vector<double> b = impl->getRealAttrArr(meta);
			
			vector<double>::const_iterator ci_a = a.begin();
			while (ci_a != a.end())
				b.push_back(*ci_a++);
			//impl->setRealAttrArr(meta,b);	
			*this = b;
			return b;
		}


		const vector<double> operator +=(const double a) 
		{
			vector<double> b = impl->getRealAttrArr(meta);
			b.push_back(a);
			//	impl->setRealAttrArr(meta,b);	
			*this = b;
			return b;
		}


		bool operator !() const 
		{ 
			vector<double> b = impl->getRealAttrArr(meta);
			return  (b.size() == 0);
		};
		operator bool()  const 
		{ 
			vector<double> b = impl->getRealAttrArr(meta);
			return  (b.size() != 0); 
		};


		RealArrAttrItem  operator[](const int index)
		{
			vector<double> a = impl->getRealAttrArr(meta);
			RealArrAttrItem array_indexer(a, index, impl, meta);
			return array_indexer;
		}

		RealArrAttrItem begin()
		{
			vector<double> a = impl->getRealAttrArr(meta);
			RealArrAttrItem array_indexer(a, 0, impl, meta);
			return array_indexer;
		}

		RealArrAttrItem end()
		{
			vector<double> a = impl->getRealAttrArr(meta);
			RealArrAttrItem  array_indexer(a, (int)a.size(), impl, meta);
			return array_indexer;
		}


	};

	
	template<class CLASS>
	struct AttrPointer
	{
		CLASS parent;
		AttrPointer(CLASS parent)
#ifdef UDM_RVALUE
			: parent(std::move(parent)) { }
#else
			: parent(parent) { }
#endif
		CLASS *operator*()
		{
			return &parent;
		}

		CLASS *operator->()
		{
			return &parent;
		}
	};
	
// --------------------------- CrossAssocAttr
	template<class CLASS, class Pred = less<CLASS> >
	class CrossAssocAttr
	{
	private:
		ObjectImpl *impl;
		const ::Uml::AssociationRole &meta;

	public:
		typedef CLASS dst;
		typedef Pred Predicate;

		CrossAssocAttr(ObjectImpl *i, const ::Uml::AssociationRole &m) : impl(i), meta(m) { }

		operator set<CLASS, Pred>() const;
		CrossAssocAttr<CLASS, Pred> &operator =(const set<CLASS, Pred> &a);

		CrossAssocAttr<CLASS, Pred> &operator =(const CrossAssocAttr<CLASS, Pred> &a)
		{
			set<CLASS, Pred> as = a;
			return operator =(as);
		}

		const set<CLASS, Pred> operator +=(const CLASS &a) {
			set<CLASS, Pred> xx(*this);
			xx.insert(a);
			return *this = xx;
		}
		const set<CLASS, Pred> operator -=(const CLASS &a) {
			set<CLASS, Pred> xx(*this);
			xx.erase(a);
			return *this = xx;
		}
	};


// --------------------------- AssocAttr


	template<class CLASS, class Pred = less<CLASS> >
	class AssocAttr
	{
	private:
		ObjectImpl *impl;
		const ::Uml::AssociationRole &meta;

	public:
		typedef CLASS dst;
		typedef Pred Predicate;

		AssocAttr(ObjectImpl *i, const ::Uml::AssociationRole &m) : impl(i), meta(m) { }

		operator set<CLASS, Pred>() const 
		{
			
			set<CLASS, Pred> ret;

			vector<ObjectImpl*> a = impl->getAssociation(meta);
			vector<ObjectImpl*>::iterator i = a.begin();
			while( i != a.end() )
			{
				// TODO: assert the type of the object
				ret.insert(*i);
				++i;
			}
			
			return ret;
		}

		AssocAttr<CLASS, Pred> &operator =(const set<CLASS, Pred> &a)
		{
			vector<ObjectImpl*> b;

			TYPENAME set<CLASS, Pred>::const_iterator i = a.begin();
			while( i != a.end() )
			{
				b.push_back((*i).__impl());
				++i;
			}

			impl->setAssociation(meta, b);

			return *this;
		}

		AssocAttr<CLASS, Pred> &operator =(const AssocAttr<CLASS, Pred> &a)
		{
			set<CLASS, Pred> as = a;
			return operator =(as);
		}

		const set<CLASS,Pred> operator +=(const CLASS &a) {
			UDM_ASSERT(a);
			set<CLASS,Pred> xx(*this);
			xx.insert(a);
			return *this = xx;
		}
		const set<CLASS,Pred> operator -=(const CLASS &a) {
			UDM_ASSERT(a);
			set<CLASS,Pred> xx(*this);
			xx.erase(a);
			return *this = xx;
		}
	};

// --------------------------- CrossPointerAttr

	template<class CLASS>
	class CrossPointerAttr
	{
	private:
		ObjectImpl *impl;
		const ::Uml::AssociationRole &meta;

	public:
		typedef CLASS dst;

		CrossPointerAttr(ObjectImpl *i, const ::Uml::AssociationRole &m) : impl(i), meta(m) { }

		operator CLASS() const;

		const CLASS &operator =(const CLASS &a);

		const CLASS &operator =(const CrossPointerAttr<CLASS> &a)
		{
			// FIXME: (CLASS) a is destructed then returned by reference. return *this instead
			return operator =( (CLASS) a );
		}

		AttrPointer<CLASS> operator->() const 
		{
			return AttrPointer<CLASS>(static_cast<CLASS>(*this));
		}

		CLASS operator*() const
		{
			return static_cast<CLASS>(*this);
		}

	};



// --------------------------- PointerAttr

	template<class CLASS>
	class PointerAttr
	{
	private:
		ObjectImpl *impl;
		const ::Uml::AssociationRole &meta;

	public:
		typedef CLASS dst;

		PointerAttr(ObjectImpl *i, const ::Uml::AssociationRole &m) : impl(i), meta(m) { }

		operator CLASS() const 
		{
			vector<ObjectImpl*> a = impl->getAssociation(meta);
			UDM_ASSERT( a.size() <= 1 );

			// TODO: assert the type of the object
			return a.empty() ? &_null : a.front();
		}

		const CLASS &operator =(const CLASS &a)
		{
			vector<ObjectImpl*> b;

			if( a != null )
				b.push_back(a.__impl());

			impl->setAssociation(meta, b);

			return a;
		}

		const CLASS &operator =(const PointerAttr<CLASS> &a)
		{
			// FIXME: (CLASS) a is destructed then returned by reference. return *this instead
			return operator =( (CLASS) a );
		}

		AttrPointer<CLASS> operator->() const 
		{
			return AttrPointer<CLASS>(static_cast<CLASS>(*this));
		}

		CLASS operator*() const
		{
			return static_cast<CLASS>(*this);
		}

	};

// --------------------------- CrossAssocEndAttr

	template<class CLASS>
	class CrossAssocEndAttr
	{
	private:
		ObjectImpl *impl;
		const ::Uml::AssociationRole &meta;

	public:
		typedef CLASS dst;

		CrossAssocEndAttr(ObjectImpl *i, const ::Uml::AssociationRole &m) : impl(i), meta(m) { }

		operator CLASS() const;
		const CLASS &operator =(const CLASS &a);

		const CLASS &operator =(const CrossAssocEndAttr<CLASS> &a)
		{
			// FIXME: (CLASS) a is destructed then returned by reference. return *this instead
			return operator =( (CLASS) a );
		}

		AttrPointer<CLASS> operator->() const 
		{
			return AttrPointer<CLASS>(static_cast<CLASS>(*this));
		}

		CLASS operator*() const
		{
			return static_cast<CLASS>(*this);
		}

	};


// --------------------------- AssocEndAttr

	template<class CLASS>
	class AssocEndAttr
	{
	private:
		ObjectImpl *impl;
		const ::Uml::AssociationRole &meta;

	public:
		typedef CLASS dst;

		AssocEndAttr(ObjectImpl *i, const ::Uml::AssociationRole &m) : impl(i), meta(m) { }

		operator CLASS() const 
		{
			vector<ObjectImpl*> a = impl->getAssociation(meta, TARGETFROMCLASS);
			UDM_ASSERT( a.size() <= 1 );

			// TODO: assert the type of the object
			return a.empty() ? &_null : a.front();
		}

		const CLASS &operator =(const CLASS &a)
		{
			vector<ObjectImpl*> b;

			if( a != null )
				b.push_back(a.__impl());

			impl->setAssociation(meta, b, TARGETFROMCLASS);

			return a;
		}

		const CLASS &operator =(const AssocEndAttr<CLASS> &a)
		{
			// FIXME: (CLASS) a is destructed then returned by reference. return *this instead
			return operator =( (CLASS) a );
		}

		AttrPointer<CLASS> operator->() const 
		{
			return AttrPointer<CLASS>(static_cast<CLASS>(*this));
		}

		CLASS operator*() const
		{
			return static_cast<CLASS>(*this);
		}
	};

// --------------------------- AClassCrossPointerAttr

	template<class CLASS, class TARGETCLASS>
	class AClassCrossPointerAttr
	{
	private:
		ObjectImpl *impl;
		const ::Uml::AssociationRole &peermeta, &selfmeta;

	public:
		typedef CLASS connector;
		typedef TARGETCLASS dst;

		AClassCrossPointerAttr(ObjectImpl *i, const ::Uml::AssociationRole &m, const ::Uml::AssociationRole &m2) : impl(i), peermeta(m), selfmeta(m2) { }

		operator CLASS() const;
		const CLASS &operator =(const CLASS &a);

		const CLASS &operator =(const AClassCrossPointerAttr<CLASS, TARGETCLASS> &a)
		{
			// FIXME: (CLASS) a is destructed then returned by reference. return *this instead
			return operator =( (CLASS) a );
		}

		AttrPointer<CLASS> operator->() const 
		{
			return AttrPointer<CLASS>(static_cast<CLASS>(*this));
		}

		CLASS operator*() const
		{
			return static_cast<CLASS>(*this);
		}

	};
// --------------------------- AClassPointerAttr

	template<class CLASS, class TARGETCLASS>
	class AClassPointerAttr
	{
	private:
		ObjectImpl *impl;
		const ::Uml::AssociationRole &peermeta, &selfmeta;

	public:
		typedef CLASS connector;
		typedef TARGETCLASS dst;

		AClassPointerAttr(ObjectImpl *i, const ::Uml::AssociationRole &m, const ::Uml::AssociationRole &m2) : impl(i), peermeta(m), selfmeta(m2) { }

		operator CLASS() const 
		{
			vector<ObjectImpl*> a = impl->getAssociation(peermeta, CLASSFROMTARGET);
			UDM_ASSERT( a.size() <= 1 );

			// TODO: assert the type of the object
			return a.empty() ? &_null : a.front();
		}

		const CLASS &operator =(const CLASS &a)
		{
			vector<ObjectImpl*> b;

			if( a != null )
				b.push_back(a.__impl());

			impl->setAssociation(peermeta, b, CLASSFROMTARGET);

			return a;
		}

		const CLASS &operator =(const AClassPointerAttr<CLASS, TARGETCLASS> &a)
		{
			// FIXME: (CLASS) a is destructed then returned by reference. return *this instead
			return operator =( (CLASS) a );
		}

		AttrPointer<CLASS> operator->() const 
		{
			return AttrPointer<CLASS>(static_cast<CLASS>(*this));
		}

		CLASS operator*() const
		{
			return static_cast<CLASS>(*this);
		}

		CLASS SetLink(TARGETCLASS peer, Object parent, 
			const ::Uml::Class &meta = CLASS::meta,
			const ::Uml::CompositionChildRole &role = NULLCHILDROLE);
	};

	// --------------------------- AClassCrossAssocAttr

	template<class CLASS, class TARGETCLASS, class Pred = less<CLASS> >
	class AClassCrossAssocAttr
	{
	private:
		ObjectImpl *impl;
// selfmeta is the role pointing towards the owner
// the peer points to the other
		const ::Uml::AssociationRole &peermeta, &selfmeta;

	public:
		typedef CLASS connector;
		typedef TARGETCLASS dst;
		typedef Pred Predicate;

		AClassCrossAssocAttr(ObjectImpl *i, const ::Uml::AssociationRole &m, const ::Uml::AssociationRole &m2) : impl(i), peermeta(m), selfmeta(m2) { }

		operator set<CLASS, Pred>() const;
		AClassCrossAssocAttr<CLASS, TARGETCLASS, Pred> &operator =(const set<CLASS, Pred> &a);

		AClassCrossAssocAttr<CLASS, TARGETCLASS, Pred> &operator =(const AClassCrossAssocAttr<CLASS, TARGETCLASS, Pred> &a)
		{
			set<CLASS, Pred> as = a;
			return operator =( as );
		}

	};



// --------------------------- AClassAssocAttr

	template<class CLASS, class TARGETCLASS, class Pred=less<CLASS> >
	class AClassAssocAttr
	{
	private:
		ObjectImpl *impl;
// selfmeta is the role pointing towards the owner
// the peer points to the other
		const ::Uml::AssociationRole &peermeta, &selfmeta;

	public:
		typedef CLASS connector;
		typedef TARGETCLASS dst;
		typedef Pred Predicate;

		AClassAssocAttr(ObjectImpl *i, const ::Uml::AssociationRole &m, const ::Uml::AssociationRole &m2) : impl(i), peermeta(m), selfmeta(m2) { }

		operator set<CLASS, Pred>() const 
		{
			set<CLASS, Pred> ret;

			vector<ObjectImpl*> a = impl->getAssociation(peermeta, CLASSFROMTARGET);
			vector<ObjectImpl*>::iterator i = a.begin();
			while( i != a.end() )
			{
				ret.insert(*i);
				++i;
			}

			return ret;
		}

		AClassAssocAttr<CLASS, TARGETCLASS, Pred> &operator =(const set<CLASS, Pred> &a)
		{
			vector<ObjectImpl*> b;

			TYPENAME set<CLASS, Pred>::const_iterator i = a.begin();
			while( i != a.end() )
			{
				b.push_back((*i).__impl());
				++i;
			}

			impl->setAssociation(peermeta, b, CLASSFROMTARGET);

			return *this;
		}

		AClassAssocAttr<CLASS, TARGETCLASS, Pred> &operator =(const AClassAssocAttr<CLASS, TARGETCLASS, Pred> &a)
		{
			set<CLASS, Pred> as = a;
			return operator =( as );
		}

		CLASS AddLink(TARGETCLASS peer, Object parent, 
			const ::Uml::Class &meta = CLASS::meta,
			const ::Uml::CompositionChildRole &role = NULLCHILDROLE);
	};

// --------------------------- ChildrenAttr

	template<class CLASS, class Pred = less<CLASS> >
	class ChildrenAttr
	{
	private:
		ObjectImpl *impl;
		const ::Uml::CompositionChildRole &meta;

	public:
		typedef CLASS dst;
		typedef Pred Predicate;

		ChildrenAttr(ObjectImpl *i, const ::Uml::CompositionChildRole &m) : impl(i), meta(m) { }

		operator set<CLASS, Pred>() const 
		{
			set<CLASS, Pred> ret;

			vector<ObjectImpl*> a = impl->getChildren(meta, CLASS::meta);
			vector<ObjectImpl*>::iterator i = a.begin();
			while( i != a.end() )
			{
				// TODO: assert the type of the object
				ret.insert(*i);
				++i;
			}

			return ret;
		}

		operator vector<CLASS>() const 
		{
			vector<CLASS> ret;

			vector<ObjectImpl*> a = impl->getChildren(meta, CLASS::meta);
			vector<ObjectImpl*>::iterator i = a.begin();
			while( i != a.end() )
			{
				// TODO: assert the type of the object
				ret.push_back(*i);
				++i;
			}

			return ret;
		}



		ChildrenAttr<CLASS, Pred> &operator =(const set<CLASS,Pred> &a)
		{
			vector<ObjectImpl *> b;

			TYPENAME set<CLASS, Pred>::const_iterator i = a.begin();
			while( i != a.end() )
			{
				b.push_back((*i).__impl());
				++i;
			}

			impl->setChildren(meta, b);

			return *this;
		}

		ChildrenAttr<CLASS, Pred> &operator =(const vector<CLASS> &a)
		{
			vector<ObjectImpl *> b;
			set<CLASS, Pred> uniq;

			TYPENAME vector<CLASS>::const_iterator i = a.begin();
			while( i != a.end() )
			{
				pair<TYPENAME set<CLASS, Pred>::iterator, bool> ins_res = uniq.insert(*i);
				if (ins_res.second)
				{
					//object is unique
					b.push_back((*i).__impl());
					++i;
				}
			}

			impl->setChildren(meta, b);

			return *this;
		}

		ChildrenAttr<CLASS, Pred> &operator =(const ChildrenAttr<CLASS, Pred> &a)
		{
			vector<CLASS> av = a;
			return operator =( av );
		}





		const vector<CLASS> operator +=(const CLASS &a) 
		{
			//set<CLASS> xx(*this);
			//xx.insert(a);
			vector<CLASS> xx(*this);
			xx.push_back(a);
			return *this = xx;
			
		}

		const vector<CLASS> operator -=(const CLASS &a) 
		{
			//set<CLASS> xx(*this);
			//xx.erase(a);
			vector<CLASS> xx(*this);
			TYPENAME vector<CLASS>::iterator i = xx.begin();
			while ( i != xx.end())
			{
				if (*i == a)
				{
					i = xx.erase(i);
				}
				else
					++i;
			}
			return *this = xx;
		}
	};

// --------------------------- ChildAttr

	template<class CLASS>
	class ChildAttr
	{
	private:
		ObjectImpl *impl;
		const ::Uml::CompositionChildRole &meta;

	public:
		typedef CLASS dst;

		ChildAttr(ObjectImpl *i, const ::Uml::CompositionChildRole &m) : impl(i), meta(m) { }

		operator CLASS() const 
		{
			vector<ObjectImpl*> a = impl->getChildren(meta, CLASS::meta);
			UDM_ASSERT( a.size() <= 1 );

			// TODO: assert the type of the object
			return a.empty() ? &_null : a.front();
		}

		const CLASS &operator =(const CLASS &a)
		{
			vector<ObjectImpl*> b;

			if( a != null )
				b.push_back(a.__impl());

			impl->setChildren(meta, b);

			return a;
		}

		const CLASS &operator =(const ChildAttr<CLASS> &a)
		{
			return operator =( (CLASS) a);
		}

		AttrPointer<CLASS> operator->() const 
		{
			return AttrPointer<CLASS>(static_cast<CLASS>(*this));
		}

		CLASS operator*() const
		{
			return static_cast<CLASS>(*this);
		}
	};

// --------------------------- ParentAttr

	template<class CLASS>
	class ParentAttr
	{
	private:
		ObjectImpl *impl;
		const ::Uml::CompositionParentRole &meta;

	public:
		typedef CLASS dst;

		ParentAttr(ObjectImpl *i, const ::Uml::CompositionParentRole &m) : impl(i), meta(m) { }

		operator CLASS() const {
			return CLASS::Cast(impl->getParent(meta));
		}

		// we should speed this up using some iterator class
		const CLASS &operator =(const CLASS &a)
		{
// CYT4 - to allow the effective removal of node by setting parent to null
//			UDM_ASSERT( a != null );

			impl->setParent(a.__impl(), meta);

			return a;
		}

		const CLASS &operator =(const ParentAttr<CLASS> &a)
		{
			// FIXME: (CLASS) a is destructed then returned by reference. return *this instead
			return operator =( (CLASS) a );
		}

		AttrPointer<CLASS> operator->() const 
		{
			return AttrPointer<CLASS>(static_cast<CLASS>(*this));
		}

		CLASS operator*() const
		{
			return static_cast<CLASS>(*this);
		}
	};
// -----------------------------Typed set<> container

	template<class CLASS, class Pred = less<CLASS> >
	class DerivedAttr
	{
		private:
			ObjectImpl *impl;
		public:
			typedef CLASS dst;
			typedef Pred Predicate;

			DerivedAttr(ObjectImpl *i) : impl(i) {}
			operator set<CLASS, Pred>() const 
			{
				set<CLASS, Pred> ret;
				vector<ObjectImpl*> a = impl->getDerived();
				vector<ObjectImpl*>::iterator i = a.begin();
				while( i != a.end() )
				{
					// TODO: assert the type of the object
					ret.insert(*i);
					++i;
				}

				return ret;
			}
	};


	template<class CLASS, class Pred = less<CLASS> >
	class InstantiatedAttr
	{
		private:
			ObjectImpl *impl;
		public:
			typedef CLASS dst;
			typedef Pred Predicate;

			InstantiatedAttr(ObjectImpl *i) : impl(i) {}
			operator set<CLASS, Pred>() const 
			{
				set<CLASS,Pred> ret;
				vector<ObjectImpl*> a = impl->getInstances();
				vector<ObjectImpl*>::iterator i = a.begin();
				while( i != a.end() )
				{
					// TODO: assert the type of the object
					ret.insert(*i);
					++i;
				}

				return ret;
			}
	};

	template<class CLASS>
	class ArchetypeAttr
	{
		private:
			ObjectImpl * impl;
		public:
			typedef CLASS dst;

			ArchetypeAttr(ObjectImpl *i) : impl(i) {}
			operator CLASS() const
			{
#ifdef _DEBUG
				return CLASS::Cast(impl->getArchetype());
#else
				return CLASS(impl->getArchetype());
#endif
			};
	};

// --------------------------- AssocEndChainAttr

	template<class CLASS, TYPENAME RESULTTYPE>
	class AssocEndChainAttr
	{
	private:
		ObjectImpl *impl;
		const ::Uml::AssociationRole &meta;

	public:
		typedef CLASS dst;

		AssocEndChainAttr(ObjectImpl *i, const ::Uml::AssociationRole &m) : impl(i), meta(m) { }

		operator RESULTTYPE() const
		{
			vector<ObjectImpl*> a = impl->getAssociation(meta, TARGETFROMCLASS);
			UDM_ASSERT( a.size() <= 1 );

			if (!a.empty())
			{
				CLASS end = a.front();
				vector<ObjectImpl*> chain = impl->getConnectingChain(meta, end.__impl());
				TYPENAME RESULTTYPE::second_type v;
				for (vector<ObjectImpl*>::const_iterator i = chain.begin(); i != chain.end(); i++)
					v.push_back(*i);
				return make_pair(end, v);
			}

			TYPENAME RESULTTYPE::second_type empty_v;
			return make_pair(&_null, empty_v);
		}

		AssocEndChainAttr<CLASS, RESULTTYPE> &operator =(const RESULTTYPE &a)
		{
			vector<ObjectImpl *> chain;

			TYPENAME RESULTTYPE::second_type::const_iterator i = a.second.begin();
			while( i != a.second.end() )
			{
				chain.push_back((*i).__impl());
				++i;
			}

			impl->connectTo(meta, a.first.__impl(), chain);

			return *this;
		}

		AssocEndChainAttr<CLASS, RESULTTYPE> &operator =(const AssocEndChainAttr<CLASS, RESULTTYPE> &a)
		{
			RESULTTYPE b = a;
			return operator =(b);
		}

		AssocEndChainAttr<CLASS, RESULTTYPE> &operator =(const CLASS &a)
		{
			typename RESULTTYPE::second_type empty_v;
			RESULTTYPE b = make_pair(a, empty_v);
			return operator =(b);
		}

		AssocEndChainAttr<CLASS, RESULTTYPE> &disconnect()
		{
			vector<ObjectImpl*> a = impl->getAssociation(meta, TARGETFROMCLASS);
			UDM_ASSERT( a.size() <= 1 );

			if (!a.empty())
			{
				CLASS end = a.front();
				impl->disconnectFrom(meta, end.__impl());
			}

			return *this;
		}
	};

// --------------------------- BaseVisitor

	class UDM_DLL BaseVisitor
	{
	public:
		virtual void Visit___BaseVisitor(const BaseVisitor&){};
		virtual ~BaseVisitor(){};
	};
// --------------------------- Object

	class UDM_DLL Object
	{
	// --- friends

		friend void Initialize();
		friend void Terminate();
		
	protected:
		ObjectImpl *impl;	// this is a NULL pointer only after being std::move()d (or being destructed under _DEBUG)

	public:
		ObjectImpl *__impl() const { return impl; }

	// --- construction

	public:
		Object() : impl(&_null) { UDM_ASSERT(impl != NULL); }
		Object(ObjectImpl *i) : impl(i != NULL ? i : &_null) { UDM_ASSERT(impl != NULL); }
		
		Object(const Object &a) : impl(a.impl->clone()) { UDM_ASSERT(impl != NULL); }
#ifdef UDM_RVALUE
		Object(Object&& a) : impl(a.impl) { UDM_ASSERT(impl != NULL); a.impl = NULL; }
#endif
		~Object() 
		{
#ifdef UDM_RVALUE
			if (impl)
#endif
			{
			UDM_ASSERT(impl != NULL);
			impl->release(); 
#ifdef _DEBUG
			impl = NULL;
#endif
			}
		}

		const Object &operator =(const Object &a)
		{

			UDM_ASSERT(a.impl != NULL);
			ObjectImpl *t = a.impl->clone();

			UDM_ASSERT(impl != NULL);
			impl->release();

			impl = t;
			return a;
		}

#ifdef UDM_RVALUE
		Object& operator=(Object&& a)
		{
			if (this != &a) {
				if (impl != NULL)
					impl->release();
				impl = a.impl;
				a.impl = NULL;
			}
			return *this;
		}
#endif

		static Object Cast(const Object &a, const ::Uml::Class &meta) { return __Cast(a, meta); }
		
		//only dummy. ;) This is needed because when ParentAttr is invoked with Udm::Object template 
		//parameter, some method of that template class CLASS require the existence of a static method
		// CLASS::Cast(Object &) 

		static Object Cast(const Object &a) { return a; }	
		static Object Create(const ::Uml::Class &meta, const Object &parent, 
			const ::Uml::CompositionChildRole &role, 
			const ObjectImpl * archetype = &_null,
			const bool subtype = false) 
		{ 
			return  __Create(meta, parent, role, archetype, subtype); 
		}

	protected:
		static ObjectImpl *__Cast(const Object &a, const ::Uml::Class &meta);
#ifdef UDM_RVALUE
		static ObjectImpl *__Cast(Object &&a, const ::Uml::Class &meta);
#endif
		static ObjectImpl *__Create(const ::Uml::Class &meta, const Object &parent,
			const ::Uml::CompositionChildRole &role,
			const ObjectImpl *  archetype = &_null,
			const bool subtype = false);

	
	public:
		// --- non-array/single value attributes

		string getStringAttr(const ::Uml::Attribute &meta) const { return impl->getStringAttr(meta); }
		void setStringAttr(const ::Uml::Attribute &meta, const string &a) { impl->setStringAttr(meta, a); }

		bool getBooleanAttr(const ::Uml::Attribute &meta) const { return impl->getBooleanAttr(meta); }
		void setBooleanAttr(const ::Uml::Attribute &meta, bool a) { impl->setBooleanAttr(meta, a); }

		__int64 getIntegerAttr(const ::Uml::Attribute &meta) const { return impl->getIntegerAttr(meta); }
		void setIntegerAttr(const ::Uml::Attribute &meta, __int64 a) { impl->setIntegerAttr(meta, a); }

		double getRealAttr(const ::Uml::Attribute &meta) const { return impl->getRealAttr(meta); }
		void setRealAttr(const ::Uml::Attribute &meta, double a) { impl->setRealAttr(meta, a); }

		// --- multiple value /array attributes
		vector<string> getStringAttrArr(const ::Uml::Attribute &meta) const { return impl->getStringAttrArr(meta); }
		void setStringAttrArr(const ::Uml::Attribute &meta, const vector<string> &a) { impl->setStringAttrArr(meta, a); }

		vector<bool> getBooleanAttrArr(const ::Uml::Attribute &meta) const { return impl->getBooleanAttrArr(meta); }
		void setBooleanAttrArr(const ::Uml::Attribute &meta, const vector<bool> &a) { impl->setBooleanAttrArr(meta, a); }

		vector<__int64> getIntegerAttrArr(const ::Uml::Attribute &meta) const { return impl->getIntegerAttrArr(meta); }
		void setIntegerAttrArr(const ::Uml::Attribute &meta, const vector<__int64> &a) { impl->setIntegerAttrArr(meta, a); }

		vector<double> getRealAttrArr(const ::Uml::Attribute &meta) const { return impl->getRealAttrArr(meta); }
		void setRealAttrArr(const ::Uml::Attribute &meta, const vector<double> &a) { impl->setRealAttrArr(meta, a); }


		long getAttrStatus(const ::Uml::Attribute &meta) const { return impl->getAttrStatus(meta); }


	// --- associations

	public:
/*
		template
		class AssocClass

  a.bs()
  a.bs_Classes().Class
  a.bs_Classes().bs_end
  a.bs_Classes().as_end
*/
		set<Object> getAssociation(const ::Uml::AssociationRole &meta, int mode = TARGETFROMPEER) const
		{
			set<Object> ret;

			vector<ObjectImpl*> a = impl->getAssociation(meta, mode);
			vector<ObjectImpl*>::const_iterator i = a.begin();
			while( i != a.end() )
			{
				ret.insert(*i);
				++i;
			}

			return ret;
		}

		

		void setAssociation(const ::Uml::AssociationRole &meta, const set<Object> &a, int mode = TARGETFROMPEER)
		{
			vector<ObjectImpl*> b;

			set<Object>::const_iterator i = a.begin();
			while( i != a.end() )
			{
				b.push_back((*i).impl);
				++i;
			}

			impl->setAssociation(meta, b, mode);
		}

		void connectTo(const ::Uml::AssociationRole &meta, const Object &target, const vector<Object> &refs = vector<Object>())
		{
			vector<ObjectImpl*> refs_impl;

			vector<Object>::const_iterator i = refs.begin();
			while ( i != refs.end() )
			{
				refs_impl.push_back((*i).impl);
				++i;
			}

			impl->connectTo(meta, target.impl, refs_impl);
		}

		void disconnectFrom(const ::Uml::AssociationRole &meta, const Object &peer)
		{
			impl->disconnectFrom(meta, peer.impl);
		}

		vector<Object> getConnectingChain(const ::Uml::AssociationRole &meta, const Object &peer) const
		{
			vector<Object> ret;

			vector<ObjectImpl*> a = impl->getConnectingChain(meta, peer.impl);
			vector<ObjectImpl*>::const_iterator i = a.begin();
			while( i != a.end() )
			{
				ret.push_back(*i);
				++i;
			}

			return ret;
		}

	// --- type

	public:
		const ::Uml::Class &type() const { return impl->type(); }

	// --- order

	public:
		typedef ObjectImpl::uniqueId_type uniqueId_type;

		uniqueId_type uniqueId() const { return impl->uniqueId(); }

		bool operator ==(const Object &a) const { return uniqueId() == a.uniqueId(); }
		bool operator !=(const Object &a) const { return uniqueId() != a.uniqueId(); }
		bool operator <(const Object &a) const { return uniqueId() < a.uniqueId(); }
		bool operator >(const Object &a) const { return uniqueId() > a.uniqueId(); }
		bool operator <=(const Object &a) const { return uniqueId() <= a.uniqueId(); }
		bool operator >=(const Object &a) const { return uniqueId() >= a.uniqueId(); }
		bool operator !() const { return uniqueId() == 0; }
		operator bool()  const { return  uniqueId() != 0; }

		// UDM TOMI Paradigm Independent Interface
		struct AssociationInfo
		{
			// clsAssociation can be empty (ClassType(NULL)) - simple association.
			const ::Uml::Class & clsAssociation;
			string strSrcRoleName;
			string strDstRoleName;
			AssociationInfo(const ::Uml::Class & ref) : clsAssociation(ref){};
		};

		// UDM TOMI Paradigm Independent Interface
		struct CompositionInfo
		{	
			string strParentRoleName;
			string strChildRoleName;
		};
		// UDM TOMI Paradigm Independent Interface
		// Retrieves the adjacent objects of an object associated via simple association or association class. 
		// The returned set can be empty. Composition relationships are not considered here.
		multiset<Object> GetAdjacentObjects();
		set<Object> GetAdjacentUniqueObjects();
		// UDM TOMI Paradigm Independent Interface
		// Retrieves the adjacent objects of an object. The adjacent objects 
		// are of the type of clsType or derived from it. The returned set can 
		// be empty. Composition relationships are not considered here.
		multiset<Object> GetAdjacentObjects(const ::Uml::Class & clsDstType);
		set<Object> GetAdjacentUniqueObjects(const ::Uml::Class & clsDstType);
		// UDM TOMI Paradigm Independent Interface
		// Retrieves the adjacent objects of an object via link instance of ascType. 
		// The adjacent objects are of the type of clsType or derived from it. 
		// The returned set can be empty. Composition relationships are not considered here. 
		// Parameter clsType can be null.
		multiset<Object> GetAdjacentObjects(const ::Uml::Class & clsDstType, const AssociationInfo& ascType);
		set<Object> GetAdjacentUniqueObjects(const ::Uml::Class & clsDstType, const AssociationInfo& ascType);
		// UDM TOMI Paradigm Independent Interface
		// Retrieves the adjacent objects, together with the association class, of an object via link instance of ascType. 
		// The adjacent objects are of the type of clsType or derived from it. 
		// The returned set can be empty. Composition relationships are not considered here. Associations without an association class are ignored.
		// Parameter clsType can be null.
		multiset< pair<Object, Object> > GetAdjacentObjectsWithAssocClasses(const ::Uml::Class & clsDstType, const AssociationInfo& ascType);
		// UDM TOMI Paradigm Independent Interface
		// Get attributes by name. These are INEFFICIENT functions using iteration.
		// These functions return false if the attribute with the specified type
		// and attribute name does not exist. Hence these parameters are specified
		// in the metamodel, it can be serious error. If no problem they retrieve true.
		bool GetIntValue(string strAttrName, __int64& value) const;
		bool GetIntValue(string strAttrName, string& value) const;
		
		// Himanshu: array attribute getters
		bool GetIntValues(string strAttrName, vector<__int64>& values) const;
		bool GetRealValues(string strAttrName, vector<double>& values) const;
		bool GetStrValues(string strAttrName, vector<string>& values) const;
		bool GetBoolValues(string strAttrName, vector<bool>& value) const;

		// Himanshu: array attribute setters
		bool SetIntValues(string strAttrName, const vector<__int64>& values);
		bool SetRealValues(string strAttrName, const vector<double>& values);
		bool SetStrValues(string strAttrName, const vector<string>& values);
		bool SetBoolValues(string strAttrName, const vector<bool>& values);


		// UDM TOMI Paradigm Independent Interface
		// get the value of Attribute named strAttrName of type String
		// return false if not found,true if ok
		bool GetStrValue( string strAttrName, string& value) const;
		bool GetStrValue( string strAttrName, __int64& value) const;

		// UDM TOMI Paradigm Independent Interface
		// get the value of Attribute named strAttrName of type Real
		// return false if not found,true if ok
		bool GetRealValue( string strAttrName, double& value) const;
		// UDM TOMI Paradigm Independent Interface
		// get the value of Attribute named strAttrName of type Bool
		// return false if not found,true if ok
		bool GetBoolValue( string strAttrName, bool& value) const;
		// UDM TOMI Paradigm Independent Interface
		// get the value of Attribute named strAttrName of type Int
		// return false if not found,true if ok
		bool SetIntValue( string strAttrName, const __int64& value);
		bool SetIntValue( string strAttrName, const string& value);
		// UDM TOMI Paradigm Independent Interface
		// set the value of Attribute named strAttrName of type String
		// return false if not found,true if ok
		bool SetStrValue(string strAttrName, const __int64& value);
		bool SetStrValue(string strAttrName, const string& value);
		// UDM TOMI Paradigm Independent Interface
		// set the value of Attribute named strAttrName of type Real
		// return false if not found,true if ok
		bool SetRealValue( string strAttrName, double value);
		// UDM TOMI Paradigm Independent Interface
		// set the value of Attribute named strAttrName of type Bool
		// return false if not found,true if ok
		bool SetBoolValue( string strAttrName, bool value);
		// UDM TOMI Paradigm Independent Interface
		// Functions for composition
		// Returns the parent object.
		Object GetParent() const;
		// UDM TOMI Paradigm Independent Interface
		// Retrieves all children not considering types and role names.
		set<Object> GetChildObjects() const;
		// UDM TOMI Paradigm Independent Interface
		// Retrieves all children considering child types but not role names.
		set<Object> GetChildObjects(const ::Uml::Class & clsType) const;
		// UDM TOMI Paradigm Independent Interface
		// Retrieves all children considering role names and child types.
		// To ignore child types set clsChildType to ClassType(NULL).
		set<Object> GetChildObjects(const CompositionInfo& cmpType, const ::Uml::Class & clsChildType) const;
		// UDM TOMI Paradigm Independent Interface
		// Get the composition child role the given child has in this parent. Sets the ret parameter to NULL if the given child is not actually a child.
		void GetChildRole(const Object &child, ::Uml::CompositionChildRole &ret) const;
		// UDM TOMI Paradigm Independent Interface
		// Test if the object is in the tree rooted at where
		bool IsNodeOfTree(const Object &where);
		
		// Functions for associations and links
		// UDM TOMI Paradigm Independent Interface
		// Gets the objects of the association class from between two objects. 
		// ascType.clsAssociation  should be NULL to be ignored.
		set<Object> GetAssociationClassObjects(Object dstObject, const AssociationInfo& ascType, bool tryCrossLink = false);
		// UDM TOMI Paradigm Independent Interface
		// Gets the two peers from an object of association class type
		pair<Object,Object> GetPeersFromAssociationClassObject();
		// UDM TOMI Paradigm Independent Interface
		// For an association class instance, returns the source and, respectively, the
		// destination, if the source role or destination role can be established (MGA backend).
		// The Null object is returned if the roles cannot be established.
		Object getSrcObject();
		Object getDstObject();
		// Functions for creating Udm entities
		// UDM TOMI Paradigm Independent Interface
		// Creates an object of clsType
		Object CreateObject(const ::Uml::Class & clsType, const Udm::Object& archetype = &Udm::_null, const bool subtype = false);

		Object CreateObject(const ::Uml::Class & clsType, const CompositionInfo& compType, const Udm::Object& archetype = &Udm::_null, const bool subtype = false);
		// UDM TOMI Paradigm Independent Interface
		// Creates a link of a simple association or an association with association class. 
		// If ascType.clsAssociation is not valid a simple association will be tried.
		// On error results in false, true otherwise.
		bool CreateLink(Object dstObject, const AssociationInfo& ascType,const bool direct = true);
		// Removes a link of a simple association or an association with association class
		// if AscType.clsAssociation is not valid a simple association will be attempted
		// On error or when there is no such link false, otherwise true is returned.	
		bool DeleteLink(Object dstObject, const AssociationInfo& ascType);
		// Removes an object from the persistent storage.
		void DeleteObject();
		//get one specific end of an association class object
		Object GetAssociationEnd(string roleName);

		//UdmPseudoLike API

		bool GetIntVal( const cint_string &cstrAttrName, long &value) const;
		bool GetRealVal( const cint_string &cstrAttrName, double& value) const;
		bool GetStrVal( const cint_string &cstrAttrName, cint_string &value) const;
		bool GetBoolVal( const cint_string &cstrAttrName, bool& value) const;
		bool GetBoolVal( const cint_string &cstrAttrName, long& value) const;

		bool SetIntVal( const cint_string &cstrAttrName, long value);
		bool SetRealVal( const cint_string &cstrAttrName, double value);
		bool SetStrVal( const cint_string &cstrAttrName, cint_string &value);
		bool SetStrVal( const cint_string &cstrAttrName, char *value);
		bool SetBoolVal( const cint_string &cstrAttrName, bool value);





		//archetype/derived/instances relationship

		Object archetype() const;
		set<Object> derived() const;
		set<Object> instances() const;
		bool hasRealArchetype() const;
		bool isInstance() const;
		bool isSubtype() const;


		//some tomi extension functions
		
		// find the (first, there should be only one) child (only first-level children)Object in 
		// where, which is an inherited subtype of this(it's there because the 
		// parent(where) is directly(Primary) derived)
		//throws exception if not found
		Object FindCorrespondingObjectInSubtypes(const Object & where, const bool prim_derived = false) const;


		// find the (first, there should be only one) child (only first-level children)Object in 
		// where, which is an inherited instance of this(it's there because the 
		// parent(where) is directly(Primary) instantiated)
		//throws exception if not found
		Object FindCorrespondingObjectInInstances(const Object & where, const bool prim_derived = false) const;


		// find the (first, there should be only one) child (recursive, whole three)Object in 
		// where, which is an inherited subtype of this(it's there because the 
		// parent(where) is directly(Primary) derived)
		//return Udm::_null, if none found
		//Object reqFindCorrespondingObjectInSubtypes(const Object& where, const bool prim_derived = false) const;

		// find the (first, there should be only one) child (recursive, whole three)Object in 
		// where, which is an inherited instance of this(it's there because the 
		// parent(where) is directly(Primary) derived)
		//return Udm::_null, if none found
		//Object reqFindCorrespondingObjectInInstances(const Object& where, const bool prim_derived = false) const;

		//functions which invoke the above 2 functions,
		//and throw exception if they return Udm::_null
		Object FindCorrespondingObjectInInstancesTree(const Object& where, const bool prim_derived = false) const;
		Object FindCorrespondingObjectInSubtypesTree(const Object& where, const bool prim_derived = false) const;

		//get the set of my parent's (primary derived)instances/subtypes
		set<Object> GetMyParentsSubtypes(const bool only_primary_derived = false) const;
		set<Object> GetMyParentsInstances(const bool only_primary_derived = false) const;	
		
		//check if an object has derived or instantiated objects
		bool HasObjectSubtypes(const bool only_primary_derived = false) const;
		bool HasObjectInstances(const bool only_primary_derived = false) const;
		
		//find the closest parent, which is primarily derived(has a real archetye);
		Object FindClosestPrimarilyDerivedParent() const;

		//finds a derived object in of this in where
		Object getDerivedObjectInTree(const Udm::Object &where) const;
		//check if which is within the same derived/instantiated block
		Object getDerivedObjectInPrimarilyDerivedBlock(const Udm::Object &where) const;


		//finds an instantied object in of this in where
		Object getInstantiatedObjectInTree(const Udm::Object &where) const;
		//check if which is within the same derived/instantiated block
		Object getInstantiatedObjectInPrimarilyDerivedBlock(const Udm::Object &where) const;


		//checks if the object is a reference object. 
		//if so, and it's not an empty reference, returns the referred object
		//Note: References in UDM does not really exist, it's merely an MGA concept.
		//in UDM references are emulated with associations with rolename "ref" at the referenced object
		Object getReferencedObject(const string refrole = "ref") const;


		//general container class
		Object container() const;

		set<Object> getChildrenByChildRoleChain(const ::Uml::Class& meta, vector< ::Uml::CompositionChildRole> chain) const;

		//general accept function
		virtual void Accept(const BaseVisitor&){};

		string getPath( const std::string& strDelimiter = "./.", bool bReverseOrder = false, bool bNeedRootFolder = false, const string &att_name = "name", bool omit_lead_delim = false) const;
		// root folder is added by default, no reverse order suport, delimiter is never added to the front
		string getPath2( const std::string& strDelimiter = "./.", bool bNeedRootFolder = true ) const;
		// distance from root; root is at level zero
		int depth_level() const;

		string toString() const;

		// libraries
		virtual bool isLibObject() const;
		virtual bool isLibRoot() const;
		virtual bool getLibraryName(string &name) const;
		virtual void setLibraryName(const char *name);
		virtual Object AttachLibrary(const Object &lib_src, const string &lib_name, t_lib_to_copy_map *copy_map = NULL);


	};

	class udmvariant {
	public: 
		//simple attribute values
		udmvariant(const string &a) : strval(a) { ; }
		udmvariant(const __int64 &a) : longval(a) { ; }
		udmvariant(const double &a) : doubleval(a) { ; }
		udmvariant(const bool &a) : boolval(a) { ; }

		//array attribute values
		udmvariant(const vector<string> &a) : strarrval(a) { ; }
		udmvariant(const vector<__int64> &a) : longarrval(a) { ; }
		udmvariant(const vector<double> &a) : doublearrval(a) { ; }
		udmvariant(const vector<bool> &a) : boolarrval(a) { ; }

		udmvariant(const Object &a) : objval(a) { ; }
		udmvariant(const set<Object> &a) : setval(a) { ; }
		//enum udmt type;			//what's this? 
		
		set<Object> setval;
		Object  objval;
		
		//simple attributes
		string strval;
		__int64    longval;
		double  doubleval;
		bool   boolval;
		//array attributes
		vector<string>	strarrval;
		vector<__int64>    longarrval;
		vector<double>  doublearrval;
		vector<bool>	boolarrval;
		

	};



// --------------------------- DataNetwork

enum BackendSemantics { 
	CHANGES_PERSIST_ALWAYS,		// committed sequences are recorded in original backend
								// no CloseNoUpdate,  CloseAs, SaveAs will be used
	CHANGES_PERSIST_DEFAULT,	// committed sequences are recorded in current backed unless
								// CloseNoUpdate,  CloseAs, or SaveAs is used
	CHANGES_LOST_DEFAULT };		// committed sequences are lost until 
								// Close, CloseAs, or SaveAs are used

struct UdmDiagram { 
	::Uml::Diagram *dgr; 
		void (*init)(void);
};

class DataNetwork {
	friend class ObjectImpl;
public:
	UDM_DLL DataNetwork & operator=(DataNetwork &a); // enable assignment (was disable assignment)

protected:
// Non-persistent attributes
	ttmap tempvarmap;
	Object rootobject;
	UdmProject * pr;
	bool _static_pr;			//true if belongs to a static project
	const Udm::UdmDiagram &metaroot;
	UDM_DLL DataNetwork(const Udm::UdmDiagram &metainfo, UdmProject * project = NULL);

public:
	struct bt {
		string sig, ext;
		Udm::DataNetwork *(*crea)(const Udm::UdmDiagram &, Udm::UdmProject*);
		struct bt *next;
	};
	typedef struct bt backendtabt;
private: 
	
	static backendtabt *backendtab;
	static unsigned long dn_id_gen;
	unsigned long dn_id;

public:
	virtual UDM_DLL ~DataNetwork();
// Load/Persist
	virtual void CreateNew(const string &systemname, 
									const string &metalocator, const ::Uml::Class &rootclass, 
									enum BackendSemantics sem = CHANGES_PERSIST_ALWAYS) = 0;
	virtual void OpenExisting(const string &systemname, 
									const string &metalocator = "",
									enum BackendSemantics sem = CHANGES_PERSIST_ALWAYS) = 0;
	virtual void CloseWithUpdate() = 0;
	virtual void CloseNoUpdate()			{  throw udm_exception("Unsupported method"); }
	virtual void SaveAs(string systemname) {  throw udm_exception("Unsupported method"); }
	virtual void CloseAs(string systemname) {  throw udm_exception("Unsupported method"); }


	bool isOpen() { return rootobject; }
	unsigned long uniqueId(){return dn_id;};
// transaction control
	virtual void CommitEditSequence() = 0;
	virtual void AbortEditSequence() = 0;

// basic access
	virtual Object GetRootObject() const { 
//		if(!rootobject) throw udm_exception("Null rootobject!!"); 
		return rootobject; 
	}

	virtual UdmProject* GetProject() {return pr;};
	virtual void UDM_DLL setStaticUdmProject(StaticUdmProject* _pr);
	virtual void resetStaticUdmProject() { if (!(pr && _static_pr)) throw("Datanetwork is either not part of a project or is part of a non-static project!"); pr = NULL; };

	virtual Object ObjectById(Object::uniqueId_type) { 
		throw udm_exception("This backend does not support ObjectByID"); 
	}
	const ::Uml::Diagram &GetRootMeta() const { 
		return *metaroot.dgr;
	}

	static void UDM_DLL RegisterBackend(const string &sig, const string &ext, DataNetwork *(*crea)(const UdmDiagram &, UdmProject*));
	static void UDM_DLL UnRegisterBackends();
	static UDM_DLL string DumpBackendNames();
	static UDM_DLL DataNetwork *CreateBackend(string backendID, const UdmDiagram &metainfo, UdmProject* project = NULL);
	static UDM_DLL DataNetwork *GetDnById(const unsigned long id);

//type safe
// StaticObjects do not copy the types in constructors
// They instead just reference it
// so care needs to be taken when using them
// DOM&GME are TypeSafe Data Networks
// we override this for Static and Smart
	virtual UDM_DLL bool IsTypeSafe();
//string support
	virtual	UDM_DLL void CreateNewToString(const string &metalocator, const ::Uml::Class &rootclass, 
									enum Udm::BackendSemantics sem = Udm::CHANGES_PERSIST_ALWAYS);
	virtual UDM_DLL void OpenExistingFromString(string &str, 
									const string &metalocator, 
									enum Udm::BackendSemantics sem = Udm::CHANGES_PERSIST_ALWAYS);
	virtual UDM_DLL const string & Str();

	virtual UDM_DLL set<Object> GetAllInstancesOf(const ::Uml::Class& meta);

	virtual UDM_DLL struct AssociationRoleInfo GetAssociationRoleInfo(const ::Uml::AssociationRole &role) const;

	friend void ::Uml::Initialize();
};

// This function is provided for backwards-compatibility:
//  If a virtual function is added to DataNetwork, programs that are compiled against the augmented version and
//  instantiate a SmartDataNetwork will not link against older version of Udm
// TLDR: use CreateSmartDataNetwork instead of new SmartDataNetwork
UDM_DLL DataNetwork* CreateSmartDataNetwork(const UdmDiagram &metainfo, UdmProject * pr = NULL);

class SmartDataNetwork : public DataNetwork 
{
	DataNetwork *dn;
	bool str_based;
	string str;

public:
	//made public for CORBA to reach the inside Static DN
	DataNetwork *testdn() const 
	{
		if(dn) return dn;
		throw udm_exception("Missing data network");
	}

	SmartDataNetwork(const UdmDiagram &metainfo, UdmProject * pr = NULL) :
	  	DataNetwork(metainfo, pr) 
	{	
			dn = NULL;	
			str_based = false;
	}

	~SmartDataNetwork() 
	{ 
		delete dn;
	}
	
	SmartDataNetwork & operator=(SmartDataNetwork &a)
	{
		if (dn && a.dn)
		{
			*dn = *(a.dn);
		}
		else 
		{
			throw udm_exception("Either the source or the destination SmartDataNetwork does not exist.");
		}
		return *this;
	};

	SmartDataNetwork & operator=(DataNetwork &a)
	{
		if (dn )
		{
			*dn = a;
		}
		else 
		{
			throw udm_exception("The destination SmartDataNetwork does not exist.");
		}
		return *this;
	};
	
	virtual void CreateNew(const string &systemname, 
									const string &metalocator, const ::Uml::Class &rootclass, 
									enum BackendSemantics sem = CHANGES_PERSIST_ALWAYS) {
		if(dn) throw udm_exception("DataNetwork is already open"); 
		std::auto_ptr<DataNetwork> dn1(CreateBackend(systemname, metaroot, pr));
		if(!dn1.get()) throw udm_exception("Cannot deduce Udm backend type from " + systemname + "\n"
									 "Available backends: " + DumpBackendNames().c_str());
		dn1->CreateNew(systemname, metalocator, rootclass, sem);
		dn = dn1.release();
	}

	template<class CreateF>
	void CreateNew_(CreateF& f) {
		if(dn)
			throw udm_exception("DataNetwork is already open"); 
		dn = f(metaroot, pr);
	}

	virtual void OpenExisting(const string &systemname, 
									const string &metalocator = "", 
									enum BackendSemantics sem = CHANGES_PERSIST_ALWAYS) {
		if(dn) throw udm_exception("DataNetwork is already open"); 
		std::auto_ptr<DataNetwork> dn1(CreateBackend(systemname, metaroot,pr));
		if(!dn1.get()) throw udm_exception("Cannot deduce Udm backend type from " + systemname + "\n"
									 "Available backends: " + DumpBackendNames().c_str());
		dn1->OpenExisting(systemname, metalocator, sem);
		dn = dn1.release();
	}

	template<class OpenF>
	void OpenExisting_(OpenF& f) {
		if(dn)
			throw udm_exception("DataNetwork is already open"); 
		dn = f(metaroot, pr);
	}

	virtual void CloseWithUpdate() 
	{
		testdn()->CloseWithUpdate();

		if (str_based)
		{
			str = dn->Str();
			str_based = false;
		}
		delete dn;
		dn = NULL;
	}

	virtual void CloseNoUpdate()	
	{  
		testdn()->CloseNoUpdate();
		
		if (str_based)
		{
			str = dn->Str();
			str_based = false;
		}
	
		delete dn; 
		dn = NULL; 
	}
	virtual void SaveAs(string systemname) {  testdn()->SaveAs(systemname); }
	virtual void CloseAs(string systemname)
	{
		testdn()->CloseAs(systemname); 
		if (str_based)
		{
			str = dn->Str();
			str_based = false;
		}
	
		delete dn; 
		dn = NULL;
	}

	bool isOpen() { return dn ? dn->isOpen() : false; }
// transaction control
	virtual void CommitEditSequence() { testdn()->CommitEditSequence(); }
	virtual void AbortEditSequence() { testdn()->AbortEditSequence(); }

// basic access
	Object GetRootObject() const { return testdn()->GetRootObject(); }
	virtual Object ObjectById(Object::uniqueId_type t) { 
		return testdn()->ObjectById(t); 
	}
	const ::Uml::Diagram &GetRootMeta() const { 
		return testdn()->GetRootMeta(); 
	}
	virtual bool IsTypeSafe() { return testdn()->IsTypeSafe();};

	virtual UdmProject* GetProject() 
	{
		return testdn()->GetProject();
	};
	
	virtual void setStaticUdmProject(StaticUdmProject* _pr)
	{
		testdn()->setStaticUdmProject(_pr);
	};

	virtual void resetStaticUdmProject() 
	{ 
		testdn()->resetStaticUdmProject();
	};

	virtual void release()
	{
		dn = NULL;
	};

	//string support
	virtual	void CreateNewToString(const string &metalocator, const ::Uml::Class &rootclass, 
									enum Udm::BackendSemantics sem = Udm::CHANGES_PERSIST_ALWAYS)
	{
		if(dn) throw udm_exception("DataNetwork is already open"); 
		std::auto_ptr<DataNetwork> dn1(CreateBackend("string_dom.xml", metaroot, pr));//the name can be anything, which ends in .xml. String backend is supported only by DOM
		if(dn1.get() == NULL) throw udm_exception(string("Cannot deduce Udm backend type .xml from available backends: ") + DumpBackendNames().c_str());
		
		dn1->CreateNewToString(metalocator, rootclass,sem);
		dn = dn1.release();
		str_based = true;
	};
	virtual void OpenExistingFromString(string &str, 
									const string &metalocator, 
									enum Udm::BackendSemantics sem = Udm::CHANGES_PERSIST_ALWAYS)
	{
		if(dn) throw udm_exception("DataNetwork is already open"); 
		std::auto_ptr<DataNetwork> dn1(CreateBackend("string_dom.xml", metaroot, pr));//the name can be anything, which ends in .xml. String backend is supported only by DOM
		if(dn1.get() == NULL) throw udm_exception(string("Cannot deduce Udm backend type .xml from available backends: ") + DumpBackendNames().c_str());

		dn1->OpenExistingFromString(str,metalocator,sem);
		dn = dn1.release();
		str_based = true;
	};
	virtual const string & Str(){return str;};
	virtual set<Object> GetAllInstancesOf(const ::Uml::Class& meta){return testdn()->GetAllInstancesOf(meta);};

	virtual UDM_DLL struct AssociationRoleInfo GetAssociationRoleInfo(const ::Uml::AssociationRole &role) const
	{
		return testdn()->GetAssociationRoleInfo(role);
	}
};

	class UDM_DLL  MetaDepository
	{
		//we need to use pointers here because reference to reference is illegal

		static map<string, const UdmDiagram*>* meta_dep;
	public:

		//but we can handle the diagrams as references
		static const  UdmDiagram & LocateDiagram(const string& DgrName);
		static bool StoreDiagram(const string& DgrName, const UdmDiagram& dgr);
		static void RemoveDiagram(const string& DgrName);
	};

	


	
	//structure to specify a datanetwork inside a project
	class DataNetworkSpecifier
	{
		string _filename;
		string _metalocator;
		::Uml::Class& _rootclass;
		
	public:
		
		UDM_DLL DataNetworkSpecifier( string fn,  string ml, ::Uml::Class& rc);
		UDM_DLL DataNetworkSpecifier& operator=(const DataNetworkSpecifier &a);
		UDM_DLL const string filename();
		UDM_DLL const string metalocator();
		UDM_DLL const ::Uml::Class& rootclass();

	};

	//structure to specify an existing datanetwork inside a static project
	class StaticDataNetworkSpecifier
	{
		string _filename;
		//one of these must be NULL
		SmartDataNetwork * datanetwork;
		DataNetwork *simpledatanetwork;

	public:
		
		UDM_DLL DataNetwork* GetDataNetwork();
		UDM_DLL StaticDataNetworkSpecifier(string fn, SmartDataNetwork * dn);
		UDM_DLL StaticDataNetworkSpecifier(string fn, DataNetwork * dn);
		UDM_DLL const string filename();
		
	};



	class UDM_DLL DynamicMetaSpecifier
	{
	public:
#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable:4251) // Disable 'needs to have dll-interface' warning
#endif
		const string metalocator;
#ifdef _WIN32
#pragma warning(pop)
#endif
		const Udm::UdmDiagram& dgr;
		DynamicMetaSpecifier(const string& ml, const UdmDiagram& d) : metalocator(ml), dgr(d) {};
	};


	//These should be the same with the delimiter defined in the UML2XML interpretter!
	static const char * cross_delimiter = "_cross_ph_";
	
	class UdmProject
	{
			
	private:
		string temp_path;
		//file I/O routins doing the ZIP compression
		//here we expect that zipFile is not empty
		void DeCompress();
		void Compress();

		
	protected:		
		//bidirectional mapping
		map<string, DataNetwork*> datanetworks;
		map<const DataNetwork*, string> rev_datanetworks;

		//semantics - as defined for Udm::Datanetworks
		Udm::BackendSemantics sem;
		Udm::BackendSemantics cross_meta_dn_sem;
		
		//the name of the project file
		string zipfile;

		//this will be the project datanetwork
		Udm::DataNetwork * project_dn;

		// this will be an ::UdmProject::Project object - the roo object of the Project data network
		Object Project; 

		//projects containing metainformation for cross-data links
		DataNetwork* cross_meta_dn;	

		//metainformation for projects containing cross-data links
		UdmDiagram* cross_meta;		

		//the project containing crosslinks
		DataNetwork * cross_links;	

		//vector holding UdmDiagram objects which were allocated on the heap during the CreateNew process
		//vector<DynamicMetaSpecifier>  dms;
		map<const string, const UdmDiagram*> dms;

		
		/*
		 *functions which handle the "placeholders" - that is - a reference in the 
		 *datanetwork containg the cross links to the real object
		 * we needed to make GetPlaceHolder them public because of the CreateLink Udm TOMI function
		 * which needed to be cross-link compatible
		 */

	public:
		//obtaing a reference to a real object. (if create is true, the reference is created)
		Udm::Object GetPlaceHolder(const Udm::Object& o, bool create = true);	
		//get the real object from a place holder object
		Udm::Object GetRealObject(const Udm::Object& ph);

	protected:
		//private function which attempts to look up the requested meta
		//dynamically and if the lookup failed will use the static
		//metadepository
		const  UdmDiagram & LocateDiagram(const string& DgrName);


	public:
		//constructor
		UDM_DLL UdmProject(bool static_pr = false);

		//this will automatically open the datenetwork containing the cross links
		virtual void UDM_DLL OpenExisting(const string & project_file, enum BackendSemantics = Udm::CHANGES_PERSIST_ALWAYS);

		//create new Udm project
		virtual void UDM_DLL CreateNew(const string & project_file, vector<DataNetworkSpecifier>, const Udm::UdmDiagram& cross_diag , enum BackendSemantics = Udm::CHANGES_PERSIST_ALWAYS);

		//create new meta Udm project
		virtual void UDM_DLL CreateNewMeta(const string & project_name, const string & project_file, vector<DataNetworkSpecifier>, enum BackendSemantics = Udm::CHANGES_PERSIST_ALWAYS);

		// Returns true if project is open.
		bool IsOpen() { return Project;}

		//get all the datanetworks inside a project
		UDM_DLL vector<DataNetwork*> GetDataNetworks();

		//get a specific datanetwork (by it's systemname)
		UDM_DLL Udm::DataNetwork&  GetDataNetwork(const string &which);
		

		//get all the metadiagrams
		UDM_DLL vector< ::Uml::Diagram> GetMetaDiagrams() const;

		/*
			A Udm Project will have either a Cross Meta Specifier(if the project is the result of
			the interpreter or a cross meta information.
			if IsCrossMetaSpecifier() returns true, GetCrossNetwork() can be called.
			if HassCrosMeta() returns true, GetCrossMeta() can be called.
			
		*/
		UDM_DLL bool IsCrossMetaSpecifier() const;
		UDM_DLL bool HasCrossMeta() const;
		
		UDM_DLL DataNetwork& GetCrossMetaNetwork();
		UDM_DLL UdmDiagram& GetCrossMeta() const;


		virtual UDM_DLL ~UdmProject();


		//crucial part -- simple cross-network association

		//setting simple associations
		void UDM_DLL setSimpleAssociation(Udm::Object& peer, set<Udm::Object>& peers,const ::Uml::AssociationRole& meta);

		//getting simple associations
		UDM_DLL set<Udm::Object> getSimpleAssociation(Udm::Object& peer,const ::Uml::AssociationRole& meta);

		//even more crucial part -- assoc. class cross-network associations
		

		//setting the links (objects in the middle) from either end
		void UDM_DLL setLinks(Udm::Object& peer, set<Udm::Object>& links, const ::Uml::AssociationRole& meta);
		//setting either end-point from the middle 
		void UDM_DLL setEnd(Udm::Object& link, Udm::Object& peer, const ::Uml::AssociationRole& meta);



		//getting the links (objects in the middle) from either end
		UDM_DLL set<Udm::Object> getLinks(Udm::Object& peer, const ::Uml::AssociationRole &meta);

		//getting either end from the object in the middle
		UDM_DLL Udm::Object getEnd(Udm::Object& link, const ::Uml::AssociationRole &meta);
		
		//getting the other end from one end
		UDM_DLL Udm::Object getPeer(Udm::Object& peer, const ::Uml::AssociationRole &meta);


		/*Remote ID validation mechanism
		Problem: ObjectIDs of UDM objects are stored in the cross-link datanetwork.
		In some backends (MEM for example) the ID of the objects are changed whenever 
		the datanetwork is loaded, so they need to be updated. 
		This function will be called the backend, after loading a datanetwork file which
		is part of a project.
		 */
		virtual void UDM_DLL ValidateIDs(const string& sysname, const map<const unsigned long, const unsigned long> &tr_map);

		/* Dynamic meta support */
		//add dynamic meta specifier
		virtual void UDM_DLL AddDynamicMeta(const DynamicMetaSpecifier&);

		//reset dynamic meta specifiers
		virtual void UDM_DLL ResetDynamicMetas();
		

	};

	/*
		A non-persistent UdmProject which is constructed from existing datanetworks
	*/
	class StaticUdmProject : public UdmProject
	{
	public:

		UDM_DLL ~StaticUdmProject();		// just a normal clean-up, no persistence issues
		void UDM_DLL Close();					//StaticUdmProjects must be closed before destruction


		
		UDM_DLL StaticUdmProject(vector<StaticDataNetworkSpecifier> sdns, const Udm::UdmDiagram& cross_diag);


		//all of these will throw an exception because they are not suitable for Static projects
		UDM_DLL StaticUdmProject();		//this will throw an exception - StaticUdmProject is constructed from existing datanetworks
		UDM_DLL StaticUdmProject(const string & project_file, vector<DataNetworkSpecifier>, const Udm::UdmDiagram& cross_diag , enum BackendSemantics = Udm::CHANGES_PERSIST_ALWAYS);
		void UDM_DLL OpenExisting(const string & project_file, enum BackendSemantics = Udm::CHANGES_PERSIST_ALWAYS);
		void UDM_DLL ValidateIDs(const string& sysname, const map<const unsigned long, const unsigned long> &tr_map);
		void UDM_DLL AddDynamicMeta(const DynamicMetaSpecifier&);
		void UDM_DLL ResetDynamicMetas();



	};


	template<class CLASS, class Pred>
	CrossAssocAttr<CLASS, Pred>::operator set<CLASS, Pred>() const
	{

		set<CLASS, Pred> ret;

		UdmProject* pr = NULL;
		if (impl && impl->__getdn())
			pr = impl->__getdn()->GetProject();
		if (pr != NULL)
		{
			Object peer = impl->clone();
			set<Object> peers = pr->getSimpleAssociation(peer, meta);
			set<Object>::iterator peers_i = peers.begin();
			while (peers_i != peers.end())
			{
				ret.insert(CLASS::Cast(*peers_i));
				peers_i++;
			};
		}
		else
		{
			throw udm_exception("Project is NULL!");
		}
		return ret;
	}

	template<class CLASS, class Pred>
	CrossAssocAttr<CLASS, Pred> & CrossAssocAttr<CLASS, Pred>::operator =(const set<CLASS, Pred> &a)
	{
		UdmProject* pr = impl->__getdn()->GetProject();
		if (pr != NULL)
		{
			Object peer = impl->clone();
			set<Object> peers;
			TYPENAME set<CLASS, Pred>::const_iterator i = a.begin();
			while( i != a.end() )
			{
				peers.insert((*i));
				++i;
			}

			pr->setSimpleAssociation(peer,peers,meta);
		}
		else
		{
			throw udm_exception("Project is NULL!");
		}

		return *this;
	}

	template<class CLASS>
	CrossPointerAttr<CLASS>::operator CLASS() const
	{
		UdmProject* pr = NULL;
		set<Object> peers;
		if (impl && impl->__getdn())
			pr = impl->__getdn()->GetProject();
		if (pr != NULL)
		{
			Object peer = impl->clone();
			peers = pr->getSimpleAssociation(peer, meta);
			if (peers.begin() != peers.end())
				return CLASS::Cast(*(peers.begin()));
		}
		else
		{
			throw udm_exception("Project is NULL!");
		}

		return &_null;

	}

	template<class CLASS>
	const CLASS & CrossPointerAttr<CLASS>::operator =(const CLASS &a)
	{
		UdmProject* pr = impl->__getdn()->GetProject();
		if (pr != NULL)
		{
			Object peer = impl->clone();
			set<Object> peers;
			peers.insert(a);

			pr->setSimpleAssociation(peer,peers,meta);
		}
		else
		{
			throw udm_exception("Project is NULL!");
		}

		return a;			
	}

	template<class CLASS>
	CrossAssocEndAttr<CLASS>::operator CLASS() const 
	{
		UdmProject *pr = NULL;
		CLASS ret;
		if (impl && impl->__getdn())
			pr = impl->__getdn()->GetProject();

		if (pr != NULL)
		{
			Object link = impl->clone();
			ret = CLASS::Cast(pr->getEnd(link, meta));
		}
		else
		{
			throw udm_exception("Project is NULL!");
		}

		return ret;
	}

	template<class CLASS>
	const CLASS & CrossAssocEndAttr<CLASS>::operator =(const CLASS &a)
	{
		UdmProject *pr = NULL;

		if (impl && impl->__getdn())
			pr = impl->__getdn()->GetProject();

		Object a_obj(a.__impl()->clone());
		if (pr != NULL)
		{
			Object link = impl->clone();
			pr->setEnd(link, a_obj, meta);
		}
		else
		{
			throw udm_exception("Project is NULL!");
		}

		return a;
	}

	template<class CLASS, class TARGETCLASS>
	AClassCrossPointerAttr<CLASS, TARGETCLASS>::operator CLASS() const
	{
		CLASS ret;
		UdmProject *pr = NULL;
			
		if (impl && impl->__getdn())
			pr = impl->__getdn()->GetProject();

		if (pr != NULL)
		{
			Object peer = impl->clone();
			set<Object> links = pr->getLinks(peer, peermeta);
			if (!links.empty())
				return CLASS::Cast(*(links.begin()));

		}
		else
		{
			throw udm_exception("Project is NULL!");
		}

		return ret;

	}

	template<class CLASS, class TARGETCLASS>
	const CLASS & AClassCrossPointerAttr<CLASS, TARGETCLASS>::operator =(const CLASS &a)
	{
		UdmProject *pr = NULL;

		if (impl && impl->__getdn())
			pr = impl->__getdn()->GetProject();

		if (pr != NULL)
		{
			Object peer = impl->clone();
			set<Object> links;
			links.insert(a);
			pr->setLinks(peer, links, peermeta);
		}
		else
		{
			throw udm_exception("Project is NULL!");
		}

		return a;

	}

	template<class CLASS, class TARGETCLASS, class Pred>
	AClassCrossAssocAttr<CLASS, TARGETCLASS, Pred>::operator set<CLASS, Pred>() const 
	{
		set<CLASS, Pred> ret;

		UdmProject *pr = NULL;

		if (impl && impl->__getdn())
			pr = impl->__getdn()->GetProject();

		if (pr != NULL)
		{
			Object peer = impl->clone();
			set<Object> links = pr->getLinks(peer, peermeta);
			set<Object>::iterator l_i = links.begin();
			while (l_i != links.end())
			{
				ret.insert(CLASS::Cast(*l_i));
				l_i++;
			};
		}
		else
		{
			throw udm_exception("Project is NULL!");
		}

		return ret;
	}

	template<class CLASS, class TARGETCLASS, class Pred>
	AClassCrossAssocAttr<CLASS, TARGETCLASS, Pred> & AClassCrossAssocAttr<CLASS, TARGETCLASS, Pred>::operator =(const set<CLASS, Pred> &a)
	{

		UdmProject *pr = NULL;

		if (impl && impl->__getdn())
			pr = impl->__getdn()->GetProject();

		if (pr != NULL)
		{
			Object peer = impl->clone();
			set<Object> links;
			TYPENAME set<CLASS, Pred>::const_iterator a_i = a.begin();
			while (a_i != a.end())
			{
				links.insert(*a_i);
				a_i++;
			};
			pr->setLinks(peer, links, peermeta);
		}
		else
		{
			throw udm_exception("Project is NULL!");
		}

		return *this;
	}

	template<class CLASS, class TARGETCLASS>
	CLASS AClassPointerAttr<CLASS, TARGETCLASS>::SetLink(TARGETCLASS peer, Object parent, 
		const ::Uml::Class &meta,
		const ::Uml::CompositionChildRole &role)
	{

		if(!IsDerivedFrom(meta, CLASS::meta)) {
					throw udm_exception("Invalid type for assoc class");
		}

		CLASS nn = CLASS::Cast(Object::Create(meta, parent, role));
		AssocEndAttr<TARGETCLASS> zz = AssocEndAttr<TARGETCLASS>(nn.__impl(), peermeta);
		zz = peer;
		*this = nn;
		return nn;
	}	

	template<class CLASS, class TARGETCLASS, class Pred>
	CLASS AClassAssocAttr<CLASS, TARGETCLASS, Pred>::AddLink(TARGETCLASS peer, Object parent, 
		const ::Uml::Class &meta,
		const ::Uml::CompositionChildRole &role)
	{

		if(!IsDerivedFrom(meta, CLASS::meta)) {
					throw udm_exception("Invalid type for assoc class");
		}

		CLASS nn = CLASS::Cast(Object::Create(meta, parent, role));
		AssocEndAttr<TARGETCLASS> zz = AssocEndAttr<TARGETCLASS>(nn.__impl(), peermeta);
		zz = peer;
		set<CLASS, Pred> b = *this;
		b.insert(nn);
		*this = b;
		return nn;
	}	

// Not part of the public API
// All internal Udm static data is stored here to be able to control the order of cleanup code and destructors
class UdmStaticData {
private:
	// DataNetwork id => DataNetwork
	map <const unsigned long, DataNetwork *> dntab;

	// UdmStatic
	set<UdmStatic::StaticObject*> so_set;
	list<UdmStatic::StaticDataNetwork*> SDNs;

	// Uml SafeTypes
	typedef pair<unsigned long, Udm::Object::uniqueId_type> type_ext_id_t;
	typedef map<type_ext_id_t, const Uml::Class *> type_map_t;
	typedef map<const Uml::Class *, long> ref_map_t;
	// type's uniqueId => SafeType Class*
	type_map_t type_map;
	// reference counting for SafeTypes
	ref_map_t ref_map;

	friend class DataNetwork;
	friend class UdmStatic::StaticDataNetwork;
	friend class UdmStatic::StaticObject;
	friend class Uml::SafeTypeContainer;

public:
	~UdmStaticData();
};
extern UdmStaticData _UdmStaticData;
} // namespace UDM_NAMESPACE

//static constants for UdmDom internal attributes
namespace UdmDom
{
	static const char * _udm_dom_ia_archetype = "_archetype";
	static const char * _udm_dom_ia_derived = "_derived";
	static const char * _udm_dom_ia_instances = "_instances";
	static const char * _udm_dom_ia_desynched_atts = "_desynched_atts";
	static const char * _udm_dom_ia_real_archetype = "_real_archetype";
	static const char * _udm_dom_ia_subtype = "_subtype";
	static const char * _udm_dom_ia_libname = "_libname";

};

//the biggest i64 integer is 20 characters long in decimal format.
#ifdef _WIN32
#if (_MSC_VER == 1200)
inline UDM_DLL std::ostream & operator<< (ostream &o, __int64 i) { char k[30];sprintf(k,"%I64d", i);	o << k; return o;};
#endif
#endif


inline UDM_DLL std::ostream & operator<< (std::ostream &o, Udm::StringAttr c) {
	o << static_cast<std::string>(c);
	return o;
}

//defines for using backends
#ifdef _WIN32
    #if !defined(UDM_DYNAMIC_LINKING)
        #define UDM_USE_DOM __pragma(comment(linker, "/include:?DummyFL@UdmDom@@3HA"))
        #define UDM_USE_MGA __pragma(comment(linker, "/include:?DummyFL@UdmGme@@3HA"))
    #else
        #define UDM_USE_DOM
        #define UDM_USE_MGA
    #endif
    #else
        #if (defined(__MACH__) || defined(__APPLE__) )
             //g++ on Mac considers this variable as static anyway
             //TODO: remove completly this Macro on MAC OS X, we use full dynamic linking
            #define UDM_USE_DOM namespace UdmDom{extern UDM_DLL int DummyFL;static int *Dummy_For_Linker = &DummyFL; struct _use_var{_use_var(){Dummy_For_Linker++;}} __unused;};
        #else
            #define UDM_USE_DOM namespace UdmDom{extern UDM_DLL int DummyFL;static int *Dummy_For_Linker = &DummyFL; static struct _use_var{_use_var(){Dummy_For_Linker++;}} __unused;};
        #endif
#endif



#pragma deprecated(CHANGES_PERSIST_ALWAYS, CHANGES_PERSIST_DEFAULT)
// CHANGES_PERSIST_ALWAYS and CHANGES_PERSIST_DEFAULT indicate that the DataNetwork should save
// data upon being destructed. However, the DataNetwork may throw an exception during destruction.
// If the DataNetwork is being destructed during a stack unwinding due to an exception and throws
// an exception, C++ guarantees that terminate() will be called.
// Therefore, these BackendSemantics should not be used. Instead, use an explicit call to
// CloseWithUpdate() (with catch clauses where appropriate).

// tldr: use CHANGES_LOST_DEFAULT and CloseWithUpdate() instead


// Version 3.2.15
#define UDM_VERSION_MAJOR 3
#define UDM_VERSION_MINOR (2 * 10 + 15 * 1)
#define UDM_VERSION UDM_VERSION_MAJOR * 100 + UDM_VERSION_MINOR
#endif//MOBIES_UDMBASE_H
