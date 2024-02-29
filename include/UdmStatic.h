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
	
  23/11/04	-	endre

			-		fixed bug which appeared under .net.
					(id_map was delete before some StaticObjects)

  05/31/04	-	endre
			-		porting issues to Linux/GCC
  04/13/04	-	endre

			-		added boolean var m_type_is_safetype to StaticObject to indicate if the m_type of 
					StaticObject is obtained through SafeTypeContainer.

  04/07/04	-	endre
  
		Exported CreateObject to DLL.	
  03/24/04	-	endre

		Changet the signature of StaticObject::createChild, as required by Udm::ObjectImpl

  06/19/02	-	endre
		
		  Array attribute setters/getters override declaration

  03/19/02	-	endre
		
		  Added class StaticDataNetwork. See comments in .cpp
		  also moved to $/include/udm


  */

#ifndef MOBIES_UDMSTATIC_H
#define MOBIES_UDMSTATIC_H

#ifndef MOBIES_UDMBASE_H
#include "UdmBase.h"
#endif


#include <cstdio> //file operations

/*
	this class is needed to prevent inserting exactly the same element-pair in a multimap
	( I wish STL had some solution for this issue )
*/

template <class T, class Z>
class udm_multimap : public std::multimap<T, Z>
{
	//no private members, others than those in multimap
public:

	udm_multimap<T,Z>() : std::multimap<T,Z>(){};
	std::pair<typename std::multimap<T, Z>::iterator, bool> safe_insert(typename std::multimap<T,Z>::value_type& item)
	{
		TYPENAME std::pair<TYPENAME std::multimap<T, Z>::iterator, TYPENAME std::multimap<T, Z>::iterator> it_pair = this->equal_range(item.first);
		TYPENAME std::multimap<T, Z>::iterator i = it_pair.first;
		bool found = false;
		while (i != it_pair.second && !found)
		{
			if (i->second == item.second)found = true;
			i++;
		}
		
		if (!found)
		{
			TYPENAME std::multimap<T, Z>::iterator inserted_at = this->insert(item);
			return TYPENAME std::pair<TYPENAME std::multimap<T, Z>::iterator,bool> (inserted_at, true);
		}
		return TYPENAME std::pair<TYPENAME std::multimap<T, Z>::iterator,bool>(this->end(), false);
	}
};

namespace UdmStatic
{
	using namespace std;
	typedef Udm::Object Object;
	typedef Udm::ObjectImpl ObjectImpl;
	typedef ObjectImpl::uniqueId_type  uniqueId_type;

	class StaticObject;




	uniqueId_type GetIdForObject(StaticObject* p);
	StaticObject* GetObjectForId(uniqueId_type l);
	void InsertObject(StaticObject* so);
	

	//-------------------------StaticDataNetwork

	class StaticDataNetwork : public Udm::DataNetwork 
	{
		string systemname;
		Udm::BackendSemantics sem;
		
		//pointer to the datanetwork which can ObjectByID() metaobjects;
		//this is not always trivial, see comment in constructor
		Udm::DataNetwork * meta_dn;		
	public:
	
		static bool SafeObjectById;
		UDM_DLL StaticDataNetwork(const Udm::UdmDiagram &metainfo, Udm::UdmProject* project = NULL);
		UDM_DLL ~StaticDataNetwork();
		UDM_DLL Object ObjectById(Object::uniqueId_type id);

#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable:4995) // Disable CHANGES_PERSIST_ALWAYS deprecated warning
#endif
		UDM_DLL void CreateNew(const string &systemname, 
								const string &metalocator, const ::Uml::Class &rootclass, 
								enum Udm::BackendSemantics sem = Udm::CHANGES_PERSIST_ALWAYS);
		
		

		UDM_DLL void OpenExisting(const string &systemname, 
								const string &metalocator, 
								enum Udm::BackendSemantics sem = Udm::CHANGES_PERSIST_ALWAYS);
#ifdef _WIN32
#pragma warning(pop)
#endif

		
		UDM_DLL void SaveAs(string systemname);
		
		UDM_DLL void CloseWithUpdate()				{SaveAs(systemname); CloseNoUpdate();};
		UDM_DLL void CloseAs(string systemname)		{SaveAs(systemname); CloseNoUpdate();};
		UDM_DLL void CommitEditSequence()			{SaveAs(systemname);};
		
		//do nothing here

		UDM_DLL void AbortEditSequence()			{};
		UDM_DLL void CloseNoUpdate();
		
		bool isOpen()							
		{
			return rootobject;
		};

		static Udm::DataNetwork *factory(const Udm::UdmDiagram &meta, Udm::UdmProject* project = NULL) {
			return new StaticDataNetwork(meta, project);}
		//Static Data Network is _not_ a type safe data network
		// because m_type in StaticObject is an UML::Class reference!
		UDM_DLL bool IsTypeSafe() { return false;};
		unsigned long DeSerialize(FILE *f, map<unsigned long, const StaticObject*>& tr_map, Object& root_o);
		Udm::DataNetwork * GetMetaDn(){ return meta_dn;};

		friend void ::Uml::Initialize();
	};

	
	//------------------------StaticObject

	class StaticObject : public ObjectImpl
	{
		static void Cleanup();
		friend class Udm::UdmStaticData; // needs to alter the id_map_so_set_deleted flag
		friend unsigned long StaticDataNetwork::DeSerialize(FILE*, map<unsigned long, const StaticObject*>&, Object&);
		friend void CreateComposition(const Object &, const ::Uml::CompositionChildRole &, const Object &, const ::Uml::CompositionParentRole &);
		friend void Uml::Initialize();
	private:
		StaticDataNetwork * mydn;

		bool deleted;							//this becomes true when the object is deleted
												//(detach-ed from the object tree)
												//but it still exists, because of reffering Object-s

		//archetype/derived/instances support
		set<StaticObject*> st_and_i;			// in this will be stored all derived and instantiated objects
		StaticObject* archetype;				// in this will be stored the archetype
		bool subtype;							// this tells whether is subtype or instance
												// if subtype, this variable is true
												// if instance, it's false


		bool real_archetype;					// true when *this is a real archetype(set by the user)
												// false when *this	is not a real archetype. Is a child
												// of an object which has a real  archetype, and thus all 
												// it's children are archetyped recursively.


		//we should store the attributes desynched from archetype

		
		bool id_map_so_set_deleted;				// by default is false,
												// but will be set to true when ~id_map is invoked.
		
		string lib_name;
		bool lib_object;
		bool lib_root;

		// initialization needed from constructors
		void init();

	public:

		StaticObject(const ::Uml::Class &meta, 
					int ref, 
					StaticObject * archetype = NULL, 
					const bool subtype = false,
					const bool real_archetype = true);
		StaticObject(const ::Uml::Class &meta,
					StaticDataNetwork *dn,
					int ref);
		void Destroy(bool root);		//destroy the object network
										// this deletes all the objects explicitly,
										// to be used only when the data network is going away ..


		
		static udm_exception e;
		virtual ~StaticObject();



	// --- reference counting
		int refCount;
	//desynched attributes
		set<uniqueId_type> desynched;

		ObjectImpl *clone();
		void release();
		Udm::DataNetwork *__getdn();
		const ::Uml::Class &m_type;
		bool m_type_is_safetype;
	
		const ::Uml::Class &type() const;
		//void dumpref() {cout << refCount << endl;};
		//for debugging only!
		int ref(){  return refCount;};
	
	// --- order
	
		uniqueId_type uniqueId() const;

	// --- attributes (single-value /no-array/ type)
		typedef map<uniqueId_type, string> stringAttrs_type;
		stringAttrs_type stringAttrs;

		typedef map<uniqueId_type, bool> booleanAttrs_type;
		booleanAttrs_type booleanAttrs;

	//	typedef map<uniqueId_type, long> integerAttrs_type;
	//	integerAttrs_type integerAttrs;

		typedef map<uniqueId_type, __int64> longintAttrs_type;
		longintAttrs_type longintAttrs;

		typedef map<uniqueId_type, double> realAttrs_type;
		realAttrs_type realAttrs;
	
	// --- attributes (multiple value /array/ type)
		typedef vector<string> string_array;
		typedef map<uniqueId_type, string_array> stringAttrArrs_type;
		stringAttrArrs_type stringAttrArrs;
		
		typedef vector<bool> bool_array;
		typedef map<uniqueId_type, bool_array> booleanAttrArrs_type;
		booleanAttrArrs_type booleanAttrArrs;

		//typedef vector<long> long_array;
		//typedef map<uniqueId_type, long_array> integerAttrArrs_type;
		//integerAttrArrs_type integerAttrArrs;

		typedef vector<__int64> longint_array;
		typedef map<uniqueId_type, longint_array> longintAttrArrs_type;
		longintAttrArrs_type longintAttrArrs;


		typedef vector<double> double_array;
		typedef map<uniqueId_type, double_array> realAttrArrs_type;
		realAttrArrs_type realAttrArrs;
		


	// --- attribute setters/getters  for single value (no-array) type attributes
		string getStringAttr(const ::Uml::Attribute &meta) const;
		void setStringAttr(const ::Uml::Attribute &meta, const string &a, const bool direct = true);
		
		bool getBooleanAttr(const ::Uml::Attribute &meta) const;
		void setBooleanAttr(const ::Uml::Attribute &meta, bool a, const bool direct = true);
		
		__int64 getIntegerAttr(const ::Uml::Attribute &meta) const;
		void setIntegerAttr(const ::Uml::Attribute &meta, __int64 a, const bool direct = true);
		
		double getRealAttr(const ::Uml::Attribute &meta) const;
		void setRealAttr(const ::Uml::Attribute &meta, double a, const bool direct = true);
	
	//	--- attribute setters/getters for multiple value (array) type attributes

		vector<string> getStringAttrArr(const ::Uml::Attribute &meta) const;
		void setStringAttrArr(const ::Uml::Attribute &meta, const vector<string> &a, const bool direct = true);

		vector<bool> getBooleanAttrArr(const ::Uml::Attribute &meta) const;
		void setBooleanAttrArr(const ::Uml::Attribute &meta, const vector<bool> &a, const bool direct = true);

		vector<__int64> getIntegerAttrArr(const ::Uml::Attribute &meta) const;
		void setIntegerAttrArr(const ::Uml::Attribute &meta, const vector<__int64> &a, const bool direct = true);

		vector<double> getRealAttrArr(const ::Uml::Attribute &meta) const;
		void setRealAttrArr(const ::Uml::Attribute &meta, const vector<double> &a, const bool direct = true);

	//	--- attribute status

		long getAttrStatus(const ::Uml::Attribute &meta) const;


	// --- containment, maintance(create, delete)
		StaticObject *m_parent;	// not reference counted

		typedef multimap<uniqueId_type, StaticObject*> children_type;
		children_type m_children;	// reference counted

		ObjectImpl *getParent(const ::Uml::CompositionParentRole &role) const;
		void setParent(ObjectImpl *a, const ::Uml::CompositionParentRole &role, const bool direct = true);
		void detach();
		vector<ObjectImpl*> getChildren(const ::Uml::CompositionChildRole &meta, const ::Uml::Class &cls) const;
		void setChildren(const ::Uml::CompositionChildRole &meta, const vector<ObjectImpl*> &a, const bool direct = true);
		ObjectImpl *createChild(const ::Uml::CompositionChildRole &childrole, const ::Uml::Class &meta, const Udm::ObjectImpl* archetype = NULL, const bool subtype = false, const bool real_archetype = true, const bool need_safetype = false);
		void getChildRole(ObjectImpl *c, ::Uml::CompositionChildRole &ret) const;

	// --- associations
		typedef udm_multimap<uniqueId_type, StaticObject*> assoc_type;
		assoc_type associations;	// not reference counted, bidirectional
		

		vector<ObjectImpl*> getAssociation(const ::Uml::AssociationRole &meta, int mode = Udm::TARGETFROMPEER) const;
		void setAssociation(
			const ::Uml::AssociationRole &meta, 
			const vector<ObjectImpl*> &nvect, 
			int mode = Udm::TARGETFROMPEER,
			const bool direct = true);
	
		// --- persistency
		unsigned long Serialize(FILE * f);											// return the length of the object
		void ValidatePointers(map<unsigned long, const StaticObject *>& tr_map);	// changing the associations and the children to the new pointers

	// --- acrhetype/derived/instances
		vector<ObjectImpl*> getDerived() const;
		vector<ObjectImpl*> getInstances() const;
		ObjectImpl* getArchetype() const;
		bool hasRealArchetype() const;
		bool isSubtype() const ;
		bool isInstance() const ;
	

		
		StaticObject * FindCorrespondingObjectInStOrITree( StaticObject * where);
		StaticObject * FindCorrespondingObjectInStOrI( StaticObject * where);
		
		// libraries
	protected:
		void setLOFOnChildren(bool is_lib_object);
	public:
		bool isLibObject() const;
		bool isLibRoot() const;
		bool getLibraryName(string &name) const;
		void setLibraryName(const char *name);
		ObjectImpl *createLibRootChild(const ::Uml::Class &meta, const bool need_safetype = false);
		vector<ObjectImpl*> getLibRoots() const;
	};



	UDM_DLL Object CreateObject(const ::Uml::Class &meta);
	void CreateComposition(const Object &parent, const ::Uml::CompositionChildRole &childRole,
		const Object &child, const ::Uml::CompositionParentRole &parentRole);
	void CreateAssociation(const Object &src, const ::Uml::AssociationRole &dstRole,
		const Object &dst, const ::Uml::AssociationRole &srcRole);
}


#endif//MOBIES_UDMSTATIC_H
