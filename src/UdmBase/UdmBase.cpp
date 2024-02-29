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
	03/22/06	-	Feng

		Add the code for set the attribute "name" with the object class name.

	12/08/04	-	endre

		Small bugfix: CopyAttributes fucntion could not handle "Text" Attributes.

	12/06/04	-	endre

		New function 
			set<Object> DataNetwork::GetAllInstancesOf(const ::Uml::Class& meta)
		added, which retrives all instances of an Class in a DataNetwork


	11/22/04	-	endre

		VC7++ .NET porting issues

	10/24/04	-	endre

		slight modification because of introducing a new type "Text", which is the
		same as "String", but it gets persisted differently in DOM backend.
	  
	09/03/04	-	endre
				
		fixed small bug in getIntegerAttrArr function
		which converted the last item in the array to long instead of __int64

	04/26/04	-	endre
		
		changed the DataNetwork assignment operator to first remove 
		existing objects in destination datanetwork

	02/10/04	-	endre
	
		added the generic container() function to class Udm::Object

	01/26/04	-	endre
		
		  Mainly porting issues to g++ 2.96, specially the compress/decompress stuff.
			void type had to be add to all the functions declared with no return type
  
	06/19/02	-	endre

			Added array attribute setter/getter implementations,
	  which basically are wrapper functions which use
	  get/set StringAttr.

		Array representation:
		integer, real :	values delimited by ';'
		bool:			sequence of 'true' or 'false', delimited by ';', case insensitive
		
		string:			strings are delimited by ';'. 
						';'s in strings are escaped with '\'
						'\'s in strings are escaped with '\'
	  

		DOM:	works OK
		MGA:	works OK, but since MGA lacks support
				for attribute arrays, these array 
				attributes must be declared in MGA
				paradigm as string attributes
		MEM:	it would work OK, but in MEM
				back-end these functions are overriden,
				so they are more performant.
				MEM backend provides real support for 
				attribute arrays

		This approach has no problems with 
	  DOM backend, it also works with MGA backend, but in 
	  the MGA paradigm the attribute must be declared as a 
	  string attribute. It would also work for MEM backend,
	  but in MEM backend 

*/

#include "UdmBase.h"
#include "UmlExt.h"
#include <UdmUtil.h>
#include "UdmStatic.h"
#include <algorithm>
#include <string>
#include <cstdlib>
#include <cstring>
#include <sstream>

#ifndef _WIN32
long long _atoi64(const char *x)  { long long i; sscanf(x,"%lld",&i);return i;};
#endif



//unix - windows portability issues
//1. _stricmp, _strnicmp on WIN32 are strcasecmp, strncasecmp on UNIX

#ifndef WIN32
#define _stricmp(a, b) strcasecmp(a,b)
#define _strnicmp(a, b, n) strncasecmp(a,b,n)
#endif

namespace UDM_NAMESPACE
{

	const ::Uml::CompositionChildRole NULLCHILDROLE;
	const ::Uml::CompositionParentRole NULLPARENTROLE;

	
	UDM_DLL ObjectImpl *Object::__Cast(const Object &a, const ::Uml::Class &meta)
	{
		if(a && !Uml::IsDerivedFrom(a.type(), meta) )
			throw udm_exception("Invalid cast from type '" + UdmUtil::ExtractName(a.type()) + "' to type '" + UdmUtil::ExtractName(meta) + "'");

		return a.impl->clone();
	}

#ifdef UDM_RVALUE
	UDM_DLL ObjectImpl *Object::__Cast(Object &&a, const ::Uml::Class &meta)
	{
		if(a && !Uml::IsDerivedFrom(a.type(), meta) )
			throw udm_exception("Invalid cast from type '" + UdmUtil::ExtractName(a.type()) + "' to type '" + UdmUtil::ExtractName(meta) + "'");

		ObjectImpl *ret = a.impl;
		a.impl = 0;
		return ret;
	}
#endif

	UDM_DLL ObjectImpl *Object::__Create(const ::Uml::Class &meta, const Object &parent, 
		const ::Uml::CompositionChildRole &role, const ObjectImpl * archetype, const bool subtype )
	{
		if (archetype && archetype != &_null)
		{
			if (archetype->type() != meta )
				throw udm_exception("Type of archetype and type must be the same!");
			
			if (parent.impl->__getdn()->IsTypeSafe())
				return parent.impl->createChild(role, meta, archetype, subtype);
			else
				return parent.impl->createChild(role, Uml::SafeTypeContainer::GetSafeType(meta), archetype, subtype);
		}
		else
			if (parent.impl->__getdn()->IsTypeSafe())
				return parent.impl->createChild(role, meta);
			else
				return parent.impl->createChild(role, Uml::SafeTypeContainer::GetSafeType(meta));
	}

	UDM_DLL Object Object::archetype() const
	{
		return impl->getArchetype();
	};

	UDM_DLL set<Object> Object::derived() const
	{
		set<Object> ret;
		vector<ObjectImpl*> der_vect = impl->getDerived();
		vector<ObjectImpl*>::iterator dv_i  = der_vect.begin();
		while (dv_i != der_vect.end())
			ret.insert(*dv_i++);
		return ret;
	};

	UDM_DLL set<Object> Object::instances() const
	{
		set<Object> ret;
		vector<ObjectImpl*> der_vect = impl->getInstances();
		vector<ObjectImpl*>::iterator dv_i  = der_vect.begin();
		while (dv_i != der_vect.end())
			ret.insert(*dv_i++);
		return ret;
	};

	UDM_DLL bool Object::hasRealArchetype() const
	{
		return impl->hasRealArchetype();
	};
	UDM_DLL bool Object::isInstance() const
	{
		return impl->isInstance();
	};
	UDM_DLL bool Object::isSubtype() const
	{
		return impl->isSubtype();
	};
	
	UDM_DLL Object Object::container() const
	{
		return impl->getParent(NULL);
	};

	UDM_DLL bool Object::isLibObject() const
	{
		return impl->isLibObject();
	}

	UDM_DLL bool Object::isLibRoot() const
	{
		return impl->isLibRoot();
	}

	UDM_DLL bool Object::getLibraryName(string &name) const
	{
		return impl->getLibraryName(name);
	}

	UDM_DLL void Object::setLibraryName(const char *name)
	{
		impl->setLibraryName(name);
	}

	UDM_DLL Object Object::AttachLibrary(const Object &lib_src, const string &lib_name, t_lib_to_copy_map *copy_map)
	{
		ObjectImpl *lib_root;
		if (!copy_map) {
			lib_root = impl->AttachLibrary(lib_src.__impl(), lib_name);
		}
		else {
			t_lib_to_copy_impl_map impl_map;
			lib_root = impl->AttachLibrary(lib_src.__impl(), lib_name, &impl_map);
			for (t_lib_to_copy_impl_map::const_iterator i = impl_map.begin(); i != impl_map.end(); i++)
				copy_map->insert(make_pair(Object(i->first), Object(i->second)));
		}

		return Object(lib_root);
	}

	UDM_DLL string Object::toString() const
	{
		if (impl != &Udm::_null)
			return impl->toString();

		return "NULL impl";
	}


	udm_exception NullObject::e("Object handle is null");

	UDM_DLL bool IsDerivedFrom(const ::Uml::Class &derived, const ::Uml::Class &base) {
		return Uml::IsDerivedFrom(derived, base);
	}

	UDM_DLL NullObject _null;
	UDM_DLL Object null(&_null);

	//static member var initialization
	unsigned long DataNetwork::dn_id_gen = 0;
	UdmStaticData _UdmStaticData;
	UdmStaticData::~UdmStaticData() {
		UdmStatic::StaticObject::Cleanup();
	}

	UDM_DLL DataNetwork::DataNetwork(const Udm::UdmDiagram &metainfo, UdmProject* project) : metaroot(metainfo), pr(project) 
	{ 
		metainfo.init();
		dn_id = dn_id_gen++;
		pair <const unsigned long, DataNetwork *> ins_item(dn_id, this);
		pair <map <const unsigned long, DataNetwork *>::iterator, bool> ins_res = _UdmStaticData.dntab.insert(ins_item);
		if (!ins_res.second)
			throw udm_exception("Can not register data network!");

		_static_pr = false;//it is either not part of a project or part of a non-static project

	}

	UDM_DLL DataNetwork *DataNetwork::GetDnById(const unsigned long id)
	{
		map <const unsigned long, DataNetwork *>::iterator dntab_i = _UdmStaticData.dntab.find(id);
		if (dntab_i != _UdmStaticData.dntab.end()) return dntab_i->second;
		return NULL;

	}

	UDM_DLL DataNetwork & DataNetwork::operator=(DataNetwork &a)
	{
		UdmUtil::copy_assoc_map dummy;
		vector<ObjectImpl*> vec = rootobject.__impl()->getChildren(NULL, NULL);
		for (vector<ObjectImpl*>::iterator vec_i = vec.begin(); vec_i != vec.end(); vec_i++)
		{
			(*vec_i)->detach();		//detach the object
			(*vec_i)->release();	//getChildren() cloned() them 
		};

		UdmUtil::CopyObjectHierarchy(a.GetRootObject().__impl(), rootobject.__impl(), this, dummy);
		return *this;
	}

	UDM_DLL void DataNetwork::setStaticUdmProject(StaticUdmProject* _pr)
	{
		pr = _pr; 
		_static_pr = true;
	};


	void recurseGetAllInstancesOf(const ::Uml::Class &meta, const Object &parent, set<Object> &ret)
	{
		vector<Udm::ObjectImpl*> a = parent.__impl()->getChildren(NULL, NULL);
		vector<Udm::ObjectImpl*>::iterator i = a.begin();
		while( i != a.end() )
		{
			Udm::Object obj(*i);
			if (::Uml::IsDerivedFrom(obj.type(), meta))
				ret.insert(obj);
			recurseGetAllInstancesOf(meta, obj, ret);
			++i;
		}
	}

	/*
		Returns all instances of a certain class in a Data Network
	*/
	UDM_DLL set<Object> DataNetwork::GetAllInstancesOf(const ::Uml::Class& meta)
	{
		set<Object> ret;
		const ::Uml::Class root_meta = rootobject.type();
		vector<Uml::ChildRoleChain> chains;
		if (Uml::GetChildRoleChain(root_meta, meta, chains))
		{
			while (chains.size())
			{
				Uml::ChildRoleChain chain = *(chains.rbegin());

				chains.pop_back();

				set<Object> ret_set = rootobject.getChildrenByChildRoleChain(meta, chain);
				if (ret_set.size()) ret.insert(ret_set.begin(), ret_set.end());
			};
		}
		else
		{
			if (::Uml::IsDerivedFrom(rootobject.type(), meta))
				ret.insert(rootobject);

			recurseGetAllInstancesOf(meta, rootobject, ret);
		}
		return ret;
	};

	UDM_DLL DataNetwork::~DataNetwork()
	{
		_UdmStaticData.dntab.erase(dn_id);

		if (rootobject) throw udm_exception("Dirty abort!");
	}

	UDM_DLL bool DataNetwork::IsTypeSafe()
	{
		return true;
	}

	UDM_DLL void DataNetwork::CreateNewToString(const string &metalocator, const ::Uml::Class &rootclass, 
									enum Udm::BackendSemantics sem)
	{
		throw udm_exception("Unsupported method");
	}

	UDM_DLL void DataNetwork::OpenExistingFromString(string &str, 
									const string &metalocator, 
									enum Udm::BackendSemantics sem)
	{
		throw udm_exception("Unsupported method");
	}

	UDM_DLL const string & DataNetwork::Str()
	{
		throw udm_exception("Unsupported method");
	}

	UDM_DLL struct AssociationRoleInfo DataNetwork::GetAssociationRoleInfo(const ::Uml::AssociationRole &role) const
	{
		struct AssociationRoleInfo ret;
		ret.has_direction = ret.is_src = false;

		return ret;
	}


	//	--- attribute setters/getters for multiple value (array) type attributes

	/*
		
		Basic idea behind array'ing with DOM & MGA: 
		===========================================
		getters: get the attribute as a single value string;
				 then tokenize by ';', and try to parse back
				 return elements in a vector(order kept)

		setters: compose a string from the input vector,
				 by enumeration, delimit them with character ';' 
				 set the attribute to DOM as a single value string attribute

		MGA note:
				MGA paradigm version must declare the array attributes
				as string attributes for this to work
	*/


	UDM_DLL vector<string> ObjectImpl::getStringAttrArr(const ::Uml::Attribute &meta) const
	{
		/*
		Special characters:

			; - array field delimiter, unless escaped
			\ - escape character, unless escaped 
		*/

		return UdmUtil::string_to_vector(getStringAttr(meta), ';');

	};

	UDM_DLL void ObjectImpl::setStringAttrArr(const ::Uml::Attribute &meta, const vector<string> &a, const bool direct)
	{
		/*
		Special characters:
			- ';' -s and '\'-s which really occur in strings in a are escaped with '\'
			- fields are delimited with ';'
		*/

		//use setStringAttr w/ the constructed string
		setStringAttr(meta, UdmUtil::vector_to_string(a, ';', true), direct);

	};

	UDM_DLL vector<bool> ObjectImpl::getBooleanAttrArr(const ::Uml::Attribute &meta) const
	{
		vector<bool> ret;
		string sc_delimited = getStringAttr(meta);
		if (!sc_delimited.size()) return ret;

		vector<string> vals = UdmUtil::string_to_vector(sc_delimited, ';');
		ret.reserve(vals.size());

		for (vector<string>::const_iterator i = vals.begin(); i != vals.end(); i++)
		{
			if (!_strnicmp(i->c_str(), "true", 4))
				ret.push_back(true);
			else if (!_strnicmp(i->c_str(), "false", 5))
				ret.push_back(false);
			else
				throw udm_exception(string("Parsing of bool-array failed: ") + sc_delimited);
		}

		return ret;

	};

	UDM_DLL void ObjectImpl::setBooleanAttrArr(const ::Uml::Attribute &meta, const vector<bool> &a, const bool direct)
	{
		vector<string> vals;
		vals.reserve(a.size());

		vector<bool>::const_iterator a_ci = a.begin();

		while (a_ci != a.end())
		{
			if (*a_ci)
				vals.push_back("true");				//literal true
			else
				vals.push_back("false");			//literal false
			a_ci++;									//go on 
		}
		
		setStringAttr(meta, UdmUtil::vector_to_string(vals, ';'), direct);

	};

	UDM_DLL vector<__int64> ObjectImpl::getIntegerAttrArr(const ::Uml::Attribute &meta) const
	{
		vector<__int64> ret;
		vector<string> vals = UdmUtil::string_to_vector(getStringAttr(meta), ';');

		ret.reserve(vals.size());

		for (vector<string>::const_iterator i = vals.begin(); i != vals.end(); i++)
		{
			ret.push_back(_atoi64(i->c_str()));
		}

		return ret;

	};

	UDM_DLL void ObjectImpl::setIntegerAttrArr(const ::Uml::Attribute &meta, const vector<__int64> &a, const bool direct)
	{
		vector<string> vals;
		vals.reserve(a.size());

		vector<__int64>::const_iterator a_ci = a.begin();
		
		char * lit_integer = new char[20];			//32-bit integers in decimal representation
													//can go up to max 9 characters, +1 (sign)
													//so 20 is safe enough
			
		while (a_ci != a.end())
		{

#ifdef _WIN32
			sprintf(lit_integer, "%I64d", *a_ci);		
#else
			sprintf(lit_integer, "%lld", *a_ci);		
#endif

			vals.push_back(lit_integer);
			
			a_ci++;									//go on 
		}
		
		delete [] lit_integer;

		setStringAttr(meta, UdmUtil::vector_to_string(vals, ';'), direct);

	};

	UDM_DLL vector<double> ObjectImpl::getRealAttrArr(const ::Uml::Attribute &meta) const
	{
		vector<double> ret;
		string sc_delimited = getStringAttr(meta);
		if (!sc_delimited.size()) return ret;

		vector<string> vals = UdmUtil::string_to_vector(sc_delimited, ';');
		ret.reserve(vals.size());

		double d;
		for (vector<string>::const_iterator i = vals.begin(); i != vals.end(); i++)
		{
			if (!UdmUtil::stringToDouble(i->c_str(), d))
				throw udm_exception("Attr is of non-float format: '" + sc_delimited + "'");
			ret.push_back(d);
		}

		return ret;
	};

	UDM_DLL void ObjectImpl::setRealAttrArr(const ::Uml::Attribute &meta, const vector<double> &a, const bool direct)
	{
		vector<string> vals;
		vals.reserve(a.size());

		vector<double>::const_iterator a_ci = a.begin();

		while (a_ci != a.end())
		{
			vals.push_back(UdmUtil::doubleToString(*a_ci));

			a_ci++;
		}

		//set the attribute as a string attribute
		setStringAttr(meta, UdmUtil::vector_to_string(vals, ';'), direct);

	};

	
	UDM_DLL tmap &ObjectImpl::locatemap(bool create) const
	{
		static tmap emptymap;
		DataNetwork *dn = const_cast<ObjectImpl*>(this)->__getdn();
		uniqueId_type u = uniqueId();
		ttmap::iterator found = dn->tempvarmap.find(u);
		if(found == dn->tempvarmap.end()) 
		{
			if(create) 
				found = dn->tempvarmap.insert(ttmap::value_type(u,emptymap)).first;
			else 
				return emptymap;
		}
		return found->second;
	}

	//non-persistent , single-value attributes 
///
	UDM_DLL string ObjectImpl::getTempStringAttr(const ::Uml::Attribute &meta) const 
	{
			tmap &tempvarmap = locatemap(false);
			string ret;
			map<string, udmvariant>::iterator f = tempvarmap.find(string(meta.name()) + "@" + string(::Uml::Class(meta.parent()).name()) + "@string");
			if(f != tempvarmap.end()) ret = f->second.strval;
			return ret;
	}

	UDM_DLL void ObjectImpl::setTempStringAttr(const ::Uml::Attribute &meta, const string &a, const bool direct) 
	{
			tmap &tempvarmap = locatemap(true);
			string sig = string(meta.name()) + "@" + string(::Uml::Class(meta.parent()).name()) + "@string";
			

			ObjectImpl * archetype = getArchetype();
			if (archetype && (archetype != (ObjectImpl*)&Udm::_null) )
			{
				string desynch_sig = sig + "_is_desynched";
				if (direct)
				{
					//set the attribute
					tmap::iterator f = tempvarmap.find(sig);
					if(f != tempvarmap.end()) f->second.strval = a;
					else tempvarmap.insert(tmap::value_type(sig, a)); 

					//desynch from archetype
					tmap::iterator d = tempvarmap.find(desynch_sig);
					if (d != tempvarmap.end()) d->second.boolval = true;
					else tempvarmap.insert(tmap::value_type(desynch_sig, true)); 

				}
				else
				{
					//check if desynched
					tmap::iterator d = tempvarmap.find(desynch_sig);
					if  ( (d == tempvarmap.end()) || !(d->second.boolval))
					{
						//set the attribute
						tmap::iterator f = tempvarmap.find(sig);
						if(f != tempvarmap.end()) f->second.strval = a;
						else tempvarmap.insert(tmap::value_type(sig, a)); 
					}
				}
				archetype->release();
			}
			else
			{
				//set the attrbute
				tmap::iterator f = tempvarmap.find(sig);
				if(f != tempvarmap.end()) f->second.strval = a;
				else tempvarmap.insert(tmap::value_type(sig, a)); 
			}

			//go through all derived and instances
			vector<ObjectImpl*>::iterator i;
			vector<ObjectImpl*> deriveds = getDerived();
			for (i = deriveds.begin(); i != deriveds.end(); i++)
			{
				ObjectImpl* derived = *i;
				derived->setTempStringAttr(meta, a, false);
				derived->release();
			}

			vector<ObjectImpl*> instances = getInstances();
			for (i = instances.begin(); i != instances.end(); i++)
			{
				ObjectImpl* instance = *i;
				instance->setTempStringAttr(meta, a, false);
				instance->release();
			}
	}

///
	UDM_DLL __int64 ObjectImpl::getTempIntegerAttr(const ::Uml::Attribute &meta) const
	{
			tmap &tempvarmap = locatemap(false);
			__int64 ret=0;
			map<string, udmvariant>::iterator f = tempvarmap.find(string(meta.name()) + "@" + string(::Uml::Class(meta.parent()).name()) + "@long");
			if(f != tempvarmap.end()) ret = f->second.longval;
			return ret;
	}

	UDM_DLL void ObjectImpl::setTempIntegerAttr(const ::Uml::Attribute &meta, const __int64 &a, const bool direct) 
	{
			tmap &tempvarmap = locatemap(true);
			string sig = string(meta.name()) + "@" + string(::Uml::Class(meta.parent()).name()) + "@long";
			
			ObjectImpl * archetype = getArchetype();
			if (archetype && (archetype != (ObjectImpl*)&Udm::_null) )
			{
				string desynch_sig = sig + "_is_desynched";
				if (direct)
				{
					//set the attribute
					tmap::iterator f = tempvarmap.find(sig);
					if(f != tempvarmap.end()) f->second.longval = a;
					else tempvarmap.insert(tmap::value_type(sig, a)); 
			
					//desynch from archetype
					tmap::iterator d = tempvarmap.find(desynch_sig);
					if (d != tempvarmap.end()) d->second.boolval = true;
					else tempvarmap.insert(tmap::value_type(desynch_sig, true)); 

				}
				else
				{
					//check if desynched
					tmap::iterator d = tempvarmap.find(desynch_sig);
					if  ( (d == tempvarmap.end()) || !(d->second.boolval))
					{
						//set the attribute
						tmap::iterator f = tempvarmap.find(sig);
						if(f != tempvarmap.end()) f->second.longval = a;
						else tempvarmap.insert(tmap::value_type(sig, a)); 
						}
				}
				archetype->release();
			}
			else
			{
				//set the attrbute
				tmap::iterator f = tempvarmap.find(sig);
				if(f != tempvarmap.end()) f->second.longval = a;
				else tempvarmap.insert(tmap::value_type(sig, a)); 
			}

			//go through all derived and instances
			vector<ObjectImpl*>::iterator i;
			vector<ObjectImpl*> deriveds = getDerived();
			for (i = deriveds.begin(); i != deriveds.end(); i++)
			{
				ObjectImpl* derived = *i;
				derived->setTempIntegerAttr(meta, a, false);
				derived->release();
			}

			vector<ObjectImpl*> instances = getInstances();
			for (i = instances.begin(); i != instances.end(); i++)
			{
				ObjectImpl* instance = *i;
				instance->setTempIntegerAttr(meta, a, false);
				instance->release();
			}

			/*
			tmap::iterator f = tempvarmap.find(sig);
			if(f != tempvarmap.end()) f->second.longval = a;
			else tempvarmap.insert(tmap::value_type(sig, a)); 
			*/
	}

///
	UDM_DLL double ObjectImpl::getTempRealAttr(const ::Uml::Attribute &meta) const 
	{
			tmap &tempvarmap = locatemap(false);
			double ret=0;
			map<string, udmvariant>::iterator f = tempvarmap.find(string(meta.name()) + "@" + string(::Uml::Class(meta.parent()).name()) + "@double");
			if(f != tempvarmap.end()) ret = f->second.doubleval;
			return ret;
	}

	UDM_DLL void ObjectImpl::setTempRealAttr(const ::Uml::Attribute &meta, const double &a, const bool direct) 
	{
			tmap &tempvarmap = locatemap(true);
			string sig = string(meta.name()) + "@" + string(::Uml::Class(meta.parent()).name()) + "@double";



			ObjectImpl * archetype = getArchetype();
			if (archetype && (archetype != (ObjectImpl*)&Udm::_null) )
			{
				string desynch_sig = sig + "_is_desynched";
				if (direct)
				{
					//set the attribute
					tmap::iterator f = tempvarmap.find(sig);
					if(f != tempvarmap.end()) f->second.doubleval = a;
					else tempvarmap.insert(tmap::value_type(sig, a)); 
			
					//desynch from archetype
					tmap::iterator d = tempvarmap.find(desynch_sig);
					if (d != tempvarmap.end()) d->second.boolval = true;
					else tempvarmap.insert(tmap::value_type(desynch_sig, true)); 

				}
				else
				{
					//check if desynched
					tmap::iterator d = tempvarmap.find(desynch_sig);
					if  ( (d == tempvarmap.end()) || !(d->second.boolval))
					{
						//set the attribute
						tmap::iterator f = tempvarmap.find(sig);
						if(f != tempvarmap.end()) f->second.doubleval = a;
						else tempvarmap.insert(tmap::value_type(sig, a)); 
						}
				}
				archetype->release();
			}
			else
			{
				//set the attrbute
				tmap::iterator f = tempvarmap.find(sig);
				if(f != tempvarmap.end()) f->second.doubleval = a;
				else tempvarmap.insert(tmap::value_type(sig, a)); 
			}

			//go through all derived and instances
			vector<ObjectImpl*>::iterator i;
			vector<ObjectImpl*> deriveds = getDerived();
			for (i = deriveds.begin(); i != deriveds.end(); i++)
			{
				ObjectImpl* derived = *i;
				derived->setTempRealAttr(meta, a, false);
				derived->release();
			}

			vector<ObjectImpl*> instances = getInstances();
			for (i = instances.begin(); i != instances.end(); i++)
			{
				ObjectImpl* instance = *i;
				instance->setTempRealAttr(meta, a, false);
				instance->release();
			}

			/*
			tmap::iterator f = tempvarmap.find(sig);
			if(f != tempvarmap.end()) f->second.doubleval = a;
			else tempvarmap.insert(tmap::value_type(sig, a)); 
			*/
	}

///
	UDM_DLL bool ObjectImpl::getTempBooleanAttr(const ::Uml::Attribute &meta) const
	{
			tmap &tempvarmap = locatemap(false);
			bool ret=false;
			map<string, udmvariant>::iterator f = tempvarmap.find(string(meta.name()) + "@" + string(::Uml::Class(meta.parent()).name()) + "@bool");
			if(f != tempvarmap.end()) ret = f->second.boolval;
			return ret;
	}

	UDM_DLL void ObjectImpl::setTempBooleanAttr(const ::Uml::Attribute &meta, const bool &a, const bool direct) 
	{
			tmap &tempvarmap = locatemap(true);
			string sig = string(meta.name()) + "@" + string(::Uml::Class(meta.parent()).name()) + "@bool";
	
			ObjectImpl * archetype = getArchetype();
			if (archetype && (archetype != (ObjectImpl*)&Udm::_null) )
			{
				string desynch_sig = sig + "_is_desynched";
				if (direct)
				{
					//set the attribute
					tmap::iterator f = tempvarmap.find(sig);
					if(f != tempvarmap.end()) f->second.boolval = a;
					else tempvarmap.insert(tmap::value_type(sig, a)); 
					
					//desynch from archetype
					tmap::iterator d = tempvarmap.find(desynch_sig);
					if (d != tempvarmap.end()) d->second.boolval = true;
					else tempvarmap.insert(tmap::value_type(desynch_sig, true)); 

				}
				else
				{
					//check if desynched
					tmap::iterator d = tempvarmap.find(desynch_sig);
					if  ( (d == tempvarmap.end()) || !(d->second.boolval))
					{
						//set the attribute
						tmap::iterator f = tempvarmap.find(sig);
						if(f != tempvarmap.end()) f->second.boolval = a;
						else tempvarmap.insert(tmap::value_type(sig, a)); 
				}
				}
				archetype->release();
			}
			else
			{
				//set the attrbute
				tmap::iterator f = tempvarmap.find(sig);
				if(f != tempvarmap.end()) f->second.boolval = a;
				else tempvarmap.insert(tmap::value_type(sig, a)); 
			
			}

			//go through all derived and instances
			vector<ObjectImpl*>::iterator i;
			vector<ObjectImpl*> deriveds = getDerived();
			for (i = deriveds.begin(); i != deriveds.end(); i++)
			{
				ObjectImpl* derived = *i;
				derived->setTempBooleanAttr(meta, a, false);
				derived->release();
			}

			vector<ObjectImpl*> instances = getInstances();
			for (i = instances.begin(); i != instances.end(); i++)
			{
				ObjectImpl* instance = *i;
				instance->setTempBooleanAttr(meta, a, false);
				instance->release();
			}

			/*
			tmap::iterator f = tempvarmap.find(sig);
			if(f != tempvarmap.end()) f->second.boolval = a;
			else tempvarmap.insert(tmap::value_type(sig, a)); 
			*/
	}

	//non-persistent , array attributes
///
	UDM_DLL vector<string> ObjectImpl::getTempStringAttrArr(const ::Uml::Attribute &meta) const
	{
			tmap &tempvarmap = locatemap(false);
			vector<string> ret;
			map<string, udmvariant>::iterator f = tempvarmap.find(string(meta.name()) + "@" + string(::Uml::Class(meta.parent()).name()) + "@string_array");
			if(f != tempvarmap.end()) ret = f->second.strarrval;
			return ret;
	}

	UDM_DLL void ObjectImpl::setTempStringAttrArr(const ::Uml::Attribute &meta, const vector<string> &a, const bool direct) 
	{
			tmap &tempvarmap = locatemap(true);
			string sig = string(meta.name()) + "@" + string(::Uml::Class(meta.parent()).name()) + "@string_array";

			ObjectImpl * archetype = getArchetype();
			if (archetype && (archetype != (ObjectImpl*)&Udm::_null) )
			{
				string desynch_sig = sig + "_is_desynched";
				if (direct)
				{
					//set the attribute
					tmap::iterator f = tempvarmap.find(sig);
					if(f != tempvarmap.end()) f->second.strarrval = a;
					else tempvarmap.insert(tmap::value_type(sig, a)); 
			
					//desynch from archetype
					tmap::iterator d = tempvarmap.find(desynch_sig);
					if (d != tempvarmap.end()) d->second.boolval = true;
					else tempvarmap.insert(tmap::value_type(desynch_sig, true)); 

				}
				else
				{
					//check if desynched
					tmap::iterator d = tempvarmap.find(desynch_sig);
					if  ( (d == tempvarmap.end()) || !(d->second.boolval))
					{
						//set the attribute
						tmap::iterator f = tempvarmap.find(sig);
						if(f != tempvarmap.end()) f->second.strarrval = a;
						else tempvarmap.insert(tmap::value_type(sig, a)); 
						}
				}
				archetype->release();
			}
			else
			{
				//set the attrbute
				tmap::iterator f = tempvarmap.find(sig);
				if(f != tempvarmap.end()) f->second.strarrval = a;
				else tempvarmap.insert(tmap::value_type(sig, a)); 
			}

			//go through all derived and instances
			vector<ObjectImpl*>::iterator i;
			vector<ObjectImpl*> deriveds = getDerived();
			for (i = deriveds.begin(); i != deriveds.end(); i++)
			{
				ObjectImpl* derived = *i;
				derived->setTempStringAttrArr(meta, a, false);
				derived->release();
			}

			vector<ObjectImpl*> instances = getInstances();
			for (i = instances.begin(); i != instances.end(); i++)
			{
				ObjectImpl* instance = *i;
				instance->setTempStringAttrArr(meta, a, false);
				instance->release();
			}

			/*
			tmap::iterator f = tempvarmap.find(sig);
			if(f != tempvarmap.end()) f->second.strarrval = a;
			else tempvarmap.insert(tmap::value_type(sig, a)); 
			*/
	}

///
	UDM_DLL vector<__int64> ObjectImpl::getTempIntegerAttrArr(const ::Uml::Attribute &meta) const
	{
			tmap &tempvarmap = locatemap(false);
			vector<__int64> ret;
			map<string, udmvariant>::iterator f = tempvarmap.find(string(meta.name()) + "@" + string(::Uml::Class(meta.parent()).name()) + "@long_array");
			if(f != tempvarmap.end()) ret = f->second.longarrval;
			return ret;
	}

	UDM_DLL void ObjectImpl::setTempIntegerAttrArr(const ::Uml::Attribute &meta, const vector<__int64> &a, const bool direct) 
	{
			tmap &tempvarmap = locatemap(true);
			string sig = string(meta.name()) + "@" + string(::Uml::Class(meta.parent()).name()) + "@long_array";


			ObjectImpl * archetype = getArchetype();
			if (archetype && (archetype != (ObjectImpl*)&Udm::_null) )
			{
				string desynch_sig = sig + "_is_desynched";
				if (direct)
				{
					//set the attribute
					tmap::iterator f = tempvarmap.find(sig);
					if(f != tempvarmap.end()) f->second.longarrval = a;
					else tempvarmap.insert(tmap::value_type(sig, a)); 
			
					//desynch from archetype
					tmap::iterator d = tempvarmap.find(desynch_sig);
					if (d != tempvarmap.end()) d->second.boolval = true;
					else tempvarmap.insert(tmap::value_type(desynch_sig, true)); 

				}
				else
				{
					//check if desynched
					tmap::iterator d = tempvarmap.find(desynch_sig);
					if  ( (d == tempvarmap.end()) || !(d->second.boolval))
					{
						//set the attribute
						tmap::iterator f = tempvarmap.find(sig);
						if(f != tempvarmap.end()) f->second.longarrval = a;
						else tempvarmap.insert(tmap::value_type(sig, a)); 
					}
				}
				archetype->release();
			}
			else
			{
				//set the attrbute
				tmap::iterator f = tempvarmap.find(sig);
				if(f != tempvarmap.end()) f->second.longarrval = a;
				else tempvarmap.insert(tmap::value_type(sig, a)); 
			}

			//go through all derived and instances
			vector<ObjectImpl*>::iterator i;
			vector<ObjectImpl*> deriveds = getDerived();
			for (i = deriveds.begin(); i != deriveds.end(); i++)
			{
				ObjectImpl* derived = *i;
				derived->setTempIntegerAttrArr(meta, a, false);
				derived->release();
			}

			vector<ObjectImpl*> instances = getInstances();
			for (i = instances.begin(); i != instances.end(); i++)
			{
				ObjectImpl* instance = *i;
				instance->setTempIntegerAttrArr(meta, a, false);
				instance->release();
			}

			/*
			tmap::iterator f = tempvarmap.find(sig);
			if(f != tempvarmap.end()) f->second.longarrval = a;
			else tempvarmap.insert(tmap::value_type(sig, a)); 
			*/
	}

///
	UDM_DLL vector<double> ObjectImpl::getTempRealAttrArr(const ::Uml::Attribute &meta) const
	{
			tmap &tempvarmap = locatemap(false);
			vector<double> ret;
			map<string, udmvariant>::iterator f = tempvarmap.find(string(meta.name()) + "@" + string(::Uml::Class(meta.parent()).name()) + "@double_array");
			if(f != tempvarmap.end()) ret = f->second.doublearrval;
			return ret;
	}

	UDM_DLL void ObjectImpl::setTempRealAttrArr(const ::Uml::Attribute &meta, const vector<double> &a, const bool direct) 
	{
			tmap &tempvarmap = locatemap(true);
			string sig = string(meta.name()) + "@" + string(::Uml::Class(meta.parent()).name()) + "@double_array";

			ObjectImpl * archetype = getArchetype();
			if (archetype && (archetype != (ObjectImpl*)&Udm::_null) )
			{
				string desynch_sig = sig + "_is_desynched";
				if (direct)
				{
					//set the attribute
					tmap::iterator f = tempvarmap.find(sig);
					if(f != tempvarmap.end()) f->second.doublearrval = a;
					else tempvarmap.insert(tmap::value_type(sig, a)); 
			
					//desynch from archetype
					tmap::iterator d = tempvarmap.find(desynch_sig);
					if (d != tempvarmap.end()) d->second.boolval = true;
					else tempvarmap.insert(tmap::value_type(desynch_sig, true)); 

				}
				else
				{
					//check if desynched
					tmap::iterator d = tempvarmap.find(desynch_sig);
					if  ( (d == tempvarmap.end()) || !(d->second.boolval))
					{
						//set the attribute
						tmap::iterator f = tempvarmap.find(sig);
						if(f != tempvarmap.end()) f->second.doublearrval = a;
						else tempvarmap.insert(tmap::value_type(sig, a)); 
					}
				}
				archetype->release();
			}
			else
			{
				//set the attrbute
				tmap::iterator f = tempvarmap.find(sig);
				if(f != tempvarmap.end()) f->second.doublearrval = a;
				else tempvarmap.insert(tmap::value_type(sig, a)); 
			
			}

			//go through all derived and instances
			vector<ObjectImpl*>::iterator i;
			vector<ObjectImpl*> deriveds = getDerived();
			for (i = deriveds.begin(); i != deriveds.end(); i++)
			{
				ObjectImpl* derived = *i;
				derived->setTempRealAttrArr(meta, a, false);
				derived->release();
			}

			vector<ObjectImpl*> instances = getInstances();
			for (i = instances.begin(); i != instances.end(); i++)
			{
				ObjectImpl* instance = *i;
				instance->setTempRealAttrArr(meta, a, false);
				instance->release();
			}

			/*
			tmap::iterator f = tempvarmap.find(sig);
			if(f != tempvarmap.end()) f->second.doublearrval = a;
			else tempvarmap.insert(tmap::value_type(sig, a)); 
			*/
	}

///
	UDM_DLL vector<bool> ObjectImpl::getTempBooleanAttrArr(const ::Uml::Attribute &meta) const
	{
			tmap &tempvarmap = locatemap(false);
			vector<bool> ret;
			map<string, udmvariant>::iterator f = tempvarmap.find(string(meta.name()) + "@" + string(::Uml::Class(meta.parent()).name()) + "@bool_array");
			if(f != tempvarmap.end()) ret = f->second.boolarrval;
			return ret;
	}

	UDM_DLL void ObjectImpl::setTempBooleanAttrArr(const ::Uml::Attribute &meta, const vector<bool> &a, const bool direct) 
	{
			tmap &tempvarmap = locatemap(true);
			string sig = string(meta.name()) + "@" + string(::Uml::Class(meta.parent()).name()) + "@bool_array";

			ObjectImpl * archetype = getArchetype();
			if (archetype && (archetype != (ObjectImpl*)&Udm::_null) )
			{
				string desynch_sig = sig + "_is_desynched";
				if (direct)
				{
					//set the attribute
					tmap::iterator f = tempvarmap.find(sig);
					if(f != tempvarmap.end()) f->second.boolarrval = a;
					else tempvarmap.insert(tmap::value_type(sig, a)); 
			
					//desynch from archetype
					tmap::iterator d = tempvarmap.find(desynch_sig);
					if (d != tempvarmap.end()) d->second.boolval = true;
					else tempvarmap.insert(tmap::value_type(desynch_sig, true)); 

				}
				else
				{
					//check if desynched
					tmap::iterator d = tempvarmap.find(desynch_sig);
					if  ( (d == tempvarmap.end()) || !(d->second.boolval))
					{
						//set the attribute
						tmap::iterator f = tempvarmap.find(sig);
						if(f != tempvarmap.end()) f->second.boolarrval = a;
						else tempvarmap.insert(tmap::value_type(sig, a)); 
					}
				}
				archetype->release();
			}
			else
			{
				//set the attrbute
				tmap::iterator f = tempvarmap.find(sig);
				if(f != tempvarmap.end()) f->second.boolarrval = a;
				else tempvarmap.insert(tmap::value_type(sig, a)); 
			
			}

			//go through all derived and instances
			vector<ObjectImpl*>::iterator i;
			vector<ObjectImpl*> deriveds = getDerived();
			for (i = deriveds.begin(); i != deriveds.end(); i++)
			{
				ObjectImpl* derived = *i;
				derived->setTempBooleanAttrArr(meta, a, false);
				derived->release();
			}

			vector<ObjectImpl*> instances = getInstances();
			for (i = instances.begin(); i != instances.end(); i++)
			{
				ObjectImpl* instance = *i;
				instance->setTempBooleanAttrArr(meta, a, false);
				instance->release();
			}
		
			/*
			tmap::iterator f = tempvarmap.find(sig);
			if(f != tempvarmap.end()) f->second.boolarrval = a;
			else tempvarmap.insert(tmap::value_type(sig, a)); 
			*/
	}

	UDM_DLL long ObjectImpl::getTempAttrStatus(const ::Uml::Attribute &meta) const
	{
		long status = Udm::ATTSTATUS_HERE;

		tmap &tempvarmap = locatemap(false);

		string sig;
		string attrType = meta.type();
		bool attrArray = ((meta.max() != 0) && (meta.max() != 1));

		if ((string)meta.name() == "name")
			sig = string(meta.name()) + "@" + string(::Uml::Class(meta.parent()).name()) + "@string";
		else if (attrType == "String" || attrType == "Text")
		{
			if (attrArray)
				sig = string(meta.name()) + "@" + string(::Uml::Class(meta.parent()).name()) + "@string_array";
			else
				string(meta.name()) + "@" + string(::Uml::Class(meta.parent()).name()) + "@string";
		}
		else if (attrType=="Integer")
		{
			if (attrArray)
				sig = string(meta.name()) + "@" + string(::Uml::Class(meta.parent()).name()) + "@long_array";
			else
				string(meta.name()) + "@" + string(::Uml::Class(meta.parent()).name()) + "@long";
		}
		else if (attrType=="Real")
		{
			if (attrArray)
				sig = string(meta.name()) + "@" + string(::Uml::Class(meta.parent()).name()) + "@double_array";
			else
				string(meta.name()) + "@" + string(::Uml::Class(meta.parent()).name()) + "@double";
		}
		else if (attrType=="Boolean")
		{
			if (attrArray)
				sig = string(meta.name()) + "@" + string(::Uml::Class(meta.parent()).name()) + "@bool_array";
			else
				string(meta.name()) + "@" + string(::Uml::Class(meta.parent()).name()) + "@bool";
		}
		else
		{
			// Unknown attribute type
			throw udm_exception(string("Unknown attribute type: ") + attrType);
		}

		string desynch_sig = sig + "_is_desynched";

		ObjectImpl * archetype = getArchetype();
		if (archetype && (archetype != (ObjectImpl*)&Udm::_null))
		{
			// for now just set a value telling that the attribute value
			// is not changed here
			tmap::iterator d = tempvarmap.find(desynch_sig);
			if  (d == tempvarmap.end() || !(d->second.boolval))
				status = Udm::ATTSTATUS_IN_ARCHETYPE1;

			archetype->release();
		}

		return status;
	};

	UDM_DLL ObjectImpl* ObjectImpl::AttachLibrary(ObjectImpl *lib_src, const string &lib_name, t_lib_to_copy_impl_map *copy_map)
	{
		UDM_ASSERT(lib_name.length() > 0);

		ObjectImpl *lib_root;
		if (__getdn()->IsTypeSafe())
			lib_root = createLibRootChild(type());
		else
			lib_root = createLibRootChild(Uml::SafeTypeContainer::GetSafeType(type()));

		UdmUtil::copy_assoc_map cam;
		UdmUtil::CopyObjectHierarchy(lib_src, lib_root, lib_root->__getdn(), cam);

		lib_root->setLibraryName(lib_name.c_str());

		if (copy_map != NULL) {
			copy_map->insert( make_pair(lib_src->clone(), lib_root->clone()) );
			for (UdmUtil::copy_assoc_map::const_iterator i = cam.begin(); i != cam.end(); i++)
				copy_map->insert( make_pair(i->first.__impl()->clone(), i->second.__impl()->clone()) );
		}

		return lib_root;
	}



	DataNetwork::backendtabt *DataNetwork::backendtab = NULL;

	UDM_DLL void DataNetwork::RegisterBackend(const string &sig, const string &ext, DataNetwork *(*crea)(const UdmDiagram &, UdmProject*)) 
	{
		backendtabt **p = &backendtab;
		while(*p) {
			p = &((*p)->next);
		}
		*p = new backendtabt;
		(*p)->sig = sig;
		(*p)->ext = ext;
		(*p)->crea = crea;
		(*p)->next = NULL;
	}
	UDM_DLL void DataNetwork::UnRegisterBackends() 
	{
		backendtabt *p = backendtab, *tp;
		while(p) {
			tp = p;
			p = p->next;
			delete tp;
		}
		backendtab = 0;
	}
	UDM_DLL string DataNetwork::DumpBackendNames() 
	{
		backendtabt *p = backendtab;
		string r;
		if(!p) r = "<none>";
		else {
			while(p) {
				r += ' ';
				r += p->sig;
				p = p->next;
			}
		}
		return r;
	}
	UDM_DLL DataNetwork *DataNetwork::CreateBackend(string bid, const UdmDiagram &metainfo, UdmProject* project) 
	{
		backendtabt *p = backendtab;
		string::size_type l;
		/*l = bid.find(':');
		if(l >= 3) {
			bid.erase(l);
			while(p) {
				if(!_stricmp(p->sig.c_str(), bid.c_str())) {
					return p->crea(metainfo);
				}
				p = p->next;
			}
		}
		else */if((l = bid.rfind('.')) >= bid.size()-4) {
			bid.erase(0,l+1);
			while(p) {
				if(!_stricmp(p->ext.c_str(), bid.c_str())) {
					return p->crea(metainfo, project);
				}
				p = p->next;
			}
		}
		return NULL;
	}


	UDM_DLL bool CheckAttributeOrdering(const ::Uml::Attribute & att)
	{
		return (bool) att.ordered();
	};

	UDM_DLL vector<string> SortStrArrAttr(const vector<string> &a)
	{
		multiset<string, less<string> > b;
		vector<string> c;
		b.insert(a.begin(), a.end());
		multiset<string>::iterator i = b.begin();
		while (i != b.end()) c.push_back(*i++);

		return c;
	};

	UDM_DLL vector<__int64> StrToLongArrAttr(const vector<string> &a)
	{
		vector<__int64> c;
		vector<string>::const_iterator i = a.begin();
		while (i != a.end()) 
		{
			c.push_back(_atoi64(i->c_str()));
			i++;
		}
		return c;
	};

	UDM_DLL vector<double> StrToRealArrAttr(const vector<string> &a)
	{
		vector<double> c;
		vector<string>::const_iterator i = a.begin();
		while (i != a.end()) 
		{
			c.push_back(atof(i->c_str()));
			i++;
		}
		return c;
	};

	UDM_DLL vector<bool> StrToBoolArrAttr(const vector<string> &a)
	{
		vector<bool> c;
		vector<string>::const_iterator i = a.begin();
		while (i != a.end()) 
		{
			c.push_back(i->compare("true") ? false : true);
			i++;
		}
		return c;
	};




	UDM_DLL void ObjectImpl::setDefaultAttributes(const bool all)
	{
	
		//if all is false, only nonpersistent attributes will be modified
		set< ::Uml::Attribute> attributes = Uml::AncestorAttributes(type());
		set< ::Uml::Attribute>::const_iterator c_i = attributes.begin();

			while (c_i != attributes.end())
			{
				::Uml::Attribute att = *c_i++;
				if (att.defvalue() || (att.min() > 0))
				{
					if((string)att.name()=="name") setStringAttr(att,type().name(), false);
				
					//has a default value specified in the MetaModel
					//so we set it's default value to this
					string attrType=att.type();
					bool attrArray = ((att.max() != 0) && (att.max() != 1));
					bool np_attr = att.nonpersistent();
					bool ordered = att.ordered();

					if (!np_attr && !all) continue;

					if((attrType=="String")||(attrType=="Text"))
					{

						if(attrArray)
						{
							if (!np_attr)
							{
								if (ordered)
									setStringAttrArr(att, SortStrArrAttr(att.defvalue()), false);
								else
									setStringAttrArr(att, att.defvalue(), false);
							}//if (!np_attr)
							else
							{
								if (ordered)
									setTempStringAttrArr(att, SortStrArrAttr(att.defvalue()), false);
								else
									setTempStringAttrArr(att,att.defvalue(), false);
							}//else (!np_attr)
						}

						else
						{
							if (!np_attr)
								setStringAttr(att,att.defvalue()[0], false);
							else
								setTempStringAttr(att,att.defvalue()[0], false);
						}

					}
					
					else if(attrType=="Integer")
					{
						if (attrArray)
						{
							if (!np_attr)
							{
								if (ordered)
									setIntegerAttrArr(att,SortArrAttr<__int64>(StrToLongArrAttr(att.defvalue())), false);
								else
									setIntegerAttrArr(att,StrToLongArrAttr(att.defvalue()), false);
							}//if (!np_attr)
							else
							{
								if (ordered)
									setTempIntegerAttrArr(att,SortArrAttr<__int64>(StrToLongArrAttr(att.defvalue())), false);
								else
									setTempIntegerAttrArr(att,StrToLongArrAttr(att.defvalue()), false);

							}//else  (!np_attr)
						}//if (attrArray)
						else
						{
							if (!np_attr)
								setIntegerAttr(att,atol(((string)att.defvalue()[0]).c_str()), false);
							else
								setTempIntegerAttr(att,atol(((string)att.defvalue()[0]).c_str()), false);
						}//else (attrArray)

					}
					
					else if(attrType=="Real")
					{
						if (attrArray)
						{
							if (!np_attr)
							{
								if (ordered)
									setRealAttrArr(att,SortArrAttr(StrToRealArrAttr(att.defvalue())), false);
								else
									setRealAttrArr(att,StrToRealArrAttr(att.defvalue()), false);
							}
							else
							{
								if (ordered)
									setTempRealAttrArr(att,SortArrAttr(StrToRealArrAttr(att.defvalue())), false);
								else
									setTempRealAttrArr(att,StrToRealArrAttr(att.defvalue()), false);
							}
						}//if (attrArray)
						else
						{
							if (!np_attr)
								setRealAttr(att, atof(((string)att.defvalue()[0]).c_str()), false);
							else
								setTempRealAttr(att, atof(((string)att.defvalue()[0]).c_str()), false);
						}


					}
					
					else if(attrType=="Boolean")
					{
						if (attrArray)
						{
							if (!np_attr)
							{
								if (ordered)
								{
									setBooleanAttrArr(att,SortArrAttr<bool>(StrToBoolArrAttr(att.defvalue())), false);
								}
								else
								{
									setBooleanAttrArr(att,StrToBoolArrAttr(att.defvalue()), false);
								}
							}
							else
							{
								if (ordered)
								{
									setTempBooleanAttrArr(att,SortArrAttr<bool>(StrToBoolArrAttr(att.defvalue())), false);
								}
								else
								{
									setTempBooleanAttrArr(att,StrToBoolArrAttr(att.defvalue()), false);
								}
							}
						}
						else
						{
							if (!np_attr)
								setBooleanAttr(att,( (((string)att.defvalue()[0])).compare("true") ? false : true ), false);
							else
								setTempBooleanAttr(att,( (((string)att.defvalue()[0])).compare("true") ? false : true ), false);
						}

					}
					else
					{
						// Unknown attribute type
						throw udm_exception(string("Unknown attribute type: ") + attrType);
					}
				}
			}
		}

		UDM_DLL void ObjectImpl::CopyAttributeFrom(const ::Uml::Attribute& which, const ObjectImpl*  from, bool direct)
		{
			if (direct) {
				// If both source and destination have an archetype
				// and the attribute value is inherited from archetype
				// then a copy is not needed. We suppose that the
				// destination has been newly created and all its
				// attribute values are inherited from the archetype.

				ObjectImpl * fromArchetype = from->getArchetype();
				ObjectImpl * archetype = getArchetype();

				bool copy_needed = true;

				if ( archetype && (archetype != (ObjectImpl*)&Udm::_null) &&
					fromArchetype && (fromArchetype != (ObjectImpl*)&Udm::_null) &&
					from->getAttrStatus(which) != Udm::ATTSTATUS_HERE )
					copy_needed = false;

				if (archetype && (archetype != (ObjectImpl*)&Udm::_null))
					archetype->release();

				if (fromArchetype && (fromArchetype != (ObjectImpl*)&Udm::_null))
					fromArchetype->release();

				if (!copy_needed)
					return;
			}

			string attrType=which.type();
			bool attrArray = ((which.max() != 0) && (which.max() != 1));
			bool np_attr = which.nonpersistent();

			if((attrType=="String") || (attrType=="Text") )
			{
				if(attrArray)
					if (!np_attr)
						setStringAttrArr(which,from->getStringAttrArr(which), direct);
					else
						setTempStringAttrArr(which,from->getTempStringAttrArr(which), direct);

				else
					if (!np_attr)
						setStringAttr(which,from->getStringAttr(which), direct);
					else
						setTempStringAttr(which,from->getTempStringAttr(which), direct);			

			}
			else if(attrType=="Integer")
			{
				if (attrArray)
					if (!np_attr)
						setIntegerAttrArr(which,from->getIntegerAttrArr(which), direct);
					else
						setTempIntegerAttrArr(which,from->getTempIntegerAttrArr(which), direct);

				else
					if (!np_attr)
						setIntegerAttr(which,from->getIntegerAttr(which), direct);
					else
						setTempIntegerAttr(which,from->getTempIntegerAttr(which), direct);

			}
			else if(attrType=="Real")
			{
				if (attrArray)
					if (!np_attr)
						setRealAttrArr(which,from->getRealAttrArr(which), direct);
					else
						setTempRealAttrArr(which,from->getTempRealAttrArr(which), direct);

				else
					if (!np_attr)
						setRealAttr(which,from->getRealAttr(which), direct);
					else
						setTempRealAttr(which,from->getTempRealAttr(which), direct);
			}
			else if(attrType=="Boolean")
			{
				if (attrArray)
					if (!np_attr)
						setBooleanAttrArr(which,from->getBooleanAttrArr(which), direct);
					else
						setTempBooleanAttrArr(which,from->getTempBooleanAttrArr(which), direct);

				else
					if (!np_attr)
						setBooleanAttr(which,from->getBooleanAttr(which), direct);
					else
						setTempBooleanAttr(which,from->getTempBooleanAttr(which), direct);

					
			}
			else
			{
				// Unknown attribute type
				throw udm_exception(string("Unknown attribute type: ") + attrType);
			}

		};

		UDM_DLL void ObjectImpl::CopyAttributesFrom(const ObjectImpl*  from, bool direct)
		{
			set < ::Uml::Attribute> attributes = Uml::AncestorAttributes(type());
			set < ::Uml::Attribute>::iterator attr_i = attributes.begin();
			while (attr_i != attributes.end())
				CopyAttributeFrom(*attr_i++, from, direct);
		};

		UDM_DLL void ObjectImpl::CopyAttributeFromArchetype(const ::Uml::Attribute& which)
		{
			ObjectImpl * archetype = getArchetype();
			CopyAttributeFrom(which, archetype, false);
			archetype->release();
		};

		UDM_DLL void ObjectImpl::CopyAttributesFromArchetype()
		{
			ObjectImpl * archetype = getArchetype();
			CopyAttributesFrom(archetype, false);
			archetype->release();
		};


		bool canCompleteRefsChain(const ObjectImpl* target, const vector<ObjectImpl*> &refs)
		{
			UDM_ASSERT(refs.size());

			Object ref = refs.back()->clone();

			// exit loop when reaching the parent of the reference port
			ObjectImpl *target_parent = target->getParent(NULLPARENTROLE);

			while (ref.__impl() != &_null) {

				Object referred = Object(ref.__impl()->clone()).getReferencedObject();
				if (referred.uniqueId() == target_parent->uniqueId())
					break;

				ref = referred.__impl()->clone();
			}

			if (target_parent != &_null)
				target_parent->release();
			return ref != &_null;
		}

		UDM_DLL void ObjectImpl::connectTo(const ::Uml::AssociationRole &meta, ObjectImpl* target, const vector<ObjectImpl*> &refs)
		{
			if ((string)type().stereotype() != "Connection")
				throw udm_exception("Only setting the connection src/dst is supported");

			::Uml::AssociationRole rp_helper = meta.rp_helper();
			ObjectImpl* rp_helper_ref = &_null;
			if (rp_helper) {
				vector<ObjectImpl*> v = getAssociation(rp_helper);
				if (v.size()) {
					rp_helper_ref = v.front();
				}
			}

			if (refs.size()) {
				if (!canCompleteRefsChain(target, refs))
					throw udm_exception("Could not reach target " + UdmUtil::ExtractName(target) + " starting with the given chain of references");
				
				// change rp_helper because the user is requesting a specific chain
				if (rp_helper_ref != &_null && rp_helper_ref != refs.front()) {
					vector<ObjectImpl*> v;
					v.push_back(refs.front());
					setAssociation(rp_helper, v);
					rp_helper_ref->release();
				}
			} else {
				if (rp_helper_ref != &_null) {
					// if a reference port helper is set then check if it can be used to reach the target
					// is there a reference port helper set for this connection?
					vector<ObjectImpl*> v;
					v.push_back(rp_helper_ref);
					if (!canCompleteRefsChain(target, v)) {
						rp_helper_ref->release();
						throw udm_exception("Could not reach target " + UdmUtil::ExtractName(target) + " starting with the reference port helper set in the model");
					}
					rp_helper_ref->release();
				} else {
					Object parent = getParent(NULLPARENTROLE);
					Object parent_target = target->getParent(NULLPARENTROLE);
					if (parent.__impl() != parent_target.__impl()) {
						vector<ObjectImpl*> children = parent.__impl()->getChildren(NULL, NULL);
						vector<ObjectImpl*>::const_iterator i = children.begin();
						for (; i != children.end(); i++) {
							ObjectImpl* child = *i;
							vector<ObjectImpl*> v;
							v.push_back(child);
							if (child->type().stereotype() == "Reference" && canCompleteRefsChain(target, v)) {
								// found a child that can be used to reach the target
								setAssociation(rp_helper, v);
								//(*i)->release();
								break;
							} else {
								(*i)->release();
							}
						}
						if (i == children.end())
							throw udm_exception("Could not reach target " + UdmUtil::ExtractName(target) + " from any reference found in the parent of connection");
						for (; i != children.end(); i++) {
							(*i)->release();
						}
					}
				}
			}

			vector<ObjectImpl*> v;
			v.push_back(target);
			setAssociation(meta, v, TARGETFROMCLASS);
		}

		UDM_DLL void ObjectImpl::disconnectFrom(const ::Uml::AssociationRole &meta, ObjectImpl* peer)
		{
			bool peer_is_conn = peer->type().stereotype() == "Connection";

			vector<ObjectImpl*> empty_v;

#if 0
			// Don't disconnect the references, they may have been set explicitly by user.
			// Maybe this method should accept a boolean parameter telling us whether to
			// disconnect the references or not?
			::Uml::AssociationRole rp_helper = meta.rp_helper();
			if (rp_helper) {

				// unset references from reference port helper to reference port parent
				vector<ObjectImpl*> chain = getConnectingChain(meta, peer);

				if (peer_is_conn)
					std::reverse(chain.begin(), chain.end());

				for (vector<ObjectImpl*>::const_reverse_iterator i = chain.rbegin(); i != chain.rend(); i++) {
					ObjectImpl *ref = *i;
					set< ::Uml::AssociationRole> aroles = Uml::AncestorAssociationTargetRoles(ref->type());
					for (set< ::Uml::AssociationRole>::const_iterator aroles_i = aroles.begin(); aroles_i != aroles.end(); aroles_i++) {
						::Uml::AssociationRole arole = *aroles_i;
						if ((string)arole.name() == "ref") {
							ref->setAssociation(arole, empty_v);
							break;
						}
					}
				}
			}
#endif

			if (peer_is_conn)
				peer->setAssociation(meta, empty_v, TARGETFROMCLASS);
			else
				setAssociation(meta, empty_v, TARGETFROMCLASS);
		}

		UDM_DLL vector<ObjectImpl*> ObjectImpl::getConnectingChain(const ::Uml::AssociationRole &meta, const ObjectImpl* peer) const
		{
			vector<ObjectImpl*> ret;

			::Uml::AssociationRole rp_helper = meta.rp_helper();
			if (!rp_helper) return ret;

			const ObjectImpl *conn, *target;
			if (this->type().stereotype() == "Connection") {
				conn = this;
				target = peer;
			} else if (peer->type().stereotype() == "Connection") {
				conn = peer;
				target = this;
			} else
				throw udm_exception("Neither this object nor its peer seems to be a type of connection");

			vector<ObjectImpl*> v = conn->getAssociation(rp_helper);
			if (v.size()) {
				ObjectImpl* ref = v[0];

				// exit loop when reaching the parent of the reference port
				Object target_parent = target->getParent(NULLPARENTROLE);

				do {
					ret.push_back(ref);

					Object referred = Object(ref->clone()).getReferencedObject();
					if (referred.uniqueId() == target_parent.__impl()->uniqueId())
						break;

					ref = referred.__impl()->clone();
				} while (ref != &_null);

				if (ref == &_null)
					// the chain of references does not reach the reference port parent
					ret.clear();
			}

			if (conn == peer)
				std::reverse(ret.begin(), ret.end());

			return ret;
		}

		UDM_DLL string ObjectImpl::getPath(const string &strDelimiter, bool bNeedRootFolder) const
		{
			DataNetwork *dn = const_cast<ObjectImpl*>(this)->__getdn();
			ObjectImpl *parent = getParent(NULLPARENTROLE);

			if ( (dn && this == dn->GetRootObject().__impl()) || parent == &Udm::_null)
			{
				if (bNeedRootFolder)
					return UdmUtil::ExtractName(this, "name");
				else
					return "";
			}
			else
			{
				string path = parent->getPath(strDelimiter, bNeedRootFolder);
				if (path.length())
					path += strDelimiter;	// parent could be the root folder, if bNeedRootFolder is
								// false then path to parent is empty and delimiter must not be added

				path += UdmUtil::ExtractName(this, "name");
				return path;
			}
		}

		UDM_DLL string ObjectImpl::toString() const
		{
			ostringstream ret;

			// path or name
			ret << getPath("/");

			// unique Id
			ret << " [" << uniqueId() << "]";

			if (type().__impl() != &Udm::_null) {
				ret << " of type " << type().getPath2("::");
			} else
				ret << " of unknown type";

			DataNetwork *dn = const_cast<ObjectImpl*>(this)->__getdn();
			if (dn) {
				ret << ", in DN " << dn->uniqueId();
			} else
				ret << ", not in a DN";

			return ret.str();
		}


		//the map for static metadepository
		map<string, const UdmDiagram*>* MetaDepository::meta_dep = NULL;
		

		UDM_DLL const UdmDiagram & MetaDepository::LocateDiagram(const string& DgrName) 
		{
			
			map<string, const UdmDiagram*>::const_iterator i = meta_dep->find(DgrName);
			if ( i == meta_dep->end()) 	
				throw udm_exception(string("Meta diagram ") + DgrName + string(" was not found in metadepository!"));
			
			return *(i->second);
			

		};

		UDM_DLL bool MetaDepository::StoreDiagram(const string& DgrName, const UdmDiagram& dgr)
		{
			if (meta_dep == NULL)
				meta_dep = new map<string, const UdmDiagram*>;

			pair<const string, const UdmDiagram *> item(DgrName, &dgr);
			pair<map<string, const UdmDiagram*>::iterator, bool> res = meta_dep->insert(item);
			return res.second;
			
		};

		UDM_DLL void MetaDepository::RemoveDiagram(const string& DgrName)
		{
			if (meta_dep != NULL) {
				meta_dep->erase(DgrName);
				if (meta_dep->size() == 0) {
					delete meta_dep;
					meta_dep = NULL;
				}
			}

		};

		UDM_DLL DataNetwork* CreateSmartDataNetwork(const UdmDiagram &metainfo, UdmProject * pr)
		{
			return new SmartDataNetwork(metainfo, pr);
		}
}


