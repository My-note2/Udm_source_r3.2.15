
#ifndef MOBIES_DTDGEN_H
#define MOBIES_DTDGEN_H

/*
CHANGELOG

	11/07/04	-	endre

		added the bool parameter 'has_text_attr' to get_cwcp_order, which is false by default.
		If it is true, all the cardinalities are reset to 0..1
*/

namespace DTDGen
{
	using namespace std;
	typedef pair<__int64, __int64> card_prod;			//cardinality specification: min, max
	typedef pair< ::Uml::Class, card_prod> class_w_cp;		//class with cardinality prod
	typedef vector<class_w_cp> cwcp_order;				//list of classes with cardinality info

	//sumarizes cardinalities into one cardinality
	card_prod sum_cardinalities(const vector<card_prod> &ccrs);

	//create a vector fo classes with card. info in alphabetical order
	cwcp_order get_cwcp_order(const ::Uml::Class & parent, bool uxsdi = false, bool has_text_attr = false);

	//translates a cardinality specification vector to a DTD cardinality string
	string card_to_dtdspec(const class_w_cp& cwcp);

	void AddUMLContainerNameToURIMapping(const char *optp, map<string, string> &ns_map);

	void AddUMLContainerNameToIgnoreList(const char *optp, set<string> &ns_ignore_set);
	void AddUMLContainerNameToQualifiedAttrsNSList(const char *optp, set<string> &qualifiedAtrrsNS_set);

	//generates DTD elements
	void GenerateDTDElement(const ::Uml::Class &c,  ostream &output);

	//generate DTD attribute list
	void GenerateDTDAttlist(const ::Uml::Class &c,  ostream &output);

	//generates complete DTD 
	void GenerateDTD(const ::Uml::Namespace &ns,  ostream &output);
	void GenerateDTD(const ::Uml::Diagram &dgr,  ostream &output);

	//generate XSD attribute list
	void GenerateXMLSchemaAttributes(const ::Uml::Class &c,  ostream &output, bool uxsdi = false);

	//generates XSD elements
	void GenerateXMLSchemaElement(const ::Uml::Class &c,  ostream &output, const set<string> &ns_ignore_set, bool uxsdi = false, bool xsd_el_ta = false);

	//generates complete XML Schema 
	void GenerateXMLSchema(const ::Uml::Namespace &ns,  ostream &output, const map<string, string> &ns_map, const set<string> &ns_ignore_set, bool uxsdi = false, bool xsd_el_ta = false, bool qualified_attrs_ns = false);
	void GenerateXMLSchema(const ::Uml::Namespace &ns,  ostream &output);
	void GenerateXMLSchema(const ::Uml::Diagram &dgr,  ostream &output, const map<string, string> &ns_map, const set<string> &ns_ignore_set, bool uxsdi = false, bool xsd_el_ta = false, bool qualified_attrs_ns = false);
	void GenerateXMLSchema(const ::Uml::Diagram &dgr,  ostream &output);


};

#endif

