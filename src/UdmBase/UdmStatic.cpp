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
	28/07/06	-	endre
			-		fix for UDM-37: in setAssociation() try to associate corresponding
					subtypes and instances even if an association exists between the
					main objects

	23/11/04	-	endre

			-		fixed bug which appeared under .net.
					(id_map was delete before some StaticObjects)
					also fixed some STL porting issues with VC++7.x (.NET)

	
	23/07/04	-	endre
			
			-		Changed the usage of reqcopyLinks. The function has to be called twice, with the
					first with simpleLinks = true, then with simpleLinks = false, in order to copy 
					both types of links

    14/07/04	-	endre

			-		Changed SetChildRole, setParentRole to include both role names as key.
					There could be a composition relationship between the same two classes and
					the same rolenames at one end. These two associations could not be differentiated
					by the target classes and one rolename.

    05/06/04	-	endre

			-		splitted setGenericRole() into SetChildRole, SetParentRole and SetAssocRole functions.
					Also added the other role's target as a key in finding a specific role, so name doesn't
					have to be unique among the roles pointing to a certaing class.


	05/06/04	-	endre

			-		added SetDiagram(), SetClass(), SetGenericRole() classes. They search and set a target diagram/class/role
					out from an existing diagram based on names and role targets.

	04/19/04	-	endre
			-		small bug in setAssociation(), before calling FindClosestPrimarilyDerivedParent on an object,
					it should be verified whether the object has an archetype at all.
	04/18/04	-	endre
			-		changed setAssociation(). Just learned that links can be added/removed from/to instance objects,
					if they point "outof the box" - that is, if the added or removed peers are not within the same inherited
					block
			-		when an object is detached, and the host datanetwork of the object is being part of a Udm project, 
					the corresponding placeholder object (if any) should be detached as well.
	04/13/04	-	endre

			-		added boolean var m_type_is_safetype to StaticObject to indicate if the m_type of 
					StaticObject is obtained through SafeTypeContainer.
			-		~StaticObject will invoke RemoveSafeType() only if m_type_is_safetype is true
	04/07/04	-	endre

			-		Removed RemoveSafeType() from StaticObject destructor until a better solution is found for the
				destructor order problem.
			-		Exported CreateObject() in DLL.

	04/05/04	-	endre

			StaticObject checks if the m_type reference is valid before invoking RemoveSafeType(m_type)
			Since we can not control the construction/destruction of global objects, 
			it can happen the the meta-meta objects (Uml::Class::meta, Uml::Diagram::meta) are deleted 
			before the metaobjects.

		
	03/24/04	-	endre

			Changed createChild(), new flag need_safetype added.
			When creating an object with an archetype, all objects inside the archetype have to be copied.
			In the case of StaticDataNetwork backend,  a safe type object has to be obtained before creating the object
			

	03/12/04	-	endre  
		
			setASssociation() allows references to be set on instance objects

	24/02/04	-	endre

			Extending InitDiagram() with version.

	10/04/02	-	endre

			New Attribute initializer function to use in meta .cpp format.
					This supports attribute intialization of : ordering, defvalue,
					and visibility attribute attributes.

	10/04/02	-	endre

			Serialize(), DeSerialize() bug discovered by Feng. 

					The MEM backend was incorrectly assuming that composition childroles
					are always non-empty. This holds when the meta is the one generated
					& build up by the .cpp file, because the code generator generated some
					default rolenames even when there were no rolenames.
					But when the metainformation was loaded runtime from a backend, 
					(for example: UdmCopy) the empty child role names caused problems.
				
	07/21/02	-	endre
		
			Serialize() bug discovered TiHammer fixed.

					Metaobjects were obtained based on their Ids (ObjectByID)
					using the DataNetwork of the StaticObject to be Serialized.
					This was fine for StaticObjects with Static meta objects, because
					StaticDataNetwork's ObjectById() works for all StaticObjects, even if
					they are not in the DataNetwork on which ObjectById() is called. 
					(this behaviour is controlled by static bool StaticDataNetwork::SafeObjectById), which
					is false during serialize to make things faster.)
					

	06/19/02	-	endre

			Attribute arrays support:

					Implemented the attribute array setter/getter functions.
					Changed the binary file format to support attribute arrays,
					Serialize(), DeSerialize() also modified to reflect these changes.


	06/02/02	-	endre

			uniqueIDs:

					Changed back, the uniqueID of a StaticObject is again the pointer value.
					Thus, two maps were removed, and the uniqueID() function is very fast- just returns the pointer.
					Instead of that two maps we have just one set, which contains all the valid pointers.
					objectByID() checks against this set and if the pointer is found in the set, it *dereferences
					HowEver, this can be avoided by setting the StaticDataNetwork::SafeObjectById boolean variable to false.
					(Serialize will use this function to speed up the write out procedure)

			DeSerialize(): 

					Removed the recursion, now it can load really big files. Also some optimisation was done,

			Serialize():

					Heavily optimized. AssociationRoles, ChildRoles and attribute names are obtained via a getdn()->ObjectByID call,
					and SaveAs() turns off map consistency checking, so ObjectByID is really fast.

			Destroy(), ~staticDataNetwork:

					A Destroy function for StaticObjects, to be used when the DataNetwork dies. In this case we needed some 
					more efficient delete mechanism then SetParent(NULL, NULL) for each object. SetParent(NULL, NULL) and thus detach()
					deleted an object in a safe, but costly way: removing from child and association maps of other objects.

					When the DataNetwork as a whole goes away, there is no need for such thing. We just go recursively, and decrement
					each objects refcount by the correspinding value. If no other reference exists than the links and containments,
					then the object will just die.



	04/10/02	- endre
		
		  1. getChildren() and getParent() should not cast down the meta
			object to StaticObject*, because the meta could be for instance
			CORBAObject, too. - fixed, now they are using meta objects as ObjectImpls.
		  
		  2. release() deleted the object when refcount() reached 0 which is terribly wrong
		     the object is only deleted when it becomes orphan. In this case the whole subtree 
			 is deleted
		  
		  3. uniqueID(), objectbyID() now uses a number&map which is incremented instead of the 
			 address of the object. 

	04/09/02	-	endre
		
			because of the Corba stuff, moved the class StaticObject declaration
			to UdmStatic.h file

	04/08/02	-	endre
		
		Fixed two bugs:
		
		  1. setAssociation() for simple associations did not work; Throwed
			 exception saying that the rolename is invalid even if it was. fixed
		  2. getParent() returned the one&only parent regardless of the requested 
			 parentRole. fixed.
	
	03/19/02	-	endre

		Also, implemented class StaticObject completly,
		which was only able to act as a meta-meta and meta-model.
		No set()-ers were implemented.
		Now one can use it exactly the same way as Dom or GME objects.

		From the hacker's point of view, this is great: The Meta objects and even
		the UML meta->meta objects can be changed runtime, for example: one could
		det rid of association classes by simply changing the meta-meta,


  
	03/18/02	-	endre
	  
		an attempt to create a Static Data Network,
		UdmStatic:StaticDataNetWork. With these, 
		one will be able to create objects in the memory 
		and keep it in a DataNetwork. 
		
		One great use of this is to create at runtime metamodels, 
		and use as runtime meta information.
		
		The other use is to create a StaticDataNetwork with 
		for ex. CORBA Meta Objects, and pass to an other CORBA 
		application. But you may think of lots of other applications
		as well.

*/

#include "Uml.h"
#include "UdmStatic.h"
#include "UmlExt.h"
#include <UdmBase.h>
#include <UdmUtil.h>


#include <map>
#include <cstdlib>
#include <cstring>

		

namespace UdmStatic
{
	/*
		hack: To be GME-backend conformant,
		this tells whether a link is a GME reference
	*/
	bool isRefLink(const ::Uml::AssociationRole &meta, const int mode, const vector<Udm::ObjectImpl*>& nvect)
	{
		return ( ((string)(meta.name()) == "ref") && (mode == Udm::TARGETFROMPEER) && nvect.size()==1);
	};

	typedef Udm::ObjectImpl ObjectImpl;
	
	
	class StaticDataNetwork;
	typedef list<StaticDataNetwork *> SDNList; 

	void StaticObject::Cleanup() {
		//first remove Static Data Networks
		//this should recursively delete all the user's objects
		//and the new .cpp format -created meta objects as well.
		set<UdmStatic::StaticObject*>& so_set = Udm::_UdmStaticData.so_set;
		while (!so_set.empty())
		{
			set<UdmStatic::StaticObject*>::iterator so_set_i = so_set.begin();
			(*so_set_i)->id_map_so_set_deleted = true;
			if ((*so_set_i)->m_type_is_safetype) {
				Uml::SafeTypeContainer::RemoveSafeType((*so_set_i)->m_type);
				(*so_set_i)->m_type_is_safetype = false;
			}

			so_set.erase(so_set_i);
		};

		SDNList& SDNs = Udm::_UdmStaticData.SDNs;
		while (!SDNs.empty())
			delete *SDNs.begin();
	}



	// --------------------------- StaticObject


	StaticObject::StaticObject(const ::Uml::Class &meta, 
								int ref, StaticObject * archetype, 
								bool subtype,
								const bool real_archetype) :
	deleted(false), 
	archetype(archetype), 
	subtype(subtype),  
	real_archetype(real_archetype), 
	refCount(ref),
	m_type(meta), 
	id_map_so_set_deleted(false),
	m_parent(NULL),
	mydn(NULL),
	lib_object(false),
	lib_root(false),
	lib_name("")
	{ 
		init();
	}

	StaticObject::StaticObject(const ::Uml::Class &meta, StaticDataNetwork *dn, int ref) :
	deleted(false), 
	archetype(NULL), 
	subtype(false),  
	real_archetype(true), 
	refCount(ref),
	m_type(meta), 
	id_map_so_set_deleted(false),
	m_parent(NULL),
	mydn(dn),
	lib_object(false),
	lib_root(false),
	lib_name("")
	{
		init();
	}

	void StaticObject::init()
	{
		pair<set<StaticObject*>::iterator, bool> ins_res  = Udm::_UdmStaticData.so_set.insert(this);
		if (!ins_res.second)
			throw udm_exception(" Corrupt set, pointer already exists in the map ?");

		m_type_is_safetype = Uml::SafeTypeContainer::IsSafeType(m_type);
	}

	StaticObject::~StaticObject()
	{


		//we remove it from the map

		if (!id_map_so_set_deleted)
		{
			set<StaticObject*>::iterator i = Udm::_UdmStaticData.so_set.find(this);
			if (i != Udm::_UdmStaticData.so_set.end())
				Udm::_UdmStaticData.so_set.erase(i);
		}
		//check if my type is obtained by SafeTypeContainer
		//if so, release it.

		//check if m_type is still valid, - it is only a reference
		//might be alread deleted


		if (m_type_is_safetype)	
			Uml::SafeTypeContainer::RemoveSafeType(m_type);

		
	}

		
	ObjectImpl* StaticObject::clone()
	{
		++refCount;
		
		return this; 
	}

	void StaticObject::release() 
	{
		if( --refCount == 0 )
		{
			delete this;
		}
	}


	void StaticObject::Destroy(bool root)			//destroy the object network
													// this deletes all the objects explicitly,
													// to be used only when the data network is going away ..
	{

		//destroy the subtree
		
		children_type::iterator i = m_children.begin();
		while (i != m_children.end())
		{

			((*i).second)->Destroy(false);			
			i++;
		}

		unsigned int j;
		for (j=0; j< associations.size() ; j++) release();//associated objects refer to me

		//for the derived/archetype relations the reference counters are not incremented

		if(!root) 
		{
			release();		// delete me , root will be released by ~StaticDataNetwork()
			release();		// my parents refer to me, root has no parent
		}
	}


	Udm::DataNetwork * StaticObject::__getdn() 
	{ 
#ifdef _DEBUG
		if (!mydn)
		{
			// Check if the object belongs to a DN but its mydn member is not set
			const StaticObject * p = this;
			while (p->m_parent) p = p->m_parent;				
			for(SDNList::iterator ff = Udm::_UdmStaticData.SDNs.begin(); ff != Udm::_UdmStaticData.SDNs.end(); ff++) 
			{
				Object root = (*ff)->GetRootObject();
				if ( static_cast<StaticObject *>(root.__impl()) == p ) {
					const StaticObject *p = this;
					while (p->m_parent) {
						StaticObject *p2 = p->m_parent;
						p = p->m_parent;
					}
				}
				UDM_ASSERT( static_cast<StaticObject *>(root.__impl()) != p );
			}
		}
#endif
		return mydn;

	}

	const ::Uml::Class& StaticObject::type() const { return m_type; };
//	const ::Uml::Class& StaticObject::type() const { return ret_m_type; };

	
	uniqueId_type StaticObject::uniqueId() const 
	{ 
		//return GetIdForObject((StaticObject*)this);
		return reinterpret_cast<uniqueId_type>(this); 
	}

	//atribute setters/getters for single value

	string StaticObject::getStringAttr(const ::Uml::Attribute &meta) const
	{
		stringAttrs_type::const_iterator i = stringAttrs.find(meta.uniqueId());
		if ( i != stringAttrs.end())
			return (*i).second;

		// ----------------------------
		else {
			stringAttrArrs_type::const_iterator i = stringAttrArrs.find(meta.uniqueId());
			if (i != stringAttrArrs.end()) {
				vector<string> temp = (*i).second;
				string ret = "";
				for(vector<string>::iterator i = temp.begin(); i!=temp.end(); i++) ret += (string)(*i) + (string)";";
					return ret;
				}
			}
		// ----------------------------

		if(meta.defvalue())
			return meta.defvalue()[0];

		return string();

		//return i == stringAttrs.end() ? string() : (*i).second;
	}

	void StaticObject::setStringAttr(const ::Uml::Attribute &meta, const string &a, const bool direct)
	{
		
		/*1.
			if it has archetype, and direct is false, the change is coming
			from the archetype. so, we do it unless the attribute is desynched
			from the archetype.
		  2.
			if it has archetype, and direct is true, we do it anyway.
			We also desync the attribute.
	      3. 
			if it has no archetype, we just do the change, regardless of the flag 
			direct and the desynched attributes

		*/
			if (archetype && (archetype != (ObjectImpl*)&Udm::_null) )
			{
			if (direct)
			{
				//set the attribute
				stringAttrs[meta.uniqueId()] = a;
				//desynch the attribute
				desynched.insert(meta.uniqueId());
			}
			else
			{
				if (desynched.find(meta.uniqueId()) == desynched.end())
					//set the attribute
					stringAttrs[meta.uniqueId()] = a;
			}
			
		}
		else
		{
			//set the attribute
			stringAttrs[meta.uniqueId()] = a;
		}

		//go through all deriveds and instantiated objects
		//change this attribute(meta) in all of them,
		//via this call, with direct = false
		set<StaticObject*>::iterator i;
		for (i = st_and_i.begin(); i != st_and_i.end(); i++)
		{
			StaticObject * so = *i;
			so->setStringAttr(meta, a, false);
		}
	}

	bool StaticObject::getBooleanAttr(const ::Uml::Attribute &meta) const
	{
		booleanAttrs_type::const_iterator i = booleanAttrs.find(meta.uniqueId());
		
		if ( i != booleanAttrs.end())
			return (*i).second;

		return false;
		//return i == booleanAttrs.end() ? false : (*i).second;
	}

	void StaticObject::setBooleanAttr(const ::Uml::Attribute &meta, bool a, const bool direct)
	{
		if (archetype && (archetype != (ObjectImpl*)&Udm::_null) )
		{
			if (direct)
			{
				//set the attribute
				booleanAttrs[meta.uniqueId()] = a;
				//desynch the attribute
				desynched.insert(meta.uniqueId());
			}
			else
			{
				if (desynched.find(meta.uniqueId()) == desynched.end())
					//set the attribute
					booleanAttrs[meta.uniqueId()] = a;
			}
			
		}
		else
		{
			//set the attribute
			booleanAttrs[meta.uniqueId()] = a;
		}

		//go through all deriveds and instantiated objects
		//change this attribute(meta) in all of them,
		//via this call, with direct = false
		set<StaticObject*>::iterator i;
		for (i = st_and_i.begin(); i != st_and_i.end(); i++)
		{
			StaticObject * so = *i;
			so->setBooleanAttr(meta, a, false);
		}
		//booleanAttrs[meta.uniqueId()] = a;
	}

	__int64 StaticObject::getIntegerAttr(const ::Uml::Attribute &meta) const
	{
		longintAttrs_type::const_iterator i = longintAttrs.find(meta.uniqueId());
		
		if ( i != longintAttrs.end()) return (*i).second;

		if (meta.defvalue())
			return _atoi64(((string)meta.defvalue()[0]).c_str());
	 
		return 0;
		
		//return i == integerAttrs.end() ? false : (*i).second;
	}

	void StaticObject::setIntegerAttr(const ::Uml::Attribute &meta, __int64 a, const bool direct)
	{
		if (archetype && (archetype != (ObjectImpl*)&Udm::_null) )
		{
			if (direct)
			{
				//set the attribute
				longintAttrs[meta.uniqueId()] = a;
				//desynch the attribute
				desynched.insert(meta.uniqueId());
			}
			else
			{
				if (desynched.find(meta.uniqueId()) == desynched.end())
					//set the attribute
					longintAttrs[meta.uniqueId()] = a;
			}
			
		}
		else
		{
			//set the attribute
			longintAttrs[meta.uniqueId()] = a;
		}

		//go through all deriveds and instantiated objects
		//change this attribute(meta) in all of them,
		//via this call, with direct = false
		set<StaticObject*>::iterator i;
		for (i = st_and_i.begin(); i != st_and_i.end(); i++)
		{
			StaticObject * so = *i;
			so->setIntegerAttr(meta, a, false);
		}
		//integerAttrs[meta.uniqueId()] = a;
	}

	double StaticObject::getRealAttr(const ::Uml::Attribute &meta) const
	{
		realAttrs_type::const_iterator i = realAttrs.find(meta.uniqueId());
		if ( i != realAttrs.end()) return (*i).second;

		if (meta.defvalue())
			return atof(((string)meta.defvalue()[0]).c_str());

		return 0.0;
		//return i == realAttrs.end() ? false : (*i).second;
	}

	void StaticObject::setRealAttr(const ::Uml::Attribute &meta, double a, const bool direct)
	{
		if (archetype && (archetype != (ObjectImpl*)&Udm::_null) )
		{
			if (direct)
			{
				//set the attribute
				realAttrs[meta.uniqueId()] = a;
				//desynch the attribute
				desynched.insert(meta.uniqueId());
			}
			else
			{
				if (desynched.find(meta.uniqueId()) == desynched.end())
					//set the attribute
					realAttrs[meta.uniqueId()] = a;
			}
			
		}
		else
		{
			//set the attribute
			realAttrs[meta.uniqueId()] = a;
		}

		//go through all deriveds and instantiated objects
		//change this attribute(meta) in all of them,
		//via this call, with direct = false
		set<StaticObject*>::iterator i;
		for (i = st_and_i.begin(); i != st_and_i.end(); i++)
		{
			StaticObject * so = *i;
			so->setRealAttr(meta, a, false);
		}
		
		//realAttrs[meta.uniqueId()] = a;
	}

	//	--- attribute setters/getters for multiple value (array) type attributes

	vector<string> StaticObject::getStringAttrArr(const ::Uml::Attribute &meta) const
	{

		stringAttrArrs_type::const_iterator i = stringAttrArrs.find(meta.uniqueId());

		if (i != stringAttrArrs.end())
			return (*i).second;

		//avoid stack overflow here ;-)
		if (meta == ::Uml::Attribute::meta_defvalue)
			return vector<string>();	//it has no default value

		return meta.defvalue();
	};

	void StaticObject::setStringAttrArr(const ::Uml::Attribute &meta, const vector<string> &a, const bool direct)
	{
		if (archetype && (archetype != (ObjectImpl*)&Udm::_null) )
		{
			if (direct)
			{
				//set the attribute
				stringAttrArrs[meta.uniqueId()] = a;
				//desynch the attribute
				desynched.insert(meta.uniqueId());
			}
			else
			{
				if (desynched.find(meta.uniqueId()) == desynched.end())
					//set the attribute
					stringAttrArrs[meta.uniqueId()] = a;
			}
			
		}
		else
		{
			//set the attribute
			stringAttrArrs[meta.uniqueId()] = a;
		}

		//go through all deriveds and instantiated objects
		//change this attribute(meta) in all of them,
		//via this call, with direct = false
		set<StaticObject*>::iterator i;
		for (i = st_and_i.begin(); i != st_and_i.end(); i++)
		{
			StaticObject * so = *i;
			so->setStringAttrArr(meta, a, false);
		}
		
		//stringAttrArrs[meta.uniqueId()] = a;
	};

	vector<bool> StaticObject::getBooleanAttrArr(const ::Uml::Attribute &meta) const
	{
		booleanAttrArrs_type::const_iterator i = booleanAttrArrs.find(meta.uniqueId());
		if (i != booleanAttrArrs.end())
			return (*i).second;

		//if not found, return defvalue
		if (meta.defvalue())
		{
			if (meta.ordered())
				return Udm::SortArrAttr<bool>(Udm::StrToBoolArrAttr(meta.defvalue()));
			else 
				return Udm::StrToBoolArrAttr(meta.defvalue());

		}


		vector<bool> a;
		return a;
	};

	void StaticObject::setBooleanAttrArr(const ::Uml::Attribute &meta, const vector<bool> &a, const bool direct)
	{
		if (archetype && (archetype != (ObjectImpl*)&Udm::_null) )
		{
			if (direct)
			{
				//set the attribute
				booleanAttrArrs[meta.uniqueId()] = a;
				//desynch the attribute
				desynched.insert(meta.uniqueId());
			}
			else
			{
				if (desynched.find(meta.uniqueId()) == desynched.end())
					//set the attribute
					booleanAttrArrs[meta.uniqueId()] = a;
			}
		
		}
		else
		{
			//set the attribute
			booleanAttrArrs[meta.uniqueId()] = a;
		}

		//go through all deriveds and instantiated objects
		//change this attribute(meta) in all of them,
		//via this call, with direct = false
		set<StaticObject*>::iterator i;
		for (i = st_and_i.begin(); i != st_and_i.end(); i++)
		{
			StaticObject * so = *i;
			so->setBooleanAttrArr(meta, a, false);
		}
		
		//booleanAttrArrs[meta.uniqueId()] = a;
	};

	vector<__int64> StaticObject::getIntegerAttrArr(const ::Uml::Attribute &meta) const
	{
		longintAttrArrs_type::const_iterator i = longintAttrArrs.find(meta.uniqueId());
		if (i != longintAttrArrs.end())
		{
		
			return (*i).second;
			/*
			vector<long> c;
			vector<__int64> b = (*i).second;
			vector<__int64>::iterator b_i = b.begin();
			while (b_i != b.end())
				c.push_back((long)(*b_i++));
				*/
		}

		//if not found, return defvalue
		if (meta.defvalue())
		{
			if (meta.ordered())
				return Udm::SortArrAttr<__int64>(Udm::StrToLongArrAttr(meta.defvalue()));
			else 
				return Udm::StrToLongArrAttr(meta.defvalue());

		}

		vector<__int64> a;
		return a;
	};

	void StaticObject::setIntegerAttrArr(const ::Uml::Attribute &meta, const vector<__int64> &a, const bool direct)
	{
		if (archetype && (archetype != (ObjectImpl*)&Udm::_null) )
		{
			if (direct)
			{
				//set the attribute
				/*vector<__int64> b;
				vector<long>::const_iterator a_i = a.begin();
				while (a_i != a.end())
					b.push_back((__int64)(*a_i++));
				*/
				longintAttrArrs[meta.uniqueId()] = a;
				//desynch the attribute
				desynched.insert(meta.uniqueId());
			}
			else
			{
				if (desynched.find(meta.uniqueId()) == desynched.end())
				{
					/*vector<__int64> b;
					vector<long>::const_iterator a_i = a.begin();
					while (a_i != a.end())
						b.push_back((__int64)(*a_i++));
					*/
					//set the attribute
					longintAttrArrs[meta.uniqueId()] = a;
				}
			}
		
		}
		else
		{
		/*	vector<__int64> b;
			vector<long>::const_iterator a_i = a.begin();
			while (a_i != a.end())
				b.push_back((__int64)(*a_i++));
		*/
			//set the attribute
			longintAttrArrs[meta.uniqueId()] = a;
		}

		//go through all deriveds and instantiated objects
		//change this attribute(meta) in all of them,
		//via this call, with direct = false
		set<StaticObject*>::iterator i;
		for (i = st_and_i.begin(); i != st_and_i.end(); i++)
		{
			StaticObject * so = *i;
			so->setIntegerAttrArr(meta, a, false);
		}
		
		//integerAttrArrs[meta.uniqueId()] = a;
	};

	vector<double> StaticObject::getRealAttrArr(const ::Uml::Attribute &meta) const
	{
		realAttrArrs_type::const_iterator i = realAttrArrs.find(meta.uniqueId());
		if (i != realAttrArrs.end())
			return (*i).second;

		//if not found, return defvalue
		if (meta.defvalue())
		{
			if (meta.ordered())
				return Udm::SortArrAttr<double>(Udm::StrToRealArrAttr(meta.defvalue()));
			else 
				return Udm::StrToRealArrAttr(meta.defvalue());

		}
		vector<double> a;
		return a;
	};

	void StaticObject::setRealAttrArr(const ::Uml::Attribute &meta, const vector<double> &a, const bool direct)
	{
		if (archetype && (archetype != (ObjectImpl*)&Udm::_null) )
		{
			if (direct)
			{
				//set the attribute
				realAttrArrs[meta.uniqueId()] = a;
				//desynch the attribute
				desynched.insert(meta.uniqueId());
			}
			else
			{
				if (desynched.find(meta.uniqueId()) == desynched.end())
					//set the attribute
					realAttrArrs[meta.uniqueId()] = a;
			}
		
		}
		else
		{
			//set the attribute
			realAttrArrs[meta.uniqueId()] = a;
		}

		//go through all deriveds and instantiated objects
		//change this attribute(meta) in all of them,
		//via this call, with direct = false
		set<StaticObject*>::iterator i;
		for (i = st_and_i.begin(); i != st_and_i.end(); i++)
		{
			StaticObject * so = *i;
			so->setRealAttrArr(meta, a, false);
		}
		
		//realAttrArrs[meta.uniqueId()] = a;
	};

	long StaticObject::getAttrStatus(const ::Uml::Attribute &meta) const
	{
		long status = Udm::ATTSTATUS_HERE;

		if (archetype && (archetype != (ObjectImpl*)&Udm::_null) )
		{
			// for now just set a value telling that the attribute value
			// is not changed here
			if (desynched.find(meta.uniqueId()) == desynched.end())
				status = Udm::ATTSTATUS_IN_ARCHETYPE1;
		}

		return status;
	};


	ObjectImpl * StaticObject::getParent(const ::Uml::CompositionParentRole &role) const 
	{

		if (!m_parent) 
			return &Udm::_null;

		//at this point we surely have a parent
		if(role) 
		{
			//rolename was provided
			//check if our parent is a parent via the provided rolename
			//obtaining the corresponding CompositionChildRole
			//theOther can not be used
			
		
			
			ObjectImpl * comp_o = role.__impl()->getParent(Udm::NULLPARENTROLE);
			if (!comp_o) throw udm_exception("parent of type meta object is NULL");
			vector<ObjectImpl*> vec = comp_o->getChildren(::Uml::Composition::meta_childRole, ::Uml::CompositionChildRole::meta); 			
			comp_o->release();
			if (vec.size() != 1) throw udm_exception("Meta Composition does not contain 1 and only 1 childRole!");
			
			ObjectImpl * ccr_o = *(vec.begin());
			::Uml::CompositionChildRole ccr = ccr_o;

			//get my parent's children with this rolename
			//don't use the API because it increments the ref. counters,
			//access directly the map!
			
			pair<children_type::iterator, children_type::iterator> t = 
			m_parent->m_children.equal_range(ccr.uniqueId());

			while( t.first != t.second )
			{
				if ((*t.first).second == this)
					return m_parent->clone();
				++t.first;
			}

			//if parent has no such child, we return null
			return &Udm::_null;

		}//eo if(role)
		//no rolename was provided,
		//just return our parent, regardless of it's parentRole	
		else return m_parent->clone(); 
	}

	void StaticObject::setParent(ObjectImpl *a, const ::Uml::CompositionParentRole &role, const bool direct) 
	{ 
		::Uml::CompositionChildRole ccr;

		//check no 1
		//children of instances can not be removed

		if (direct &&  m_parent && m_parent->archetype && !m_parent->subtype)
			throw udm_exception("Instantiated objects can not be modified!");
			
		//check no 2
		//inherited children of subtypes can not be removed 
		if (direct &&  m_parent && m_parent->archetype && m_parent->subtype && !real_archetype)
			throw udm_exception("Inherited objects can not be modified!");
		

		if(a != NULL && a != &Udm::_null) 
		{
			StaticObject &aa = *static_cast<StaticObject*>(a);
			//aa is the parent object

			//check no 3
			if (direct && aa.archetype && !aa.subtype)
				throw udm_exception("Instantiated objects can not be modified!");
			


					
			::Uml::Composition comp = Uml::matchChildToParent(m_type, aa.m_type);

			if(!role) 
			{
				//no ParentRole was provided, which means,
				//that only one composition is possible
				//otherwise it's ok to crash -- the user was dump
				if(!comp) throw udm_exception("Childrole has to be specified");
				ccr = comp.childRole();
			}

			else 
			{
				//rolename was provided

				if(comp != role.parent()) 
				{
					//either 0 or multiple possible compositions
					comp = role.parent();
					
					// if different, or comp is 0 because of 0 roles, one of the followings will catch it:
					if(!Uml::IsDerivedFrom(m_type,::Uml::CompositionChildRole(comp.childRole()).target())) {
						throw udm_exception("Invalid parentrole specified");
					}

					if(!Uml::IsDerivedFrom(aa.m_type, ::Uml::CompositionParentRole(comp.parentRole()).target())) {
						throw udm_exception("Invalid parent specified");
					}
					//comp should have the right composition.
					
					
				};

				ccr = Uml::theOther(role);
			}

		}
		

		//if we already have a parent, remove it first from my parent's children
		
		set<StaticObject*> moved_to_parents_ok; //this will contain the parent for which 

		if (m_parent != NULL )
		{
			//I have a parent.
			//Since parents increments children's refcount,
			//we should decrement it here 
			//what if it becomes zero ?

		

			children_type::iterator i = m_parent->m_children.begin();
			int cnt = 0;
			
			while (i != m_parent->m_children.end())
			{
				if (i->second == this)
				{	
					children_type::iterator j = i++;
					m_parent->m_children.erase(j);
					this->refCount--;
					cnt++;
					
				} else i++;
			}
			if (cnt > 1) cout <<"child had two parents?!" <<endl;

			
			if (direct)
			{
				set<StaticObject*>::iterator st_and_i_i = st_and_i.begin();
				while (st_and_i_i != st_and_i.end())
				{
					StaticObject * st_and_i_o = *st_and_i_i++;
									
					if (st_and_i_o->m_parent && (st_and_i_o->m_parent->archetype == m_parent) && !st_and_i_o->real_archetype)
					{
						children_type::iterator ii = st_and_i_o->m_parent->m_children.begin();
						int cntt = 0;
						
						while (ii != st_and_i_o->m_parent->m_children.end())
						{
							if (ii->second == st_and_i_o)
							{	
								children_type::iterator j = ii++;
								st_and_i_o->m_parent->m_children.erase(j);
								st_and_i_o->refCount--;//we don't release it, it might be reused. if not, it will be deleted
								st_and_i_o->mydn = NULL;
								cntt++;
								
							} else ii++;
						}
						if (cntt > 1) cout <<"child had two parents?!" <<endl;
					}

					//we have to decide whether we set a new parent or not
					//we go through the new parent st_and_i
					//we will check if we can 
			
					if (a && a != &Udm::_null && !st_and_i_o->real_archetype)
					{
						//get the closest parent which is primary inherited
						StaticObject * st_and_i_o_pi = st_and_i_o;
						while (st_and_i_o_pi && !st_and_i_o_pi->real_archetype)
							st_and_i_o_pi = st_and_i_o_pi->m_parent;
						
						if (!st_and_i_o_pi) throw udm_exception("Expected a primary derived parent in the hiearchy");
						

						StaticObject & new_parent = *static_cast<StaticObject*>(a);
						
						
						//unfortunatly we need some recursion here
						//we have to check if there is a corresponding derived/instantiated object rooted at np_st_and_i_o_pi
						//TOMI function, so we have to create Objects
						
						Udm::Object where = st_and_i_o_pi->clone();
						Udm::Object what = new_parent.clone();

						Udm::Object coresp_parent;

						if (st_and_i_o->subtype)
							coresp_parent = what.FindCorrespondingObjectInSubtypesTree(where, false);
						else
							coresp_parent = what.FindCorrespondingObjectInInstancesTree(where, false);
						
						if (coresp_parent)
						{
							StaticObject * cp_impl = (StaticObject*)coresp_parent.__impl();
							pair<uniqueId_type const, StaticObject *> child(ccr.uniqueId(), (StaticObject*)(st_and_i_o->clone()));
							/*children_type::iterator ins_res = */cp_impl->m_children.insert(child);
							st_and_i_o->m_parent = cp_impl;
							st_and_i_o->mydn = static_cast<StaticDataNetwork*>(cp_impl->__getdn());
							moved_to_parents_ok.insert(cp_impl);
						}
						else
						{
							st_and_i_o->m_parent = NULL; //refcount is already decremented because of this
							st_and_i_o->mydn = NULL;
							st_and_i_o->setParent(NULL, NULL, false);
						}
						
					}
				
				}
			}
			m_parent = NULL;
			mydn = NULL;
		};

		//end of removal from children of my parent

		if(a != NULL && a != &Udm::_null) 
		{
			//parent is not NULL

			
			StaticObject &aa = *static_cast<StaticObject*>(a);
			//aa is the parent object
					
			pair<uniqueId_type const, StaticObject *> child(ccr.uniqueId(), (StaticObject*)(this->clone()));
			/*children_type::iterator ins_res = */aa.m_children.insert(child);
			m_parent =(UdmStatic::StaticObject*)a;

			// set our dn
			mydn = static_cast<StaticDataNetwork*>(a->__getdn());

			//in the situation which the new parent has derived/instantiated objects
			//and the new child is not coming from the archetype block
			if (direct)
			{
				set<StaticObject*>::iterator p_st_and_ii = aa.st_and_i.begin();
				while (p_st_and_ii != aa.st_and_i.end())
				{
		
					StaticObject * p_st_and_io = *p_st_and_ii++;
					
					if (moved_to_parents_ok.find(p_st_and_io) == moved_to_parents_ok.end())
					{
						Object newObject = p_st_and_io->createChild(ccr, Uml::SafeTypeContainer::GetSafeType(type()), this, p_st_and_io->subtype, false);
					}
				}
			}
		}
		else
		{
			//this becomes orphan
			//we should delete the object and remove it from the map
			//what about associations ?
			//we should remove all the links from peers

		
	
			
			
			//remove the links from my peers
			assoc_type::iterator assoc_i = associations.begin();
			
			for (; assoc_i != associations.end(); assoc_i++)
			{
				
				StaticObject * peer =  (StaticObject*)((*assoc_i).second);
				if (!peer)
					throw udm_exception("Peer does not exists!");
				
				//find out what to erase, put those iterators in a vector
				vector<assoc_type::iterator> to_erase_from_peer;
				assoc_type::iterator assoc_j = peer->associations.begin();
				
				for(; assoc_j != peer->associations.end(); assoc_j++)
				{
					StaticObject * am_i = (StaticObject*)((*assoc_j).second);	
					if (am_i == this) 
						to_erase_from_peer.push_back(assoc_j);
				}

				//actually erase from peer
				for (vector<assoc_type::iterator>::iterator erase_i = to_erase_from_peer.begin(); erase_i != to_erase_from_peer.end(); erase_i++)
				{
					peer->associations.erase(*erase_i);
					if (refCount <= 1)
						cout << " Warning: Refcount reached 1 when removing associations!" << endl;;
					this->release();
				}
				//release my peer to, since I'm going to die;)
				if (peer->refCount <= 1 )
					cout << "Warning: Peer's refcount reached 1 when removing associations!" << endl;
				peer->release();
			}
			//delete my assoc. map.
			associations.erase(associations.begin(), associations.end());

			//detach all my children 

			

			while (m_children.size())
			{
				children_type::iterator children_i = m_children.begin();
				((StaticObject*)((*children_i).second) )->setParent(NULL, NULL, false);
			}

			//for (; children_i != m_children.end(); children_i++)
			//	((StaticObject*)((*children_i).second) )->setParent(NULL, NULL, false);

			

			//at this point I should have no children's
			//because all my childrens deleted theirself from my map
			if (m_children.size()) cout << "After recursive wipe I still have children!" << endl;

			//I might have derived objects that refer to me

			//I might have an archetype 
			if (archetype)
				archetype->st_and_i.erase(this);
			
			if (st_and_i.size())
			{
				//all my derived objects also go away
				set<StaticObject*>::iterator ad_i = st_and_i.begin();
				while (ad_i != st_and_i.end())
					(*ad_i++)->setParent(NULL, NULL, false);

				//they should remove themselves from my derived set, so
				// we expect that is empty by now
				if (st_and_i.size())
					throw udm_exception("Internal Error: derived should be empty after removing all instances.");
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
		
			if (!deleted)
			{
				//release...
				deleted = true;
				this->release();
			}
		}
		
	};	
		
	void StaticObject::detach() 
	{
		setParent(NULL,NULL);
	}

   	vector<ObjectImpl*> StaticObject::getChildren(const ::Uml::CompositionChildRole &meta, const ::Uml::Class &cls) const
	{
		vector<ObjectImpl*> ret;

		/*
			A new approach to this function, I believe we don't need to deduce the rolename
			when it's not proided.
			
			Basically, we have 4 cases:

		+-------+-----------+-------------------+--------------------+
		|case	|meta		|	cls				| action			 |
		|no.	|provided	|	provided		| taken				 |
		+-------+-----------+-------------------+--------------------+
		|0		|	no		|	no				| return all children|
		|------------------------------------------------------------+
		|1		|	no		|	yes				| return all children| 
		|		|			|					| of type `cls`		 |
		+-------+-----------+-------------------+--------------------+
		|2		|	yes     |   no				| return all children|
		|		|			|					| via role `meta`	 |
		+-------+-----------+-------------------+--------------------+
		|3		|	yes		|	yes				| return all children|
		|		|			|					| of type `cls`		 |
		|		|			|					| via role `meta`	 |
		+-------+-----------+-------------------+--------------------+	
		
		
			we first make a distinction between cases {0,1} and {2,3}.
			if {0, 1} then we start returning our children, and if it's 1 we check the type before.
			if {2, 3} then we get the children with corresponding roles, then we start returning them.
					 if 3, we check their type before.

			hope that's it, sounds a lot easier than rolename deducing
			(Of course, one may debate what is faster: deducing my rolename from meta info,
			or simply iterate on my children in case 1. - that's the least efficient)


		*/	
		

		if (meta)
		{
			//cases 2 & 3
			int c = m_children.count(meta.uniqueId());
			if (c)
			{
				ret.reserve(c);

				pair<children_type::const_iterator, children_type::const_iterator> t = 
				m_children.equal_range(meta.uniqueId());

				if (cls)
				{
					//case 3
					while( t.first != t.second )
					{
						if (Uml::IsDerivedFrom((*t.first).second->m_type, cls))
							//type is ok
							ret.push_back((*t.first).second->clone());
						++t.first;
					}
				}
				else
				{
					//case 2
					while( t.first != t.second )
					{
						ret.push_back((*t.first).second->clone());
						++t.first;
					}
				}
			}
		}
		else
		{
			//cases 0 & 1
			ret.reserve(m_children.size());
			children_type::const_iterator t = m_children.begin();

			if (cls)
			{
				while (t!= m_children.end())
				{
					//case 1
					if (Uml::IsDerivedFrom((*t).second->m_type, cls))
						//type is ok
						ret.push_back((*t).second->clone());

					++t;
				}
			}
			else {
				while (t!= m_children.end())
				{
					ret.push_back((*t).second->clone());
					//++((*t).second->refCount);

					++t;
				}
			}
		}

	
		return ret;
	}

	void StaticObject::setChildren(const ::Uml::CompositionChildRole &meta, const vector<ObjectImpl*> &a, const bool direct) 
	{ 
		
		
		set< ::Uml::CompositionChildRole> inserted_via;			//this will contain the roles altered this time
		children_type already_there;							//this will contain the childrens which where there (only for the previous rolenames)
		children_type inserted_now;								//this will contain the inserted items (including thos already there)
		

		if (direct && archetype && !subtype)
		{
			throw udm_exception("Instance objects can not be modified!");
		}

		for (vector<ObjectImpl*>::const_iterator i = a.begin(); i != a.end(); i++)
		{
			//find out the rolename		
			::Uml::Class type = (*i)->type();
			string casestr = (string)type.name() + "-s in " + (string)m_type.name() + "-s";
		

			::Uml::CompositionChildRole role = meta;
			if(!role) 
			{
				::Uml::Composition comp = Uml::matchChildToParent(type, m_type);
				if(!comp) throw  udm_exception("Role must be specified for " + casestr);
				role = comp.childRole();
			}
			
			if(!Uml::IsDerivedFrom(m_type, Uml::theOther(role).target())) 
			{
				throw udm_exception("Invalid childrole specified for creating " + casestr);
			}
			if(!Uml::IsDerivedFrom(type, role.target())) 
			{
				throw udm_exception("Invalid child specified: " + casestr);
			}

			//role is ok!
			
			
			pair<set< ::Uml::CompositionChildRole>::iterator, bool> ins_res = inserted_via.insert(role);
			
			if (ins_res.second)
			{
				//if there was no previous children with this role 
				//keep a track of children which where already there , via this role
				//save my children with this rolename
				pair<children_type::iterator, children_type::iterator> t = 	m_children.equal_range(role.uniqueId());
				already_there.insert(t.first, t.second);
			}


			pair<uniqueId_type const, StaticObject *> to_insert(role.uniqueId(), (StaticObject*)*i);
			inserted_now.insert(to_insert);


			
			StaticObject * so = (StaticObject*)*i;			//this cast is safe, however
			so->setParent(this, Uml::theOther(role), direct);		//setParent takes care about ref. counting

	


		
		};//eo for on childrens

		//go thru inserted via, for each role
		//get the already there childrens
		//get the inserted now childrens
		//if not found detach it..

		for(set< ::Uml::CompositionChildRole> ::iterator j = inserted_via.begin(); j!= inserted_via.end(); j++)
		{
			pair<children_type::iterator, children_type::iterator> m = 	already_there.equal_range((*j).uniqueId());
			while(m.first != m.second)
			{
				bool found = false;
				pair<children_type::iterator, children_type::iterator> n = 	inserted_now.equal_range((*j).uniqueId());
				while ((n.first != n.second) && !found)
				{
					if ((m.first)->second == (n.first)->second)
					{
						found = true; break;
					}
					n.first++;
					
				}
				if (!found)
				{
					//inherited child objects can not be deleted

					if ( direct && ((m.first)->second)->archetype && !((m.first)->second)->real_archetype)
						throw udm_exception("Inherited child objects can not be deleted!");

					((m.first)->second)->setParent(NULL, NULL, false);
				}
				
				m.first++;
			}

		}

		set<StaticObject*>::iterator st_and_i_i = st_and_i.begin();
		while (st_and_i_i != st_and_i.end())
		{
			StaticObject * st_and_i_o = *st_and_i_i++;

			StaticObject * st_and_i_o_pd = st_and_i_o;

			//get the primarily derived block
			while (st_and_i_o_pd->archetype && !st_and_i_o_pd->real_archetype)
				st_and_i_o_pd = st_and_i_o_pd->m_parent;

			vector<ObjectImpl *> inside;
			vector<ObjectImpl*>::const_iterator a_i = a.begin();
			while (a_i != a.end())
			{
				StaticObject * a_o = (StaticObject*) *a_i++;
				//find a derived or instantiated inherited child object of a_o in st_and_i_o_pd
				StaticObject * a_o_st_and_i = a_o->FindCorrespondingObjectInStOrITree(st_and_i_o_pd);
				if (a_o_st_and_i) inside.push_back(a_o_st_and_i->clone());
				else
				{
					//we moved in a child to the block which was derived/instantiated
					inside.push_back(st_and_i_o->createChild(meta, Uml::SafeTypeContainer::GetSafeType(a_o->m_type), a_o, st_and_i_o->subtype, false));
				}
				
				if (st_and_i_o->subtype)
				{
					pair<children_type::iterator,children_type::iterator>  sai_ch_i_pair;
					//add code here which adds st_and_i_o -s children objects to inside
					if (meta)
						sai_ch_i_pair = st_and_i_o->m_children.equal_range(meta.uniqueId());
					else
					{
						sai_ch_i_pair.first = st_and_i_o->m_children.begin();
						sai_ch_i_pair.second = st_and_i_o->m_children.end();
					}

					for(; sai_ch_i_pair.first != sai_ch_i_pair.second; sai_ch_i_pair.first++)
					{
						inside.push_back(sai_ch_i_pair.first->second->clone());
					}


				}
				
				st_and_i_o->setChildren(meta, inside, false);
				/*
				//this release()-ing should not cause a crash
				//however, it _does_, so investigation is required in the near future

				vector<ObjectImpl*>::iterator inside_i = inside.begin();
				while (inside_i != inside.end())
				{
					(*inside_i)->release();
					inside_i++;
				}*/

			}

		}

		
	
		

	}

	ObjectImpl* StaticObject::createChild(
		const ::Uml::CompositionChildRole &childrole,
		const ::Uml::Class &meta, 
		const Udm::ObjectImpl* archetype, 
		const bool subtype,
		const bool real_archetype,
		const bool need_safetype) 
	{
		
		//throw exception if this is an instance,(archetype && !subtype)
		//and this call is not recursive (real_archetype is true)
		//instances can not be modifief
	

		if (this->archetype && !this->subtype && real_archetype)
			throw udm_exception("Instance objects can not be modified!");

		string casestr = (string)meta.name() + "-s in " + (string)m_type.name() + "-s";
		::Uml::CompositionChildRole role = childrole;
		if(!role) 
		{
 			::Uml::Composition comp = Uml::matchChildToParent(meta, m_type);
			if(!comp)
				throw udm_exception("Role must be specified for " + casestr);
			role = comp.childRole();
		}

		::Uml::CompositionParentRole parentrole = Uml::theOther(role);

		if(!Uml::IsDerivedFrom(m_type, parentrole.target())) 
		{
			throw udm_exception("Invalid childrole specified for creating " + casestr);
		}
		if(!Uml::IsDerivedFrom(meta, role.target())) 
		{
			throw udm_exception("Invalid child specified: " + casestr);
		}

		//caller must pass meta as a safe type
		StaticObject *dep = new StaticObject(meta, 1, (StaticObject*)archetype, subtype, real_archetype);
		//set the parent
		dep->setParent(this, parentrole, false);

		
		
		if (archetype && archetype != &Udm::_null)
		{
		
			//we don't increment the refcounts.
			//simply because: 
			// if the archetype gets deleted, it will detach automatically all derived objects
			// if on of the deriveds gets deleted, it will remove itself automatically 
			// from it's archetype's set of derived objects
			StaticObject * static_at = (StaticObject *) archetype;
			static_at->st_and_i.insert(dep);

			//copy attribute values from archetype
			dep->CopyAttributesFromArchetype();

			//check if the archetype has children
			//if so, all children should be recreated here, 
			//as derived/instances (depending on subtype) of the 
			//corresponding children of the archetype object

			//we need a copy mechanism here.
			//createChild needs the ChildRole name.
			//we can not obtain it, we have to iterate throug childroles
			//and that way we will know the role via each child id contained.
			//too bad.;(
			//hopefully, this will ease the copy process:
			//an object with an archetype will copy itself from the archetype.
			//with one createChild() call.

			static UdmUtil::copy_assoc_map cam;
			if (real_archetype)
				cam.erase(cam.begin(), cam.end());
		
			
			set< ::Uml::Class> ancestorClasses=Uml::AncestorClasses(archetype->type());

			for(set< ::Uml::Class>::iterator p_currClass=ancestorClasses.begin(); 
			p_currClass!=ancestorClasses.end(); p_currClass++)
			{
				set< ::Uml::CompositionParentRole> compParentRoles=p_currClass->parentRoles();
				for(set< ::Uml::CompositionParentRole>::iterator p_currParentRole = compParentRoles.begin();
					p_currParentRole != compParentRoles.end(); p_currParentRole++)
					{
						::Uml::CompositionChildRole p_currChildRole = Uml::theOther(*p_currParentRole);
						::Uml::Class childClass = p_currChildRole.target();
						vector<ObjectImpl*>children = archetype->getChildren(p_currChildRole, childClass);
						for(vector<ObjectImpl*>::iterator p_currImpl=children.begin();
							p_currImpl!=children.end();p_currImpl++)
							{
								ObjectImpl* p_srcChild=*p_currImpl;

								Udm::Object srcChild = p_srcChild;	//it will be released OK
								
								Object newchild;
								if (need_safetype)
								{
									newchild = dep->createChild(p_currChildRole, Uml::SafeTypeContainer::GetSafeType(p_srcChild->type()), p_srcChild, subtype, false, true);
								}
								else
								{
									newchild = dep->createChild(p_currChildRole, p_srcChild->type(), p_srcChild, subtype, false, false);

								}

								UdmUtil::copy_assoc_map::value_type map_item(srcChild, newchild);
								cam.insert(map_item);

								//check if object is a non-empty reference
								//in that case we need to store in cam the referenced object as well.
								//in the new derived/instantiated object
								//the references should be set to the same object
								//so we put the refered object in cam and we map it to itself
								//this is needed  because reqcopyLinks should not fail
								//when connecting references

								Object srcChildrefersto = srcChild.getReferencedObject();
								if (srcChildrefersto)
								{
									UdmUtil::copy_assoc_map::value_type map_item1(srcChildrefersto, srcChildrefersto);
									cam.insert(map_item1);
								}
							}
					}
			}



			//copy links from archetype
			vector<ObjectImpl*>children= archetype->getChildren(NULL, NULL);
			for(vector<ObjectImpl*>::iterator p_currImpl=children.begin();p_currImpl!=children.end();p_currImpl++)
			{
					ObjectImpl* p_srcChild=*p_currImpl;
					Udm::Object srcChild = p_srcChild;	//it will be released OK
					
					UdmUtil::copy_assoc_map::iterator cam_i = cam.find(srcChild);
					if (cam_i == cam.end()) 
						throw udm_exception("Internal UdmCopy error");
						
					Udm::Object dstChild = cam_i->second;		//this is a safe Object to Object copy
					
					//copy all links recursively for this child
					UdmUtil::reqCopyLinks(p_srcChild, dstChild.__impl(),cam, false);		//copy simple links first
					UdmUtil::reqCopyLinks(p_srcChild, dstChild.__impl(),cam, false, false);	//then assoc. class based ones

			}
						
			
		}
		else
		{
			//if it has no archetype, default attributes are set
			//accordung to metainformation
			//this call works only after setParent()!!!
			dep->setDefaultAttributes(false);
		}

			//if this has subtypes/instances, 
			//a new child has to be added to those objects as well

			/*
			set<StaticObject*> st_and_i;			// in this will be stored all derived and instantiated objects
			StaticObject* archetype;				// in this will be stored the archetype
			bool subtype;							// this tells whether is subtype or instance
													// if subtype, this variable is true
													// if instance, it's false

	
			bool real_archetype;					// true when *this is a real archetype(set by the user)
													// false when *this	is not a real archetype. Is a child
													// of an object which has a real  archetype, and thus all 
													// it's children are archetyped recursively.
			*/
			
			
		set<StaticObject*>::iterator st_and_i_i = st_and_i.begin();
		while ( st_and_i_i != st_and_i.end())
		{
			Object newobject = (*st_and_i_i)->createChild(childrole, meta, dep, (*st_and_i_i)->subtype, false);
			st_and_i_i ++;
		}
	
		
		return dep->clone();		// it gets created with refCounter = 1
									// the setParent adds one more, because it stores it in his local children map
									// there will be a local object with this value, so this adds one more ,
									// the final refCount is 3 after leaving this function !
	}

	void StaticObject::getChildRole(ObjectImpl *c, ::Uml::CompositionChildRole &ret) const
	{
		ret = NULL;
		if (c == NULL || c == &Udm::_null) return;

		for (children_type::const_iterator i = m_children.begin(); i != m_children.end(); i++)
			if (i->second == c)
			{
				Udm::DataNetwork * meta_dn = ((StaticDataNetwork*)c->__getdn())->GetMetaDn();
				ret = ::Uml::CompositionChildRole::Cast(meta_dn->ObjectById(i->first));
				return;
			}
	}


	vector<ObjectImpl*> StaticObject::getAssociation(const ::Uml::AssociationRole &meta, int mode ) const
	{
		/*

		cout << " ===============getAssociation() invoked================= " << endl;
		cout << " type of this: " << (string)m_type.name() << endl;
		cout << " AssociationRole: " << (string)meta.name() << endl;
		
		string ms;
		if (mode == Udm::TARGETFROMCLASS) ms = "TARGETFROMCLASS";
		else if (mode == Udm::TARGETFROMPEER) ms = "TARGETFROMPEER";
		else if (mode == Udm::CLASSFROMTARGET) ms = "CLASSFROMTARGET";
		cout << " mode: " << ms << endl;
		*/

		vector<ObjectImpl*> ret;
		
		if (m_type == ::Uml::Association::meta && meta == ::Uml::Association::meta_assocClass)
		{
		

			// well, I know this looks ugly,
			// but that's the price of having an ObjectImpl implementation
			// which can possibly hold the meta-meta, the meta, and the instance objects ;)
			
			// we need this if to avoid a stack overflow,
			// since to check whether we are an assoc. class or not,
			// we need to go recursive
			// anyway, It's kind of interesting, but logic and impressive at the same time
			// that there is only one 'recursive-stop if' in the whole StaticObject - truly recursive

			//of course, I could just cast down the implementation of the parent of the meta to StaticObject
			//and access directly the association maps
			//but the meta-meta may be CORBA as well, so we pretty much stuck to the API and/or ObjectImpl methods

			//so let's just return my associations
			//without any further checking
			pair<assoc_type::const_iterator, assoc_type::const_iterator> t = 
			associations.equal_range(meta.uniqueId());
			ret.reserve(associations.count(meta.uniqueId()));

			while( t.first != t.second )
			{
				ret.push_back((*t.first).second->clone());
				++t.first;
			}
			return ret;
			
		}
		
		
		
		if (mode == Udm::TARGETFROMPEER)
		{
			//if not assocation class & TARGETFROMPEER is requested
			//we need to return the associated objects, not the classes of the associations
			
			
			//this call gets recursive if we are implementing any two of the three abstraction levels 
			//(meta-meta, meta, instance)

			//i believe this next line is free of inheritence issues ... 
			::Uml::Class assClass = Uml::Association(meta.__impl()->getParent(Udm::NULLPARENTROLE)).assocClass();
			set< ::Uml::Class> assClassDsc_s;
			if (assClass)
			{
				assClassDsc_s = Uml::DescendantClasses(assClass);
				ret.reserve(ret.size() + assClassDsc_s.size());
			}

			pair<assoc_type::const_iterator, assoc_type::const_iterator> t = 
			associations.equal_range(meta.uniqueId());
			ret.reserve(associations.count(meta.uniqueId()));

			while( t.first != t.second )
			{
				StaticObject * assoc_so = (*t.first).second;
				
				if (!assoc_so) 
					throw udm_exception("StaticObject in map is NULL");
				
				if (assClass)
				{

					//now let's deal with assoc. classes.
					//we can be a descendant of an assoc. class either
					set< ::Uml::Class>::iterator dsc_i = assClassDsc_s.find(assoc_so->m_type);	

					if (dsc_i != assClassDsc_s.end())
					{
						//peer is an assoc class
						//via this parentrole
						pair<assoc_type::iterator, assoc_type::iterator> tt = 
						assoc_so->associations.equal_range(meta.uniqueId());
						int size = 0;
						while (tt.first != tt.second)
						{
							ret.push_back((*tt.first).second->clone());	
							++tt.first;
							size++;
						}
						if (size > 1) 
							throw udm_exception("Corrupt StaticObject maps: One end of an association class is associated to more than one objects!");
					}
					else
						//peer is a normal object
						ret.push_back((*t.first).second->clone());
				}
				else
					//peer is a normal object
					ret.push_back((*t.first).second->clone());
				
				++t.first;
			}
			return ret;
		}
		
		//default behaviour is OK
		// whenever I'm an assoc class  or a peer, and the assoc. classes are needed
		pair<assoc_type::const_iterator, assoc_type::const_iterator> t = 
			associations.equal_range(meta.uniqueId());
		ret.reserve(associations.count(meta.uniqueId()));

		while( t.first != t.second )
		{
			ret.push_back((*t.first).second->clone());
			++t.first;
		}
		
		/*
		cout << " ===============end of getAssociation() invoked================= " << endl;
		*/
		return ret;
	}


	StaticObject * StaticObject::FindCorrespondingObjectInStOrITree(StaticObject * where)
	{
		
			//debug
		children_type::iterator children_i = where->m_children.begin();
		while ((children_i != where->m_children.end()) )
		{
			
			if ((st_and_i.find(children_i->second)!= st_and_i.end())  && !(children_i->second->real_archetype))
				return children_i->second;

			StaticObject * ret = FindCorrespondingObjectInStOrITree(children_i->second);
			if(ret) return ret;
			children_i++;

		}
		return NULL;
	};




	//this function looks up a subtype or derived object
	//that is subtype or instance of 'what', but not directly(!)
	//among the one-level children of 'where'
	

	StaticObject * StaticObject::FindCorrespondingObjectInStOrI( StaticObject * where)
	{
		
		children_type::iterator children_i = where->m_children.begin();
		while ( children_i != where->m_children.end())
		{
			
			if ((st_and_i.find(children_i->second) != st_and_i.end())  && !children_i->second->real_archetype)
				return children_i->second;
			children_i++;

		}
		throw udm_exception("Could not find the corresponding subtype or instance in one of the parent's subtype/instance object.");

	};

	void StaticObject::setAssociation(
		const ::Uml::AssociationRole &meta, 
		const vector<ObjectImpl*> &nvect, 
		int mode,
		const bool direct) 
	{ 

		//check the role name
		
		
		/*
		cout << " ===============setAssociation() invoked================= " << endl;
		
		cout << stringAttrs.size() << endl;
		
		for (stringAttrs_type::iterator sa_i = stringAttrs.begin(); sa_i != stringAttrs.end(); sa_i++)
		{
			cout << " String attribute: " << (*sa_i).second << endl;
		}

		
		cout << m_type.uniqueId() << endl;

		
		cout << " type of this: " << (string)m_type.name() << endl;
		cout << " AssociationRole: " << (string)meta.name() << endl;

		if (nvect.size())
		{
			StaticObject * so = (StaticObject*)*(nvect.begin());
			cout << " unique ID of first associated object: "  << so->uniqueId() << endl;

			cout << " type of first associated objects: " << (string) so->m_type.name() << endl;
		}
		
		string ms;
		if (mode == Udm::TARGETFROMCLASS) ms = "TARGETFROMCLASS";
		else if (mode == Udm::TARGETFROMPEER) ms = "TARGETFROMPEER";
		else if (mode == Udm::CLASSFROMTARGET) ms = "CLASSFROMTARGET";
		cout << " mode: " << ms << endl;
		*/


		//inherited children of instances
		//
		
		

		//if parent is a derived or subtype object,
		// refuse to set an association unless it's coming from archetype
		// (reference links are exceptions)

		::Uml::AssociationRole orole = Uml::theOther(meta);

		if (m_parent->archetype && !m_parent->subtype && direct && !isRefLink(meta, mode, nvect))
		{
			//my parent is an instance object
			//this is an attempt to change my links, other than reference links
			
			//we have to make sure whether the links which are added/removed are pointing out of the box
			set<ObjectImpl*> removed_links;
			set<ObjectImpl*> added_links;

			
			//we will call release() when we remove them from removed_links
			vector<ObjectImpl*> cl_vec = getAssociation(meta,mode);
			removed_links.insert(cl_vec.begin(), cl_vec.end());


			for(vector<ObjectImpl*>::const_iterator nvect_i = nvect.begin(); nvect_i != nvect.end(); nvect_i++)
			{
				set<ObjectImpl*>::iterator cl_i = removed_links.find(*nvect_i);
				if (cl_i != removed_links.end())
				{
					//nvect contains a link which we already have
					//this it won't be removed
					//thus, we remove from removed_links
					//first, we have to release() it, it was obtained through getAssociation(), which clone()-s it.
					(*cl_i)->release();
					removed_links.erase(cl_i);
				}
				else added_links.insert(*nvect_i);

			}

		
			Object this_o = clone();
			Object cpdp_this = this_o.FindClosestPrimarilyDerivedParent();
			

			bool all_removed_ok = true;
			if (removed_links.size())
			{
				//we must check that the peers of removed links point "out of the box" 
				//that is, my closest parent which has real archetype is different from 
				//his closest parent which has real archetype
				set<ObjectImpl*>::iterator rl_i = removed_links.begin();
				while (rl_i != removed_links.end())
				{
					Object rl_o = *rl_i;//it's ok, objectImpl* in removed_links were cloned
					if (cpdp_this == rl_o.FindClosestPrimarilyDerivedParent())
						all_removed_ok = false;
					rl_i++;

				};

			};
			if (!all_removed_ok)
				throw udm_exception("Inherited links can not be removed from instance objects.");

			if (added_links.size())
			{
				set<ObjectImpl*>::iterator al_i = added_links.begin();
				while (al_i != added_links.end())
				{
					Object al_o = (*al_i)->clone();//have to clone it, the pointers are from nvect
					
					if (al_o.archetype() && (cpdp_this == al_o.FindClosestPrimarilyDerivedParent()))
						throw udm_exception("Only links that point \"out of the box\" can be added to instance objects.");
					al_i++;

				};


			};
		}


		//check the mode parameter			
		if (mode != Udm::TARGETFROMPEER)
		{

			//i'm either an association class, 
			//or one end of an association with an association class
			// that is because setting diretly a peer in a association with an assoc class
			// it's meaningless

			if ( mode == Udm::TARGETFROMCLASS)
			{
				// I'm an association class, one target is needed

				// Find the association
				::Uml::Association ass = ::Uml::GetAncestorAssociation(m_type);
				if (!ass)
					throw udm_exception("TARGETFROMCLASS was requested and I'm not an association class!");
				
				set< ::Uml::AssociationRole> ass_roles = ass.roles();
				if (ass_roles.size() != 2) 
					throw udm_exception("Invalid association " + (string) ass.name() + " found in meta information");

				set< ::Uml::AssociationRole >::iterator role_it = ass_roles.begin();

				::Uml::AssociationRole role_end = *(role_it);

				if (role_end != meta)
				{
					role_end = *(++role_it);
					if (role_end != meta)
						throw udm_exception("Invalid role name specified in StaticObject::setAssociation!");
				}

				//now role_end is the right AssociationRole
				
				::Uml::Class target_class = role_end.target();
		

				for(vector<ObjectImpl*>::const_iterator vect_it_1 = nvect.begin(); vect_it_1 != nvect.end(); vect_it_1++)
				{
					StaticObject * obj  = (StaticObject*) (*vect_it_1);
					
					if(!Uml::IsDerivedFrom(obj->m_type,target_class))
						 throw udm_exception("Invalid association!");	

				}


			}//if ( mode == Udm::TARGETFROMCLASS)
			else
			{
				if (mode != Udm::CLASSFROMTARGET)
					throw udm_exception("Invalid mode in StaticObject::setAssociation!");
				
				//one end of an association class

				//get all my rolenames (from ancestor classes, too)
				set< ::Uml::AssociationRole> roles = ::Uml::AncestorAssociationRoles(m_type);
				
				//find the role provided
				set< ::Uml::AssociationRole>::iterator i = roles.find(orole);
			
				if (i == roles.end()) 
					throw udm_exception("Invalid meta specified in StaticObject::setAssociation!");
				
			}//else if ( mode == Udm::TARGETFROMCLASS)

		}//if (mode != Udm::TARGETFROMPEER)
		else
		{
			//this is a simple association
			// TARGETFROMPEER makes no sense
			// when setting up an assoc. class association
			
	
			//get all my rolenames (from ancestor classes, too)
			set< ::Uml::AssociationRole> roles = ::Uml::AncestorAssociationRoles(m_type);
			
			set< ::Uml::AssociationRole>::iterator i = roles.find(orole);
			if (i == roles.end()) 
				throw udm_exception("Invalid meta specified in StaticObject::setAssociation!");
			
		}//else if (mode != Udm::TARGETFROMPEER)

		
		//at this point we are ensured that meta rolename  is valid
		//set up a searchable set with object to associate
		//btw, why the hell are we working with vectors on inside APIs?!

		// we don't increment the refcounter because this local map will be deleted
		set<ObjectImpl*> to_associate(nvect.begin(), nvect.end());

		//fix for UDM-37:
		//even if we remove items from to_associate because an association already exists,
		//keep associating the corresponding subtypes and instances because their
		//associations may be missing
		set<ObjectImpl*> to_associate_in_st_and_i(to_associate);

		//set up a map of my children with the current role,
		//such that I can reach my  and my parties iterators for all the associations:
		//
		// <the Object associated to me>		<iterator of the association in my map>  <iterator of the association in his map>		

		//type definitions for this...
		typedef pair<assoc_type::iterator, assoc_type::iterator > bidir_ass_pair;
		typedef map<StaticObject *, bidir_ass_pair> so_bidir_ass_map;
		typedef pair<StaticObject * const, bidir_ass_pair> so_bidir_ass_item;

		//get the existing associations with this rolename
		pair<assoc_type::iterator, assoc_type::iterator> t = associations.equal_range(meta.uniqueId());

		so_bidir_ass_map bidir_ass_map;

		for (assoc_type::iterator j = t.first; j != t.second; j++)
		{
			pair<assoc_type::iterator, assoc_type::iterator> w = 
				j->second->associations.equal_range(orole.uniqueId());
			
			for (assoc_type::iterator k = w.first; k != w.second; k++)
			{
				if (k->second == this)
				{
					bidir_ass_pair ass_pair(j, k);
					so_bidir_ass_item so_ass_item(j->second, ass_pair);
					//again, we are insert in a local map ...
					pair<so_bidir_ass_map::iterator, bool> ins_res = bidir_ass_map.insert(so_ass_item);
					if (!ins_res.second) throw udm_exception("Fatal error in StaticObject::setAssociation(). Map insertion failed: 2");
				}
			}
			
		}
		//iterate on the map of existing associations
		//for each association, if it's still needed remove from the set to_associate
		//if it's not , remove associations from both, my and my partie's ass. map
		
		
		so_bidir_ass_map::iterator bam_i;
		
		for (bam_i = bidir_ass_map.begin(); bam_i != bidir_ass_map.end(); bam_i++)
		{
			StaticObject *so = (*bam_i).first;
			set<ObjectImpl*>::iterator it_to_ass = to_associate.find(so);
			if ( it_to_ass  == to_associate.end())
			{
				bidir_ass_pair iterators = bam_i->second;
				
				//remove from both association maps
				associations.erase(iterators.first);
				so->associations.erase(iterators.second);

				//archetype/derived stuff.
				//this link needs to be deleted in all my instantiated and derived objects

				//it is a reference setting, we'll do it afterwards
				//if ( ((string)(meta.name()) == "ref") && (mode == Udm::TARGETFROMPEER) && nvect.size()==1) continue;
				if (isRefLink(meta, mode, nvect)) continue;

				//1. get parent
				//2. get it's derived/subtypes objects
				//3. for each such object, get their direct children
				//4. find among them the one which is derived/subtype of so
				//5. find among them the one which is derived/subtype of this
				//6, find the iterators in their association_map
				//7. delete the corresponding associations
				//8. decrement the recounts

				set<StaticObject*>::iterator psti_i = m_parent->st_and_i.begin();
				while (psti_i != m_parent->st_and_i.end())
				{
					//this is the subtype/instance object
					StaticObject * s_or_i = *psti_i++;

					//these will be those subtype/instance object of so and this, 
					//which are direct children of s_or_i
					StaticObject * s_or_i_of_so = so->FindCorrespondingObjectInStOrITree(s_or_i);
					StaticObject * s_or_i_of_this = FindCorrespondingObjectInStOrI(s_or_i);

				
					if (!s_or_i_of_so) continue;//this means that an object corresponding to the connecting party was not found
					
					//lookup the iterators.
					assoc_type::iterator ass_erase_so, ass_erase_this;
					bool found = false;

					pair<assoc_type::iterator, assoc_type::iterator> w;

					w = s_or_i_of_so->associations.equal_range(orole.uniqueId());
					while (w.first != w.second)
					{
						if (w.first->second == s_or_i_of_this)
						{
							ass_erase_so = w.first;
							found = true;
							break;
						}
						w.first ++;
					};

					if (!found)
						throw udm_exception("Static Datanetwork error: Link corresponding to archetype link could not be located in derived/instantiated object!");
					
					found = false;

					
					w = s_or_i_of_this->associations.equal_range(meta.uniqueId());
					while (w.first != w.second)
					{
						if (w.first->second == s_or_i_of_so)
						{
							ass_erase_this = w.first;
							found = true;
							break;
						}
						w.first ++;
					};

					if (!found)
						throw udm_exception("Static Datanetwork error: Link corresponding to archetype link could not be located in derived/instantiated object!");
					

					//remove the link

					s_or_i_of_this->associations.erase(ass_erase_this);
					s_or_i_of_so->associations.erase(ass_erase_so);

					//decrement the refcounts
					s_or_i_of_this->release();
					s_or_i_of_so->release();
	
					
				}

				so->release();				//because deleted my party from my assoc. table
				if (this->refCount <= 1)
					throw udm_exception("Recount error is setassociation. At least my parent should have a reference to me!");
				this->release();			//because deleted myself from my party's assoc. table
			}
			else
				to_associate.erase(it_to_ass);
		}



		for(set<ObjectImpl*>::iterator it_to_ass = to_associate.begin(); it_to_ass != to_associate.end(); it_to_ass++)
		{
			//remove association between peer and its peers when the model says that peer can be connected to a single object
			if (orole.max() == 1 && mode == Udm::TARGETFROMPEER)
				((StaticObject*)(*it_to_ass))->setAssociation(orole, vector<ObjectImpl*>(), mode, false);

			//ok, create the link(increment ref. count's)
		
			//one for me,
			pair<uniqueId_type const, StaticObject*> associated(meta.uniqueId(), ((StaticObject*)(*it_to_ass)->clone()));
			associations.safe_insert(associated);
			
			//and the other one for my party
			pair<uniqueId_type const, StaticObject*> ass_me(orole.uniqueId(), (StaticObject*)this->clone());
			((StaticObject*)(*it_to_ass))->associations.safe_insert(ass_me);
		}

		//it is a reference setting, we'll do it afterwards
		if (!isRefLink (meta, mode, nvect)) {

			//archetype/derived/instance stuff
			//1. get parent
			//2. get it's derived/subtypes objects
			//3. for each such object, get their direct children
			//4. find among them the one which is derived/subtype of so
			//5. find among them the one which is derived/subtype of this
			//6, find the iterators in their association_map
			//7. insert the corresponding associations
			//8. increment the refcounts
			
			set<StaticObject*>::iterator psti_i = m_parent->st_and_i.begin();
			while (psti_i != m_parent->st_and_i.end())
			{
				//this is the subtype/instance object
				StaticObject * s_or_i = *psti_i++;

				//we need to walk up the hierarchy
				//until the parent which is primarily derived

				StaticObject * s_or_i_pd = s_or_i;

				while (s_or_i_pd && !s_or_i_pd->real_archetype)
					s_or_i_pd = s_or_i_pd->m_parent;

				if (!s_or_i_pd) 
					throw udm_exception("Expected a primarily derived parent somewhere in the hierarchy!");

				for(set<ObjectImpl*>::iterator it_to_ass = to_associate_in_st_and_i.begin(); it_to_ass != to_associate_in_st_and_i.end(); it_to_ass++)
				{

					//these will be those subtype/instance object of so and this, 
					//which are direct children of s_or_i
					StaticObject * s_or_i_of_so = ((StaticObject*)(*it_to_ass))->FindCorrespondingObjectInStOrITree(s_or_i_pd);
					StaticObject * s_or_i_of_this = FindCorrespondingObjectInStOrI(s_or_i);

					if (!s_or_i_of_so) continue; // a corresponding object could not be located

					//add the new link to these objects
					//one for me,

					// it might be already set...

					pair<uniqueId_type const, StaticObject*> ass1(meta.uniqueId(), (StaticObject*)(s_or_i_of_so->clone()));
					s_or_i_of_this->associations.safe_insert(ass1);
			
					//and the other one for my party
					pair<uniqueId_type const, StaticObject*> ass2(orole.uniqueId(), (StaticObject*)(s_or_i_of_this->clone()));
					s_or_i_of_so->associations.safe_insert(ass2);
				}
			}
		}

		//it is a reference setting, we'll do it afterwards
		//if ( ((string)(meta.name()) == "ref") && (mode == Udm::TARGETFROMPEER) && nvect.size()==1) 
		if (isRefLink(meta, mode, nvect)) 
		{
			set<StaticObject*>::iterator st_and_i_i = st_and_i.begin();
			while (st_and_i_i != st_and_i.end())
			{
				if ( (*st_and_i_i)->subtype)
				{
					vector<ObjectImpl*> existing_references = getAssociation(meta, mode);
					if (existing_references.size() == 0)
						(*st_and_i_i)->setAssociation(meta, nvect, mode, false);
					else
					{
						//release them
						vector<ObjectImpl*>::iterator er_i = existing_references.begin();
						while (er_i != existing_references.end())
						{
							(*er_i)->release();
							er_i++;
						}
					}
				}
				else
				{
					(*st_and_i_i)->setAssociation(meta, nvect, mode, false);
				}

				st_and_i_i++;
			}
		};



		/*
		cout << " ===============end of setAssociation() invoked================= " << endl;
		*/
	};

	vector<ObjectImpl*> StaticObject::getInstances() const
	{
		vector<ObjectImpl*> ret;
		set<StaticObject*>::const_iterator d_i = st_and_i.begin();
		while (d_i != st_and_i.end())
		{
			StaticObject * st_and_i_item = *d_i++;
			
			if (!st_and_i_item->subtype)
				ret.push_back(st_and_i_item->clone());
		}
		return ret;
	};

	vector<ObjectImpl*> StaticObject::getDerived() const
	{
		vector<ObjectImpl*> ret;
		set<StaticObject*>::const_iterator d_i = st_and_i.begin();
		while (d_i != st_and_i.end())
		{
			StaticObject * st_and_i_item = *d_i++;
			
			if (st_and_i_item->subtype)
				ret.push_back(st_and_i_item->clone());
		}
		return ret;
	};
	
	ObjectImpl* StaticObject::getArchetype() const
	{
		return archetype ? archetype->clone() : NULL;
	};

	bool StaticObject::hasRealArchetype() const
	{
		//if it has archetype, and that archetype is real,
		//ie. it's not there because a parent inherited it
		if (!archetype)
			throw udm_exception("hasRealArchetype() can be evaluated only on instantiated  or derived objects, or (inherited)children of derived objects.");
		return real_archetype;
	};

	bool StaticObject::isSubtype() const 
	{
		return (archetype && subtype);
	};

	bool StaticObject::isInstance() const 
	{
		return (archetype && (!subtype));

	};
	
	static void DeSerializeString(FILE* f, std::string &result) {
		result.reserve(500);
		char t;
		do
		{
			if(!fread(&t, sizeof(char), 1, f))
				throw udm_exception("can't read from file, probably MEM file is corrupt");
			result.push_back(t);
		} while (t != 0x00);
		result.erase(result.end() - 1); // remove terminating nul
	}

	unsigned long StaticDataNetwork::DeSerialize(FILE *f, map<unsigned long, const StaticObject*>& tr_map,  Object & root_o)
	{
		//make some caches
		typedef pair< ::Uml::Class, string> role_key_t; 

		//cache maps
		typedef map<string, ::Uml::Class> type_cache_t;
		typedef map<role_key_t, ::Uml::CompositionChildRole> ccr_cache_t;
		typedef map<role_key_t, ::Uml::AssociationRole> asr_cache_t;


		//cache attributes
		
		typedef map<string, uniqueId_type> att_name_t;
		typedef map<string, att_name_t> att_cache_t;
		typedef pair<string const, att_name_t> att_cache_item_t;
		typedef pair<att_cache_t::iterator, bool> att_cache_ir_t;

		static att_cache_t att_cache;
		static att_cache_ir_t att_cache_ir;


		//map items
		typedef pair<string const, ::Uml::Class> type_cache_item_t;
		typedef pair<role_key_t const, ::Uml::CompositionChildRole> ccr_cache_item_t;
		typedef pair<role_key_t const, ::Uml::AssociationRole> asr_cache_item_t;

		//insert results
		typedef pair<type_cache_t::iterator, bool> type_cache_ir_t;
		typedef pair<ccr_cache_t::iterator, bool> ccr_cache_ir_t;
		typedef pair<asr_cache_t::iterator, bool> asr_cache_ir_t;



		//map instances for caching
		static type_cache_t type_cache;
		static ccr_cache_t ccr_cache;
		static asr_cache_t asr_cache;

		//insert result instance
		static type_cache_ir_t type_cache_ir;
		static ccr_cache_ir_t ccr_cache_ir;
		static asr_cache_ir_t asr_cache_ir;

		//static role key
		static role_key_t role_key;




#define MAX_NAME 50
		
		bool is_first = true;
		unsigned long read = 0;

		if (!f) throw udm_exception("Invalid file handler in StaticDataNetwork::Deserialize()!");

		list<StaticObject*> read_list;			//put the read object in a simple list, than translate the id's 			
		static ::Uml::Class type;

		// library roots
		list<StaticObject*> lib_roots;


		while (!feof(f))
		{
			//cout <<"\t new obejct " << endl;
			//read uniqueID from file
			static unsigned long my_id;
			if (!fread(&my_id, sizeof(unsigned long), 1, f))
			{
				if (feof(f)) continue;	//read the file

				throw udm_exception("can't read from file, probably MEM file is corrupted");
			}

			//read my type from file
			string stl_type_name;
			DeSerializeString(f, stl_type_name);
			if (stl_type_name.at(0) == ':') stl_type_name.erase(0, 1);
			
			//now we have the type name.
			//find out type by searching my meta diagram's classes -- it must be there ;-)
			

		
			static type_cache_t::iterator tc_i;
			tc_i =  type_cache.find(stl_type_name);

			if (tc_i == type_cache.end())
			{
				/*
					This code will run hopefully only once for each type
					Because of the const -s, I could not manage
					to have type_cache_item as a static variable, 
					it would be good for the stack ;)
				*/

				/*
					1. first look up the namesace
					2. Then the class
					we will separate them by colon
				*/
				type = ::Uml::classByPath(*metaroot.dgr, stl_type_name, ":");
				if (!type) throw udm_exception(string("No meta information could be located for ") + stl_type_name);
				

				type_cache_item_t type_cache_item(stl_type_name, type);
				type_cache_ir = type_cache.insert(type_cache_item);

				if (!type_cache_ir.second)
					throw udm_exception("Type cache map insertion failed!");
				

			} else type = (*tc_i).second;


			//get archetype
			static unsigned long archetype_id;
			if (!fread(&archetype_id, sizeof(unsigned long), 1, f))
				throw udm_exception("can't read from file, probably MEM file is corrupted");

			static bool subtype_val;
			if (!fread(&subtype_val, sizeof(bool), 1, f))
				throw udm_exception("can't read from file, probably MEM file is corrupted");


			static bool real_archetype_val;
			if (!fread(&real_archetype_val, sizeof(bool), 1, f))
				throw udm_exception("can't read from file, probably MEM file is corrupted");


			//create &obtain reference to  a safe type
			const ::Uml::Class &safe_type = Uml::SafeTypeContainer::GetSafeType(type);
			
			//create the `so` object with the meta information
			StaticObject * so = new StaticObject(safe_type, 1, reinterpret_cast<StaticObject *>(archetype_id), subtype_val, real_archetype_val );
			so->mydn = this;
			read_list.push_front(so);		//add to the list of read objects

			//if this is the first call, then assign the first StaticObject to rootobject
			if(is_first)
			{
				so->mydn = this;
				root_o = so;
				is_first = false;
			}
			if (!so) throw udm_exception("New failed in StaticDataNetwork::Deserialize()!");

			//add the new and the old id to transition map
			
			pair<unsigned long const, const StaticObject*> tr_map_item(my_id, so);
//			(unsigned long)(tr_map_item.first) = my_id;
//			tr_map_item.second = so;

			static pair<map<unsigned long, const StaticObject*>::iterator, bool> ins_res;
			ins_res = tr_map.insert(tr_map_item);
			
			if(!ins_res.second) throw udm_exception("Insert failed in transition map in StaticDataNetwork::Deserialize()!");
			

			
			//read in my type's ancestors
			//we will need them 
			static set< ::Uml::Class> ancestors;
			ancestors = Uml::AncestorClasses(type);

			static set< ::Uml::Class>::iterator anc_i;

			//read the childrens
			static unsigned long children_no;
			if (!fread(&children_no, sizeof(unsigned long), 1, f))
				throw udm_exception("can't read from file, probably MEM file is corrupted");

			role_key.first = type;		//set the first part of the key to our type, this remains the same when adding childrens and associations
			//composition child roles are ordered in the file
			//so we just make use of this

			static string ccr_name_prev;
			static ::Uml::CompositionChildRole ccr;

			ccr_name_prev = "";	//reset previous child role name to empty string
			ccr = &Udm::_null;		//reset previous composition child role to null

			//for each children
			unsigned int j;			
			for (j = 0; j < children_no; j++)
			{
				//read composition childrole
				string ccr_name;
				DeSerializeString(f, ccr_name);

				//read children's unique ID
				static unsigned long child_id;
				if (!fread(&child_id, sizeof(unsigned long), 1, f))
					throw udm_exception("can't read from file, probably MEM file is corrupted");

				// libroot child
				if (ccr_name.length() == 0) {
					pair<uniqueId_type const, StaticObject*> m_ch_item(0, reinterpret_cast<StaticObject*>(child_id));
					so->m_children.insert(m_ch_item);
					continue;
				}
				
				
				//Level 1 cache: there's is a chance that it's the same childrole name as with the previous child
				if (ccr_name != ccr_name_prev || !ccr)
				{
					ccr = &Udm::_null;
					ccr_name_prev = ccr_name;
				
					//level 2 cache, check with the ccr_cache map
					role_key.second = ccr_name;
					
					static ccr_cache_t::iterator cc_i;
					cc_i = ccr_cache.find(role_key);

					if (cc_i == ccr_cache.end())
					{

						//find composition childrole
						//as theother's of my composition parent roles
						//need to consider here my ancestor's, too
			
						for (anc_i = ancestors.begin(); anc_i != ancestors.end(); anc_i++)
						{
							if (!ccr)
							{
								static set< ::Uml::CompositionParentRole> cpr_s;
								static set< ::Uml::CompositionParentRole>::iterator cpr_s_i;
								static string ccr_try_name;
								
								cpr_s.erase(cpr_s.begin(), cpr_s.end());
							
								cpr_s = (*anc_i).parentRoles();

								for(cpr_s_i = cpr_s.begin(); cpr_s_i != cpr_s.end(); cpr_s_i++)
								{
								
									//ccr_try_name = (string) Uml::theOther((*cpr_s_i)).name();
									/*
										it might be an empty childrole!!!
									*/
									ccr_try_name = Uml::MakeShortRoleName(Uml::theOther(*cpr_s_i));

									if (!ccr_try_name.compare(ccr_name))
									{
										ccr = Uml::theOther((*cpr_s_i));
										//cache insertion
										//again, this will be run only once for each compostion child role
										ccr_cache_item_t ccr_cache_item(role_key, ccr);
										ccr_cache_ir = ccr_cache.insert(ccr_cache_item);

										if (!ccr_cache_ir.second)
											throw udm_exception("CCR cache insert operation failed!");

										break;
									}
								}
								if (ccr) break;
							}
							if	(ccr) break;
						}
						if (!ccr) throw udm_exception(string(" Composition child role ") + ccr_name + string(" was not found in meta information"));
					}
					else
						ccr = (*cc_i).second;
				}

				//insert into  children's map 
				//since our children is not created yet, 
				//we put it's id for the value of the pointer
				//anyway, it won't be referenced and we'll change it when coming back 
				//from recursion

				pair<uniqueId_type const, StaticObject*> m_ch_item(ccr.uniqueId(),  reinterpret_cast<StaticObject*>(child_id));
				
//				(uniqueId_type)(m_ch_item.first) = ccr.uniqueId();
//				m_ch_item.second =  reinterpret_cast<StaticObject*>(child_id);

				so->m_children.insert(m_ch_item);	

			};

			//read the associations
			static unsigned long ass_no;
			if (!fread(&ass_no, sizeof(unsigned long), 1, f))
				throw udm_exception("can't read from file, probably MEM file is corrupted");

			::Uml::AssociationRole asr;
			string asr_name_prev;

			//for each association
			for (j = 0; j < ass_no; j++)
			{
				//read composition childrole
				string asr_name;
				DeSerializeString(f, asr_name);

				//find composition childrole
				//as theother's of my composition parent roles
				//need to consider here my ancestor's, too
				//static ::Uml::AssociationRole asr;
				
				if (asr_name != asr_name_prev || !asr)
				{	//level 1 cache failed
					asr_name_prev = asr_name;
					asr = &Udm::_null;
					
					//level 2 cache
					//get it from asr_cache
					role_key.second = asr_name;
					
					static asr_cache_t::iterator ac_i;
					ac_i = asr_cache.find(role_key);
					if (ac_i == asr_cache.end())
					{//level 2 cache failed
						for (anc_i = ancestors.begin(); anc_i != ancestors.end(); anc_i++)
						{
							if (!asr)
							{
							
								static set< ::Uml::AssociationRole> asr_s, ass_asr_s;
								static set< ::Uml::AssociationRole>::iterator asr_s_i ;

								asr_s.erase(asr_s.begin(), asr_s.end());
								ass_asr_s.erase(ass_asr_s.begin(), ass_asr_s.end());

								static ::Uml::Association ass ;
								ass = &Udm::_null;
						
								asr_s = (*anc_i).associationRoles();
								ass = (*anc_i).association();

								if (ass)
									 ass_asr_s = ass.AssociationRole_kind_children();

								
								for(asr_s_i = asr_s.begin(); asr_s_i != asr_s.end(); asr_s_i++)
								{
									string asr_try_name = (string) Uml::theOther((*asr_s_i)).name();
									if (!asr_try_name.compare(asr_name))
									{
										asr = Uml::theOther((*asr_s_i));
										//cache insertion
										//again, this will be run only once for each association role
										asr_cache_item_t asr_cache_item(role_key, asr);
										asr_cache_ir = asr_cache.insert(asr_cache_item);

										if (!ccr_cache_ir.second)
											throw udm_exception("CCR cache insert operation failed!");

										break;
									}
								}

								if (asr) break;

								for(asr_s_i = ass_asr_s.begin(); asr_s_i != ass_asr_s.end(); asr_s_i++)
								{
									string asr_try_name = (string) Uml::theOther((*asr_s_i)).name();
									if (!asr_try_name.compare(asr_name))
									{
										asr = Uml::theOther((*asr_s_i));
										asr_cache_item_t asr_cache_item(role_key, asr);
										asr_cache_ir = asr_cache.insert(asr_cache_item);

										if (!ccr_cache_ir.second)
											throw udm_exception("CCR cache insert operation failed!");

										break;
									}
								}
								
								if (asr) break;
							}
							if (asr) break;
						}
						if (!asr) throw udm_exception(string(" Composition child role ") + string(asr_name) + string(" was not found in meta information"));
					}
					else asr = (*ac_i).second;		//level 2 cache hit
				}//level 1 cache hit

				//read associateds' unique ID
				static unsigned long assed_id;
				if (!fread(&assed_id, sizeof(unsigned long), 1, f))
					throw udm_exception("can't read from file, probably MEM file is corrupted");

				//insert into  associations map 
				//since our associated objects are not created yet, 
				//we put its id for the value of the pointer
				//anyway, it won't be referenced and we'll change it when coming back 
				//from recursion
				pair<uniqueId_type const, StaticObject*> m_ass_item(asr.uniqueId(), reinterpret_cast<StaticObject*>(assed_id));
				so->associations.insert(m_ass_item);	
				
			//	cout << "reading association with role name: " << asr_name << endl;
			};

		
			
			static att_cache_t::iterator att_ci;
			static att_name_t att_map;

			att_ci = att_cache.find(stl_type_name);
			if (att_ci == att_cache.end())
			{
			
				att_map.erase(att_map.begin(), att_map.end());
				
				for (anc_i = ancestors.begin(); anc_i != ancestors.end(); anc_i++)
				{
					set< ::Uml::Attribute> att_l = (*anc_i).attributes();
					for(set< ::Uml::Attribute>::iterator att_i = att_l.begin(); att_i != att_l.end(); att_i++)
					{
						//this can't be static
						pair<string const, uniqueId_type> att_map_item((string)(*att_i).name(), (*att_i).uniqueId());
						
						static pair<map<string, uniqueId_type>::iterator, bool> ins_res;
						ins_res = att_map.insert(att_map_item);
						if (!ins_res.second) throw udm_exception(string("Non-unique attribute name: ") + (string)(*att_i).name() + string("found for class: ") + (string)type.name());
					};
				}
				
				att_cache_item_t att_cache_item(stl_type_name, att_map);
				att_cache_ir = 	att_cache.insert(att_cache_item);
				if (!att_cache_ir.second)
					throw udm_exception("Attribute cache operation failed!");
			}else att_map = (*att_ci).second;
				


			//read the string attributes
			static unsigned long sa_no;
			if (!fread(&sa_no, sizeof(unsigned long), 1, f))
				throw udm_exception("can't read from file, probably MEM file is corrupted");
			
			//for each string attribute
			for (j = 0; j < sa_no; j++)
			{
				
				//read the attribute name
				string sa_name;
				DeSerializeString(f, sa_name);

				//get its id from the att_map 
				static map<string, uniqueId_type>::iterator sa_i;
				sa_i = att_map.find(sa_name);
				if (sa_i == att_map.end()) throw udm_exception(string("Attribute: '") + sa_name + string("' was not found for class ") + (string)type.name());
				
				//read the attribute value

				std::string sa_val;
				DeSerializeString(f, sa_val);

				//add the attribute to the object
				
				pair<uniqueId_type const, string> sa_item((*sa_i).second, sa_val );
				
				static pair<map<uniqueId_type, string>::iterator, bool> ins_res;

				ins_res = so->stringAttrs.insert(sa_item);
				if (!ins_res.second) throw udm_exception(" String Attribute could not be added to StaticObject!");
				
				/* read it's desynched attribute */
				static bool desynched_val;
				if (!fread(&desynched_val, sizeof(bool), 1, f))
					throw udm_exception("can't read from file, probably MEM file is corrupted");
				if (desynched_val) so->desynched.insert((*sa_i).second);



			}

			//read the boolean attributes
			static unsigned long ba_no;
			fread(&ba_no, sizeof(unsigned long), 1, f);

			//for each bool attribute
			for (j = 0; j < ba_no; j++)
			{
			
				//read the attribute name
				string ba_name;
				DeSerializeString(f, ba_name);

				//get its id from the att_map 
				static map<string, uniqueId_type>::iterator ba_i;
				ba_i = att_map.find(ba_name);
				if (ba_i == att_map.end()) throw udm_exception(string("Attribute: ") + ba_name + string(" was not found for class ") + (string)type.name());
				
				//read the attribute value
				static bool ba_val;
				if(!fread(&ba_val, sizeof(bool), 1, f))
					throw udm_exception("can't read from file, probably MEM file is corrupted");

				// add the attribute to the object
				//uniqueId_type ba_id = (*ba_i).second;
				pair<uniqueId_type const, bool> ba_item((*ba_i).second, ba_val);
				static pair<map<uniqueId_type, bool>::iterator, bool> ins_res;
				
				
//				(uniqueId_type) ba_item.first = (*ba_i).second;
//				ba_item.second = ba_val;
				
				ins_res = so->booleanAttrs.insert(ba_item);

				if (!ins_res.second) 
					throw udm_exception(" Boolean Attribute could not be added to StaticObject!");

				/* read it's desynched attribute */
				static bool desynched_val;
				if (!fread(&desynched_val, sizeof(bool), 1, f))
					throw udm_exception("can't read from file, probably MEM file is corrupted");
				if (desynched_val) so->desynched.insert((*ba_i).second);


			}

			//read the integer attributes
			static unsigned long ia_no;
			if(!fread(&ia_no, sizeof(unsigned long), 1, f))
				throw udm_exception("can't read from file, probably MEM file is corrupted");

			//for each integer attribute
			for (j = 0; j < ia_no; j++)
			{
			
				//read the attribute name
				string ia_name;
				DeSerializeString(f, ia_name);

				//get it's id from the att_map 
				static map<string, uniqueId_type>::iterator ia_i;
				ia_i = att_map.find(ia_name);
				if (ia_i == att_map.end()) throw udm_exception(string("Attribute: ") + ia_name + string(" was not found for class ") + (string)type.name());
				
				//read the attribute value
				static __int64 ia_val;
				fread(&ia_val, sizeof(__int64), 1, f);

				// add the attribute to the object
				//uniqueId_type ia_id = (*ia_i).second;
				pair<uniqueId_type const, __int64> ia_item((*ia_i).second, ia_val);
				static pair<map<uniqueId_type,__int64>::iterator, bool> ins_res;
				
//				(uniqueId_type)(ia_item.first) = (*ia_i).second;
//				ia_item.second = ia_val;

				ins_res = so->longintAttrs.insert(ia_item);

				if (!ins_res.second) throw udm_exception(" Integer Attribute could not be added to StaticObject!");

				/* read it's desynched attribute */
				static bool desynched_val;
				if (!fread(&desynched_val, sizeof(bool), 1, f))
					throw udm_exception("can't read from file, probably MEM file is corrupted");
				if (desynched_val) so->desynched.insert((*ia_i).second);


			}


			//read the real attributes
			static unsigned long ra_no;
			if(!fread(&ra_no, sizeof(unsigned long), 1, f))
				throw udm_exception("can't read from file, probably MEM file is corrupted");

			//for each real attribute
			for (j = 0; j < ra_no; j++)
			{
			
				//read the attribute name
				string ra_name;
				DeSerializeString(f, ra_name);

				//get it's id from the att_map 
				static map<string, uniqueId_type>::iterator ra_i;
				ra_i = att_map.find(ra_name);
				if (ra_i == att_map.end()) throw udm_exception	(string("Attribute: ") + ra_name + string(" was not found for class ") + (string)type.name());
				
				//read the attribute value
				static double ra_val;
				fread(&ra_val, sizeof(double), 1, f);

				// add the attribute to the object
				//uniqueId_type ra_id = (*ra_i).second;
				
				pair<uniqueId_type const, double> ra_item((*ra_i).second, ra_val);
				static pair<map<uniqueId_type, double>::iterator, bool> ins_res ;
//				(uniqueId_type) ra_item.first = (*ra_i).second;
//				ra_item.second = ra_val;
				
				ins_res = so->realAttrs.insert(ra_item);


				if (!ins_res.second) throw udm_exception(" Real Attribute could not be added to StaticObject!");

				/* read it's desynched attribute */
				static bool desynched_val;
				if (!fread(&desynched_val, sizeof(bool), 1, f))
					throw udm_exception("can't read from file, probably MEM file is corrupted");
				if (desynched_val) so->desynched.insert((*ra_i).second);


			}

			//read the stringArray attributes
			static unsigned long sa_arr_no;
			if (!fread(&sa_arr_no, sizeof(unsigned long), 1, f))
				throw udm_exception("can't read from file, probably MEM file is corrupted");
			
			//for each string attribute
			for (j = 0; j < sa_arr_no; j++)
			{
				
				//read the attribute name
				string sa_arr_name;
				DeSerializeString(f, sa_arr_name);

				//get it's id from the att_map 
				static map<string, uniqueId_type>::iterator sa_arr_i;
				sa_arr_i = att_map.find(sa_arr_name);
				if (sa_arr_i == att_map.end()) throw udm_exception(string("Attribute: ") + sa_arr_name + string(" was not found for class ") + (string)type.name());
				
				//read the number of attributes
				static unsigned long sa_arr_item_no;
				if (!fread(&sa_arr_item_no, sizeof(unsigned long), 1, f))
					throw udm_exception("can't read from file, probably MEM file is corrupted");
			
				static vector<string> sa_arr_value;
				if (!sa_arr_value.empty()) sa_arr_value.erase(sa_arr_value.begin(), sa_arr_value.end());


				static unsigned long sa_arr_item_count;

				for( sa_arr_item_count = 0; sa_arr_item_count < sa_arr_item_no; sa_arr_item_count++)
				{
					//read the attribute value
					string sa_val;
					DeSerializeString(f, sa_val);
					sa_arr_value.push_back(sa_val);
				}
				//add the attribute to the object
			
				
				pair<uniqueId_type const, StaticObject::string_array> sa_item((*sa_arr_i).second, sa_arr_value);
				static pair<map<uniqueId_type, StaticObject::string_array>::iterator, bool> ins_res;
				
//				(uniqueId_type)(sa_item.first) = (*sa_arr_i).second;
//				sa_item.second = sa_arr_value;

				ins_res = so->stringAttrArrs.insert(sa_item);
				if (!ins_res.second) throw udm_exception(" String Attribute Array could not be added to StaticObject!");

				/* read it's desynched attribute */
				static bool desynched_val;
				if (!fread(&desynched_val, sizeof(bool), 1, f))
					throw udm_exception("can't read from file, probably MEM file is corrupted");
				if (desynched_val) so->desynched.insert((*sa_arr_i).second);


			}

			//read the booleanArray attributes
			static unsigned long ba_arr_no;
			if (!fread(&ba_arr_no, sizeof(unsigned long), 1, f))
				throw udm_exception("can't read from file, probably MEM file is corrupted");
			
			//for each boolean array attribute
			for (j = 0; j < ba_arr_no; j++)
			{
				
				//read the attribute name
				string ba_arr_name;
				DeSerializeString(f, ba_arr_name);

				//get it's id from the att_map 
				static map<string, uniqueId_type>::iterator ba_arr_i;
				ba_arr_i = att_map.find(ba_arr_name);
				if (ba_arr_i == att_map.end()) throw udm_exception(string("Attribute: ") + ba_arr_name + string(" was not found for class ") + (string)type.name());
				
				//read the number of attributes
				static unsigned long ba_arr_item_no;
				if (!fread(&ba_arr_item_no, sizeof(unsigned long), 1, f))
					throw udm_exception("can't read from file, probably MEM file is corrupted");
			
				static vector<bool> ba_arr_value;
				if (!ba_arr_value.empty()) ba_arr_value.erase(ba_arr_value.begin(), ba_arr_value.end());


				static unsigned long ba_arr_item_count;

				static bool * bool_val;
				bool_val = new bool[ba_arr_item_no];
				
				//read in the array from file
				if(!fread(bool_val, sizeof(bool), ba_arr_item_no, f))
					throw udm_exception("can't read from file, probably MEM file is corrupted");
	
				//add to vector
				for (ba_arr_item_count=0; ba_arr_item_count < ba_arr_item_no; ba_arr_item_count++)
					ba_arr_value.push_back(*(bool_val + ba_arr_item_count));

				delete [] bool_val;
				
				//add the attribute to the object
			
				
				pair<uniqueId_type const, StaticObject::bool_array> ba_item((*ba_arr_i).second, ba_arr_value);
				static pair<map<uniqueId_type, StaticObject::bool_array>::iterator, bool> ins_res;
				
//				(uniqueId_type)(ba_item.first) = (*ba_arr_i).second;
//				ba_item.second = ba_arr_value;

				ins_res = so->booleanAttrArrs.insert(ba_item);
				if (!ins_res.second) throw udm_exception(" Bool Attribute Array could not be added to StaticObject!");

				/* read it's desynched attribute */
				static bool desynched_val;
				if (!fread(&desynched_val, sizeof(bool), 1, f))
					throw udm_exception("can't read from file, probably MEM file is corrupted");
				if (desynched_val) so->desynched.insert((*ba_arr_i).second);


			}

			//read the integerArray attributes
			static unsigned long ia_arr_no;
			if (!fread(&ia_arr_no, sizeof(unsigned long), 1, f))
				throw udm_exception("can't read from file, probably MEM file is corrupted");
			
			//for each boolean array attribute
			for (j = 0; j < ia_arr_no; j++)
			{
				
				//read the attribute name
				string ia_arr_name;
				DeSerializeString(f, ia_arr_name);

				//get it's id from the att_map 
				map<string, uniqueId_type>::iterator ia_arr_i;
				ia_arr_i = att_map.find(ia_arr_name);
				if (ia_arr_i == att_map.end()) throw udm_exception(string("Attribute: ") + ia_arr_name + string(" was not found for class ") + (string)type.name());
				
				//read the number of attributes
				unsigned long ia_arr_item_no;
				if (!fread(&ia_arr_item_no, sizeof(unsigned long), 1, f))
					throw udm_exception("can't read from file, probably MEM file is corrupted");
			
				static vector<__int64> ia_arr_value;
				if (!ia_arr_value.empty()) ia_arr_value.erase(ia_arr_value.begin(), ia_arr_value.end());


				static unsigned long ia_arr_item_count;

				static  __int64 * long_val;
				long_val = new __int64[ia_arr_item_no];
				
				//read in the array from file
				if(!fread(long_val, sizeof(__int64), ia_arr_item_no, f))
					throw udm_exception("can't read from file, probably MEM file is corrupted");
	
				//add to vector
				for (ia_arr_item_count=0; ia_arr_item_count < ia_arr_item_no; ia_arr_item_count++)
					ia_arr_value.push_back(*(long_val + ia_arr_item_count));

				delete [] long_val;
				
				//add the attribute to the object
			
				
				pair<uniqueId_type const, StaticObject::longint_array> ia_item((*ia_arr_i).second, ia_arr_value);
				static pair<map<uniqueId_type, StaticObject::longint_array>::iterator, bool> ins_res;
				
//				(uniqueId_type)(ia_item.first) = (*ia_arr_i).second;
//				ia_item.second = ia_arr_value;

				ins_res = so->longintAttrArrs.insert(ia_item);
				if (!ins_res.second) throw udm_exception(" Integer Attribute Array could not be added to StaticObject!");

				/* read it's desynched attribute */
				static bool desynched_val;
				if (!fread(&desynched_val, sizeof(bool), 1, f))
					throw udm_exception("can't read from file, probably MEM file is corrupted");
				if (desynched_val) so->desynched.insert((*ia_arr_i).second);


			}

			//read the realArray attributes
			static unsigned long ra_arr_no;
			if (!fread(&ra_arr_no, sizeof(unsigned long), 1, f))
				throw udm_exception("can't read from file, probably MEM file is corrupted");
			
			//for each real array attribute
			for (j = 0; j < ra_arr_no; j++)
			{
				
				//read the attribute name
				string ra_arr_name;
				DeSerializeString(f, ra_arr_name);

				//get it's id from the att_map 
				static map<string, uniqueId_type>::iterator ra_arr_i;
				ra_arr_i = att_map.find(ra_arr_name);
				if (ra_arr_i == att_map.end()) throw udm_exception(string("Attribute: ") + ra_arr_name + string(" was not found for class ") + (string)type.name());
				
				//read the number of attributes
				static unsigned long ra_arr_item_no;
				if (!fread(&ra_arr_item_no, sizeof(unsigned long), 1, f))
					throw udm_exception("can't read from file, probably MEM file is corrupted");
			
				static vector<double> ra_arr_value;
				if (!ra_arr_value.empty()) ra_arr_value.erase(ra_arr_value.begin(), ra_arr_value.end());


				static unsigned long ra_arr_item_count;

				static  double * double_val;
				double_val = new double[ra_arr_item_no];
				
				//read in the array from file
				if(!fread(double_val, sizeof(double), ra_arr_item_no, f))
					throw udm_exception("can't read from file, probably MEM file is corrupted");
	
				//add to vector
				for (ra_arr_item_count=0; ra_arr_item_count < ra_arr_item_no; ra_arr_item_count++)
					ra_arr_value.push_back(*(double_val + ra_arr_item_count));

				delete [] double_val;
				
				//add the attribute to the object
			
				
				pair<uniqueId_type const, StaticObject::double_array> ra_item((*ra_arr_i).second, ra_arr_value);
				static pair<map<uniqueId_type, StaticObject::double_array>::iterator, bool> ins_res;
				
//				(uniqueId_type)(ra_item.first) = (*ra_arr_i).second;
//				ra_item.second = ra_arr_value;

				ins_res = so->realAttrArrs.insert(ra_item);
				if (!ins_res.second) throw udm_exception(" Real Attribute Array could not be added to StaticObject!");

				/* read it's desynched attribute */
				static bool desynched_val;
				if (!fread(&desynched_val, sizeof(bool), 1, f))
					throw udm_exception("can't read from file, probably MEM file is corrupted");
				if (desynched_val) so->desynched.insert((*ra_arr_i).second);


			}

			// either the end of object marker or continuation marker
			static unsigned char marker;
			if (!fread(&marker, sizeof(unsigned char), 1, f))
				throw udm_exception("can't read from file, probably MEM file is corrupted");
			if (marker == 0xFE) {
				// library root flag and possible library name
				static bool lib_root_val;
				if (!fread(&lib_root_val, sizeof(bool), 1, f))
					throw udm_exception("can't read from file, probably MEM file is corrupted");
				so->lib_root = lib_root_val;

				if (lib_root_val) {
					static char tmp_lib_name[MAX_NAME+1];
					static char *tmp_lib_name_p;
					static char t;
					static int i;

					tmp_lib_name_p = tmp_lib_name;
					i = 0;
					do
					{
						if (!fread(&t, sizeof(char), 1, f))
							throw udm_exception("can't read from file, probably MEM file is corrupted");

						*tmp_lib_name_p++ = t; i++;

					} while ((t != 0x00 )&& (i < MAX_NAME));
					if (t != 0x00) throw udm_exception("MEM file corrupt!");
					so->lib_name = tmp_lib_name;

					lib_roots.push_back(so);
				}

				if (!fread(&marker, sizeof(unsigned char), 1, f))
					throw udm_exception("can't read from file, probably MEM file is corrupted");
			}







			if (marker != 0xFF) throw udm_exception("MEM file corrupt!");
		}
		


		//recursive step
		//read += StaticDataNetwork::DeSerialize(f, tr_map, false, root_o);

		//walk the list, change the pointers to the new ones
	
	
		while (!read_list.empty())
		{
			read_list.front()->ValidatePointers(tr_map);
			read_list.pop_front();
		}

		while (!lib_roots.empty()) {
			lib_roots.front()->setLOFOnChildren(true);
			lib_roots.pop_front();
		}

		//clear the caches
		//and static variables
		//that may hold a reference to an Impl object
			
		type_cache.erase(type_cache.begin(), type_cache.end());
		
		//the other caches may remain as they are,
		//since their key is ::Uml::Class, which won't be the same for other paradigms
		type = &Udm::_null;

		return ftell(f);

	};

	
	unsigned long StaticObject::Serialize(FILE *f)
	{
#if defined(_WIN32) && defined(_M_AMD64)
		// FIXME: add map<uniqueId_type, unsigned long> id_map;
#endif
		//this will return the length in bytes of space 
		//and write this object to file


		unsigned long length = 0;

		//write my uniqueId to file
		unsigned long my_id = uniqueId();
		fwrite(&my_id, sizeof(unsigned long), 1, f);
		length += sizeof(unsigned long);

		//m_type
		//write the name of my type to file
		//namespacename:classname
		string m_type_path = m_type.getPath2(":", false);
		if (!((::Uml::Namespace) m_type.parent_ns()))
			m_type_path.insert(0, ":");		// backward compatible with ":class" identifiers (no namespace)
		fwrite(m_type_path.c_str(), m_type_path.size() + 1, 1, f);
		length += (m_type_path.size() + 1);


		//here we should write two things:
		//my archetype, and whether I'm instance or subtype 
		//of that archetype
		unsigned long archetype_to_wr = archetype ? archetype->uniqueId() : 0;
		

		//first, write the uniqueId of archetype
		fwrite(&archetype_to_wr, sizeof(unsigned long), 1, f);
		length += sizeof(unsigned long);

		//then, write the bool flag
		fwrite(&subtype, sizeof(bool), 1, f);
		length += sizeof(bool);

		//then, write the real_archetype flag
		fwrite(&real_archetype, sizeof(bool), 1, f);
		length += sizeof(bool);



		//m_parent --this is redundant
		
		//m_children
		//we create a map here of ObjectID's to ChildRolename's
		//we don't want to use the __getdn(), objectByID() functions,
		//because the meta-meta for instance never will have a data network

		//we need to store how many children we have
		unsigned long children_no = m_children.size();
		fwrite(&children_no, sizeof(unsigned long), 1, f);
		length += sizeof(unsigned long);
		
		
		static set< ::Uml::Class> ancestors;
		ancestors = Uml::AncestorClasses(m_type);
				
		static map<uniqueId_type, string> ccr_map;
		ccr_map.erase(ccr_map.begin(), ccr_map.end());


		
			
		children_type::iterator ch_i =  m_children.begin();	

		Udm::DataNetwork * meta_dn = ((StaticDataNetwork*)__getdn())->GetMetaDn();

		for (; ch_i != m_children.end(); ch_i++)
		{

			//ccr_name_str might be empty,
			//use MakeShortRolenames to be able to
			//distinguish between them
			//string ccr_name_str = ccr.name();
			string ccr_name_str;

			if (ch_i->first) {
				const ::Uml::CompositionChildRole ccr = ::Uml::CompositionChildRole::Cast(meta_dn->ObjectById((*ch_i).first));
				ccr_name_str = Uml::MakeShortRoleName(ccr);
			}
#if 0
			
			const ::Uml::CompositionChildRole ccr = ::Uml::CompositionChildRole::Cast(meta_dn->ObjectById((*ch_i).first));
			

			//ccr_name_str might be empty,
			//use MakeShortRolenames to be able to
			//distinguish between them
			//string ccr_name_str = ccr.name();
			string ccr_name_str = Uml::MakeShortRoleName(ccr);
#endif

			//write the name of the child role
			//long ccr_str_length = ((*ccr_map_i).second).size();
			long ccr_str_length = ccr_name_str.size();
			const char * ccr_name = ccr_name_str.c_str();

			fwrite(ccr_name, ccr_str_length + 1, 1, f);
			
			length += (ccr_str_length +1);
			
			//write the unique id of the child
			unsigned long child_id = ((*ch_i).second)->uniqueId();
			fwrite(&child_id, sizeof(unsigned long), 1, f);
			length += sizeof(unsigned long);

		};

		// associations
		// we need to store how many associations we have
		//cout << "-----new object: " << my_id << ", associations " << endl;
		unsigned long ass_no = associations.size();
		fwrite(&ass_no, sizeof(unsigned long), 1, f);
		length += sizeof(unsigned long);
		
		assoc_type::iterator as_i =  associations.begin();
		for (; as_i != associations.end(); as_i ++)
		{

			::Uml::AssociationRole asr = 	::Uml::AssociationRole::Cast(meta_dn->ObjectById((*as_i).first));
			string asr_name_stl = asr.name();

			//write the name of the association role
			//long asr_str_length = ((*asr_map_i).second).size();
			long asr_str_length = asr_name_stl.size();
			const char * asr_name = asr_name_stl.c_str();
			
			fwrite(asr_name, asr_str_length + 1, 1, f);
			length += (asr_str_length +1);

			//write the object id of the association role
			unsigned long ass_id = ((*as_i).second)->uniqueId();
			fwrite(&ass_id, sizeof(unsigned long), 1, f);
			length += sizeof(unsigned long);
			//debug
		//	cout << "association role: " << asr_name << " asr unique id: " << as_i->first << ", associated obj.id: " << ass_id << endl;

		}

		//stringAttrs
		//we need to store how many string attributes we have
		unsigned long sa_no = stringAttrs.size();
		fwrite(&sa_no, sizeof(unsigned long), 1, f);
		length += sizeof(unsigned long);
		
		stringAttrs_type::iterator sa_i =  stringAttrs.begin();
		
		for (; sa_i != stringAttrs.end(); sa_i++)
		{
			::Uml::Attribute sa = ::Uml::Attribute::Cast(meta_dn->ObjectById(sa_i->first));
			string sa_name_stl = sa.name();

			//write the name of the string Attribute role
			long sa_str_length = sa_name_stl.size();
			const char * sa_name = sa_name_stl.c_str();

			fwrite(sa_name, sa_str_length + 1, 1, f);
			length += (sa_str_length +1);

			//write the string value of the attribute
			long sa_val_length = ((*sa_i).second).size();
			char * sa_val = new char[sa_val_length + 1];
			strcpy(sa_val, ((*sa_i).second).c_str());
			fwrite(sa_val, sa_val_length + 1, 1, f);
			delete [] sa_val;
			length += (sa_val_length + 1);

			/*
				add here code that saves whether it's a desynched attribute
			*/
			bool is_dsync = (desynched.find(sa.uniqueId()) != desynched.end());
			fwrite(&is_dsync, sizeof(bool), 1, f);
			length += sizeof(bool);


		}


		//booleanAttrs
		//we need to know how many boolean attributes we have
		unsigned long ba_no = booleanAttrs.size();
		fwrite(&ba_no, sizeof(unsigned long), 1, f);
		length += sizeof(unsigned long);

		booleanAttrs_type::iterator ba_i =  booleanAttrs.begin();
		
		for (; ba_i != booleanAttrs.end(); ba_i++)
		{
			::Uml::Attribute ba = ::Uml::Attribute::Cast(meta_dn->ObjectById(ba_i->first));
			string ba_name_stl = ba.name();

			//write the name of the boolean Attribute role
			long ba_str_length = ba_name_stl.size();
			const char * ba_name = ba_name_stl.c_str();

			fwrite(ba_name, ba_str_length + 1, 1, f);
			length += (ba_str_length +1);

			//write the boolean value of the attribute
			bool ba_val = (*ba_i).second;
			fwrite(&ba_val, sizeof(bool), 1, f);
			length += sizeof(bool);

			/*
				add here code that saves whether it's a desynched attribute
			*/
			bool is_dsync = (desynched.find(ba.uniqueId()) != desynched.end());
			fwrite(&is_dsync, sizeof(bool), 1, f);
			length += sizeof(bool);


		}


		//integerAttrs
		//we need to know how many integer attributes we have
		unsigned long ia_no = longintAttrs.size();
		fwrite(&ia_no, sizeof(unsigned long), 1, f);
		length += sizeof(unsigned long);

		longintAttrs_type::iterator ia_i =  longintAttrs.begin();
																																														
		for (; ia_i != longintAttrs.end(); ia_i++)
		{

			::Uml::Attribute ia = ::Uml::Attribute::Cast(meta_dn->ObjectById(ia_i->first));
			string ia_name_stl = ia.name();

			//write the name of the integer Attribute role
			//long ia_str_length = ((*att_map_i).second).size();
			long ia_str_length = ia_name_stl.size();
			const char * ia_name = ia_name_stl.c_str();
			
			fwrite(ia_name, ia_str_length + 1, 1, f);
			length += (ia_str_length +1);

			//write the boolean value of the attribute
			__int64 ia_val = (*ia_i).second;
			fwrite(&ia_val, sizeof(__int64), 1, f);
			length += sizeof(__int64);

			/*
				add here code that saves whether it's a desynched attribute
			*/
			bool is_dsync = (desynched.find(ia.uniqueId()) != desynched.end());
			fwrite(&is_dsync, sizeof(bool), 1, f);
			length += sizeof(bool);

			
		}



		//realAttrs
		//we need to know how many real attributes we have
		unsigned long ra_no = realAttrs.size();
		fwrite(&ra_no, sizeof(unsigned long), 1, f);
		length += sizeof(unsigned long);

		realAttrs_type::iterator ra_i =  realAttrs.begin();
		
		for (; ra_i != realAttrs.end(); ra_i++)
		{
			::Uml::Attribute ra = ::Uml::Attribute::Cast(meta_dn->ObjectById(ra_i->first));
			string ra_name_stl = ra.name();


			//write the name of the integer Attribute role
			//long ra_str_length = ((*att_map_i).second).size();
			long ra_str_length = ra_name_stl.size();
			const char * ra_name = ra_name_stl.c_str();
			
			fwrite(ra_name, ra_str_length + 1, 1, f);
			length += (ra_str_length +1);

			//write the double value of the attribute
			double ra_val = (*ra_i).second;
			fwrite(&ra_val, sizeof(double), 1, f);
			length += sizeof(double);

			/*
				add here code that saves whether it's a desynched attribute
			*/
			bool is_dsync = (desynched.find(ra.uniqueId()) != desynched.end());
			fwrite(&is_dsync, sizeof(bool), 1, f);
			length += sizeof(bool);


		}

		//stringAttrArrs
		//we need to store how many string attribute Arrays we have
		unsigned long sa_arr_no = stringAttrArrs.size();
		fwrite(&sa_arr_no, sizeof(unsigned long), 1, f);
		length += sizeof(unsigned long);
		
		stringAttrArrs_type::iterator sa_arr_i =  stringAttrArrs.begin();
		
		for (; sa_arr_i != stringAttrArrs.end(); sa_arr_i++)
		{
			::Uml::Attribute sa_arr = ::Uml::Attribute::Cast(meta_dn->ObjectById(sa_arr_i->first));
			string sa_arr_name_stl = sa_arr.name();

			//write the name of the string Attribute role
			long sa_str_length = sa_arr_name_stl.size();
			const char * sa_arr_name = sa_arr_name_stl.c_str();

			fwrite(sa_arr_name, sa_str_length + 1, 1, f);
			length += (sa_str_length +1);

			//get the string array
			static vector<string> sa_arr_value;
			sa_arr_value = (*sa_arr_i).second;

			//write the number of items in the array
			unsigned long sa_arr_size = sa_arr_value.size();
			fwrite(&sa_arr_size, sizeof(unsigned long), 1, f);
			length += sizeof(unsigned long);

			
			vector<string>::const_iterator sa_arr_value_i = sa_arr_value.begin(); 

			while (sa_arr_value_i != sa_arr_value.end())
			{
				//write the string value of the attribute
				long sa_val_length = (*sa_arr_value_i).size();
				char * sa_val = new char[sa_val_length + 1];
				strcpy(sa_val, (*sa_arr_value_i).c_str());
				fwrite(sa_val, sa_val_length + 1, 1, f);
				delete [] sa_val;
				length += (sa_val_length + 1);
				sa_arr_value_i++;
			}

			/*
				add here code that saves whether it's a desynched attribute
			*/
			bool is_dsync = (desynched.find(sa_arr.uniqueId()) != desynched.end());
			fwrite(&is_dsync, sizeof(bool), 1, f);
			length += sizeof(bool);


		}

		//booleanAttrArrs
		//we need to know how many boolean attributes we have
		unsigned long ba_arr_no = booleanAttrArrs.size();
		fwrite(&ba_arr_no, sizeof(unsigned long), 1, f);
		length += sizeof(unsigned long);

		booleanAttrArrs_type::iterator ba_arr_i =  booleanAttrArrs.begin();
		
		for (; ba_arr_i != booleanAttrArrs.end(); ba_arr_i++)
		{
			::Uml::Attribute ba = ::Uml::Attribute::Cast(meta_dn->ObjectById(ba_arr_i->first));
			static string ba_name_stl;
			ba_name_stl = ba.name();

			//write the name of the boolean Attribute role
			static long ba_str_length;
			ba_str_length = ba_name_stl.size();

			static const char * ba_name;
			ba_name = ba_name_stl.c_str();

			fwrite(ba_name, ba_str_length + 1, 1, f);
			length += (ba_str_length +1);

			//get the bool-array
			static vector<bool> ba_arr_val;
			static unsigned long  ba_arr_item_size;
			static vector<bool>::iterator ba_arr_val_i;
			
			ba_arr_val = (*ba_arr_i).second;
			ba_arr_item_size = ba_arr_val.size();
		
			//write out the size of the array
			fwrite(&ba_arr_item_size, sizeof(unsigned long), 1, f);
			length += sizeof(unsigned long);


			ba_arr_val_i = ba_arr_val.begin();


			while (ba_arr_val_i != ba_arr_val.end())
			{
				//write the boolean value of the attribute
				static bool ba_val;
				ba_val = *ba_arr_val_i;
				fwrite(&ba_val, sizeof(bool), 1, f);
				length += sizeof(bool);
				ba_arr_val_i++;
			}

			/*
				add here code that saves whether it's a desynched attribute
			*/
			bool is_dsync = (desynched.find(ba.uniqueId()) != desynched.end());
			fwrite(&is_dsync, sizeof(bool), 1, f);
			length += sizeof(bool);


		}

		//integerAttrArrs
		//we need to know how many boolean attributes we have
		unsigned long ia_arr_no = longintAttrArrs.size();
		fwrite(&ia_arr_no, sizeof(unsigned long), 1, f);
		length += sizeof(unsigned long);

		longintAttrArrs_type::iterator ia_arr_i =  longintAttrArrs.begin();
		
		for (; ia_arr_i != longintAttrArrs.end(); ia_arr_i++)
		{
			::Uml::Attribute ia = ::Uml::Attribute::Cast(meta_dn->ObjectById(ia_arr_i->first));
			static string ia_name_stl;
			ia_name_stl = ia.name();

			//write the name of the boolean Attribute role
			static long ia_str_length;
			ia_str_length = ia_name_stl.size();

			static const char * ia_name;
			ia_name = ia_name_stl.c_str();

			fwrite(ia_name, ia_str_length + 1, 1, f);
			length += (ia_str_length +1);

			//get the bool-array
			static vector<__int64> ia_arr_val;
			static unsigned long  ia_arr_item_size;
			static vector<__int64>::iterator ia_arr_val_i;
			
			ia_arr_val = (*ia_arr_i).second;
			ia_arr_item_size = ia_arr_val.size();
		
			//write out the size of the array
			fwrite(&ia_arr_item_size, sizeof(unsigned long), 1, f);
			length += sizeof(unsigned long);


			ia_arr_val_i = ia_arr_val.begin();


			while (ia_arr_val_i != ia_arr_val.end())
			{
				//write the boolean value of the attribute
				static __int64 ia_val;
				ia_val = *ia_arr_val_i;
				fwrite(&ia_val, sizeof(__int64), 1, f);
				length += sizeof(__int64);
				ia_arr_val_i++;
			}

			/*
				add here code that saves whether it's a desynched attribute
			*/
			bool is_dsync = (desynched.find(ia.uniqueId()) != desynched.end());
			fwrite(&is_dsync, sizeof(bool), 1, f);
			length += sizeof(bool);


		}

		//realAttrArrs
		//we need to know how many boolean attributes we have
		unsigned long ra_arr_no = realAttrArrs.size();
		fwrite(&ra_arr_no, sizeof(unsigned long), 1, f);
		length += sizeof(unsigned long);

		realAttrArrs_type::iterator ra_arr_i =  realAttrArrs.begin();
		
		for (; ra_arr_i != realAttrArrs.end(); ra_arr_i++)
		{
			::Uml::Attribute ra = ::Uml::Attribute::Cast(meta_dn->ObjectById(ra_arr_i->first));
			static string ra_name_stl;
			ra_name_stl = ra.name();

			//write the name of the boolean Attribute role
			static long ra_str_length;
			ra_str_length = ra_name_stl.size();

			static const char * ra_name;
			ra_name = ra_name_stl.c_str();

			fwrite(ra_name, ra_str_length + 1, 1, f);
			length += (ra_str_length +1);

			//get the bool-array
			static vector<double> ra_arr_val;
			static unsigned long  ra_arr_item_size;
			static vector<double>::iterator ra_arr_val_i;
			
			ra_arr_val = (*ra_arr_i).second;
			ra_arr_item_size = ra_arr_val.size();
		
			//write out the size of the array
			fwrite(&ra_arr_item_size, sizeof(unsigned long), 1, f);
			length += sizeof(unsigned long);


			ra_arr_val_i = ra_arr_val.begin();


			while (ra_arr_val_i != ra_arr_val.end())
			{
				//write the boolean value of the attribute
				static double ra_val;
				ra_val = *ra_arr_val_i;
				fwrite(&ra_val, sizeof(double), 1, f);
				length += sizeof(double);
				ra_arr_val_i++;
			}

			/*
				add here code that saves whether it's a desynched attribute
			*/
			bool is_dsync = (desynched.find(ra.uniqueId()) != desynched.end());
			fwrite(&is_dsync, sizeof(bool), 1, f);
			length += sizeof(bool);


		}

		// library root flag and library name
		static const unsigned char not_end_of_obj = 0xFE;
		fwrite(&not_end_of_obj, sizeof(const unsigned char), 1, f);
		length += sizeof(const unsigned char);

		fwrite(&lib_root, sizeof(bool), 1, f);
		length += sizeof(bool);

		if (lib_root) {
			fwrite(lib_name.c_str(), lib_name.length() + 1, 1, f);
			length += lib_name.length() + 1;
		}




		// ending 0xFF byte of object

		static const unsigned char end_of_obj = 0xFF;
		fwrite(&end_of_obj, sizeof(const unsigned char), 1, f);
		length += sizeof(const unsigned char);
		
		
		// go recursive
		for (ch_i = m_children.begin(); ch_i != m_children.end(); ch_i++)
			 length += (*ch_i).second->Serialize(f);

		return length;

	};

	void StaticObject::ValidatePointers(map<unsigned long, const StaticObject*>& tr_map)
	{
		//change childrens' id to their new id according to tr_map, also set their parent to this


		static StaticObject::children_type::iterator so_ch_i; 
		static map<unsigned long, const StaticObject*>::iterator tr_map_i;
		static StaticObject::assoc_type::iterator ass_i;
			
		so_ch_i = m_children.begin();

		for(; so_ch_i != m_children.end(); so_ch_i++)
		{
			tr_map_i = tr_map.find(reinterpret_cast<unsigned long>((*so_ch_i).second));
			if (tr_map_i == tr_map.end()) throw udm_exception(" failed to map back id's in file to already created objects!");
			
			//change the id to real object pointer
			(*so_ch_i).second  = const_cast<StaticObject *>((*tr_map_i).second);
			
			//now it's a real object, set it's parent to so
			((*so_ch_i).second)->m_parent = this;
			((*so_ch_i).second)->mydn = static_cast<StaticDataNetwork *>(__getdn());
			((*so_ch_i).second)->clone();		//because it's in my children map
			
		}


		//change  ass.-ed objects' id to their new id according to tr_map	
		
		ass_i = associations.begin();	
		
		for(; ass_i != associations.end(); ass_i++)
		{
			tr_map_i = tr_map.find(reinterpret_cast<unsigned long>((*ass_i).second));
			if (tr_map_i == tr_map.end()) throw udm_exception(" failed to map back id's in file to already created objects!");
			
			//change the id to real object pointer
			(*ass_i).second  = const_cast<StaticObject *>((*tr_map_i).second);
			((*ass_i).second)->clone();			//because it's in my assoc. map
		
		}

		//change the archetype
		if (archetype)
		{
			//if archetype is not null
			tr_map_i = tr_map.find(reinterpret_cast<unsigned long>(archetype));
			if (tr_map_i == tr_map.end()) throw udm_exception(" failed to map back id's in file to already created objects!");

			//change the id to the real object pointer
			archetype = const_cast<StaticObject *>((*tr_map_i).second);

			//add this to the archetype's set of derived or instantiated objects
			archetype->st_and_i.insert(this);
			
		}
		
	}

	bool StaticObject::isLibObject() const
	{
		return lib_object;
	}

	bool StaticObject::isLibRoot() const
	{
		return lib_root;
	}

	bool StaticObject::getLibraryName(string &name) const
	{
		if (!isLibRoot())
			return false;

		name = lib_name;
		return true;
	}

	void StaticObject::setLOFOnChildren(bool is_lib_object)
	{
		children_type::iterator i = m_children.begin();
		while (i != m_children.end()) {
			StaticObject *co = i->second;
			co->lib_object = is_lib_object;
			co->setLOFOnChildren(is_lib_object);
			i++;
		}
	}

	void StaticObject::setLibraryName(const char *name)
	{
		if (name == NULL) {
			lib_root = false;

			// detach from all nested libraries
			children_type::iterator i = m_children.begin();
			while (i != m_children.end()) {
				i->second->setLibraryName(name);
				i++;
			}
		}
		else {
			lib_root = true;
			lib_name = name;
		}
		setLOFOnChildren(name != NULL);
	}

	ObjectImpl * StaticObject::createLibRootChild(const ::Uml::Class &meta, const bool need_safetype)
	{
		//caller must pass meta as a safe type
		StaticObject *dep = new StaticObject(meta, 1);

		// equivalent to dep->setParent:
		pair<uniqueId_type const, StaticObject *> child(0, (StaticObject *) (dep->clone()));
		m_children.insert(child);
		dep->m_parent = this;
		dep->mydn = mydn;

		dep->setDefaultAttributes(false);

		return dep->clone();
	}

   	vector<ObjectImpl*> StaticObject::getLibRoots() const
	{
		vector<ObjectImpl*> ret;
		ret.reserve(m_children.size());

		children_type::const_iterator t = m_children.begin();
		while (t!= m_children.end())
		{
			if (t->second->lib_root)
				ret.push_back((*t).second->clone());
			++t;
		}
	
		return ret;
	}

	// --------------------------- Static Data Network funcitons

	UDM_DLL StaticDataNetwork::~StaticDataNetwork()
	{
		if(isOpen()) 
		{
			//check whether we need to save the datanetwork
		
			if (sem != Udm::CHANGES_LOST_DEFAULT)
				SaveAs(systemname);
			CloseNoUpdate();
		}
		
		SDNList::iterator ff;	
		for (ff = Udm::_UdmStaticData.SDNs.begin(); ff != Udm::_UdmStaticData.SDNs.end(); ff++) 
		if (*ff == this) break;
			
		if (ff == Udm::_UdmStaticData.SDNs.end()) throw udm_exception("Corrupt Static DN map");
		Udm::_UdmStaticData.SDNs.erase(ff);
	}
		
	void StaticDataNetwork::CloseNoUpdate() {
		StaticObject* root_o = (StaticObject*)rootobject.__impl();
		
		//first detach all rootobject's children , recursively
	
/*
		StaticObject::children_type::iterator children_i= root_o->m_children.begin();
		for (; children_i != root_o->m_children.end(); children_i++)
				(*children_i).second->detach();
	
*/			
		//reset root object(it still has a valid pointer!)
		//this will decrement root_o refcount!!!(release())

		root_o->Destroy(true);
	
		rootobject = Udm::null;
	};

	
	UDM_DLL StaticDataNetwork::StaticDataNetwork(const Udm::UdmDiagram &metainfo, Udm::UdmProject* project) :	Udm::DataNetwork(metainfo, project)   
	{
		Udm::_UdmStaticData.SDNs.push_front(this);
		rootobject = Udm::null;
		
		/*
			a meta-data network is needed for getting the meta-objects(attributes, childroles, associationroles) by their IDs.
			Usually, this is the data network of the meta objects, obtained by GetRootMeta().impl()->__getdn().
			In rare cases, when the meta is the Uml meta-meta, then these meta objects don't have a Data Network, they just exists. So thge 
			line above would be NULL. In this cases, knowning that StaticDataNetwork::ObjectById() will work fine for _all_ StaticObjects, we
			use our Datanetwork's StaticDataNetwork to map back ids to objects.
		*/
		if (metainfo.dgr->__impl() != &Udm::_null)
			meta_dn = metainfo.dgr->__impl()->__getdn();
		if (!meta_dn) meta_dn = this;

	};

	
	bool StaticDataNetwork::SafeObjectById = true;			// default behaviour for ObjectById




	
	UDM_DLL void StaticDataNetwork::CreateNew(const string &systemname, const string &metalocator, 
		const ::Uml::Class &rootclass, enum Udm::BackendSemantics sem) 
	{
		rootobject = new StaticObject(rootclass, this, 1);
	//	rootobject.__impl()->setDefaultAttributes();
		this->systemname = systemname;
		this->sem = sem;
	};

	UDM_DLL void StaticDataNetwork::OpenExisting(const string &systemname, const string &metalocator, enum Udm::BackendSemantics sem )
	{
		FILE * f = fopen(systemname.c_str(), "rb");
		map<unsigned long, const StaticObject *> tr_map;
		
		this->systemname = systemname;
		this->sem = sem;

		if (f)
		{
			
			unsigned long bytes;
			try {
				bytes = DeSerialize(f, tr_map,  rootobject); 
			} catch (udm_exception& e) {
				// This may leak memory, but due to the reinterpret_casts in DeSerialize, we can't do any better without rewriting DeSerialize
				rootobject = Udm::null;
				fclose(f);
				throw e;
			}
			fclose(f);
			if (!bytes) throw udm_exception("Openexisting  failed");
		
		}
		else throw udm_exception("OpenExisting() could not open file for reading!");
		
		//if this datanetwork is part of a project
		//the project's cross-link datanetwork must be
		//validated with the new IDs
		if (pr != NULL)
		{
			//create the map
			map<const unsigned long, const unsigned long> new_tr_map;

			//start walking our tr_map
			map<unsigned long, const StaticObject*>::iterator tr_map_i = tr_map.begin();
			while (tr_map_i != tr_map.end())
			{
				//create the new item
				pair<const unsigned long, const unsigned long> new_tr_map_item(tr_map_i->first, reinterpret_cast<const unsigned long>(tr_map_i->second));
				//insert into new_tr_map
				pair<map<const unsigned long, const unsigned long>::iterator, bool> ins_res = new_tr_map.insert(new_tr_map_item);
				if (!ins_res.second)
					throw udm_exception("Insert failed in new_tr_map!");
				tr_map_i++;
			}

			//invoke validation of new IDs by project
			pr->ValidateIDs(systemname, new_tr_map);
		};
	
	};

		

	UDM_DLL void StaticDataNetwork::SaveAs(string systemname)	
	{
		FILE * f = fopen(systemname.c_str(), "wb");
		if (f)
		{
			StaticDataNetwork::SafeObjectById = false;		//speed up ObjectById() calls

 			((StaticObject*)rootobject.__impl())->Serialize(f);

			StaticDataNetwork::SafeObjectById = true;				
			fclose(f);
		}
		else throw udm_exception("CloseWithUpdate() could not open file for writing!");
		
	};



	UDM_DLL Object StaticDataNetwork::ObjectById(Object::uniqueId_type id) 
	{ 
		//id was a pointer to a Static Object
		//this shouldn't fail...
		//return reinterpret_cast<StaticObject*>(id);
		//conversion needs clone()-ing!


		StaticObject * so_try = reinterpret_cast<StaticObject*>(id);


		if (StaticDataNetwork::SafeObjectById)
		{

			set<StaticObject*>::iterator i = Udm::_UdmStaticData.so_set.find(so_try);		
			if (i == Udm::_UdmStaticData.so_set.end())
				throw udm_exception("Corrupt StaticObject set. Object by pointer is not in set!");	
		}
		
		return so_try->clone();
	};

	
	//we register static data network this way
	//cosntructor will run when this file is linked.(c) Bakay... ;-)


	static class reg {
		public:
		reg() {
			StaticDataNetwork::RegisterBackend("MEM", "mem", &StaticDataNetwork::factory);
		}	
		~reg() {
			StaticDataNetwork::UnRegisterBackends();
		}
	} _reg_unused;

	// --------------------------- Public functions

	UDM_DLL Object CreateObject(const ::Uml::Class &meta)
	{
		return new StaticObject(meta, 1);
	}

	void CreateComposition(const Object &p, const ::Uml::CompositionChildRole &childRole,
		const Object &c, const ::Uml::CompositionParentRole &parentRole)
	{
		// TODO: check the type
		StaticObject *parent = static_cast<StaticObject*>(p.__impl());
		StaticObject *child = static_cast<StaticObject*>(c.__impl());

		UDM_ASSERT( child->m_parent == NULL );

		child->refCount += 2;
		child->m_parent = parent;
		child->mydn = static_cast<StaticDataNetwork *>(parent->__getdn());
		parent->m_children.insert(StaticObject::children_type::value_type(
			childRole.uniqueId(), child));
	}

	void CreateAssociation(const Object &s, const ::Uml::AssociationRole &dstRole,
		const Object &d, const ::Uml::AssociationRole &srcRole)
	{
		// TODO: check the type
		StaticObject *src = static_cast<StaticObject*>(s.__impl());
		StaticObject *dst = static_cast<StaticObject*>(d.__impl());
		++src->refCount;
		++dst->refCount;

		src->associations.insert(StaticObject::assoc_type::value_type(dstRole.uniqueId(), dst));
		dst->associations.insert(StaticObject::assoc_type::value_type(srcRole.uniqueId(), src));
	}


	

}


namespace Uml
{
	//----------------------- Static Object Construction 
	//-- These functions will be deprecated,
	//-- and we will use the UDM UML generated API
	//-- to build metamodels at runtime
	//-- however, for the time being ....
	
	class Diagram;


	Diagram CreateDiagram() 
	{
		return Diagram::Cast(UdmStatic::CreateObject(Diagram::meta)); 
	}

	Namespace CreateNamespace() 
	{
		return Namespace::Cast(UdmStatic::CreateObject(Namespace::meta)); 
	}


	Class CreateClass() 
	{
		return Class::Cast(UdmStatic::CreateObject(Class::meta)); 
	}

	Attribute CreateAttribute() 
	{
		return Attribute::Cast(UdmStatic::CreateObject(Attribute::meta));
	}

	Association CreateAssociation() 
	{
		return Association::Cast(UdmStatic::CreateObject(Association::meta)); 
	}

	AssociationRole CreateAssociationRole()
	{ 
		return AssociationRole::Cast(UdmStatic::CreateObject(AssociationRole::meta));
	}

	Composition CreateComposition() 
	{ 
		return Composition::Cast(UdmStatic::CreateObject(Composition::meta)); 
	}

	CompositionParentRole CreateCompositionParentRole()
	{ 
		return CompositionParentRole::Cast(UdmStatic::CreateObject(CompositionParentRole::meta));
	}

	CompositionChildRole CreateCompositionChildRole()
	{
		return CompositionChildRole::Cast(UdmStatic::CreateObject(CompositionChildRole::meta));
	}

	Constraint CreateConstraint()
	{
		return Constraint::Cast(UdmStatic::CreateObject(Constraint::meta));
	}

	ConstraintDefinition CreateConstraintDefinition()
	{
		return ConstraintDefinition::Cast(UdmStatic::CreateObject(ConstraintDefinition::meta));
	}

	void InitDiagram(const Diagram &obj, const char *name, const char * version)
	{
		InitDiagramProps(obj, name, version);
	}

	void InitDiagramProps(const Diagram &obj, const char *name, const char * version)
	{
		obj.name() = name;
		obj.version() = version;
	}

	void InitNamespace(const Namespace &obj, const Diagram &parent, const char *name)
	{
		UdmStatic::CreateComposition(parent, Diagram::meta_namespaces, obj, Namespace::meta_parent);

		InitNamespaceProps(obj, name);
	}

	void InitNamespace(const Namespace &obj, const Namespace &parent, const char *name)
	{
		UdmStatic::CreateComposition(parent, Namespace::meta_namespaces, obj, Namespace::meta_parent_ns);

		InitNamespaceProps(obj, name);
	}

	void InitNamespaceProps(const Namespace &obj, const char *name)
	{
		obj.name() = name;
	}

	void InitClass(const Class &obj, const Namespace &parent, const char *name, bool isAbstract, const char *stereo, const char * from)
	{
		UdmStatic::CreateComposition(parent, Namespace::meta_classes, obj, Class::meta_parent_ns);

		InitClassProps(obj, name, isAbstract, stereo, from);
	}

	void InitClass(const Class &obj, const Diagram &parent, const char *name, bool isAbstract, const char *stereo, const char * from)
	{
		UdmStatic::CreateComposition(parent, Diagram::meta_classes, obj, Class::meta_parent);

		InitClassProps(obj, name, isAbstract, stereo, from);
	}

	void InitClassProps(const Class &obj, const char *name, bool isAbstract, const char *stereo, const char * from)
	{
		obj.name() = name;
		if (stereo) obj.stereotype() = stereo;
		if (from) obj.from() = from;
		obj.isAbstract() = isAbstract;
	}

/*
	void InitAttribute(const Attribute &obj, const Class &parent, const char *name, const char *type, bool nonpersistent, int min, int max)
	{
		UdmStatic::CreateComposition(parent, Class::meta_attributes, obj, Attribute::meta_parent);

		obj.name() = name;
		obj.type() = type;
		obj.min() = min;
		obj.max() = max;
		obj.nonpersistent() = nonpersistent;
	}
*/

	void InitAttribute(const Attribute &obj, const Class &parent, const char *name, const char *type, bool np, bool reg_val, int min, int max, const bool ordered, const string& visibility, const vector<string> & defval)
	{
		UdmStatic::CreateComposition(parent, Class::meta_attributes, obj, Attribute::meta_parent);

		InitAttributeProps(obj, name, type, np, reg_val, min, max, ordered, visibility, defval);
	}

	void InitAttributeProps(const Attribute &obj, const char *name, const char *type, bool np, bool reg_val, int min, int max, const bool ordered, const string& visibility, const vector<string> & defval)
	{
		obj.name() = name;
		obj.type() = type;
		obj.min() = min;
		obj.max() = max;
		obj.nonpersistent() = np;
		obj.registry() = reg_val;
		obj.ordered() = ordered;
		obj.visibility() = visibility;
		obj.defvalue() = defval;
	}
	
	void InitAssociation(const Association &obj, const Namespace &parent, const char *name)
	{
		UdmStatic::CreateComposition(parent, Namespace::meta_associations, obj, Association::meta_parent_ns);

		InitAssociationProps(obj, name);
	}

	void InitAssociation(const Association &obj, const Diagram &parent, const char *name)
	{
		UdmStatic::CreateComposition(parent, Diagram::meta_associations, obj, Association::meta_parent);

		InitAssociationProps(obj, name);
	}
	void InitAssociationProps(const Association &obj, const char *name)
	{
		obj.name() = name;
	}

	void InitAssociationClass(const Association &aobj, const Class &cobj)
	{
		UdmStatic::CreateAssociation(aobj, Association::meta_assocClass, cobj, Class::meta_association);
	}

	void InitAssociationRole(const AssociationRole &obj, const Association &parent, 
		const char *name, bool isnavigable, bool isprimary, long min, long max, const Class &target)
	{
		UdmStatic::CreateComposition(parent, Association::meta_roles, obj, AssociationRole::meta_parent);
		UdmStatic::CreateAssociation(obj, AssociationRole::meta_target, target, Class::meta_associationRoles);

		InitAssociationRoleProps(obj, name, isnavigable, isprimary, min, max);
	}

	void InitAssociationRoleProps(const AssociationRole &obj, const char *name, bool isnavigable, bool isprimary, long min, long max)
	{
		obj.name() = name;
		obj.isNavigable() = isnavigable;
		obj.isPrimary() = isprimary;
		obj.min() = min;
		obj.max() = max;
	}

	void InitComposition(const Composition &obj, const Namespace &parent, const char *name, bool np)
	{
		UdmStatic::CreateComposition(parent, Namespace::meta_compositions, obj, Composition::meta_parent_ns);

		InitCompositionProps(obj, name, np);
	}

	void InitComposition(const Composition &obj, const Diagram &parent, const char *name, bool np)
	{
		UdmStatic::CreateComposition(parent, Diagram::meta_compositions, obj, Composition::meta_parent);

		InitCompositionProps(obj, name, np);
	}

	void InitCompositionProps(const Composition &obj, const char *name, bool np)
	{
		obj.name() = name;
		obj.nonpersistent() = np;
	}

	void InitCompositionParentRole(const CompositionParentRole &obj, const Composition &parent, const char *name, bool isnavigable, const Class &target)
	{
		UdmStatic::CreateComposition(parent, Composition::meta_parentRole, obj, CompositionParentRole::meta_parent);
		UdmStatic::CreateAssociation(obj, CompositionParentRole::meta_target, target, Class::meta_parentRoles);

		InitCompositionParentRoleProps(obj, name, isnavigable);
	}

	void InitCompositionParentRoleProps(const CompositionParentRole &obj, const char *name, bool isnavigable)
	{
		obj.name() = name;
		obj.isNavigable() = isnavigable;
	}

	void InitCompositionChildRole(const CompositionChildRole &obj, const Composition &parent, const char *name, bool isnavigable, long min, long max, const Class &target)
	{
		UdmStatic::CreateComposition(parent, Composition::meta_childRole, obj, CompositionChildRole::meta_parent);
		UdmStatic::CreateAssociation(obj, CompositionChildRole::meta_target, target, Class::meta_childRoles);

		InitCompositionChildRoleProps(obj, name, isnavigable, min, max);
	}

	void InitCompositionChildRoleProps(const CompositionChildRole &obj, const char *name, bool isnavigable, long min, long max)
	{
		obj.name() = name;
		obj.isNavigable() = isnavigable;
		obj.min() = min;
		obj.max() = max;
	}

	void InitConstraint(const Constraint &obj, const Class &parent, const char * name, const char * description, const char * expression)
	{

		UdmStatic::CreateComposition(parent, Class::meta_constraints, obj, Constraint::meta_parent); 

		InitConstraintProps(obj, name, description, expression);
	}

	void InitConstraintProps(const Constraint &obj, const char * name, const char * description, const char * expression)
	{
		obj.name() = name;
		obj.expression() = expression;
		obj.description() = description;
	}

	void InitConstraintDefinition(const ConstraintDefinition &obj, const Class &parent, const char * name, const char * stereotype, const char * parameterList, const char * expression, const char * returnType)
	{
		UdmStatic::CreateComposition(parent, Class::meta_definitions, obj, ConstraintDefinition::meta_parent);

		InitConstraintDefinitionProps(obj, name, stereotype, parameterList, expression, returnType);
	}

	void InitConstraintDefinitionProps(const ConstraintDefinition &obj, const char * name, const char * stereotype, const char * parameterList, const char * expression, const char * returnType)
	{
		obj.name() = name;
		obj.parameterList() = parameterList;
		obj.stereotype() = stereotype;
		obj.expression() = expression;
		obj.returnType() = returnType;
	}
	
	void AddInheritance(const Class &baseType, const Class &subType)
	{
		UdmStatic::CreateAssociation(baseType, Class::meta_subTypes, subType, Class::meta_baseTypes);
	}

	UDM_DLL void SetDiagram(Diagram &what, const Diagram &target)
	{
		what = target;
	};

	UDM_DLL void SetClass(Class &what, const Namespace &what_ns, const char *target_name)
	{
		what = classByName(what_ns, target_name);
	};

	UDM_DLL void SetClass(Class &what, const Diagram &what_dgr, const char *target_name)
	{
		what = classByName(what_dgr, target_name);
	};

	UDM_DLL void SetNamespace(Namespace &what, const Diagram &what_dgr, const char *target_name)
	{
		what = namespaceByName(what_dgr, target_name);
	};

	UDM_DLL void SetNamespace(Namespace &what, const Namespace &what_ns, const char *target_name)
	{
		what = namespaceByName(what_ns, target_name);
	};

	UDM_DLL void SetAttribute(Attribute &what, Class &what_class,  const char *target_name)
	{
		set<Attribute> atts = what_class.attributes();
		bool found = false;
		for(set<Attribute>::iterator i = atts.begin(); i != atts.end(); i++)
		{
			if (!strcmp(target_name, ((string)i->name()).c_str()))
			{
				if (found) 
					throw udm_exception(string("Duplicate attribute name: ") + string(target_name) + string(" found in class: ") + (string)what_class.name() );
				found = true;
				what = *i;
			};
		};

		if (!found)
			throw udm_exception(string("No attribute named ") + string(target_name) + string(" found in class: ") + (string)what_class.name() );
	};

	UDM_DLL void SetConstraint(Constraint &what, Class &what_class,  const char *target_name)
	{
		set<Constraint> cnstrs = what_class.constraints();
		bool found = false;
		for(set<Constraint>::iterator i = cnstrs.begin(); i != cnstrs.end(); i++)
		{
			if (!strcmp(target_name, ((string)i->name()).c_str()))
			{
				if (found) 
					throw udm_exception(string("Duplicate constraint name: ") + string(target_name) + string(" found in class: ") + (string)what_class.name() );
				found = true;
				what = *i;
			};
		};

		if (!found)
			throw udm_exception(string("No constraint named ") + string(target_name) + string(" found in class: ") + (string)what_class.name() );
	}

	UDM_DLL void SetConstraintDefinition(ConstraintDefinition &what, Class &what_class,  const char *target_name)
	{
		set<ConstraintDefinition> cdefs = what_class.definitions();
		bool found = false;
		for(set<ConstraintDefinition>::iterator i = cdefs.begin(); i != cdefs.end(); i++)
		{
			if (!strcmp(target_name, ((string)i->name()).c_str()))
			{
				if (found) 
					throw udm_exception(string("Duplicate constraint definition name: ") + string(target_name) + string(" found in class: ") + (string)what_class.name() );
				found = true;
				what = *i;
			};
		};

		if (!found)
			throw udm_exception(string("No constraint definition named ") + string(target_name) + string(" found in class: ") + (string)what_class.name() );
	}

	

	UDM_DLL void SetChildRole(CompositionChildRole &what, Class &what_target_class, Class &what_theo_target_class, const char *target_name, const char * orole_name)
	{
		bool found = false;
		set<CompositionChildRole> ccrs = what_theo_target_class.childRoles();

		for ( set<CompositionChildRole>::iterator ccrs_i = ccrs.begin(); ccrs_i != ccrs.end(); ccrs_i++)
		{
			CompositionParentRole cpr = theOther(*ccrs_i);

			if ((Class)(cpr.target()) == what_target_class)
			{
				bool role_name_match = (target_name == NULL || strlen(target_name) == 0)
					? true
					: !strcmp(target_name, ((string)cpr.name()).c_str());
				bool orole_name_match = (target_name == NULL || strlen(orole_name) == 0)
					? true
					: !strcmp(orole_name, ((string)ccrs_i->name()).c_str());

				if ( role_name_match && orole_name_match )
				{
					if (found)
						throw udm_exception(string("Duplicate role name: ") + string(target_name) + string(" for class: ") + (string)what_target_class.name() );
					found = true;
					what = *ccrs_i;
				};
			};
		};
		if (!found)
			throw udm_exception(string("no role name: ") + string(target_name) + string(" found for class: ") + (string)what_target_class.name() );

	};


	UDM_DLL void SetParentRole(CompositionParentRole &what, Class &what_target_class, Class &what_theo_target_class, const char *target_name, const char * orole_name)
	{
		bool found = false;
		set<CompositionParentRole> cprs = what_theo_target_class.parentRoles();

		for ( set<CompositionParentRole>::iterator cprs_i = cprs.begin(); cprs_i != cprs.end(); cprs_i++)
		{
			CompositionChildRole ccr = theOther(*cprs_i);

			if ((Class)(ccr.target()) == what_target_class)
			{
				bool role_name_match = (target_name == NULL || strlen(target_name) == 0)
					? true
					: !strcmp(target_name, ((string)ccr.name()).c_str());
				bool orole_name_match = (target_name == NULL || strlen(orole_name) == 0)
					? true
					: !strcmp(orole_name, ((string)cprs_i->name()).c_str());

				if ( role_name_match && orole_name_match )
				{
					if (found)
						throw udm_exception(string("Duplicate role name: ") + string(target_name) + string(" for class: ") + (string)what_target_class.name() );
					found = true;
					what = *cprs_i;
				};
			};
		};
		if (!found)
			throw udm_exception(string("no role name: ") + string(target_name) + string(" found for class: ") + (string)what_target_class.name() );

	};

	UDM_DLL void SetAssocRole(AssociationRole &what, Class &what_target_class, Class &what_theo_target_class, const char *target_name)
	{
		bool found = false;
		set<AssociationRole> ars = what_theo_target_class.associationRoles();

		for ( set<AssociationRole>::iterator ars_i = ars.begin(); ars_i != ars.end(); ars_i++)
		{
			AssociationRole ar = theOther(*ars_i);

			if ((Class)(ar.target()) == what_target_class)
			{
				bool role_name_match = (target_name == NULL || strlen(target_name) == 0)
					? true
					: !strcmp(target_name, ((string)ar.name()).c_str());

				if (role_name_match)
				{
					if (found)
						throw udm_exception(string("Duplicate role name: ") + string(target_name) + string(" for class: ") + (string)what_target_class.name() );
					found = true;
					what = *ars_i;
				};
			};
		};
		if (!found)
			throw udm_exception(string("no role name: ") + string(target_name) + string(" found for class: ") + (string)what_target_class.name() );

	};

}

