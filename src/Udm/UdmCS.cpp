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

#include "Uml.h"
#include "UmlExt.h"
#include <map>
#include "Udm.h"

using namespace Uml;
// C# generators

void GenerateCSApi(const Uml::Diagram &diagram,  ostream &output, string fname)
{
	string hname = diagram.name();
	string::iterator i;
	for(i = hname.begin(); i != hname.end(); i++) 
	{
		if(!isalnum(*i)) *i = '_';
	}
	output << "// CS API file " << fname << ".cs generated from diagram " << (string)diagram.name() << endl;
	output << "// generated on " << GetTime().c_str() << endl << endl;

	output << "using System;"	<< endl;
	output << "using System.Collections;"	<< endl <<endl << endl;
	output << "namespace " << hname << endl << "{" << endl << endl;

	set<Uml::Class> classes = diagram.classes();
	set<Uml::Class>::iterator c;

	for ( c = classes.begin(); c!= classes.end(); c++)
	{
		Uml::Class cl = *c;
		set<Uml::Class> bases = cl.baseTypes();
		
		if (bases.size() > 1) 
			throw udm_exception("C# UDM does not support multiple inheritence!");
		
		bool derived = (bases.size() > 0);
		
		
		//class declaration, inheritence
		if (derived)
		{
			Uml::Class base = *(bases.begin());
			output <<"\tpublic class " << (string)cl.name() << " : " << (string)base.name() << endl;
		}
		else
			output <<"\tpublic class " << (string) cl.name() << " : " << "Udm.Object" << endl;

		string cn = (string)cl.name();

		//static meta field
		output << "\t{" << endl;;
		if (derived)
			output <<"\t\tpublic static new Uml.Class meta;"<<endl;
		else
			output <<"\t\tpublic static Uml.Class meta;"<<endl;

		//static NULL field
		output <<"\t\tpublic static readonly new " << cn << " NULL = new " << cn << "();" <<endl;
		output << endl;

		//constructors
		output <<"\t\tpublic " << cn << "() {}" << endl;
		output <<"\t\tpublic " << cn << "(Udm.ObjectImpl impl) : base(impl) {}" <<endl;
		output <<"\t\tpublic " << cn << "(" << cn <<" master) : base(master) {}" <<endl;
		output << endl;
		
		//creators
		if (derived)
			output <<"\t\tpublic static new " << cn  << " Create(Udm.Object parent)" << endl;
		else
			output <<"\t\tpublic static " << cn  << " Create(Udm.Object parent)" << endl;

		output <<"\t\t{" <<endl;
		output <<"\t\t\treturn " << cn << ".Cast((Udm.Object.Create(ref meta, parent, Uml.CompositionChildRole.NULL)));" <<endl;
		output <<"\t\t}" <<endl;
		
		if (derived)
			output <<"\t\tpublic static new " << cn  << " Create(Udm.Object parent, Uml.CompositionChildRole ccr)" << endl;
		else
			output <<"\t\tpublic static " << cn  << " Create(Udm.Object parent, Uml.CompositionChildRole ccr)" << endl;

		output <<"\t\t{" <<endl;
		output <<"\t\t\treturn " << cn << ".Cast((Udm.Object.Create(ref meta, parent, ccr)));" <<endl;
		output <<"\t\t}" <<endl;
		
		//cast
		if (derived)
			output <<"\t\tpublic static new " << cn  << " Cast(Udm.Object o)" << endl;
		else
			output <<"\t\tpublic static " << cn  << " Cast(Udm.Object o)" << endl;

		output <<"\t\t{" <<endl;
		output <<"\t\t\tif (Uml.UmlExt.IsObjectDerivedFromClass(o, meta))" << endl;
		output <<"\t\t\t\treturn new " << cn << "(o.__impl());" << endl;
		output <<"\t\t\telse throw new Udm.udm_exception(\"Invalid cast!\");" << endl;
		output <<"\t\t}" <<endl;

		
		

		set<Uml::Attribute> attributes = cl.attributes();
		set<Uml::Attribute>::iterator a;
		for ( a = attributes.begin(); a != attributes.end(); a++)
		{
			string aname = (*a).name();
			string atype = (*a).type();
			string cs_atype;
			bool multiple = ((long)(*a).max() != 0) && ((long)(*a).max() != 1);

			if (atype == "Boolean") cs_atype = "bool";
			else if (atype == "Real") cs_atype = "double";
			else if (atype == "Integer") cs_atype = "long";
			else if (atype == "String") cs_atype = "string";
			else throw new udm_exception(string("Invalid attribute type: ") + atype);
			//check for the same attribute in ascendants
				
			output << "\t\tpublic static Uml.Attribute meta_" << aname << ";"<< endl;
			if (!multiple)
				output << "\t\tpublic " << cs_atype << " " << aname << endl;
			else
				output << "\t\tpublic " << cs_atype << "[] " << aname << endl;

			output << "\t\t{" << endl;
			if (!multiple)
			{
				output << "\t\t\tget {return impl.get" << atype << "Attribute(meta_" << aname <<");}" << endl;
				output << "\t\t\tset {impl.set" << atype << "Attribute(meta_"<< aname <<", value);}" <<endl;
			}
			else
			{
				output << "\t\t\tget {return impl.get" << atype << "Attributes(meta_" << aname <<");}" << endl;
				output << "\t\t\tset {impl.set" << atype << "Attributes(meta_"<< aname <<", value);}" <<endl;
			}
			output << "\t\t}" << endl;
			
		}

		output << endl;
		//assoc. roles
		set<Uml::AssociationRole> aroles = cl.associationRoles();
		set<Uml::AssociationRole>::iterator b_i;
		for (b_i = aroles.begin(); b_i != aroles.end(); b_i++)
		{
			Uml::AssociationRole arole = Uml::theOther((*b_i));
			string arname = MakeRoleName(arole);
			bool multiple = ((arole.max() != 1) && (arole.max() != 0));
			Uml::Class target = arole.target();
			string tname = target.name();
			

			
			Uml::Association parent = arole.parent();
			Uml::Class assocClass = parent.assocClass();

			
			if (assocClass)
				output << "\t\tpublic static Uml.AssociationRole meta_" << arname << ", meta_" <<arname <<"_rev ;" << endl;
			else
				output << "\t\tpublic static Uml.AssociationRole meta_" << arname << ";" << endl;
			
			if (!multiple)
			{
			
				
				output << "\t\tpublic " << tname << " " << arname << endl;
				output << "\t\t{" << endl;

				if (assocClass)
				{
					output << "\t\t\tget{ return (" << tname <<")(getPointer(meta_" << arname << ", \"" << diagram.name() << "\", Udm.UdmAssocMode.CLASSFROMTARGET));}" <<endl; 
					output << "\t\t\tset{ setPointer(meta_" << arname << ", value, Udm.UdmAssocMode.CLASSFROMTARGET);}" <<endl; 
				}
				else
				{
					output << "\t\t\tget{ return (" << tname <<")(getPointer(meta_" << arname << ", \"" << diagram.name() << "\", Udm.UdmAssocMode.TARGETFROMPEER));}" <<endl; 
					output << "\t\t\tset{ setPointer(meta_" << arname << ", value, Udm.UdmAssocMode.TARGETFROMPEER);}" <<endl; 
				}

				output << "\t\t}" << endl;

			}
			else
			{
				output << "\t\tpublic Hashtable " <<arname << endl;
				output << "\t\t{" << endl;
				if (assocClass)
				{
					output << "\t\t\tget{ return (getPointers(meta_" << arname << ", \"" << diagram.name() << "\", Udm.UdmAssocMode.CLASSFROMTARGET));}" <<endl; 
					output << "\t\t\tset{ setPointers(meta_" << arname << ", value, Udm.UdmAssocMode.CLASSFROMTARGET);}" <<endl; 

				}
				else
				{
					output << "\t\t\tget{ return (getPointers(meta_" << arname << ", \"" << diagram.name() << "\", Udm.UdmAssocMode.TARGETFROMPEER));}" <<endl; 
					output << "\t\t\tset{ setPointers(meta_" << arname << ", value, Udm.UdmAssocMode.TARGETFROMPEER);}" <<endl; 
				}
				output << "\t\t}" << endl;

			}
		}

		//assoc. class
		Uml::Association ass = cl.association();
		if (ass)
		{
			set<Uml::Class>::iterator bas_i;
			for (bas_i = bases.begin(); bas_i != bases.end(); bas_i++)
			{
				Uml::Association new_ass = bas_i->association();
				if (new_ass)
					throw udm_exception("An association class has two associations?");
			}
			set<Uml::AssociationRole> ass_roles = ass.roles();
			set<Uml::AssociationRole>::iterator ar_i;
			for (ar_i = ass_roles.begin(); ar_i != ass_roles.end(); ar_i++)
			{
				Uml::AssociationRole arole = *ar_i;
				Uml::Class target = arole.target();
				string tname   = target.name();
				string arname  = MakeRoleName(arole);
				
				output << "\t\tpublic static Uml.AssociationRole meta_" << arname << "_end_;" << endl;
				output << "\t\tpublic " << tname << " " << arname  << endl;
				output << "\t\t{" << endl;
				output << "\t\t\tget{ return (" << tname <<")(getPointer(meta_" << arname << "_end_, \"" << diagram.name() << "\", Udm.UdmAssocMode.TARGETFROMCLASS));}" <<endl; 
				output << "\t\t\tset{ setPointer(meta_" << arname << "_end_, value, Udm.UdmAssocMode.TARGETFROMCLASS);}" <<endl; 
				output << "\t\t}" << endl;

				output << endl;


			}

		}
		output << endl;
		set<Uml::Class> childrenkinds;
		//parentroles
		set<Uml::CompositionParentRole> cprs = cl.parentRoles();
		set<Uml::CompositionParentRole>::iterator c_i;
		for (c_i = cprs.begin(); c_i != cprs.end(); c_i++)
		{
			Uml::CompositionChildRole ccr = Uml::theOther(*c_i);
			string ccr_name = MakeRoleName(ccr);
				//ccr.name();
			string tname = ((Uml::Class)ccr.target()).name();
			
			childrenkinds.insert((Uml::Class)ccr.target());
			bool multiple = ((ccr.max() != 0 ) && (ccr.max() != 1 ));
			
			output << "\t\tpublic static Uml.CompositionChildRole meta_" << ccr_name << ";" <<endl;

			if (!multiple)
			{
				output << "\t\tpublic " << tname << " " << ccr_name << endl;
				output << "\t\t{" << endl;
				output << "\t\t\tget{ return (" << tname <<")(getChild(meta_" << ccr_name << ", Uml.Class.NULL, \"" << diagram.name() << "\"));}" <<endl; 
				output << "\t\t\tset{ setChild(meta_" << ccr_name << ", value);}" <<endl; 
				output << "\t\t}" << endl;
			}
			else
			{
				output << "\t\tpublic Hashtable " <<ccr_name << endl;
				output << "\t\t{" << endl;
				output << "\t\t\tget{ return (getChildren(meta_" << ccr_name << ", Uml.Class.NULL, \"" << diagram.name() << "\"));}" <<endl; 
				output << "\t\t\tset{ setChildren(meta_" << ccr_name << ", value);}" <<endl; 
				output << "\t\t}" << endl;

			}
		}
		//__kind_children calls
		set<Uml::Class>::iterator f_i, g_i;
		for (f_i = classes.begin(); f_i != classes.end(); f_i++)
		{
			for (g_i = childrenkinds.begin(); g_i != childrenkinds.end(); g_i++)
			{
				if (Uml::IsDerivedFrom(*g_i, *f_i) || Uml::IsDerivedFrom(*f_i, *g_i)) 
				{
					string kindname = f_i->name();
					output << endl;
					output << "\t\tpublic Hashtable " << kindname << "_kind_children" << endl;
					output << "\t\t{" << endl;
					output << "\t\t\tget{ return (getChildren(Uml.CompositionChildRole.NULL, " << kindname << ".meta, \"" << diagram.name() << "\"));}" <<endl; 
					//no setter for kind_children calls
					output << "\t\t}" << endl;
					break;
				}
			}
		}

		output << endl;
		
		//childroles
		set<Uml::CompositionChildRole> ccrs = cl.childRoles();
		set<Uml::CompositionChildRole>::iterator d_i;
		bool was_parent = false;

		for (d_i = ccrs.begin(); d_i != ccrs.end(); d_i++)
		{
			Uml::CompositionParentRole cpr = Uml::theOther(*d_i);
			string cpr_name = MakeRoleName(cpr);
			string tname = ((Uml::Class)cpr.target()).name();


			bool new_required = false;
			new_required = derived && (cpr_name == "parent");

			
			if (!was_parent && cpr_name == "parent") was_parent = true;
			output << "\t\tpublic static Uml.CompositionParentRole meta_" << cpr_name << ";" <<endl;

			if (new_required)
				output << "\t\tpublic new " << tname << " " << cpr_name << endl;
			else
				output << "\t\tpublic " << tname << " " << cpr_name << endl;
			output << "\t\t{" << endl;
			output << "\t\t\tget{ return (" << tname <<")(getParent(meta_" << cpr_name << ", \"" << diagram.name() << "\"));}" <<endl; 
			output << "\t\t\tset{ setParent(meta_" << cpr_name << ", value);}" <<endl; 
			output << "\t\t}" << endl;
		}

		if (!was_parent)
		{
			if (derived) 
				output << "\t\tpublic new Udm.Object parent" <<endl;
			else
				output << "\t\tpublic Udm.Object parent" <<endl;

			output << "\t\t{" << endl;
			output << "\t\t\tget{ return getParent(Uml.CompositionParentRole.NULL, \"" << diagram.name() <<"\");}" <<endl;
			output << "\t\t\tset{ setParent(Uml.CompositionParentRole.NULL, value);}" << endl;
			output << "\t\t}" << endl;

		}


		output << "\t}" << endl;//end of class



	}
	
	
}

void GenerateCSInit(const Uml::Diagram &diagram,  ostream &output, string fname)
{
	output << "\t"		<< "public class Init" << endl;
	output << "\t"		<< "{" << endl; 
	output << "\t\t"		<< "public static bool initialized = false;" << endl;
	output << "\t\t"		<< "public static Udm.DataNetwork dn = null;" << endl;
	output << "\t\t"		<< "public static Uml.Diagram umldiagram = null;" << endl;
	output << "\t\t"		<< "static Init()" << endl;
	output << "\t\t"		<< "{"<<endl;
	output << "\t\t\t"			<< "if(initialized) return;" << endl;
	output << "\t\t\t"			<< "initialized = true;" << endl;
	output << "\t\t\t"			<< "if(!Uml.Init.initialized) throw new Udm.udm_exception(\"UML Diagram is not yet initialized!\");" << endl;
	output << "\t\t\t"			<< "umldiagram = Uml.UmlExt.CreateDiagram();" << endl;

	set<Uml::Class> classes = diagram.classes();
	set<Uml::Class>::iterator c;

	//create classes
	for(c = classes.begin(); c != classes.end(); c++ )
	{
		Uml::Class cl = *c;
		output << "\t\t\t" << cl.name() << ".meta = Uml.UmlExt.CreateClass();" << endl;
	}

	//create attributes
	for(c = classes.begin(); c != classes.end(); c++ )	
	{
		Uml::Class cl = *c;
		set<Uml::Attribute> attributes = cl.attributes();

		//todo Non-persistent attribute support for C# UDM
	
		for( set<Uml::Attribute>::iterator i = attributes.begin(); i != attributes.end(); i++) 
		{
			output << "\t\t\t" << cl.name() << ".meta_" << (*i).name() << " = Uml.UmlExt.CreateAttribute();" << endl;
		}
	}

	//create association roles
	for(c = classes.begin(); c != classes.end(); c++ ) 
	{
		Uml::Class cl = *c;
		set<Uml::AssociationRole> assocs = cl.associationRoles();
		for( set<Uml::AssociationRole>::iterator i = assocs.begin(); i != assocs.end(); i++) 
		{
			string aname(MakeRoleName(Uml::theOther(*i)));
			output << "\t\t\t" << cl.name() << ".meta_" << aname << " = Uml.UmlExt.CreateAssociationRole();" << endl;
		}
	}

	//create composition parent roles
	for(c = classes.begin(); c != classes.end(); c++ ) 
	{
		Uml::Class cl = *c;
		set<Uml::CompositionChildRole> children = cl.childRoles();
		for( set<Uml::CompositionChildRole>::iterator i = children.begin(); i != children.end(); i++) 
		{
			string aname(MakeRoleName(Uml::theOther(*i)));
			output << "\t\t\t" << cl.name() << ".meta_" << aname << " = Uml.UmlExt.CreateCompositionParentRole();" << endl;
		}
	}

	//create composition child roles

	for(c = classes.begin(); c != classes.end(); c++ ) 
	{
		Uml::Class cl = *c;
		set<Uml::CompositionParentRole> children = cl.parentRoles();
		for( set<Uml::CompositionParentRole>::iterator i = children.begin(); i != children.end(); i++) 
		{
			string aname(MakeRoleName(Uml::theOther(*i)));
			output << "\t\t\t" << cl.name() << ".meta_" << aname << " = Uml.UmlExt.CreateCompositionChildRole();" << endl;
		}
	}

	//end of creations,

	output << "\t\t\t" << "Udm.Object.SolveMetaDependencies();"<< endl;
	
	
	//init part


	//diagram
	output << "\t\t\tUml.UmlExt.InitDiagram(umldiagram, \"" << diagram.name() << "\");" << endl << endl;

	//classes
	for(c = classes.begin(); c != classes.end(); c++ )
	{
		Uml::Class cl = *c;
		string ster;
		if(((string)cl.stereotype()).length()) ster = string(", \"")+(string)cl.stereotype()+"\"";

		string from;
		if(((string)cl.from()).length()) from = string(", \"")+(string)cl.from()+"\"";

		output << "\t\t\tUml.UmlExt.InitClass(" << cl.name() << ".meta, umldiagram, \"" <<
			cl.name() << "\", " << (cl.isAbstract() ? "true" : "false")<< ster << from << ");" << endl;

	}
	
	output << "\t\t" << endl;

	//atributes
	for(c = classes.begin(); c != classes.end(); c++ )	
	{
		Uml::Class cl = *c;
	
//		Uml::Class tempcl(Uml::classByName(tempdiagram, cl.name()));
		set<Uml::Attribute> attributes = cl.attributes();
		
		for( set<Uml::Attribute>::iterator i = attributes.begin(); i != attributes.end(); i++) 
		{
			output << "\t\t\tUml.UmlExt.InitAttribute(" << cl.name() << ".meta_" << (*i).name() << ", " <<
				cl.name() << ".meta, \"" << (*i).name() << "\", \"" << (*i).type() << "\", " <<
				((*i).nonpersistent() ? "true" : "false") << ", " << (*i).min() << ", " << (*i).max() << ");" << endl;
		}
	}
		
	output << "\t\t" << endl;

	//associations
	{
		const set<Uml::Association> &asss = diagram.associations();
		set<Uml::Association>::const_iterator a;
		for(a = asss.begin(); a != asss.end(); a++ )	
		{
			set<Uml::AssociationRole> ar = a->roles();
			ASSERT(ar.size() == 2);
//	{
			output << "\t\t\t{" << endl;
//		Uml::Association ass = Uml::CreateAssociation();
			output << "\t\t\t\tUml.Association ass = Uml.UmlExt.CreateAssociation();" << endl;

//		Uml::InitAssociation(ass, diagram, "AtoB");
// or : Uml::InitAssociationClass(ass, C::meta);
			Uml::Class aclass = a->assocClass();
			output << "\t\t\t\tUml.UmlExt.InitAssociation(ass , umldiagram, \"" << a->name() << "\");" << endl;
			if(aclass) {
				output << "\t\t\tUml.UmlExt.InitAssociationClass(ass, " << aclass.name() << ".meta);" << endl;
			}

//		Uml::InitAssociationRole(A::meta_dst, ass, "dst", 0, 1, B::meta);
//								C:: meta_dst_end_ = B:meta_src_rev = A::meta_dst;
			set<Uml::AssociationRole> roles = a->roles();
			for( set<Uml::AssociationRole>::iterator i = roles.begin(); i != roles.end(); i++) {
					Uml::AssociationRole zz	= Uml::theOther(*i);
					string aname(MakeRoleName(zz));
					Uml::Class cl = i->target();
					output << "\t\t\t\tUml.UmlExt.InitAssociationRole(" << cl.name() << ".meta_" << aname 
						<< ", ass, \"" << aname << "\", " <<
						(zz.isNavigable() ? "true, " : "false, ") <<
						(zz.isPrimary() ? "true, " : "false, ") <<
						zz.min() << ", " << zz.max() << ", " << ((Uml::Class)zz.target()).name() << ".meta);" << endl;
					Uml::Class aclass = a->assocClass();
					if(aclass) {
						Uml::Class cl2 = zz.target();
						output << "\t\t\t" << aclass.name() << ".meta_" << aname << "_end_ = " <<
											cl2.name() << ".meta_" << MakeRoleName(*i) << "_rev = " << 
											cl.name() << ".meta_" << aname << ";" << endl;
					}
			}
//	}
			output << "\t\t\t}" << endl;
		}
	}

	//compositions

	{
		const set<Uml::Composition> &comps = diagram.compositions();
		set<Uml::Composition>::const_iterator c;
		for(c = comps.begin(); c != comps.end(); c++ )	{
//	{
			output << "\t\t\t{" << endl;
//		Uml::Composition comp = Uml::CreateComposition();
			output << "\t\t\t\tUml.Composition comp = Uml.UmlExt.CreateComposition();" << endl;

//		Uml::InitComposition(comp, diagram, "DtoA");
			output << "\t\t\t\tUml.UmlExt.InitComposition(comp , umldiagram, \"" << 
				c->name() << "\");" <<  endl;
			output << "\t\t" << endl;

			Uml::CompositionParentRole x1 = (*c).parentRole();
			Uml::CompositionChildRole x2 = (*c).childRole();

//		Uml::InitCompositionParentRole(A::meta_parent, comp, "parent", D::meta);
			Uml::CompositionParentRole zzp = c->parentRole();
			Uml::CompositionChildRole zzc = c->childRole();
			string aname(MakeRoleName(zzp));
			output << "\t\t\t\tUml.UmlExt.InitCompositionParentRole(" << ((Uml::Class)zzc.target()).name() << ".meta_" << aname << 
				", comp, \""<< aname << "\", " <<
				(zzp.isNavigable() ? "true, " : "false, ") <<
				((Uml::Class)zzp.target()).name() << ".meta);" << endl;
					
//		Uml::InitCompositionChildRole(D::meta_as, comp, "as", true, 0, -1, A::meta); 
			string cname(MakeRoleName(zzc));
			output << "\t\t\t\tUml.UmlExt.InitCompositionChildRole(" << ((Uml::Class)zzp.target()).name() << ".meta_" << cname << 
				", comp, \""<< MakeShortRoleName(zzc) << "\", " <<
					(zzc.isNavigable() ? "true, " : "false, ") <<
				zzc.min() << ", " << zzc.max() << ", " <<
				((Uml::Class)zzc.target()).name() << ".meta);" << endl;
//	}
			output << "\t\t\t}" << endl;
		}
	}


	//inheritence

//		Uml::AddInheritance(A::meta, C::meta);
	for(c = classes.begin(); c != classes.end(); c++ ) 
	{
		Uml::Class cl = *c;
		set<Uml::Class> st = cl.subTypes();
		for( set<Uml::Class>::iterator i = st.begin(); i != st.end(); i++) 
		{
			output << "\t\t\tUml.UmlExt.AddInheritance(" << cl.name() << ".meta, " << (*i).name() << ".meta);" << endl;
		}
	}			

	output << endl;

	output << "\t\t\t" << "Static.StaticDataNetwork sdn = new Static.StaticDataNetwork(umldiagram);" << endl;
	output << "\t\t\t" << "sdn.CreateFromRootObj(\"" << diagram.name() <<"\", umldiagram, Udm.BackEndSemantics.CHANGES_LOST_DEFAULT);" << endl;
	output << "\t\t\t" << "dn = sdn;" << endl;

	output << "\t\t"		<< "}"<<endl;		//end of Init.init();
	output << "\t"	   << "}"<<endl;			//end of class Init();
	output << "}" << endl;						//end of namespace
}
void GenerateNewCSInit(const Uml::Diagram &diagram,  ostream &output, string fname)
{
	output << "\t"		<< "public class Init" << endl;
	output << "\t"		<< "{" << endl; 
	output << "\t\t"		<< "public static bool initialized = false;" << endl;
	output << "\t\t"		<< "public static Udm.DataNetwork dn = null;" << endl;
	output << "\t\t"		<< "public static Uml.Diagram umldiagram = null;" << endl;
	output << "\t\t"		<< "static Init()" << endl;
	output << "\t\t"		<< "{"<<endl;
	output << "\t\t\t"			<< "if(initialized) return;" << endl;
	output << "\t\t\t"			<< "initialized = true;" << endl;
	output << "\t\t\t"		<< "if(!Uml.Init.initialized) throw new Udm.udm_exception(\"UML Diagram is not yet initialized!\");" << endl;


	string hname = diagram.name();

	output << "\t\t\t"			<< "dn = new Static.StaticDataNetwork(Uml.Init.dn.GetRootMeta());" << endl;
	output << "\t\t\t"			<< "dn.CreateNew(\"" << hname << ".mem\", \"\",ref Uml.Diagram.meta, Udm.BackEndSemantics.CHANGES_LOST_DEFAULT);"<< endl;
	output << "\t\t\t"			<< "umldiagram = Uml.Diagram.Cast(dn.GetRootObject());" << endl;
	output << "\t\t\t"			<< "umldiagram.name =\"" << hname << "\";" << endl;

	
	set<Uml::Class> classes = diagram.classes();
	set<Uml::Class>::iterator c;

	//classes
	for(c = classes.begin(); c != classes.end(); c++ )
	{
		Uml::Class cl = *c;
		output << "\t\t\t" << cl.name() << ".meta = Uml.Class.Create(umldiagram);" << endl;
	}

	//attributes
	for(c = classes.begin(); c != classes.end(); c++ )	
	{
		Uml::Class cl = *c;
		set<Uml::Attribute> attributes = cl.attributes();
		//todo: non-persistent attributes
	
		for( set<Uml::Attribute>::iterator i = attributes.begin(); i != attributes.end(); i++) 
		{
			output << "\t\t\t" << cl.name() << ".meta_" << (*i).name() << " = Uml.Attribute.Create(" << cl.name()<<".meta);" << endl;
		}
	}


	//class attributes
	for(c = classes.begin(); c != classes.end(); c++ )
	{
		Uml::Class cl = *c;
		string ster;
		if(((string)cl.stereotype()).length()) ster = string(", \"")+(string)cl.stereotype()+"\"";
	
		output << "\t\t\t" << cl.name() << ".meta.name = \"" << cl.name() << "\";" <<endl;
		output << "\t\t\t" << cl.name() << ".meta.isAbstract = " << (cl.isAbstract() ? "true" : "false") << ";" << endl;
		
		if (!ster.empty()) 
				output << "\t\t\t" << cl.name() << ".meta.stereotype = \"" << cl.stereotype() << "\";" <<endl;

	}
	
	//attribute attributes ;-)
	for(c = classes.begin(); c != classes.end(); c++ )	
	{
		Uml::Class cl = *c;
//		Uml::Class tempcl(Uml::classByName(tempdiagram, cl.name()));

		set<Uml::Attribute> attributes = cl.attributes();

		for(set<Uml::Attribute>::iterator att_i = attributes.begin(); att_i != attributes.end(); att_i++)
			//nonpersistent is false for attributes coming from the real diagram
			att_i->nonpersistent() = false;
			

		for( set<Uml::Attribute>::iterator i = attributes.begin(); i != attributes.end(); i++) 
		{
		
			output << "\t\t\t" << cl.name() << ".meta_" << (*i).name() << ".name = \"" << (*i).name() << "\";" << endl;
			output << "\t\t\t" << cl.name() << ".meta_" << (*i).name() << ".type = \"" << (*i).type() << "\";" << endl;
			output << "\t\t\t" << cl.name() << ".meta_" << (*i).name() << ".min = " << (*i).min() << ";" << endl;
			output << "\t\t\t" << cl.name() << ".meta_" << (*i).name() << ".max = " << (*i).max() << ";" << endl;
			output << "\t\t\t" << cl.name() << ".meta_" << (*i).name() << ".nonpersistent = " << ((*i).nonpersistent() ? "true" : "false") << ";" << endl;
		}
	}

	//associations

	{
		const set<Uml::Association> &asss = diagram.associations();
		set<Uml::Association>::const_iterator a;
		for(a = asss.begin(); a != asss.end(); a++ )
		{
			set<Uml::AssociationRole> ar = a->roles();
			ASSERT(ar.size() == 2);

			output << "\t\t\t{" << endl;
			output << "\t\t\t\tUml.Association ass = Uml.Association.Create(umldiagram);" << endl;

			Uml::Class aclass = a->assocClass();
			if (!((string)a->name()).empty())
				output << "\t\t\t\tass.name = \"" << a->name() << "\";" <<endl;
			if(aclass) 
			{
				output << "\t\t\t\tass.assocClass = " << aclass.name() << ".meta;" << endl;
			}

			set<Uml::AssociationRole> roles = a->roles();
			for( set<Uml::AssociationRole>::iterator i = roles.begin(); i != roles.end(); i++) {
					Uml::AssociationRole zz	= Uml::theOther(*i);
					string aname(MakeRoleName(zz));
					Uml::Class cl = i->target();

					output << "\t\t\t\t" << cl.name() << ".meta_" << aname << " = Uml.AssociationRole.Create(ass);" <<endl;
					output << "\t\t\t\t" << cl.name() << ".meta_" << aname << ".name = \"" << aname << "\";" <<endl;
					output << "\t\t\t\t" << cl.name() << ".meta_" << aname << ".min = " << zz.min() << ";" <<endl;
					output << "\t\t\t\t" << cl.name() << ".meta_" << aname << ".max = " << zz.max() << ";" <<endl;
					output << "\t\t\t\t" << cl.name() << ".meta_" << aname << ".isNavigable = " << (zz.isNavigable() ? "true":"false") << ";" <<endl;
					output << "\t\t\t\t" << cl.name() << ".meta_" << aname << ".isPrimary = " << (zz.isPrimary() ? "true":"false") << ";" <<endl;
					output << "\t\t\t\t" << cl.name() << ".meta_" << aname << ".target = " << ((Uml::Class)zz.target()).name() << ".meta;" <<endl;


					
					Uml::Class aclass = a->assocClass();
					if(aclass) 
					{
						Uml::Class cl2 = zz.target();
						output << "\t\t\t\t" << aclass.name() << ".meta_" << aname << "_end_ = " <<
											cl2.name() << ".meta_" << MakeRoleName(*i) << "_rev = " << 
											cl.name() << ".meta_" << aname << ";" << endl;
					}
			}
			output << "\t\t\t}" << endl;
		}
	}


	//compositions

	{
		const set<Uml::Composition> &comps = diagram.compositions();
		set<Uml::Composition>::const_iterator c;
		for(c = comps.begin(); c != comps.end(); c++ )	
		{
			output << "\t\t\t{" << endl;
			output << "\t\t\t\tUml.Composition comp = Uml.Composition.Create(umldiagram);" << endl;

			if (!((string)c->name()).empty())
				output << "\t\t\t\tcomp.name =\"" << c->name() << "\";" <<endl;
			output << "\t\t\t\tcomp.nonpersistent =" << (c->nonpersistent() ? "true" : "false") << ";" << endl;


			output << "\t\t" << endl;

			Uml::CompositionParentRole x1 = (*c).parentRole();
			Uml::CompositionChildRole x2 = (*c).childRole();

			Uml::CompositionParentRole zzp = c->parentRole();
			Uml::CompositionChildRole zzc = c->childRole();
			string aname(MakeRoleName(zzp));

			output << "\t\t\t\t" << ((Uml::Class)zzc.target()).name() << ".meta_" << aname <<
				" = Uml.CompositionParentRole.Create(comp);" << endl;
			output << "\t\t\t\t" << ((Uml::Class)zzc.target()).name() << ".meta_" << aname <<
				".name = \"" << aname << "\";" << endl;
			output << "\t\t\t\t" << ((Uml::Class)zzc.target()).name() << ".meta_" << aname <<
				".isNavigable = " << (zzp.isNavigable() ? "true;" : "false;") << endl;
			output << "\t\t\t\t" << ((Uml::Class)zzc.target()).name() << ".meta_" << aname <<
				".target = " << ((Uml::Class)zzp.target()).name() << ".meta;" << endl;




			string cname(MakeRoleName(zzc));
			output << "\t\t\t\t" << ((Uml::Class)zzp.target()).name() << ".meta_" << cname << 
				" = Uml.CompositionChildRole.Create(comp);" << endl;

			output << "\t\t\t\t" << ((Uml::Class)zzp.target()).name() << ".meta_" << cname << 
				".name = \"" << MakeShortRoleName(zzc) << "\";" << endl;
			output << "\t\t\t\t" << ((Uml::Class)zzp.target()).name() << ".meta_" << cname << 
				".min = " << zzc.min() << ";"<< endl;
			output << "\t\t\t\t" << ((Uml::Class)zzp.target()).name() << ".meta_" << cname << 
				".max = " << zzc.max() << ";"<< endl;
			output << "\t\t\t\t" << ((Uml::Class)zzp.target()).name() << ".meta_" << cname << 
				".isNavigable = " << (zzc.isNavigable() ? "true;" : "false;") << endl;
			output << "\t\t\t\t" << ((Uml::Class)zzp.target()).name() << ".meta_" << cname << 
				".target = " << ((Uml::Class)zzc.target()).name() << ".meta;" << endl;

			output << "\t\t\t}" << endl;
		}
	}

	for(c = classes.begin(); c != classes.end(); c++ ) 
	{
		Uml::Class cl = *c;
		set<Uml::Class> st = cl.subTypes();
		if (st.size())
		{
			output << "\t\t\t" <<"Hashtable " << (string)cl.name() << "_meta_subtypes = new Hashtable();" << endl;
			for( set<Uml::Class>::iterator i = st.begin(); i != st.end(); i++) 
			{
			
				output << "\t\t\t" << cl.name() << "_meta_subtypes[" << (*i).name() << ".meta] = " <<(*i).name() << ".meta;" << endl;
			}
			output << "\t\t\t" << cl.name() << ".meta.subTypes = " << cl.name() << "_meta_subtypes;" << endl;
			output << endl;
		}
	}			




	output << "\t\t"		<< "}"<<endl;		//end of Init.init();
	output << "\t"	   << "}"<<endl;			//end of class Init();
	output << "}" << endl;	//end of namespace
}

void GenerateCS(const Uml::Diagram &diagram,  ostream &output, string fname)
{
	GenerateCSApi(diagram,  output, fname);
	GenerateCSInit(diagram, output, fname);
}
void GenerateNewCS(const Uml::Diagram &diagram,  ostream &output, string fname)
{
	GenerateCSApi(diagram,   output, fname);
	GenerateNewCSInit(diagram, output, fname);
}
//////
