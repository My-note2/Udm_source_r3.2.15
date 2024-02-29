

/*
Changelog:
  07/03/06 - endre
	- classes contained by an abstract superclass are XSD global elements too

  04/22/05 - kalmar
    -added AddUMLNamespaceToIgnoreList - all the contained elements in a namespaces enlisted 
      here will be ignored during parsing, see udm.exe -i switch

   -added AddUMLNamespaceToQualifiedAttrsNSList - elementFormDefault=\"qualified\"
      all the contained attributes in a namespaces enlisted 
          here will be prefixed with the UML namespace, see udm.exe -q switch
          elementFormDefault="qualified" generated in the schema

   - Text attribute xml representation changed, it hides the attribute name in
      xml and the contained string is stored as a TEXT nodein the xml, the mixed ="true" is generated in schema sequence.
      This feature allows parsing and writting mixed containment like
      <A> Mixed <B size = "1"> containmnet </B> feature <A>
      The meta has class A, which contains class B, which has attribute size:String 
      and attribute value:Text
  
	11/07/04	-	endre

		changed the DTD and the XSD generator to generate a different content model when
		both text attributes and children are present.

		The generated content model in such cases is:
		
		  (Child1?, Child2?, Child3?, Child4?, TextAttribute1?, TextAttribute2?)*

		which basically allows any order of children nodes.
		
	10/24/04	-	endre

		changed the DTD ans XSD generator to also support text attributes.
		Text attributes get persisted as Text-nodes in the XML file, and thus corresponding
		DTD/XSD file should be generated.


	03/15/04	-	endre
			
		changed the internal dom attributes from using literal strings to constants
		defined in UdmDom namespace


	24/02/04	-	endre

		changed the XSD/DTD generator to generate the version information as processing instruction

	15/01/04 - endre
		Changed XSD Schema generator's root element selection condition: All classes, 
		which have no container classes but self are allowed to be root elements.
*/

#include <UdmBase.h>
#include <Uml.h>
#include <UmlExt.h>
#include <UdmUtil.h>
#include "DTDGen.h"

#include <ctime>
#include <fstream>

namespace DTDGen
{

	string GetTime()	
	{
		time_t t;
		time(&t);
		char *c = ctime(&t);

		c[24] = '\0';
		return c;
	}

	// -------------------------------------------------------------------------------------------------------------------------




	card_prod sum_cardinalities(const vector<card_prod> &ccrs)
	{
		card_prod ret;
		bool max_set = false;//these are the default cardinalities

		vector<card_prod>::const_iterator ccr_i = ccrs.begin();
		ret.first = ccr_i->first; //initialize the comparing process for obtaining the minimum

		while (ccr_i != ccrs.end())
		{
			//the product minimum is the minimum of the minimums
			if (ccr_i->first < ret.first) ret.first = ccr_i->first;
			
			//the product maximum is the sum of the maximums or *
			if (ccr_i->second != -1)
			{
				if (max_set) 
				{
					if (ret.second != -1)
						ret.second += ccr_i->second;
				}
				else 
				{
					ret.second = ccr_i->second;
					max_set = true;
				}
			}
			else
			{
				max_set = true;
				ret.second = -1;
			}
			ccr_i ++;
		}

		if (!max_set) ret.second = 1;


		return ret;
	};	

	cwcp_order get_cwcp_order(const ::Uml::Class & parent, bool uxsdi, bool has_text_attr)
	{
		cwcp_order ret;


		set< ::Uml::CompositionChildRole> ccr_set;
		
		if (uxsdi)
			ccr_set = Uml::CompositionPeerChildRoles(parent);
		else
			ccr_set = Uml::AncestorCompositionPeerChildRoles(parent);
		

		//stage 1: create the map of classes with all their childroles
		typedef vector<card_prod> cp_vector_type;
		map<const ::Uml::Class, cp_vector_type> class_cps;

		set< ::Uml::CompositionChildRole>::iterator ccr_set_i = ccr_set.begin();
		while (ccr_set_i != ccr_set.end())
		{
			::Uml::Class target = ccr_set_i->target();

			set< ::Uml::Class> target_descs;

			if (uxsdi)
				target_descs.insert(target);
			else
				target_descs = Uml::DescendantClasses(target);
			
			set< ::Uml::Class>::iterator td_i = target_descs.begin();

			while (td_i != target_descs.end())
			{			
				
				if (uxsdi || !td_i->isAbstract())
				{
					card_prod cp;
					if (has_text_attr)
					{
						//if there is at least one text attibute,
						// the content model is generated like this (A?, B?, C?, D?)* 
						// this enables virtually any order.
						// with text attributes we can not guarantee a certain order, 
						// and requiring a certain order also defeats the original aim:
						// to be able to load non UDM-generated XML data.
						cp.first = 0;
						cp.second = 1;
					}
					else
					{
						cp.first = ccr_set_i->min();
						cp.second = ccr_set_i->max();
					}
					//if (*td_i != target)
					if (target_descs.size() > 1)
					
						//inherited childroles, 
						//minimum should be set to 0,
						//since it's not known which inherited type is going to be there
						cp.first = 0;
					
					
					class_cps[*td_i].push_back(cp);
				}
				
				td_i++;
				
			}

			ccr_set_i++;
		}

		//stage 2: create the product of the cardinalities as a sequence sorted by path of typename
		map<const string, class_w_cp> cwcp_seq_map;

		map<const ::Uml::Class, cp_vector_type>::iterator cc_i = class_cps.begin();
		while (cc_i != class_cps.end())
		{
			cwcp_seq_map[(cc_i->first).getPath2("::")] = class_w_cp(cc_i->first, sum_cardinalities(cc_i->second));
			cc_i++;
		}

		//stage 3: retrieve the order as vector from the ordered sequence
		// (so the vector will be ordered)
		map<const string, class_w_cp>::iterator cwcp_seq_map_i = cwcp_seq_map.begin();
		while(cwcp_seq_map_i != cwcp_seq_map.end())
		{

			ret.push_back(cwcp_seq_map_i->second);
			cwcp_seq_map_i++;
		}

		return ret;
	};


	string card_to_dtdspec(const class_w_cp& cwcp)
	{
		/*

		Mail from Gabor Karsai on 10/24/2002: ;-)

		How about the following.

		Assume P contains <c1> C1 and <c2> C2 and ... and <cn> Cn,
		where <c1>, <c2>, ... <cn> are cardinalities. 

		You translate a cardinality <c> (for a class say <C>) as follows:
		<c> form: translation:
		n (integer) C ... C (n times)
		0..1 C?
		0..* C*
		0..n C? | CC | ... | C...C (n times) <= THIS SUCKS!
		n..* C...C C* // where the first hale repeats C n times
		1..* C+

		endre, 05/06/03
		Obviously the case n..m, n < m is missing
		n..m C...C(n times)|C...C(n+1 times).......|C...C(m times)

		*/

		

		string ret;
		if ((cwcp.second.first == cwcp.second.second))
		{
			for (int i = 0; i < cwcp.second.first; i++)
			{
				ret += cwcp.first.name();
			}
		}
		else if ((cwcp.second.first == 0) && (cwcp.second.second == -1))
		{
			ret = (string)cwcp.first.name() + "*";
		}
		else if ((cwcp.second.first == 0) && (cwcp.second.second > 0))
		{
			ret += "(";
			for (int i = 0; i <= cwcp.second.second; i++)
			{
				if (i == 0)
					ret += (string)cwcp.first.name() + "?";
				else if (i > 1)
				{
					ret += "|(";
					
					for (int j = 0; j < i; j++)
					{
						if ((i>1)&&(j>0)) ret += ",";
						ret += (string)cwcp.first.name();
					}
					ret += ")";
				}
			}
			ret += ")";
		}
		else if ((cwcp.second.first >1) && (cwcp.second.second == -1))
		{
			for (int i = 0; i < cwcp.second.first; i++)
			{
				ret += (string)cwcp.first.name();
				ret += " ";
			}
			ret += cwcp.first.name();
			ret += "*";

		}
		else if ((cwcp.second.first == 1) && (cwcp.second.second == -1))
		{
			ret = (string)cwcp.first.name() + "+";
		}
		else
		{
			//the generic n..m, n<m case
			if (cwcp.second.first > cwcp.second.second)
				throw udm_exception("Invalid cardinality occurred: min > max");

			ret+="(";
			for (__int64 i = cwcp.second.first; i <= cwcp.second.second; i++)
			{
				if (i != cwcp.second.first)
					ret += "|";
				
				if (i > 1) ret += "(";
				
				for (int j = 0; j < i; j++)
				{
					if ((i>1)&&(j>0)) ret += ",";
					ret += (string)cwcp.first.name();
					
				}

				if (i > 1) ret += ")";
			}
			ret += ")";

		}

		return ret;

	};


	// --------------------------- DTD generation

	// TODO?: attached libraries
	void GenerateDTDElement(const ::Uml::Class &c,  ostream &output)	
	{
		string name = c.name();
		set<string> text_atts;		//text attributes
		
		output << "<!ELEMENT " << name;
		
		//if there are text attributes, we generate a more simple content model,
		//just (A?, B?, C?)*, which makes possible any order of the children nodes
		bool has_text_attrs = Uml::HasTextAttributes(c);
		cwcp_order  cwcps = get_cwcp_order(c, false, has_text_attrs);

		bool empty = true;
		bool need_delimiter = false;
		cwcp_order::iterator cwcps_i = cwcps.begin();
		while (cwcps_i != cwcps.end())
		{
			
			if (empty)
			{
				empty = false;
				output << " (";
			}

			
			if (need_delimiter)
				output << ", ";
			
			output  << card_to_dtdspec(*cwcps_i);

			need_delimiter = true;
			cwcps_i++;

		}
		
			
		if (has_text_attrs)
		{
			/*
				Either the separator comma or the begining whitespace is needed
			*/
				
			if (empty) output << " (" ;
			else output << ',';

			set< ::Uml::Attribute> atts = Uml::TextAttributes(c);
			set< ::Uml::Attribute>::iterator atts_i = atts.begin();

			bool  first = true;
			while (atts_i != atts.end())
			{
				if (!first) output << ',';
				first = false;
				output << "(" << atts_i->name() << ")?";
				atts_i++;

			};

			output << ")*";
		}
		else
			if (!empty) output << ")";
	
		if (empty && !has_text_attrs) 
		{
			
			output << " EMPTY";
		}

		output << ">" << endl;

	
	};

	void GenerateDTDAttlist(const ::Uml::Class &c,  ostream &output)	
	{
			string name = c.name();
			
			/*string topr("<!ATTLIST " + name + "\n");*/
			
			output << "<!ATTLIST " << name << endl;

			set< ::Uml::AssociationRole> associations = Uml::AncestorAssociationTargetRoles(c);
			set< ::Uml::Class> ccl = ::Uml::AncestorClasses(c);
			::Uml::Association ass;
			for(set< ::Uml::Class>::iterator iii = ccl.begin(); iii != ccl.end(); iii++) {

				if(::Uml::Association(iii->association())) {
					if(ass) {
						throw udm_exception("Class " + name + " has multiple associations"); 
					}
					else ass = iii->association();
				}
			}

			// we always need the _id, because every object may be derived/instance objects

			//if( !associations.empty() || ass) {
				/*{ output << topr; topr.erase(); }*/
				output << "\t_id\tID\t#IMPLIED" << endl;
			//}

				set< ::Uml::Class> cls = Uml::AncestorContainerClasses(c);
				set< ::Uml::Class>::iterator cl = cls.begin();

			
			while( cl != cls.end() )
			{
				if(!Uml::matchChildToParent(c,*cl)) {
						/*{ output << topr; topr.erase(); }*/

						// Jonathan requested that __child_as should be an enumerations
						// of all possible childRoles instead of an open NMTOKENS
						//endre, 04/22/02
						
						//get all my childRoles
						

					set< ::Uml::Class> anc_s = Uml::AncestorClasses(c);
						set< ::Uml::CompositionChildRole> ccr_s;
						set< ::Uml::Class>::iterator cl_i = anc_s.begin();

						while (cl_i != anc_s.end())
						{
							set< ::Uml::CompositionChildRole> this_ccr_s = (*cl_i).childRoles();
							
							set< ::Uml::CompositionChildRole>::iterator this_ccr_i = this_ccr_s.begin();
							
							while(this_ccr_i != this_ccr_s.end())
							{
								ccr_s.insert(*this_ccr_i);
								++this_ccr_i;
							}
							++cl_i;
						}
						output << "\t__child_as\t(";
						

						set< ::Uml::CompositionChildRole>::iterator ccr_i = ccr_s.begin();
						while (ccr_i != ccr_s.end())
						{
							//compname = GetANameFor(Uml::Composition(role.parent())).c_str();
							if (ccr_i != ccr_s.begin())  output << "|"; //delimiter
							
							if ( !((string)(*ccr_i).name()).empty())
								output << (string)(*ccr_i).name();
							else
							{
								::Uml::Class target = ccr_i->target();
								output << (string)(target.name());
							}


							//output << (string)(*ccr_i).name();
							++ccr_i;
						}


						output << ")\t #IMPLIED" << endl;
						//output << "\t__child_as\tNMTOKENS\t#IMPLIED" << endl;
						break;
				}
				++cl;
			}

			set< ::Uml::Attribute> attributes = Uml::AncestorAttributes(c);
			
			set< ::Uml::Attribute>::iterator i = attributes.begin();
			while( i != attributes.end() )
			{
				name = i->name();
				if (i->nonpersistent())
				{
					//non-persistent attributes should not show up in the DTD
					i++;
					continue;
				}


				if ((string)(i->type()) == "Text")
				{
					//text attributes get persisted as text-nodes
					i++;
					continue;
				}

				output << "\t" << name.c_str() << "\t";

				string type = i->type();
				
				/*
					Boolean arrays are CDATA,
					Boolean simplevalues are either true or false.

					Todo: is there a DTD way to enforce "[true|false];" sequence?
				*/
				if( (type == "Boolean") && ((i->max()==1)||(i->max() == 0) ) )
					output << "(true|false)";
				else
					output << "CDATA";
			
				if(i->defvalue() ) 
				{
					//if it's non array, then the first element of defval(),
					//otherwise the DTD encoded version of the array

					if ((i->max()==1)||(i->max() == 0) )
						output << "\t\"" << (string)i->defvalue()[0] << "\"" << endl;
					else
						//the attribute is array type
						output << "\t\"" << UdmUtil::vector_to_string(i->defvalue(), ';', true, true) << "\"" << endl;
				}
				else 
				{
					//output << (i->optional() ?"\t#IMPLIED" : "\t#REQUIRED") << endl;
					output << ( ((long)( i->min()) < 1) ?"\t#IMPLIED" : "\t#REQUIRED") << endl;
				}
				++i;
			}

			set< ::Uml::AssociationRole>::iterator j = associations.begin();
			while( j != associations.end() )
			{
				name = Uml::MakeRoleName(*j);
				/*{ output << topr; topr.erase(); }*/
				output << "\t" << name.c_str() << "\t";

				__int64 min = j->min();
				__int64 max = j->max();
				UDM_ASSERT( 0 <= min );
				UDM_ASSERT( max == -1 || min <= max );

				if( min == 1 && max == 1 )
					output << "IDREF\t#REQUIRED" << endl;
				else if( min == 0 && max == 1 )
					output << "IDREF\t#IMPLIED" << endl;
				else if( min >= 1 )
					output << "IDREFS\t#REQUIRED" << endl;
				else
					output << "IDREFS\t#IMPLIED" << endl;

				++j;
			}

			if(ass) {
				set< ::Uml::AssociationRole> rs(ass.roles());
				set< ::Uml::AssociationRole>::iterator j = rs.begin();
				while( j != rs.end() ) {
					name = Uml::MakeRoleName(*j);
					/*{ output << topr; topr.erase(); }*/
					output << "\t" << name.c_str() << "_end_\tIDREF\t#IMPLIED" << endl;
					++j;
				}
			}

			//add the archetype/derived/instances/desynched_atts
		output << "\t" << UdmDom::_udm_dom_ia_archetype <<"\tIDREF\t#IMPLIED" << endl;
		output << "\t" << UdmDom::_udm_dom_ia_derived <<"\tIDREFS\t#IMPLIED" << endl;
		output << "\t" << UdmDom::_udm_dom_ia_instances << "\tIDREFS\t#IMPLIED" << endl;
		output << "\t" << UdmDom::_udm_dom_ia_desynched_atts <<"\tCDATA\t#IMPLIED" << endl;
		output << "\t" << UdmDom::_udm_dom_ia_real_archetype <<"\t(true|false)\t#IMPLIED" << endl;
		output << "\t" << UdmDom::_udm_dom_ia_subtype << "\t(true|false)\t#IMPLIED" << endl;
	
		output << ">" << endl << endl;
	}

	static void GenerateDTDForClasses(const set< ::Uml::Class> &classes, ostream &output)
	{
			set< ::Uml::Class>::const_iterator i = classes.begin();
			set<string> text_att_names;
			while( i != classes.end() )
			{
				if( !(bool)i->isAbstract() )
				{
					GenerateDTDElement(*i,  output);
					GenerateDTDAttlist(*i,  output);
		
					//text-only elements also should be declared
					set< ::Uml::Attribute> text_atts = Uml::TextAttributes(*i);
					set< ::Uml::Attribute>::iterator text_atts_i = text_atts.begin();

					while (text_atts_i != text_atts.end())
					{
						text_att_names.insert(text_atts_i->name());
						text_atts_i++;
					}
	
				}
				++i;
			}
			
			set<string>::iterator tan_i = text_att_names.begin();
			while (tan_i != text_att_names.end()) output << "<!ELEMENT " << *tan_i++ << " ANY>" << endl;

	}

	void GenerateDTD(const ::Uml::Namespace &ns,  ostream &output)	
	{
		::Uml::Diagram dgr = ::Uml::GetDiagram(ns);
			output << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl;
			output << "<?udm interface=\"" << dgr.name() << "\" version=\"" << dgr.version() << "\"?>" << endl;
			output << "<!-- generated on " << GetTime().c_str() << " -->" << endl << endl;
			GenerateDTDForClasses(ns.classes(), output);
	}

	void GenerateDTD(const ::Uml::Diagram &dgr,  ostream &output)	
	{
			output << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl;
			output << "<?udm interface=\"" << dgr.name() << "\" version=\"" << dgr.version() << "\"?>" << endl;
			output << "<!-- generated on " << GetTime().c_str() << " -->" << endl << endl;
			GenerateDTDForClasses(dgr.classes(), output);
	}

	// --------------------------- XML Schema generation

	static string XMLNamespaceURI(const ::Uml::Diagram &dgr, const map<string, string> &ns_map)
	{
		string name = "__dgr_" + (string)dgr.name();
		if (!ns_map.empty())
		{
			map<string, string>::const_iterator i = ns_map.find(name);
			if (i != ns_map.end())
				return i->second;
		}
		string uri("http://www.isis.vanderbilt.edu/2004/schemas/");
		uri += name;
		return uri;
	}

	static string XMLNamespaceURI(const ::Uml::Namespace &ns, const map<string, string> &ns_map)
	{
		string name = ns.getPath2("::", false);
		if (!ns_map.empty())
		{
			map<string, string>::const_iterator i = ns_map.find(name);
			if (i != ns_map.end())
				return i->second;
		}
		string uri("http://www.isis.vanderbilt.edu/2004/schemas/");
		uri += ns.getPath2("/", false);
		return uri;
	}

	void AddUMLContainerNameToQualifiedAttrsNSList(const char *optp, set<string> &qualifiedAtrrsNS_set)
 	{
		string opt(optp);
		if (opt.empty())
			throw udm_exception("UML container name must not be empty in argument \"" + opt + "\" of \"-q\" switch");
		qualifiedAtrrsNS_set.insert(optp);
	}

	void AddUMLContainerNameToIgnoreList(const char *optp, set<string> &ns_ignore_set)
	{
		string opt(optp);
		if (opt.empty())
			throw udm_exception("UML container name must not be empty in argument \"" + opt + "\" of \"-i\" switch");
		ns_ignore_set.insert(optp);
	}

	void AddUMLContainerNameToURIMapping(const char *optp, map<string, string> &ns_map)
	{
		string opt(optp);
		string::size_type loc = opt.find('=');
		if (loc != string::npos) {
			string uml_ns_path = opt.substr(0, loc);
			string uri = opt.substr(loc + 1);

			if (uml_ns_path.length() == 0 || uri.length() == 0)
				throw udm_exception("UML container path and the URI must not be empty in argument \"" + opt + "\" of \"-u\" switch");

			map<string, string>::value_type item(uml_ns_path, uri);
			ns_map.insert(item);
		} else {
			throw udm_exception("argument \"" + opt + "\" of \"-u\" switch must be in this form: uml_container_path=URI");
		}
	}

	static bool IsIgnored(const ::Uml::Namespace &ns, const set<string> &ns_ignore_set)
	{
		if (ns)
		{
			set<string>::const_iterator i = ns_ignore_set.find(ns.getPath2("::", false));
			return i != ns_ignore_set.end();
		}
		return false;
	}

	static bool IsIgnored(const ::Uml::Diagram &dgr, const set<string> &ns_ignore_set)
	{
		set<string>::const_iterator i = ns_ignore_set.find(string("__dgr_") + (string)dgr.name());
		return i != ns_ignore_set.end();
	}

	void GenerateXMLSchemaAttributes(const ::Uml::Class &c,  ostream &output, bool uxsdi, bool can_be_libroot)	
{
		string name = c.name();
		::Uml::Namespace c_ns = c.parent_ns();
		set< ::Uml::AssociationRole> associations;
		if (uxsdi)
			associations = Uml::AssociationTargetRoles(c);
		else
			associations = Uml::AncestorAssociationTargetRoles(c);

		set< ::Uml::Class> ccl = ::Uml::AncestorClasses(c);
		set< ::Uml::Association> assocations;
		for(set< ::Uml::Class>::iterator iii = ccl.begin(); iii != ccl.end(); iii++) {

			if(::Uml::Association(iii->association())) {
				assocations.insert(iii->association());
			}
		}

		// we always need the _id, because every object may be derived/instance objects
		//output << "  <xsd:attribute name=\"_id\" type=\"xsd:ID\"/>" << endl;

		set< ::Uml::Class> cls;
		if (uxsdi)
			cls = ::Uml::ContainerClasses(c);
		else
			cls = ::Uml::AncestorContainerClasses(c);

		set< ::Uml::Class>::iterator cl = cls.begin();
		while( cl != cls.end() )
		{
			if(!::Uml::matchChildToParent(c,*cl)) 
			{
					// Jonathan requested that __child_as should be an enumerations
					// of all possible childRoles instead of an open NMTOKENS
					//endre, 04/22/02
					
					//get all my childRoles
					set< ::Uml::Class> anc_s = ::Uml::AncestorClasses(c);
					set< ::Uml::CompositionChildRole> ccr_s;
					set< ::Uml::Class>::iterator cl_i = anc_s.begin();

					while (cl_i != anc_s.end())
					{
						set< ::Uml::CompositionChildRole> this_ccr_s = (*cl_i).childRoles();
						set< ::Uml::CompositionChildRole>::iterator this_ccr_i = this_ccr_s.begin();
						while(this_ccr_i != this_ccr_s.end())
						{
							if (!(((::Uml::Class)this_ccr_i->target()).isAbstract()))
								ccr_s.insert(*this_ccr_i);
							++this_ccr_i;
							
						}
						++cl_i;
					}

					output << "\t\t<xsd:attribute name=\"__child_as\">" << endl <<
						      "\t\t\t<xsd:simpleType>" << endl <<
							  "\t\t\t\t<xsd:restriction base=\"xsd:string\">" << endl;

					set< ::Uml::CompositionChildRole>::iterator ccr_i = ccr_s.begin();
					while (ccr_i != ccr_s.end())
					{
						//compname = GetANameFor(Uml::Composition(role.parent())).c_str();
						if ( !((string)(*ccr_i).name()).empty())
							output << "\t\t\t\t\t<xsd:enumeration value=\"" <<
								(string)(*ccr_i).name() << "\"/>" << endl;
						else
						{
							::Uml::Class target = ccr_i->target();
							output << "\t\t\t\t\t<xsd:enumeration value=\"";
							::Uml::Namespace target_ns = target.parent_ns();
							if (c_ns != target_ns && target_ns)
								output << target_ns.getPath2("_", false) << ":";
							output << (string)(target.name()) << "\"/>" << endl;
						}
						++ccr_i;
					}
					output << "\t\t\t\t</xsd:restriction>" << endl <<
						      "\t\t\t</xsd:simpleType>" << endl <<
							  "\t\t</xsd:attribute>" << endl;
					break;
			}
			++cl;
		}

		set< ::Uml::Attribute> attributes ;
		if (uxsdi)
			attributes = c.attributes();
		else
			attributes = ::Uml::AncestorAttributes(c);

		set< ::Uml::Attribute>::iterator i = attributes.begin();
		while( i != attributes.end() )
		{
			name = i->name();
			if (i->nonpersistent())
			{
				//non-persistent attributes should not show up in the DTD
				i++;
				continue;
			}

			if ((string)(i->type()) != "Text")
			{

				output << "\t\t<xsd:attribute name=\"" << name.c_str() << "\" type=\"";

				/*
				   For compatibility, array of attributes are encoded as strings.
				   It is possible to abuse to abuse the "list" type to encode them,
				   but the item separator would be white-space and, in the case of
				   arrays of strings, this separator will not work correctly.
				*/
				string type = i->type();
				bool isArray = !( (i->max()==1) || (i->max() == 0) );
				if (type == "Boolean")
					output << (isArray ? "xsd:string" : "xsd:boolean");
				else if (type == "Integer")
					output << (isArray ? "xsd:string" : "xsd:long");
				else if (type == "Real")
					output << (isArray ? "xsd:string" : "xsd:double");
				else if (type == "String")
					output << (isArray ? "xsd:string" : "xsd:string");
				output << "\"";

				if(i->defvalue() ) 
				{
					//if it's non array, then the first element of defval(),
					//otherwise the DTD encoded version of the array

					if ((i->max()==1)||(i->max() == 0) )
						output << " default=\"" << (string)i->defvalue()[0] << "\"";
					else
						//the attribute is array type
						output << " default=\"" << UdmUtil::vector_to_string(i->defvalue(), ';', true, true) << "\"";
				}
				else 
				{
					if ((long)( i->min() ) >= 1)
						output << " use=\"required\"";
				}
				output << "/>" << endl;
			}
			++i;
		}
		
		set< ::Uml::AssociationRole>::iterator j = associations.begin();
		while( j != associations.end() )
		{
			name = ::Uml::MakeRoleName(*j);
			output << "\t\t<xsd:attribute name=\"" << name.c_str() << "\"";

			__int64 min = j->min();
			__int64 max = j->max();
			UDM_ASSERT( 0 <= min );
			UDM_ASSERT( max == -1 || min <= max );

			if( min == 1 && max == 1 )
				output << " type=\"xsd:IDREF\" use=\"required\"/>" << endl;
			else if( min == 0 && max == 1 )
				output << " type=\"xsd:IDREF\"/>" << endl;
			else if( min >= 1 )
				output << " type=\"xsd:IDREFS\" use=\"required\"/>" << endl;
			else
				output << " type=\"xsd:IDREFS\"/>" << endl;

			++j;
		}

		for(set< ::Uml::Association>::iterator assocIt = assocations.begin(); assocIt != assocations.end(); assocIt++) {
			set< ::Uml::AssociationRole> rs(assocIt->roles());
			set< ::Uml::AssociationRole>::iterator j = rs.begin();
			while( j != rs.end() ) {
				name = ::Uml::MakeRoleName(*j);
				output << "\t\t<xsd:attribute name=\"" << name.c_str() <<
					"_end_\" type=\"xsd:IDREF\"/>" << endl;
				++j;
			}
		}

		set< ::Uml::Class> bases = c.baseTypes();
		
		if (!uxsdi ||  (bases.size()==0) )
		{
			output << "\t\t<xsd:attribute name=\"_id\" type=\"xsd:ID\"/>" << endl;
			//add the archetype/derived/instances/desynched_atts
			output << "\t\t<xsd:attribute name=\"" << UdmDom::_udm_dom_ia_archetype << "\" type=\"xsd:IDREF\"/>" << endl;
			output << "\t\t<xsd:attribute name=\"" << UdmDom::_udm_dom_ia_derived << "\" type=\"xsd:IDREFS\"/>" << endl;
			output << "\t\t<xsd:attribute name=\"" << UdmDom::_udm_dom_ia_instances << "\" type=\"xsd:IDREFS\"/>" << endl;
			output << "\t\t<xsd:attribute name=\"" << UdmDom::_udm_dom_ia_desynched_atts << "\" type=\"xsd:string\"/>" << endl;
			output << "\t\t<xsd:attribute name=\"" << UdmDom::_udm_dom_ia_real_archetype << "\" type=\"xsd:boolean\"/>" << endl;
			output << "\t\t<xsd:attribute name=\"" << UdmDom::_udm_dom_ia_subtype << "\" type=\"xsd:boolean\"/>" << endl;
			if (can_be_libroot)
				output << "\t\t<xsd:attribute name=\"" << UdmDom::_udm_dom_ia_libname << "\" type=\"xsd:string\"/>" << endl;
		}
};

void GenerateXMLSchemaElement(const ::Uml::Class &c,  ostream &output, const set<string> &ns_ignore_set, bool uxsdi, bool xsd_el_ta, bool can_be_libroot)
{
	string name = c.name();
	bool has_text_attr = Uml::HasTextAttributes(c);

	//we need a different content model generation if the class also has text attributes
	cwcp_order  cwcps = get_cwcp_order(c, uxsdi, has_text_attr || xsd_el_ta);

	if (!uxsdi)
		output << "\t<xsd:complexType name=\"" << name << "Type\"";
	else
	{
		string isabs = c.isAbstract()?"true":"false";

		output << "\t<xsd:complexType name=\"" << name << "Type\" abstract=\"" << isabs  << "\"";
	}

	if (has_text_attr)
		output << " mixed =\"true\"";

	output << ">" << endl;
	
	::Uml::Class only_base;

	if (uxsdi)
	{
		set< ::Uml::Class> bases= c.baseTypes();
	
		if (bases.size() >1)
			throw udm_exception("Multiple inheritence is not supported by XSD generator. Use DTD instead!");
		
		if (bases.size() == 1) 	only_base  =  *(bases.begin());
		
	};

	if (uxsdi && only_base)
	{
		/*<xs:complexContent>
		<xs:extension base="Person">*/
		output << "\t\t<xsd:complexContent>" << endl;
		output << "\t\t\t<xsd:extension base=\"";
		::Uml::Namespace only_base_ns = only_base.parent_ns();
		if (only_base_ns != ::Uml::Namespace(NULL))
			output << only_base_ns.getPath2("_", false) << ":";
		output << only_base.name() <<"Type\">" << endl;
	}
	

	bool need_libroot_element = can_be_libroot;
	::Uml::Namespace ns = c.parent_ns();
	if (!cwcps.empty() || has_text_attr || can_be_libroot) 
	{
		/* if text attribute is present, no order of child nodes are enforced, the content model is (A?,B?,C?)* */
		if (has_text_attr || xsd_el_ta)
			output << "\t\t<xsd:sequence minOccurs=\"0\" maxOccurs=\"unbounded\">" << endl;
		else
			output << "\t\t<xsd:sequence>" << endl;

		bool any_is_there = false;
		cwcp_order::iterator cwcps_i = cwcps.begin();
		while (cwcps_i != cwcps.end())
		{
			string i_name = cwcps_i->first.name();
			::Uml::Namespace i_ns = cwcps_i->first.parent_ns();

			bool ignore = false;
			{
				if (IsIgnored(i_ns, ns_ignore_set) ||
					(i_ns == ::Uml::Namespace(NULL) && IsIgnored(::Uml::GetDiagram(cwcps_i->first), ns_ignore_set)))
				{
					if (!any_is_there)
					{
						output << "\t\t\t<xsd:any  processContents=\"skip\" minOccurs=\"0\" maxOccurs=\"unbounded\"/>"<< endl;
						any_is_there = true;
					}
					ignore = true;
				}
			}
      
			if (!ignore)
			{
				output << "\t\t\t<xsd:element";
				if (ns == i_ns)
				{
					output << " name=\"" << i_name << "\"";
					if (i_ns)
						output << " type=\"" << i_ns.getPath2("_", false) << ":" << i_name << "Type\"";
					else
						output << " type=\"" << i_name << "Type\"";
				}
				else if (i_ns != ::Uml::Namespace(NULL))
					output << " ref=\"" << i_ns.getPath2("_", false) << ":" << i_name << "\"";
				else
					output << " ref=\"" << i_name << "\"";
				if (cwcps_i->second.first != 1)
					output << " minOccurs=\"" << cwcps_i->second.first << "\"";
				if (cwcps_i->second.second == -1)
					output << " maxOccurs=\"unbounded\"";
				else if	(cwcps_i->second.second != 1)
					output << " maxOccurs=\"" << cwcps_i->second.second << "\"";
				output << "/>" << endl;
				if (c == cwcps_i->first)
					need_libroot_element = false;
			}
			cwcps_i++;
		}

		// always attach libraries last
		if (need_libroot_element)
		{
			output << "\t\t\t<xsd:element name=\"" << name << "\"";
			if (ns != ::Uml::Namespace(NULL))
				output << " type=\"" << ns.getPath2("_", false) << ":" << name << "Type\"";
			else
				output << " type=\"" << name << "Type\"";
			output << " minOccurs=\"0\" maxOccurs=\"unbounded\"/>" << endl;
		}

		output << "\t\t</xsd:sequence>" << endl;
	}

	
	GenerateXMLSchemaAttributes(c, output, uxsdi, can_be_libroot);

	if (uxsdi && only_base)
	{
		/*<xs:complexContent>
		<xs:extension base="Person">*/
		output << "\t\t\t</xsd:extension>" << endl;
		output << "\t\t</xsd:complexContent>" << endl;
	}
	
	

	output << "\t</xsd:complexType>" << endl << endl;
}

static pair<bool, set< ::Uml::Namespace> > XMLNamespacesToImport(const ::Uml::Namespace &ns, const bool uxsdi)
{
	pair<bool, set< ::Uml::Namespace> > ret;
	ret.first = false;

	set< ::Uml::Class> classes = ns.classes();
	for (set< ::Uml::Class>::iterator i = classes.begin(); i != classes.end(); i++) {
		set< ::Uml::CompositionChildRole> comps_c = ::Uml::CompositionPeerChildRoles(*i);
		for (set< ::Uml::CompositionChildRole>::iterator j = comps_c.begin(); j != comps_c.end(); j++) {
			::Uml::Class theother = j->target();
			set< ::Uml::Class> theother_descs = ::Uml::DescendantClasses(theother);
			theother_descs.insert(theother);
			for (set< ::Uml::Class>::iterator descs_i = theother_descs.begin(); descs_i != theother_descs.end(); descs_i++) {
				::Uml::Namespace theother_ns = (::Uml::Namespace) descs_i->parent_ns();
				if (theother_ns == ::Uml::Namespace(NULL))
					ret.first = true;
				else if (ns != theother_ns)
					ret.second.insert(theother_ns);
			}

		}

		if (uxsdi) {
			set< ::Uml::Class> bases = i->baseTypes();
			for (set< ::Uml::Class>::iterator j = bases.begin(); j != bases.end(); j++) {
				::Uml::Class theother = *j;
				::Uml::Namespace theother_ns = (::Uml::Namespace)theother.parent_ns();
				if (theother_ns == ::Uml::Namespace(NULL))
					ret.first = true;
				else if (ns != theother_ns)
					ret.second.insert(theother_ns);
			}
		}
	}

	return ret;
}

static set< ::Uml::Namespace> XMLNamespacesToImport(const ::Uml::Diagram &dgr, const bool uxsdi)
{
	set< ::Uml::Namespace> ret;

	set< ::Uml::Class> classes = dgr.classes();
	for (set< ::Uml::Class>::iterator i = classes.begin(); i != classes.end(); i++) {
		set< ::Uml::CompositionChildRole> comps_c = ::Uml::CompositionPeerChildRoles(*i);
		for (set< ::Uml::CompositionChildRole>::iterator j = comps_c.begin(); j != comps_c.end(); j++) {
			::Uml::Class theother = j->target();
			set< ::Uml::Class> theother_descs = ::Uml::DescendantClasses(theother);
			theother_descs.insert(theother);
			for (set< ::Uml::Class>::iterator descs_i = theother_descs.begin(); descs_i != theother_descs.end(); descs_i++) {
				::Uml::Namespace theother_ns = (::Uml::Namespace) descs_i->parent_ns();
				if (theother_ns != ::Uml::Namespace(NULL))
					ret.insert(theother_ns);
			}

		}

		if (uxsdi) {
			set< ::Uml::Class> bases = i->baseTypes();
			for (set< ::Uml::Class>::iterator j = bases.begin(); j != bases.end(); j++) {
				::Uml::Class theother = *j;
				::Uml::Namespace theother_ns = (::Uml::Namespace)theother.parent_ns();
				if (theother_ns != ::Uml::Namespace(NULL))
					ret.insert(theother_ns);
			}
		}
	}

	return ret;
}

    /*
static bool IsCrossNSCompositionChildEnd(const ::Uml::Class &c)
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
}
*/
static void GenerateXMLSchemaForClasses(const ::Uml::Diagram &dgr,
			      ostream &output,
			      const set< ::Uml::Class> &classes,
			      const pair<bool, set< ::Uml::Namespace> > &xml_import_info,
			      const map<string, string> &ns_map,
			      const set<string> &ns_ignore_set,  
			      bool uxsdi, bool xsd_el_ta, 
			      bool qualified_attrs_ns)
{

	if (xml_import_info.first)
		output << " xmlns=\"" << XMLNamespaceURI(dgr, ns_map) << "\"" << endl;

	for (set< ::Uml::Namespace>::const_iterator other_ns_i = xml_import_info.second.begin(); other_ns_i != xml_import_info.second.end(); other_ns_i++) {
		if (!IsIgnored(*other_ns_i, ns_ignore_set))
			output << " xmlns:" << other_ns_i->getPath2("_", false) << "=\"" << XMLNamespaceURI(*other_ns_i, ns_map) << "\"" << endl;
	}

	output << " elementFormDefault=\"qualified\" " << endl;

	if (qualified_attrs_ns)
		output << " attributeFormDefault=\"qualified\" " << endl;

	output << ">" << endl;

	output << "<!-- generated on " << GetTime().c_str() << " -->" << endl << endl;

	if (xml_import_info.first)
		output << "<xsd:include schemaLocation=\"" << dgr.name() << ".xsd\">" << endl;

	for (set< ::Uml::Namespace>::const_iterator other_ns_i = xml_import_info.second.begin(); other_ns_i != xml_import_info.second.end(); other_ns_i++) {
		if (!IsIgnored(*other_ns_i, ns_ignore_set))
			output << "<xsd:import namespace=\"" << XMLNamespaceURI(*other_ns_i, ns_map) << "\" schemaLocation=\"" << other_ns_i->getPath2("_") << ".xsd\"/>" << endl;
	}
	output << endl;

	vector< ::Uml::Class> globalElements;
	set< ::Uml::Class>::const_iterator i = classes.begin();
	while( i != classes.end() )
	{
        // in these two ifs the behaviour is not affected by the value of uxsdi and of the ifs can be eliminated
		//if(uxsdi || !(bool)i->isAbstract() )
        // TODO: Use Uml::findNonContainedClasses instead of this.
		//{
			bool is_not_contained = false;

			// XSD elements can't be of abstract types,
			// don't make them globals
			if (/*!uxsdi || */!(bool)i->isAbstract()) {
				set< ::Uml::Class> p_c = Uml::AncestorContainerClasses(*i);		//all possible containers
				
				//the condition: It's not contained
				//				 or it's only container is itself or an abstract superclass
				//				 or it's the child end of a composition with the parent end in other namespace
				::Uml::Class anc;
				if ((p_c.size() == 0) ||
						(p_c.size() == 1 &&
						(anc = *(p_c.begin())) &&
						(anc == *i || (::Uml::IsDerivedFrom(*i, anc) && anc.isAbstract()))))
					is_not_contained = true;

				if (is_not_contained || ::Uml::IsCrossNSCompositionChildEnd(*i))
					globalElements.push_back(*i);
			}

			GenerateXMLSchemaElement(*i,  output, ns_ignore_set, uxsdi, xsd_el_ta, is_not_contained);

		//}
		++i;
	}

	vector< ::Uml::Class>::iterator j = globalElements.begin();
	while (j != globalElements.end()) {
		::Uml::Namespace j_ns = j->parent_ns();
		output << " <xsd:element name=\"" << j->name() << "\"";
		if (j_ns)
			output << " type=\"" << j_ns.getPath2("_", false) << ":" << j->name() << "Type\"";
		else
			output << " type=\"" << j->name() << "Type\"";
		output << "/>" << endl;
		j++;
	}
}

void GenerateXMLSchema(const ::Uml::Namespace &ns,  ostream &output,
		       const map<string, string> &ns_map,
                       const set<string> &ns_ignore_set,  
		       bool uxsdi, bool xsd_el_ta, 
                       bool qualified_attrs_ns)
{
	::Uml::Diagram dgr = ::Uml::GetDiagram(ns);

	output << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl;
	output << "<?udm interface=\"" << dgr.name() << "\" version=\"" << dgr.version() << "\"?>" << endl;

	pair<bool, set< ::Uml::Namespace> > xml_import_info = XMLNamespacesToImport(ns, uxsdi);

	output << "<xsd:schema xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" targetNamespace=\"" << XMLNamespaceURI(ns, ns_map) << "\"" <<endl;
	output << " xmlns:" << ns.getPath2("_", false) << "=\"" << XMLNamespaceURI(ns, ns_map) << "\"" << endl;

	GenerateXMLSchemaForClasses(dgr, output, ns.classes(), xml_import_info, ns_map, ns_ignore_set, uxsdi, xsd_el_ta, qualified_attrs_ns);

	output << endl << "</xsd:schema>" << endl;
}

void GenerateXMLSchema(const ::Uml::Diagram &dgr,  ostream &output,
		       const map<string, string> &ns_map,
                       const set<string> &ns_ignore_set,  
		       bool uxsdi, bool xsd_el_ta, 
                       bool qualified_attrs_ns)
{
	output << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl;
	output << "<?udm interface=\"" << dgr.name() << "\" version=\"" << dgr.version() << "\"?>" << endl;

	pair<bool, set< ::Uml::Namespace> > xml_import_info;
	xml_import_info.first = false;
	xml_import_info.second = XMLNamespacesToImport(dgr, uxsdi);

	output << "<xsd:schema xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\"" <<endl;

	GenerateXMLSchemaForClasses(dgr, output, dgr.classes(), xml_import_info, ns_map, ns_ignore_set, uxsdi, xsd_el_ta, qualified_attrs_ns);

	output << endl << "</xsd:schema>" << endl;
}


void GenerateXMLSchema(const ::Uml::Namespace &ns,  ostream &output)
{
	map<string, string> empty_ns_map;
	set<string> empty_ns_ignore_set;
	GenerateXMLSchema(ns, output, empty_ns_map, empty_ns_ignore_set);
}

void GenerateXMLSchema(const ::Uml::Diagram &dgr,  ostream &output)
{
	map<string, string> empty_ns_map;
	set<string> empty_ns_ignore_set;
	GenerateXMLSchema(dgr, output, empty_ns_map, empty_ns_ignore_set);
}


};

