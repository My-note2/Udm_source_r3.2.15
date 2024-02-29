/*
Copyright (c) Vanderbilt University, 2000-2005
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

#include "Udm.h"
#include "UdmBase/DTDGen.h"
#include "File2Code/File2Code.h"
#include "UdmUtil.h"

#include <ctime>
#include <fstream>

std::string GetTime()	{
		time_t t;
		time(&t);
		char *c = ctime(&t);

		c[24] = '\0';
		return c;
}

std::string NameToFilename(const std::string src)
{
	std::string ret = src;
	for (std::string::iterator i = ret.begin(); i != ret.end(); i++) {
		if (!isalnum(*i)) *i = '_';
	}
	return ret;
}

void GenerateDSD(const ::Uml::Namespace &ns, const UdmOpts &opts, bool qualified_attrs_ns)
{
	if (!ns) return;
	ofstream ff;
	string nsfname = NameToFilename(ns.getPath2("_"));

	if (opts.generate_dtd)
	{
		ff.open( (opts.out_dir + nsfname + ".dtd").c_str() );
		if(!ff.good()) throw udm_exception("Error opening for write " + opts.out_dir + nsfname + ".dtd");
		else DTDGen::GenerateDTD(ns, ff);
		ff.close();
		ff.clear();
	}

	ff.open( (opts.out_dir + nsfname + ".xsd").c_str() );
	if(!ff.good()) throw udm_exception("Error opening for write " + opts.out_dir + nsfname + ".xsd");
	else DTDGen::GenerateXMLSchema(ns, ff, opts.ns_map, opts.ns_ignore, opts.uxsdi, opts.xsd_el_ta, qualified_attrs_ns);
	ff.close();
}

void GenerateDSD(const ::Uml::Diagram &dgr, const string &fname, const UdmOpts &opts, bool qualified_attrs_ns)
{
	ofstream ff;

	if (opts.generate_dtd)
	{
		ff.open( (opts.out_dir + fname + ".dtd").c_str() );
		if(!ff.good()) throw udm_exception("Error opening for write " + opts.out_dir + fname + ".dtd");
		else DTDGen::GenerateDTD(dgr, ff);
		ff.close();
		ff.clear();
	}

	ff.open( (opts.out_dir + fname + ".xsd").c_str() );
	if(!ff.good()) throw udm_exception("Error opening for write " + opts.out_dir + fname + ".xsd");
	else DTDGen::GenerateXMLSchema(dgr, ff, opts.ns_map, opts.ns_ignore, opts.uxsdi, opts.xsd_el_ta, qualified_attrs_ns);
	ff.close();
}

string UmlClassCPPName(const ::Uml::Class &cl)
{
	return "::" + cl.getPath2("::");
}

string UmlClassCPPName(const ::Uml::Class &cl, const ::Uml::Namespace &ns)
{
	string ret = "::" + cl.getPath2("::");
	if ((::Uml::Namespace) cl.parent_ns() == ns)
	{
		std::set<Uml::Class> classes = Uml::AncestorClasses(cl);
		classes.erase(cl);
		for (std::set<Uml::Class>::iterator classesIt = classes.begin(); classesIt != classes.end(); classesIt++)
		{
			if (classesIt->name() == cl.name())
				return ret;
		}
		if (cl.name() == "Object")
			return ret;
		return cl.name();
	}
	return ret;
}

string join(const string &sep, const vector<string> &v)
{
	string result;

	vector<string>::const_iterator it_begin = v.begin();
	vector<string>::const_iterator it_end = v.end();

	if (it_begin != it_end) {
		result += *it_begin;
		it_begin++;
	}

	for (; it_begin != it_end; it_begin++) {
		result += sep;
		result += *it_begin;
	}

	return result;
}
