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

#include "UmlExt.h"
#include "UdmStatic.h"
#include "UdmUtil.h"
#include <cstring>

/*
	CHANGELOG
	11/24/05	-	endre

					Added GetClassFromCrossDgr, which returns the corresponding class from the cross diagram

 	04/22/05	-	kalmar
        fixed OtherCompositionPeerParentRolesNamespaces : parents DescendantClasses need to be tested, too

	12/06/04	-	endre

			-		Added the GetChildRoleChain function, which returns all the possible paths how a class can be contained in an other
	09/11/04	-	endre

			-		Small fix in HasTextAttribytes/TextAttributes calls to also consider Ancestor attributes

	07/11/04	-	endre

			-		Addd two functions, HasTextAttributes and TextAttributes for checking the existence and for getting
					all the text attributes from a class.


	19/09/04	-	endre

			-		Added new function GetAncestorAssociation, which removes the only (if any) Association
					attached to a Class.


	12/12/02	endre
		 AncestorCrossAssociationRoles(): Changed the cross class-diagram delimiter from "_" to "_cross_ph_"
*/

namespace Uml
{

	UDM_DLL const AssociationRole theOther(const AssociationRole &role)
	{

		Association assoc = role.parent();

		set<AssociationRole> roles = assoc.roles();
		UDM_ASSERT( roles.size() == 2 );

		set<AssociationRole>::iterator i = roles.begin();
		if( *i == role )
			++i;

		return *i;
	}

	UDM_DLL const CompositionChildRole theOther(const CompositionParentRole &role)
	{
		const Composition &comp = role.parent();
		return comp.childRole();
	}

	UDM_DLL const CompositionParentRole theOther(const CompositionChildRole &role)
	{
		const Composition &comp = role.parent();
		return comp.parentRole();
	}

	// find an assoc.Class by name in a Diagram
	UDM_DLL Class assocClassByName(const Diagram &d, const string &name)
	{
		set<Class> ts = d.classes();
		for(set<Class>::iterator i = ts.begin(); i != ts.end(); i++) 
		{
			if ((Association)(i->association()) && string(i->name()) == name) return(*i);
		}
		return NULL;
	}

	// find an assoc.Class by name in a Namespace
	UDM_DLL Class assocClassByName(const Namespace &ns, const string &name)
	{
		set<Class> ts = ns.classes();
		for(set<Class>::iterator i = ts.begin(); i != ts.end(); i++) 
		{
			if ((Association)(i->association()) && string(i->name()) == name) return(*i);
		}
		return NULL;

	};

	
	// find a class by name in a Namespace
	UDM_DLL Class classByName(const Namespace &ns, const string &name) 
	{
		set<Class> ts = ns.classes();
			{
				for(set<Class>::iterator i = ts.begin(); i != ts.end(); i++) {
					if(string(i->name()) == name) return(*i);
				}
			}
			return NULL;
	}

	// find a class by name in a Diagram
	UDM_DLL Class classByName(const Diagram &d, const string &name) 
	{
		set<Class> ts = d.classes();
			{
				for(set<Class>::iterator i = ts.begin(); i != ts.end(); i++) {
					if(string(i->name()) == name) return(*i);
				}
			}
			return NULL;
	}

// find association by name in a Diagram
	UDM_DLL Association associationByName(const Diagram &d, const string &name) 
	{
			set<Association> ts = d.associations();
			{
				for(set<Association>::iterator i = ts.begin(); i != ts.end(); i++) {
					if(string(i->name()) == name) return(*i);
				}
			}
			return NULL;
	}

// find association by name in a Namespace
	UDM_DLL Association associationByName(const Namespace &ns, const string &name) 
	{
			set<Association> ts = ns.associations();
			{
				for(set<Association>::iterator i = ts.begin(); i != ts.end(); i++) {
					if(string(i->name()) == name) return(*i);
				}
			}
			return NULL;
	}


	
// find composition by name in a Diagram
	UDM_DLL Composition compositionByName(const Diagram &d, const string &name) 
	{
			set<Composition> ts = d.compositions();
			{
				for(set<Composition>::iterator i = ts.begin(); i != ts.end(); i++) {
					if(string(i->name()) == name) return(*i);
				}
			}
			return NULL;
	}

// find composition by name in a Namespace
	UDM_DLL Composition compositionByName(const Namespace &ns, const string &name) 
	{
			set<Composition> ts = ns.compositions();
			{
				for(set<Composition>::iterator i = ts.begin(); i != ts.end(); i++) {
					if(string(i->name()) == name) return(*i);
				}
			}
			return NULL;
	}

	void AncestorClasses(const Class &c, set<Class>& ret)
	{
		ret.insert(c);

		set<Class> baseTypes = c.baseTypes();
		set<Class>::iterator i = baseTypes.begin();
		while( i != baseTypes.end() )
		{
			AncestorClasses(*i, ret);

			++i;
		}
	}

	UDM_DLL set<Class> AncestorClasses(const Class &c)
	{
		set<Class> ret;

		AncestorClasses(c, ret);
		
		return ret;
	}

	UDM_DLL Association GetAncestorAssociation(const Class &c)	
	{
		Association Assoc = c.association();
		if (Assoc) return Assoc;

		set<Class> ancs = c.baseTypes();
		for (set<Class>::iterator ancs_i = ancs.begin(); ancs_i != ancs.end(); ancs_i++)
			return GetAncestorAssociation(*ancs_i);
	
		
		return &Udm::_null;
	};

	UDM_DLL set<Class> CommonAncestorClasses(const set<Class> &cs)
	{

		set<Class> ret;
		if(cs.begin() != cs.end()) {
			set<Class>::const_iterator ci = cs.begin();
			ret = AncestorClasses(*ci);
			while(++ci != cs.end()) {
				for(set<Class>::iterator ri = ret.begin(); ri != ret.end();) {
					set<Class>::iterator riact = ri++;
					if(!::Uml::IsDerivedFrom(*ci, *riact)) {
						ret.erase(riact);
					}
				}
			}
			for(set<Class>::iterator ri = ret.begin(); ri != ret.end();) {
				set<Class>::iterator riact = ri++;
				for(set<Class>::iterator ri2 = ret.begin(); ri2 != ret.end(); ri2++) {
					if(riact != ri2 && ::Uml::IsDerivedFrom(*ri2, *riact)) {
						ret.erase(riact);
						break;
					}
				}
			}
		}

		return ret;
	}


	UDM_DLL set<Class> DescendantClasses(const Class &c)
	{
		set<Class> ret;
		ret.insert(c);

		set<Class> subTypes = c.subTypes();
		set<Class>::iterator i = subTypes.begin();
		while( i != subTypes.end() )
		{
			set<Class> a = DescendantClasses(*i);
			ret.insert(a.begin(), a.end());

			++i;
		}
		
		return ret;
	}

	UDM_DLL set<Class> ContainerClasses(const Class &c)
	{
		set<Class> ret;

		set<CompositionChildRole> childRoles = c.childRoles();
		set<CompositionChildRole>::iterator i = childRoles.begin();
		while( i != childRoles.end() )
		{
			Class d = theOther(*i).target();
			ret.insert(d);

			++i;
		}

		return ret;
	}

	UDM_DLL set<Class> ContainedClasses(const Class &c)
	{
		set<Class> ret;

		set<CompositionParentRole> parentRoles = c.parentRoles();
		set<CompositionParentRole>::iterator i = parentRoles.begin();
		while( i != parentRoles.end() )
		{
			Class d = theOther(*i).target();
			ret.insert(d);

			++i;
		}

		return ret;
	}

	UDM_DLL set<Class> AncestorContainerClasses(const Class &c)
	{
		set<Class> ret;

		set<Class> ancestors = AncestorClasses(c);
		set<Class>::iterator i = ancestors.begin();
		while( i != ancestors.end() )
		{
			set<Class> a = ContainerClasses(*i);
			ret.insert(a.begin(), a.end());

			++i;
		}

		return ret;
	}

	UDM_DLL set<Class> AncestorContainedClasses(const Class &c)
	{
		set<Class> ret;

		set<Class> ancestors = AncestorClasses(c);
		set<Class>::iterator i = ancestors.begin();
		while( i != ancestors.end() )
		{
			set<Class> a = ContainedClasses(*i);
			ret.insert(a.begin(), a.end());

			++i;
		}

		return ret;
	}

	UDM_DLL set<Class> AncestorContainedDescendantClasses(const Class &c)
	{
		set<Class> ret;

		set<Class> contained = AncestorContainedClasses(c);
		set<Class>::iterator i = contained.begin();
		while( i != contained.end() )
		{
  			set<Class> a = DescendantClasses(*i);
			ret.insert(a.begin(), a.end());

			++i;
		}

		return ret;
	}

	UDM_DLL set<AssociationRole> AssociationTargetRoles(const Class &c)
	{
		set<AssociationRole> roles;

		set<AssociationRole> a = c.associationRoles();
		set<AssociationRole>::iterator i = a.begin();
		while( i != a.end() )
		{
			roles.insert(theOther(*i));

			++i;
		}

		return roles;
	}

	UDM_DLL set<AssociationRole> AncestorAssociationTargetRoles(const Class &c)
	{
		set<AssociationRole> roles;

		set<Class> ancestors = AncestorClasses(c);
		set<Class>::iterator i = ancestors.begin();
		while( i != ancestors.end() )
		{
			set<AssociationRole> a = AssociationTargetRoles(*i);
			roles.insert(a.begin(), a.end());

			++i;
		}

		return roles;
	}

	// All the other ends of associations this class can have (including those defined in ancestors)
	UDM_DLL set<AssociationRole> AncestorCrossAssociationTargetRoles(const Class &c, const Diagram & cross_dgr)
	{
		set<AssociationRole> ret;
		set<Class> anc_cs = AncestorClasses(c);
		set<Class>::iterator i = anc_cs.begin();
		while (i != anc_cs.end())
		{
			Class ph_a_i = GetClassFromCrossDgr(cross_dgr, *i);
			if (ph_a_i)
			{
				set<AssociationRole> ret_i = AssociationTargetRoles(ph_a_i);
				ret.insert(ret_i.begin(), ret_i.end());
			}
			i++;
		};

		return ret;
	}

	UDM_DLL set<AssociationRole> AncestorCrossAssociationRoles(const Class &c, const Diagram & cross_dgr)
	{
		set<AssociationRole> ret;
		set<Class> anc_cs = AncestorClasses(c);
		set<Class>::iterator i = anc_cs.begin();
		while (i != anc_cs.end())
		{
			Class ph_a_i = GetClassFromCrossDgr(cross_dgr, *i);
			if (ph_a_i)
			{
				set<AssociationRole> ret_i = ph_a_i.associationRoles();
				ret.insert(ret_i.begin(), ret_i.end());
			}
			i++;
		};

		return ret;
	}

	UDM_DLL set<AssociationRole> AncestorAssociationRoles(const Class &c)
	{
		set<AssociationRole> roles;

		set<Class> ancestors = AncestorClasses(c);
		set<Class>::iterator i = ancestors.begin();
		while( i != ancestors.end() )
		{
			set<AssociationRole> a = i->associationRoles();
			roles.insert(a.begin(), a.end());

			++i;
		}

		return roles;
	}

	UDM_DLL bool IsCrossNSAssociation(const Association &a)
	{
		set<AssociationRole> roles = a.roles();
		UDM_ASSERT( roles.size() == 2 );

		set<AssociationRole>::iterator i = roles.begin();
		Class tg1 = i->target();
		i++;
		Class tg2 = i->target();

		if ((Namespace) tg1.parent_ns() != (Namespace) tg2.parent_ns())
			return true;

		Class c = a.assocClass();
		if (c && (Namespace) c.parent_ns() != (Namespace) tg1.parent_ns())
			return true;

		return false;
		
	}

	UDM_DLL set<CompositionChildRole> CompositionPeerChildRoles(const Class &c)
	{
		set<CompositionChildRole> roles;
		set<CompositionParentRole> a = c.parentRoles();
		set<CompositionParentRole>::iterator i = a.begin();
		while( i != a.end() )
		{
			roles.insert(theOther(*i));
			++i;
		}

		return roles;
	};

	UDM_DLL set<CompositionChildRole> AncestorCompositionPeerChildRoles(const Class &c)
	{
		set<CompositionChildRole> roles;

		set<Class> ancestors = AncestorClasses(c);
		set<Class>::iterator i = ancestors.begin();
		while( i != ancestors.end() )
		{
			set<CompositionChildRole> a = CompositionPeerChildRoles(*i);
			roles.insert(a.begin(), a.end());
			++i;
		}

		return roles;
	
	};

	UDM_DLL set<CompositionParentRole> CompositionPeerParentRoles(const Class &c) 
	{
		set<CompositionParentRole> roles;
		set<CompositionChildRole> a = c.childRoles();
		set<CompositionChildRole>::iterator i = a.begin();
		while( i != a.end() )
		{
			roles.insert(theOther(*i));

			++i;
		}

		return roles;
	}

 	UDM_DLL set<CompositionParentRole> AncestorCompositionPeerParentRoles(const Class &c) 
	{
		set<CompositionParentRole> roles;

		set<Class> ancestors = AncestorClasses(c);
		set<Class>::iterator i = ancestors.begin();
		while( i != ancestors.end() )
		{
			set<CompositionParentRole> a = CompositionPeerParentRoles(*i);
			roles.insert(a.begin(), a.end());
			++i;
		}

		return roles;
	}


	UDM_DLL set<CompositionParentRole> DescendantCompositionPeerParentRoles(const Class &c) 
	{
		set<CompositionParentRole> roles;

		set<Class> desc = DescendantClasses(c);
		set<Class>::iterator i = desc.begin();
		while( i != desc.end() )
		{
			set<CompositionParentRole> a = CompositionPeerParentRoles(*i);
			roles.insert(a.begin(), a.end());
			++i;
		}

		return roles;
	}

	UDM_DLL bool IsCrossNSComposition(const Composition &c)
	{
		CompositionParentRole p_r = c.parentRole();
		Class p_c = p_r.target();
		Namespace p_ns = p_c.parent_ns();

		CompositionChildRole c_r = c.childRole();
		Class c_c = c_r.target();
		Namespace c_ns = c_c.parent_ns();

		return p_ns != c_ns;

	}

	UDM_DLL set<Attribute> AncestorAttributes(const Class &c)
	{
		set<Attribute> attributes;

		set<Class> ancestors = AncestorClasses(c);
		set<Class>::iterator i = ancestors.begin();
		while( i != ancestors.end() )
		{
			set<Attribute> a = i->attributes();
			attributes.insert(a.begin(), a.end());

			++i;
		}

		return attributes;
	}

	UDM_DLL Composition matchChildToParent(Class c, Class p)
	{
		return matchChildToParent(c, p, NULL);
	}

	UDM_DLL Composition matchChildToParent(Class c, Class p, const char * crole, const char * prole)
	{
        /*
		Composition comp;
		set<Class> pancs = AncestorClasses(p);
		set<Class> cancs = AncestorClasses(c);
		for(set<Class>::iterator j = cancs.begin(); j != cancs.end(); j++)
        {
			set<CompositionChildRole> cr = (*j).childRoles();
            for(set<CompositionChildRole>::iterator i = cr.begin(); i != cr.end(); i++)
            {
                if (crole == NULL  || i->name() == string(crole))
                {
                    if (prole == NULL || theOther((*i)).name() == string(prole))
                    {
                        if(pancs.find(theOther(*i).target()) != pancs.end())
                        {
                            if(comp)
                            {
                                return NULL;
                            }
                            comp = (*i).parent();
                        }
                    }
                }
            }
		}
		return comp;
         */
        set<Composition> comps =compositionsChildToParent(c,p,crole, prole);
        if (comps.size() != 1) return NULL;
        else return *(comps.begin());
	}

    UDM_DLL set<Composition> compositionsChildToParent(Class c, Class p, const char * crole, const char * prole)
	{
		set<Composition> comp;
		set<Class> pancs = AncestorClasses(p);
		set<Class> cancs = AncestorClasses(c);
		for(set<Class>::iterator j = cancs.begin(); j != cancs.end(); j++)
        {
			set<CompositionChildRole> cr = (*j).childRoles();
            for(set<CompositionChildRole>::iterator i = cr.begin(); i != cr.end(); i++)
            {
                if (crole == NULL  || i->name() == string(crole))
                {
                    if (prole == NULL || theOther((*i)).name() == string(prole))
                    {
                        if(pancs.find(theOther(*i).target()) != pancs.end()) comp.insert((*i).parent());
                    }
                }
            }
		}
		return comp;
	}
    

    
    UDM_DLL AssociationRole matchPeerToPeer(Class c, Class target_class, Class target_aclass, const char * rolename)
    {
        
        set<AssociationRole> assocs = c.associationRoles();
        AssociationRole found;
        
        for( set<AssociationRole>::iterator i = assocs.begin(); i != assocs.end(); i++)
        {
            AssociationRole the_other = theOther(*i);
            Class oclass = the_other.target();
            
            Association pp = (*i).parent();
            Class aclass = pp.assocClass();
            std::string thisrolename = i->name();
            
            if (  (!target_aclass && (target_class == oclass)) || ( target_aclass && (target_aclass == aclass)) )
            {
                
                if (rolename != NULL)
                {
                    if (thisrolename.compare(rolename) == 0)
                    {
                        if (found) return Uml::AssociationRole();
                        found = theOther(*i);
                    }
                    
                }
                else
                {
                    if (found) return Uml::AssociationRole();
                    found = theOther(*i);
                }
                
            }
        }
        
        return found;
    }
    
	UDM_DLL bool IsAssocClass(const Class &cl) 
	{
		return (Association)cl.association();
	}

	UDM_DLL bool IsAssocClass(const Association &cl) 
	{
		return (Class)cl.assocClass();
	}

	UDM_DLL bool IsDerivedFrom(const Class &derived, const Class &base)
	{
		// speedup and to make the startup possible
		//debug
		//end of debug
		if( &derived == &base )
			return true;

		if( derived == base )
			return true;

		set<Class> baseTypes = derived.baseTypes();
		set<Class>::iterator i = baseTypes.begin();
		while( i != baseTypes.end() )
		{
			if( ::Uml::IsDerivedFrom(*i, base) )
				return true;

			++i;
		}

		return false;
	}


// --------------------------- Construction








	UDM_DLL string MakeRoleName(const GenericRole &r) 
	{
			string name = r.name();

			if(name.empty()) {
				string anonbase, auxname;
	
				if(::Uml::IsDerivedFrom(r.type(), AssociationRole::meta)) { 
					AssociationRole ar = AssociationRole::Cast(r);
					anonbase = "arole"; 
					auxname = (string)((Class)ar.target()).name();	
					auxname[0] = _tolower(auxname[0]);
					if((int)ar.max() != 1) {
						if(strchr("hsy", *auxname.rbegin())) {
							auxname.append("es");
						}
						else auxname.append("s");
					}	
				}

				if(::Uml::IsDerivedFrom(r.type(), CompositionParentRole::meta)) { 
					CompositionParentRole pr = CompositionParentRole::Cast(r);
					string revname = theOther(pr).name();
					if(!revname.empty()) revname += "_";
					anonbase = "crole"; auxname = revname +	(string)((Class)pr.target()).name() + "_parent"; 
				}

				if(::Uml::IsDerivedFrom(r.type(), CompositionChildRole::meta)) { 
					CompositionChildRole cr = CompositionChildRole::Cast(r);
					anonbase = "prole";
					if (IsCrossNSComposition(cr.parent()))
						auxname = ((Class)cr.target()).getPath2("_", false);
					else
						auxname = (string)((Class)cr.target()).name();
					auxname += ((int)cr.max() == 1 ? "_child" : "_children");	
				}

				if(!anonbase.length()) {
					return "<namegen error>";
				}	

				if(r.isNavigable()) {
					name = auxname;
				}
				else {
					char buf[25];
#if defined(_WIN32) && defined(_M_AMD64)
					sprintf_s(buf, "%Ix", r.__impl()->uniqueId());
#elif defined(_WIN32)
					_ultoa(r.__impl()->uniqueId(),buf,10);
#else
					snprintf(buf, 11, "%ld", r.__impl()->uniqueId());
#endif
					name=anonbase;
//					name.append(ultoa(r.__impl()->uniqueId(),buf,10));	
					name.append(buf);
				}
			}
			return name;
	}

	UDM_DLL string MakeShortRoleName(const GenericRole &r) 
	{
			string name = r.name();

			if(name.empty()) {
				string anonbase, auxname;

/*				
				if(::Uml::IsDerivedFrom(r.type(), AssociationRole::meta)) { 
					AssociationRole ar = AssociationRole::Cast(r);
					anonbase = "arole"; auxname = (string)((Class)ar.target()).name() + ((int)ar.max() == 1 ? "_assoc" : "_assocs");	
				}

				if(::Uml::IsDerivedFrom(r.type(), CompositionParentRole::meta)) { 
					CompositionParentRole pr = CompositionParentRole::Cast(r);
					string revname = theOther(pr).name();
					if(!revname.empty()) revname += "_";
					anonbase = "crole"; auxname = revname +	(string)((Class)pr.target()).name() + "_parent"; 
				}
*/

				if(::Uml::IsDerivedFrom(r.type(), CompositionChildRole::meta)) { 
					CompositionChildRole cr = CompositionChildRole::Cast(r);
					anonbase = "prole";
					if (IsCrossNSComposition(cr.parent()))
						auxname = ((Class)cr.target()).getPath2("_", false);
					else
						auxname = (string)((Class)cr.target()).name();
				}

				if(!anonbase.length()) {
					return "<namegen error>";
				}	

				if(r.isNavigable()) {
					name = auxname;
				}
				else name = "";
			}
			return name;
	}


	UDM_DLL bool HasChildRoleMultipleTargets(const CompositionChildRole & ccr)
	{
		int i = 0;
		set<Class> descs = DescendantClasses(ccr.target());

		for (set<Class>::iterator descs_i = descs.begin(); descs_i != descs.end(); descs_i++)
		{
			if ((bool)(descs_i->isAbstract()) == false || *descs_i == static_cast<Class>(ccr.target()))
				i++;
			if (i > 1)
				return true;
		}

		return false;
	}

	UDM_DLL set<Attribute> TextAttributes(const Class &c)
	{
		set<Attribute> ret;
		set<Attribute> atts = AncestorAttributes(c);
		for (set<Attribute>::iterator i = atts.begin(); i != atts.end(); i++)
			if ((string)(i->type()) == "Text") ret.insert(*i);
		
		return ret;
	};
	

	//hope this will be just a little bit more performant
	UDM_DLL bool HasTextAttributes(const Class &c)
	{
		set<Attribute> atts = AncestorAttributes(c);
		for (set<Attribute>::iterator i = atts.begin(); i != atts.end(); i++)
			if ((string)(i->type()) == "Text") return true;

		return false;
	};

	/*
		fills vector chains with all possibile composition child
		role chains(paths), how 'what' can be contained in 'origin'
		the results are vectors of CompositionChildRoles;
		returns true if successful, false if a loop was detected
	*/
	bool GetChildRoleChain(const Class &origin, const Class &what, vector<ChildRoleChain> &chains, ChildRoleChain curr_chain, set<CompositionChildRole> &roles_seen)
	{
		set<CompositionParentRole> cprs = origin.parentRoles();
		if (cprs.size())
		{
			set<CompositionParentRole>::iterator cprs_i = cprs.begin();
			while (cprs_i != cprs.end())
			{
				const CompositionChildRole ccr = theOther(*cprs_i);
				const Class new_orig  = ccr.target();

				if (roles_seen.find(ccr) != roles_seen.end())
					return false;	// loop detected

				curr_chain.insert(curr_chain.begin(), ccr);
				roles_seen.insert(ccr);

				set<Class> new_orig_descs = DescendantClasses(new_orig);
				
				for (set<Class>::iterator nod_i = new_orig_descs.begin(); nod_i != new_orig_descs.end(); nod_i++)
					if (!GetChildRoleChain(*nod_i, what, chains, curr_chain, roles_seen))
						return false;	// loop detected

				cprs_i++;
				curr_chain.erase(curr_chain.begin());
				roles_seen.erase(ccr);
			};
		}
		else if(::Uml::IsDerivedFrom(origin, what)) chains.push_back(curr_chain);
		return true;
	}

	UDM_DLL bool GetChildRoleChain(const Class & origin, const Class &what, vector<ChildRoleChain> &chains, ChildRoleChain curr_chain)
	{
		set<CompositionChildRole> roles_seen;
		return GetChildRoleChain(origin, what, chains, curr_chain, roles_seen);
	};

	UDM_DLL Namespace GetTheOnlyNamespace(const Diagram & dgr)
	{
		if (!dgr) return NULL;
		set<Namespace> nses = dgr.namespaces();
		if (nses.size() != 1) throw udm_exception("Uml::Namespace GetTheOnlyNamespace: More than one namespace found in diagram!");
		return *(nses.begin());
	};

	// get the corresponding class from the cross diagram
	UDM_DLL Class GetClassFromCrossDgr(const Diagram &cross_dgr, const Class &cl)
    {
		//The classname in cross diagram is: classname + cross_delimiter + diagramname ( + cross_delimiter + namespacename )*
		string cross_cl_name = string(cl.name()) + string(Udm::cross_delimiter) + cl.GetParent().getPath2(Udm::cross_delimiter);
		return classByName(cross_dgr, cross_cl_name);
	}
    
    bool IsCrossNSCompositionChildEnd(const ::Uml::Class &c)
    {
        ::Uml::Namespace ns = c.parent_ns();
        
        set< ::Uml::CompositionParentRole> comps_c = ::Uml::AncestorCompositionPeerParentRoles(c);
        for (set< ::Uml::CompositionParentRole>::iterator j = comps_c.begin(); j != comps_c.end(); j++)
        {
            ::Uml::Class theother = j->target();
            
            set< ::Uml::Class> desc = ::Uml::DescendantClasses(theother);
            for (set< ::Uml::Class>::iterator jd = desc.begin(); jd != desc.end(); jd++)
            {
                ::Uml::Class cc = *jd;
                
                ::Uml::Namespace theother_ns = (::Uml::Namespace) cc.parent_ns();
                if (ns != theother_ns)
                    return true;
            }
        }
        
        return false;
    };


    UDM_DLL vector< ::Uml::Class> findNonContainedClasses(const ::Uml::Diagram & dgr)
    {
        vector< ::Uml::Class> globalElements;
        set< ::Uml::Class> classes = dgr.classes();
        set< ::Uml::Class>::const_iterator i = classes.begin();
        while( i != classes.end() )
        {
            if (!(bool)i->isAbstract())
            {
                bool is_not_contained = false;
                
                // XSD elements can't be of abstract types,
                // don't make them globals
                set< ::Uml::Class> p_c = Uml::AncestorContainerClasses(*i);		//all possible containers
                
                //the condition: It's not contained
                //				 or it's only container is itself or an abstract superclass
                //				 or it's the child end of a composition with the parent end in other namespace
                ::Uml::Class anc;
                if ((p_c.size() == 0) || (p_c.size() == 1 && (anc = *(p_c.begin())) && (anc == *i || (::Uml::IsDerivedFrom(*i, anc) && anc.isAbstract()))))
                    is_not_contained = true;
                
                if (is_not_contained || ::Uml::IsCrossNSCompositionChildEnd(*i))
                    globalElements.push_back(*i);
                
                
            }
            ++i;
        }
        return globalElements;
    };

	// find a class by name
	UDM_DLL Class classByName(const Diagram &d, const string &ns_path, const string &name, const string &delim)
	{
		string path(ns_path);
		if (path.length())
			path += delim + name;
		else
			path += name;
		return classByPath(d, path, delim);
	};

	// find a class by path
	UDM_DLL Class classByPath(const Diagram &d, const string &path, const string &delim)
	{
		string::size_type i = path.rfind(delim);
		if (i == string::npos)
			return classByName(d, path);

		string ns_path = path.substr(0, i);
		string class_name = path.substr(i + delim.size());
		if (ns_path.length())
		{
			Namespace ns = namespaceByPath(d, ns_path, delim);
			if (ns)
				return classByName(ns, class_name);
		}
		else
		{
			// path = "" + delim + "class_name"
			return classByName(d, class_name);
		}

		return Class();
	};


// find a namespace by name
	UDM_DLL Namespace namespaceByName(const Diagram &d, const string &name)
	{
		set<Namespace> nses = d.namespaces();
		for (set<Namespace>::iterator nses_i = nses.begin(); nses_i != nses.end(); nses_i++)
			if ((string)(nses_i->name()) == name) return *nses_i;
		
		return Namespace();
	};

// find a namespace by name in a Namespace
	UDM_DLL Namespace namespaceByName(const Namespace &ns, const string &name)
	{
		set<Namespace> nses = ns.namespaces();
		for (set<Namespace>::iterator nses_i = nses.begin(); nses_i != nses.end(); nses_i++)
			if ((string)(nses_i->name()) == name) return *nses_i;
		
		return Namespace();
	};

// find a namespace by path
	UDM_DLL Namespace namespaceByPath(const Diagram &d, const string &path, const string &delim)
	{
		string::size_type i = path.rfind(delim);
		if (i == string::npos)
			return namespaceByName(d, path);

		Namespace p_ns;
		string ns_path = path;
		while(!path.empty()) {
			string ns_name;
			i = ns_path.find(delim);
			if (i == string::npos) {
				ns_name = ns_path;
				ns_path.clear();
			} else {
				ns_name = ns_path.substr(0, i);
				ns_path = ns_path.substr(i + delim.size());
			}

			Namespace ns = p_ns ? namespaceByName(p_ns, ns_name) : namespaceByName(d, ns_name);

			if (ns_path.empty() || !ns)
				return ns;

			p_ns = ns;
		}

		return Namespace();
	};

// find a diagram by name
	UDM_DLL Diagram diagramByName(const Diagram &d, const string &name)
	{
		::Udm::DataNetwork *dn = d.__impl()->__getdn();
		::Udm::UdmProject *pr = dn->GetProject();

		if (pr)
		{
			vector<Udm::DataNetwork*> pr_dns = pr->GetDataNetworks();
			vector<Udm::DataNetwork*>::iterator pr_dns_i = pr_dns.begin();
			while (pr_dns_i != pr_dns.end())
			{
				//these should be UML diagrams
				::Uml::Diagram dgr = ::Uml::Diagram::Cast((*pr_dns_i)->GetRootObject());
				if ((string) dgr.name() == name)
					return dgr;
				pr_dns_i++;
			}
		}

		return Diagram();
	}

	UDM_DLL Diagram GetDiagram(const Class &c)
	{
		Diagram dgr = c.parent();
		if (dgr)
			return dgr;
		else
			return GetDiagram(c.parent_ns());
	}

	UDM_DLL Diagram GetDiagram(const Association &assoc)
	{
		Diagram dgr = assoc.parent();
		if (dgr)
			return dgr;
		else
			return GetDiagram(assoc.parent_ns());
	}

	UDM_DLL Diagram GetDiagram(const Composition &comp)
	{
		Diagram dgr = comp.parent();
		if (dgr)
			return dgr;
		else
			return GetDiagram(comp.parent_ns());
	}

	UDM_DLL Diagram GetDiagram(const Namespace &ns)
	{
		Diagram dgr = ns.parent();
		Namespace p_ns = ns.parent_ns();
		while (!dgr) {
			dgr = p_ns.parent();
			p_ns = p_ns.parent_ns();
			UDM_ASSERT(dgr || p_ns);
		}
		return dgr;
	}


	UDM_DLL vector<Namespace> GetParentNamespaces(const Namespace &ns)
	{
		vector<Namespace> ret;
		Namespace p_ns = ns.parent_ns();
		while (p_ns) {
			ret.push_back(p_ns);
			p_ns = p_ns.parent_ns();
		}

		return vector<Namespace>(ret.rbegin(), ret.rend());
	}

	UDM_DLL const Class & SafeTypeContainer::GetSafeType(const Class &a)
	{
		//check if we already have a type equivalent with this
		::Udm::DataNetwork *dn = a.__impl()->__getdn();
		type_ext_id_t id(dn ? dn->uniqueId() : 0, a.uniqueId());
		type_map_t::const_iterator i = Udm::_UdmStaticData.type_map.find(id);
		if (i == Udm::_UdmStaticData.type_map.end())
		{

			//we don't have an equivalent type, so we create one
			//I believe this must be referenced, since we create an object from a pointer
			//make sure we don't mix the references to ObjectImpl's and references to ClassS ;-)


			const Class * new_type = new Class(a.__impl()->clone());

			//insert in the type_map with it's unique ID as key,
			//so we can find it next time
			type_map_t_item map_item(id, new_type);
			type_map_t_ires m_ins_res = Udm::_UdmStaticData.type_map.insert(map_item);
			if (!m_ins_res.second) throw udm_exception("Could not insert new type in type_map!");

			//insert in reference map
			//with 1 as initial reference count value
			ref_map_t_item ref_item(new_type, 1);
			ref_map_t_ires ref_res = Udm::_UdmStaticData.ref_map.insert(ref_item);
			if (!ref_res.second) throw udm_exception("Cound not insert new item in references map!");


			//return a reference
			return *new_type;
		}
		else
		{
			const Class * found = (*i).second;

			ref_map_t::iterator ref_i = Udm::_UdmStaticData.ref_map.find(found);
			if (ref_i == Udm::_UdmStaticData.ref_map.end())
				throw udm_exception("Pointer found in st_map was not found in references map!");
			(*ref_i).second++;



			return *found;
		}
	}

	UDM_DLL void SafeTypeContainer::RemoveSafeType(const Class &a )
	{
		//if (!dynamic_cast<UdmStatic::StaticObject*>(a.__impl())) {
			// FIXME: uniqueIds are not guaranteed to be unique across datanetworks
		//}
		//since we will invoke this in every static Object  destructor.
		// it might happen the type of a StaticObect was not obtained
		// through this class

		//we first check our reference map
		//if it's found there, it means that this type was obtained through this class

		const Class * pointer = &a;
		ref_map_t::iterator ref_i = Udm::_UdmStaticData.ref_map.find(pointer);
		if (ref_i != Udm::_UdmStaticData.ref_map.end())
		{
			(*ref_i).second--;			//decrement the counter

			if ((*ref_i).second == 0)
			{

				// now we have to find it in the other map too and delete from both
				// we can't use pointer->uniqueId() here: if it's a closed Dom or Gme backend, uniqueId() will crash
				type_map_t::iterator i;
				for (i = Udm::_UdmStaticData.type_map.begin(); i != Udm::_UdmStaticData.type_map.end(); i++) {
					if (i->second == pointer) {
						break;
					}
				}
				if (i == Udm::_UdmStaticData.type_map.end())
				{
					//we can not throw exception here 
					//because it might be called on main()-s '}' 
					//so we just complain about this
					cout << " SafeTypeContainer: Fatal error, corrupt maps. Object found in reference map was not found in type map. Likely to crash from now on... " << endl;
				} else
					Udm::_UdmStaticData.type_map.erase(i);
				Udm::_UdmStaticData.ref_map.erase(ref_i);
				delete pointer;
			}


		}
	}

	UDM_DLL bool SafeTypeContainer::IsSafeType(const Class &a)
	{
		const Class * pointer = &a;
		ref_map_t::iterator ref_i = Udm::_UdmStaticData.ref_map.find(pointer);
		return  (ref_i != Udm::_UdmStaticData.ref_map.end());
	}


	UDM_DLL std::string GetClassPath(const Uml::Class& c, std::string separator) {
		std::vector<std::string> names;
		{
			Udm::Object o = c;
			while (o != Udm::null) {
				if (o.type() == Uml::Namespace::meta) {
					names.push_back(o.__impl()->getStringAttr(Uml::Namespace::meta_name));
				//} else if (o.type() == Uml::Diagram::meta) {
				//	names.push_back(o.__impl()->getStringAttr(Uml::Namespace::meta_name));
				} else if (o.type() == Uml::Class::meta) {
					names.push_back(o.__impl()->getStringAttr(Uml::Class::meta_name));
				}
				o = o.GetParent();
			}
		}
		std::string ret = "";
		std::vector<std::string>::reverse_iterator nameIt = names.rbegin();
		std::vector<std::string>::reverse_iterator end = names.rend();
		if (nameIt < end) {
			ret += *nameIt;
			nameIt++;
		}
		while (nameIt < end) {
			ret += separator + *nameIt++;
		}
		return ret;
	}
}
