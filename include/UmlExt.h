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
=========
	11/24/05	-	endre

					Added GetClassFromCrossDgr, which returns the corresponding class from the cross diagram

	12/06/04	-	endre

					Added
					
					  UDM_DLL void GetChildRoleChain(const Class & origin, const Class &what, vector<ChildRoleChain> &chains, ChildRoleChain curr_chain = ChildRoleChain());

					which returns the possible containments paths of class what in class origin, at any level
					

	23/11/04	-	endre
		
					Added getPath to Udm::Object.

	07/11/04	-	endre

			-		Addd two functions, HasTextAttributes and TextAttributes for checking the existence and for getting
					all the text attributes from a class.

	19/09/04	-	endre

			-		Added new function GetAncestorAssociation, which removes the only (if any) Association
					attached to a Class.

    14/07/04	-	endre

			-		Changed SetChildRole, setParentRole to include both role names as key.
					There could be a composition relationship between the same two classes and
					the same rolenames at one end. These two associations could not be differentiated
					by the target classes and one rolename.

	05/24/04	-	endre

			-		splitted SetGenericRole in 3 functions, SetChildRole, SetParentRole and SetAssocRole.
					
	05/06/04	-	endre

			-		added SetDiagram(), SetClass(), SetGenericRole() classes. They search and set a target diagram/class/role
					out from an existing diagram based on names and role targets.

	04/13/04	-	endre

		Added SafeTypeContainer::IsSafeType(const Class &a)
		to indicate if a Class was obtained through SafeTypeContainer.


	24/02/04	-	endre
		
		Extended InitDiagram() with version information
*/


#ifndef MOBIES_UMLEXT_H
#define MOBIES_UMLEXT_H

#ifndef MOBIES_UML_H
#include "Uml.h"
#endif

#include <stack>

namespace Uml
{
	using namespace std;
// Get the other end of two-legged Uml classes 
	UDM_DLL const AssociationRole theOther(const AssociationRole &role);
	UDM_DLL const CompositionChildRole theOther(const CompositionParentRole &role);
	UDM_DLL const CompositionParentRole theOther(const CompositionChildRole &role);

// find an assoc.Class by name in a Namespace
	UDM_DLL Class assocClassByName(const Namespace &ns, const string &name);
// find an assoc.Class by name in a Diagram
	UDM_DLL Class assocClassByName(const Diagram &d, const string &name);

// find a class by name in a Diagram and Namespace given by path ns_path
	UDM_DLL Class classByName(const Diagram &d, const string &ns_path, const string &name, const string &delim = "::");
// find a class by name in a Diagram
	UDM_DLL Class classByName(const Diagram &d, const string &name);
// find a class by name in a Namespace
	UDM_DLL Class classByName(const Namespace &ns, const string &name);
// find a class by path
	UDM_DLL Class classByPath(const Diagram &d, const string &path, const string &delim = "::");

// find association by name in a Namespace
	UDM_DLL Association associationByName(const Namespace &ns, const string &name);
// find association by name in a Diagram
	UDM_DLL Association associationByName(const Diagram &d, const string &name);

// find composition by name in a Namespace
	UDM_DLL Composition compositionByName(const Namespace &ns, const string &name);
// find composition by name in a Diagram
	UDM_DLL Composition compositionByName(const Diagram &d, const string &name);

// find a diagram by name
	UDM_DLL Diagram diagramByName(const Diagram &d, const string &name);

// find a namespace by name
	UDM_DLL Namespace namespaceByName(const Diagram &d, const string &name);
// find a namespace by name in a Namespace
	UDM_DLL Namespace namespaceByName(const Namespace &ns, const string &name);
// find a namespace by path
	UDM_DLL Namespace namespaceByPath(const Diagram &d, const string &path, const string &delim = "::");

// Get all the classes specified as ancestors, including self
	UDM_DLL set<Class> AncestorClasses(const Class &c);
// Get all the classes specified as descendants, including self
	UDM_DLL set<Class> DescendantClasses(const Class &c);
// Get classes directly specified for this class as parents  (both ancestors and descendants are ignored)
	UDM_DLL set<Class> ContainerClasses(const Class &c);
// Get classes directly specified for this class as children  (both ancestors and descendants are ignored)
	UDM_DLL set<Class> ContainedClasses(const Class &c);
//for association classes, there is only one attached association,
//but it can be either directly attached or inherited from a base association class
//returns NULL if the class does not have an Association attached
	UDM_DLL Association GetAncestorAssociation(const Class &c);

// Get classes that are ancestors of all the classes in a set
	UDM_DLL set<Class> CommonAncestorClasses(const set<Class> &cs);

// Get classes this object or its ancestors specify as parents (ancestors in child are extracted, decendants of parents are ignored)
	UDM_DLL set<Class> AncestorContainerClasses(const Class &c);
// Get classes this object or its ancestors specify as children (ancestors in parent are extracted, decendants of children are ignored)
	UDM_DLL set<Class> AncestorContainedClasses(const Class &c);
// Get classes this object can contain (ancestors in parent and descendants in children are extracted)
	UDM_DLL set<Class> AncestorContainedDescendantClasses(const Class &c);

	// All the other ends of associations (ancestors are ignored)
	UDM_DLL set<AssociationRole> AssociationTargetRoles(const Class &c);
	// All the other ends of associations this class can have (including those defined in ancestors)
	UDM_DLL set<AssociationRole> AncestorAssociationTargetRoles(const Class &c);

	// All the other ends of associations this class can have (including those defined in ancestors)
	UDM_DLL set<AssociationRole> AncestorCrossAssociationTargetRoles(const Class &c, const Diagram & cross_dgr);

	// All local ends of associations this class can have (including those defined in ancestors)
	UDM_DLL set<AssociationRole> AncestorAssociationRoles(const Class &c);

	// All local ends of associations this class can have (including those defined in ancestors)
	UDM_DLL set<AssociationRole> AncestorCrossAssociationRoles(const Class &c, const Diagram & cross_dgr);

// Returns true if the targets and the association class (if it exists) do not belong to the same namespace
	UDM_DLL bool IsCrossNSAssociation(const Association &a);

	// returns all the Text Attributes for class C
	UDM_DLL set<Attribute> TextAttributes(const Class &c);

	//returns true if class c has any Text Attribute (should be faster:)
	UDM_DLL bool HasTextAttributes(const Class &c);


// All the other ends of of compositions defined for this class as child (ancestors are ignored)
	UDM_DLL set<CompositionParentRole> CompositionPeerParentRoles(const Class &c);
// All the parent ends of compositions this class can participate in (including those defined for ancestors)
	UDM_DLL set<CompositionParentRole> AncestorCompositionPeerParentRoles(const Class &c);

// All the child ends of compositions defined for this class as parent (ancestors are ignored)
	UDM_DLL set<CompositionChildRole> CompositionPeerChildRoles(const Class &c);
// All the child ends of compositions this class can participate in (including those defined for ancestors)
	UDM_DLL set<CompositionChildRole> AncestorCompositionPeerChildRoles(const Class &c);

// Returns true if the parent and the child of this composition are in different namespaces
	UDM_DLL bool IsCrossNSComposition(const Composition &c);
	
// All attributes this class can have (including those defined in ancestors)
	UDM_DLL set<Attribute> AncestorAttributes(const Class &c);

// find the single way a class can be contained by another, return NULL none or if multiple roles are found
    
	UDM_DLL Composition matchChildToParent(Class c, Class p);
    
    // addtionally, names of the child and parent roles can be provided to constraint the possible compositions.
	UDM_DLL Composition matchChildToParent(Class c, Class p, const char * crole, const char * prole = NULL);
    
    // addtionally, return all the possible Compositions names of the child and parent roles can be provided to constraint the possible compositions.
	UDM_DLL set<Composition> compositionsChildToParent(Class c, Class p, const char * crole, const char * prole = NULL);
    
    //finds the only suitable AssociationRole to reach 'target_class' or 'assoc_cls' from c for the association with which has the name 'rolename' on C's side
    //if target_aclass is provided than peer_type is ignored, and only those assoc. roles are considered which are in an assoc. cls. based assoc.
    //rolename can be NULL. if not null, the results will be filtered against the rolename as well
    //multiple or no result will return null object.
    
    UDM_DLL AssociationRole matchPeerToPeer(Class c, Class target_class, Class target_aclass, const char * rolename = NULL);

    // returns true if derived = base
	UDM_DLL bool IsDerivedFrom(const Class &derived, const Class &base);
	UDM_DLL bool IsAssocClass(const Class &cl);
	UDM_DLL bool IsAssocClass(const Association &ass);


	UDM_DLL string MakeRoleName(const GenericRole &r);
	UDM_DLL string MakeShortRoleName(const GenericRole &r);

//returns true if childrole's target has more than one non-abstract derivate, including itself
	UDM_DLL bool HasChildRoleMultipleTargets(const CompositionChildRole & ccr);

	typedef vector<CompositionChildRole> ChildRoleChain;
	//fills the vector chains with the possible containments paths of class what in class origin, at any level; returns true if successful, false if a loop was detected
	UDM_DLL bool GetChildRoleChain(const Class & origin, const Class &what, vector<ChildRoleChain> &chains, ChildRoleChain curr_chain = ChildRoleChain());

	UDM_DLL Namespace GetTheOnlyNamespace(const Diagram & dgr);

	// get the corresponding class from the cross diagram
	UDM_DLL Class GetClassFromCrossDgr(const Diagram &cross_dgr, const Class &cl);
    //determines whether a class is the child end of a composition with the parent end in other namespace
    UDM_DLL bool IsCrossNSCompositionChildEnd(const ::Uml::Class &c);
    //return the classes that are
    //not contained
    //or it's only container is itself or an abstract superclass
    // or it's the child end of a composition with the parent end in other namespace

    UDM_DLL vector< ::Uml::Class> findNonContainedClasses(const ::Uml::Diagram & dgr);
    


	UDM_DLL Diagram GetDiagram(const Class &c);
	UDM_DLL Diagram GetDiagram(const Association &assoc);
	UDM_DLL Diagram GetDiagram(const Composition &comp);
	UDM_DLL Diagram GetDiagram(const Namespace &ns);

	UDM_DLL vector<Namespace> GetParentNamespaces(const Namespace &ns);

	template<class CONTAINER, class CONTAINED>
	class ElemCollection {
	public:
		typedef Udm::ChildrenAttr<CONTAINED> (CONTAINER::*GetTopItems)() const;
		typedef Udm::ChildrenAttr<CONTAINED> (Namespace::*GetItems)() const;

		ElemCollection(const CONTAINER &container, GetTopItems getTopItems, GetItems getItems) : m_container(container) {
			m_top_items_getter = getTopItems;
			m_items_getter = getItems;
		};

		class iterator : public std::iterator<std::input_iterator_tag, CONTAINED> {
		public:
			iterator(const CONTAINER &container, GetTopItems getTopItems = NULL, GetItems getItems = NULL) {
				if (container) {
					m_items = (container.*getTopItems)();
					m_items_i = m_items.begin();
					m_namespaces = container.namespaces();
					m_namespaces_i = m_namespaces.begin();
					m_end_reached = false;
					m_items_getter = getItems;

					if (m_items_i == m_items.end() && !find_next())
						m_end_reached = true;
				} else
					m_end_reached = true;
			}

			iterator& operator=(const iterator& other) {
				if (!other.m_end_reached) {
					m_items = other.m_items;
					m_items_i = m_items.find(*other.m_items_i);
					m_namespaces = other.m_namespaces;
					m_namespaces_i = m_namespaces.find(*other.m_namespaces_i);
					m_namespaces_stack = other.m_namespaces_stack;
				}
				m_end_reached = other.m_end_reached;

				return *this;
			}

			bool operator==(const iterator& other) { return m_end_reached == other.m_end_reached; }
			bool operator!=(const iterator& other) { return m_end_reached != other.m_end_reached; }

			iterator& operator++() {
				if (!m_end_reached) {
					m_items_i++;
					if (m_items_i == m_items.end() && !find_next())
						m_end_reached = true;
				}

				return *this;
			}
			iterator& operator++(int) {
				++(*this);
				return *this;
			}
			CONTAINED& operator*() const {
				return (CONTAINED &) *m_items_i;
			}

			CONTAINED* operator->() {
				return &**this;
			}

		protected:
			stack< pair< set<Namespace>, Namespace > > m_namespaces_stack;
			set<Namespace> m_namespaces;
			set<Namespace>::const_iterator m_namespaces_i;
			set<CONTAINED> m_items;
			typename set<CONTAINED>::const_iterator m_items_i;
			bool m_end_reached;
			GetItems m_items_getter;

			bool find_next() {
				if (m_namespaces_i != m_namespaces.end()) {
					m_namespaces_stack.push(make_pair(set<Namespace>(m_namespaces), *m_namespaces_i));

					m_items = (*m_namespaces_i.*m_items_getter)();
					m_items_i = m_items.begin();

					m_namespaces = m_namespaces_i->namespaces();
					m_namespaces_i = m_namespaces.begin();

					if (m_items_i != m_items.end())
						return true;
					return find_next();
				}

				if (m_namespaces_stack.size() > 0) {
					const pair< set<Namespace>, Namespace > &p = m_namespaces_stack.top();
					m_namespaces = p.first;
					m_namespaces_i = m_namespaces.find(p.second);
					m_namespaces_i++;

					m_namespaces_stack.pop();

					return find_next();
				}

				return false;
			}
		};

		iterator begin() { return iterator(m_container, m_top_items_getter, m_items_getter); };
		iterator end() { return iterator(NULL); };

	protected:
		const CONTAINER &m_container;
		GetTopItems m_top_items_getter;
		GetItems m_items_getter;
	};

	// n.b. _kind_children is more performant with Dom backend
	class UDM_DLL DiagramAssociations : public ElemCollection<Diagram, Association> {
	public:
		DiagramAssociations(const Diagram &diagram) : ElemCollection<Diagram, Association>(diagram, &Diagram::Association_kind_children, &Namespace::Association_kind_children) {}
	};

	class UDM_DLL DiagramClasses : public ElemCollection<Diagram, Class> {
	public:
		DiagramClasses(const Diagram &diagram) : ElemCollection<Diagram, Class>(diagram, &Diagram::Class_kind_children, &Namespace::Class_kind_children) {}
	};

	class UDM_DLL DiagramCompositions : public ElemCollection<Diagram, Composition> {
	public:
		DiagramCompositions(const Diagram &diagram) : ElemCollection<Diagram, Composition>(diagram, &Diagram::Composition_kind_children, &Namespace::Composition_kind_children) {}
	};

	class UDM_DLL NamespaceAssociations : public ElemCollection<Namespace, Association> {
	public:
		NamespaceAssociations(const Namespace &ns) : ElemCollection<Namespace, Association>(ns, &Namespace::Association_kind_children, &Namespace::Association_kind_children) {}
	};

	class UDM_DLL NamespaceClasses : public ElemCollection<Namespace, Class> {
	public:
		NamespaceClasses(const Namespace &ns) : ElemCollection<Namespace, Class>(ns, &Namespace::Class_kind_children, &Namespace::Class_kind_children) {}
	};

	class UDM_DLL NamespaceCompositions : public ElemCollection<Namespace, Composition> {
	public:
		NamespaceCompositions(const Namespace &ns) : ElemCollection<Namespace, Composition>(ns, &Namespace::Composition_kind_children, &Namespace::Composition_kind_children) {}
	};

	template<class CONTAINER>
	class NamespaceCollection {
	public:
		NamespaceCollection(const CONTAINER &container) : m_container(container) {}

		class iterator : public std::iterator<std::input_iterator_tag, Namespace> {
		public:
			iterator(const CONTAINER &container) {
				if (container) {
					m_namespaces = container.namespaces();
					m_namespaces_i = m_namespaces.begin();
					m_end_reached = false;

					if (m_namespaces_i == m_namespaces.end())
						m_end_reached = true;
				} else
					m_end_reached = true;
			}

			iterator& operator=(const iterator& other) {
				if (!other.m_end_reached) {
					m_namespaces = other.m_namespaces;
					m_namespaces_i = m_namespaces.find(*other.m_namespaces_i);
					m_namespaces_stack = other.m_namespaces_stack;
				}
				m_end_reached = other.m_end_reached;

				return *this;
			}

			bool operator==(const iterator& other) { return m_end_reached == other.m_end_reached; }
			bool operator!=(const iterator& other) { return m_end_reached != other.m_end_reached; }
			iterator& operator++() {
				if (!m_end_reached) {
					if (!find_next())
						m_end_reached = true;
				}

				return *this;
			}
			iterator& operator++(int) {
				++(*this);
				return *this;
			}
			Namespace& operator*() const {
				return (Namespace &) *m_namespaces_i;
			}

			Namespace* operator->() {
				return &**this;
			}

		protected:
			stack< pair< set<Namespace>, Namespace > > m_namespaces_stack;
			set<Namespace> m_namespaces;
			set<Namespace>::const_iterator m_namespaces_i;
			bool m_end_reached;

			bool find_next() {
				if (m_namespaces_i != m_namespaces.end()) {
					m_namespaces_stack.push(make_pair(set<Namespace>(m_namespaces), *m_namespaces_i));

					m_namespaces = m_namespaces_i->namespaces();
					m_namespaces_i = m_namespaces.begin();

					if (m_namespaces_i != m_namespaces.end())
						return true;
					return find_next();
				}

				if (m_namespaces_stack.size() > 0) {
					pair< set<Namespace>, Namespace > &p = m_namespaces_stack.top();
					m_namespaces = p.first;
					m_namespaces_i = m_namespaces.find(p.second);
					m_namespaces_i++;

					m_namespaces_stack.pop();

					if (m_namespaces_i != m_namespaces.end())
						return true;
					return find_next();
				}

				return false;
			}
		};

		iterator begin() { return iterator(m_container); };
		iterator end() { return iterator(NULL); };

	protected:
		const CONTAINER &m_container;
	};

	class UDM_DLL DiagramNamespaces : public NamespaceCollection<Diagram> {
	public:
		DiagramNamespaces(const Diagram &diagram) : NamespaceCollection<Diagram>(diagram) {}
	};

	class UDM_DLL NamespaceNamespaces : public NamespaceCollection<Namespace> {
	public:
		NamespaceNamespaces(const Namespace &ns) : NamespaceCollection<Namespace>(ns) {}
	};


// Safe type mechanism

	class SafeTypeContainer
	{
		/*
			safe-type mechanism for StaticObject's
			their m_type variable is a reference
			so there need to be a way 
			which would guarantee that the referenced Class objects
			are not freed until th referees

		*/
		typedef Udm::ObjectImpl::uniqueId_type uniqueId_type;
		
		// datanetwork uniqueId, object uniqueId
		typedef pair<unsigned long, uniqueId_type>		type_ext_id_t;

		typedef map<type_ext_id_t, const Class *>		type_map_t;
		typedef pair<type_ext_id_t const, const Class *>	type_map_t_item;
		typedef pair<type_map_t::iterator, bool>				type_map_t_ires;


		typedef map<const Class *, long>					ref_map_t;
		typedef pair<const Class * const, long>			ref_map_t_item;
		typedef pair<ref_map_t::iterator, bool>			ref_map_t_ires;

		


		
public:
		
		static UDM_DLL const Class & GetSafeType(const Class &a); 
		static UDM_DLL void RemoveSafeType(const Class &a );
		static UDM_DLL bool IsSafeType(const Class &a);
	};


// --------------------------- Construction

UDM_DLL Diagram CreateDiagram();
UDM_DLL Namespace CreateNamespace();
UDM_DLL Class CreateClass();
UDM_DLL Attribute CreateAttribute();
UDM_DLL Association CreateAssociation();
UDM_DLL AssociationRole CreateAssociationRole();
UDM_DLL Composition CreateComposition();
UDM_DLL CompositionParentRole CreateCompositionParentRole();
UDM_DLL CompositionChildRole CreateCompositionChildRole();
UDM_DLL Constraint CreateConstraint();
UDM_DLL ConstraintDefinition CreateConstraintDefinition();

// ---------------------------- Static Meta Initialization

UDM_DLL void InitDiagram(const Diagram &obj, const char *name, const char * version = "1.00");
UDM_DLL void InitDiagramProps(const Diagram &obj, const char *name, const char * version = "1.00");
UDM_DLL void InitNamespace(const Namespace &obj, const Diagram &parent, const char *name);
UDM_DLL void InitNamespace(const Namespace &obj, const Namespace &parent, const char *name);
UDM_DLL void InitNamespaceProps(const Namespace &obj, const char *name);
UDM_DLL void InitClass(const Class &obj, const Namespace &parent, const char *name, bool isAbstract, const char *stereo = NULL, const char * from = NULL);
UDM_DLL void InitClass(const Class &obj, const Diagram &parent, const char *name, bool isAbstract, const char *stereo = NULL, const char * from = NULL);
UDM_DLL void InitClassProps(const Class &obj, const char *name, bool isAbstract, const char *stereo = NULL, const char * from = NULL);
UDM_DLL void InitAttribute(const Attribute &obj, const Class &parent, const char *name, const char *type, bool np, bool reg, int min, int max, const bool ordered, const string& visibility, const vector<string> & defval = vector<string>());
UDM_DLL void InitAttributeProps(const Attribute &obj, const char *name, const char *type, bool np, bool reg, int min, int max, const bool ordered, const string& visibility, const vector<string> & defval = vector<string>());

UDM_DLL void InitAssociation(const Association &obj, const Namespace &parent, const char *name);
UDM_DLL void InitAssociation(const Association &obj, const Diagram &parent, const char *name);
UDM_DLL void InitAssociationProps(const Association &obj, const char *name);
UDM_DLL void InitAssociationClass(const Association &aobj, const Class &cobj);
UDM_DLL void InitAssociationRole(const AssociationRole &obj, const Association &parent, const char *name, bool navigable, bool primary, long min, long max, const Class &target);
UDM_DLL void InitAssociationRoleProps(const AssociationRole &obj, const char *name, bool navigable, bool primary, long min, long max);
UDM_DLL void InitComposition(const Composition &obj, const Namespace &parent, const char *name, bool np = false);
UDM_DLL void InitComposition(const Composition &obj, const Diagram &parent, const char *name, bool np = false);
UDM_DLL void InitCompositionProps(const Composition &obj, const char *name, bool np = false);
UDM_DLL void InitCompositionParentRole(const CompositionParentRole &obj, const Composition &parent, const char *name, bool navigable, const Class &target);
UDM_DLL void InitCompositionParentRoleProps(const CompositionParentRole &obj, const char *name, bool navigable);
UDM_DLL void InitCompositionChildRole(const CompositionChildRole &obj, const Composition &parent, const char *name, bool navigable, long min, long max, const Class &target);
UDM_DLL void InitCompositionChildRoleProps(const CompositionChildRole &obj, const char *name, bool navigable, long min, long max);
UDM_DLL void InitConstraint(const Constraint &obj, const Class &parent, const char * name, const char * description, const char * expression);
UDM_DLL void InitConstraintProps(const Constraint &obj, const char * name, const char * description, const char * expression);
UDM_DLL void InitConstraintDefinition(const ConstraintDefinition &obj, const Class &parent, const char * name, const char * stereotype, const char * parameterList, const char * expression, const char * returnType);
UDM_DLL void InitConstraintDefinitionProps(const ConstraintDefinition &obj, const char * name, const char * stereotype, const char * parameterList, const char * expression, const char * returnType);
UDM_DLL void AddInheritance(const Class &baseType, const Class &subType);

	

// --------------------------- Static Corba Initialization 

Diagram CreateCORBADiagram();
Namespace CreateCORBANamespace();
Class CreateCORBAClass();
Attribute CreateCORBAAttribute();
Association CreateCORBAAssociation();
AssociationRole CreateCORBAAssociationRole();
Composition CreateCORBAComposition();
CompositionParentRole CreateCORBACompositionParentRole();
CompositionChildRole CreateCORBACompositionChildRole();

void InitCORBADiagram(const Diagram &obj, const char *name, const char * version = "1.00");
void InitCORBANamespace(const Namespace &obj, const Diagram &parent, const char *name);
void InitCORBANamespace(const Namespace &obj, const Namespace &parent, const char *name);
void InitCORBAClass(const Class &obj, const Diagram &parent, const char *name, bool isAbstract, const char *stereo = NULL);
void InitCORBAClass(const Class &obj, const Namespace &parent, const char *name, bool isAbstract, const char *stereo = NULL);
void InitCORBAAttribute(const Attribute &obj, const Class &parent, const char *name, const char *type, bool np, int min, int max, const bool ordered, const string& visibility, const vector<string> & defval = vector<string>());
void InitCORBAAssociation(const Association &obj, const Diagram &parent, const char *name);
void InitCORBAAssociation(const Association &obj, const Namespace &parent, const char *name);
void InitCORBAAssociationClass(const Association &aobj, const Class &cobj);
void InitCORBAAssociationRole(const AssociationRole &obj, const Association &parent, const char *name, bool navigable, bool primary, long min, long max, const Class &target);
void InitCORBAComposition(const Composition &obj, const Diagram &parent, const char *name);
void InitCORBAComposition(const Composition &obj, const Namespace &parent, const char *name);
void InitCORBACompositionParentRole(const CompositionParentRole &obj, const Composition &parent, const char *name, bool navigable, const Class &target);
void InitCORBACompositionChildRole(const CompositionChildRole &obj,const Composition &parent, const char *name, bool navigable, long min, long max, const Class &target);
void AddCORBAInheritance(const Class &baseType, const Class &subType);


// ---------------------------- Dynamic Meta Initialization
// used to set the static API member variables to 

	UDM_DLL void SetNamespace(Namespace &what, const Namespace &what_ns, const char *target_name);
	UDM_DLL void SetNamespace(Namespace &what, const Diagram &what_dgr, const char *target_name);
	UDM_DLL void SetClass(Class &what, const Namespace &what_ns, const char *target_name);
	UDM_DLL void SetClass(Class &what, const Diagram &what_dgr, const char *target_name);
	UDM_DLL void SetAttribute(Attribute &what, Class &what_class,  const char *target_name);
	UDM_DLL void SetConstraint(Constraint &what, Class &what_class,  const char *target_name);
	UDM_DLL void SetConstraintDefinition(ConstraintDefinition &what, Class &what_class,  const char *target_name);

	UDM_DLL void SetChildRole(CompositionChildRole &what, Class &what_target_class, Class &what_theo_target_class, const char *role_name, const char * orole_name);
	UDM_DLL void SetParentRole(CompositionParentRole &what, Class &what_target_class, Class &what_theo_target_class, const char *role_name, const char * orole_name);
	UDM_DLL void SetAssocRole(AssociationRole &what, Class &what_target_class, Class &what_theo_target_class, const char *target_name);

	UDM_DLL std::string GetClassPath(const Uml::Class& c, std::string separator = "::");
}

#endif//MOBIES_UMLEXT_H

