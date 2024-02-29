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


#include <fstream>
#include <boost/mpl/vector.hpp>

#include "Udm.h"
#include "File2Code/File2Code.h"
#include "UdmUtil.h"


namespace UdmCPPGen {





InheritanceSolver::InheritanceSolver(const ::Uml::Diagram &diagram, const UdmGen& gen, bool sort_by_namespace) : gen(gen)
{
	vector< ::Uml::Class> classes;
	for (vector< ::Uml::Class>::const_iterator it = gen.DiagramClasses.begin(); it != gen.DiagramClasses.end(); it++)
	{
		classes.push_back(*it);
	}

	bool has_namespaces = ((set< ::Uml::Namespace>)diagram.namespaces()).size() > 0;

	while (!classes.empty())
	{
		::Uml::Class cl;
		set< ::Uml::Class> bases;
		vector< ::Uml::Class>::const_iterator c;
		for( c = classes.begin(); c != classes.end(); c++ )	
		{
			cl = *c;
			bases = cl.baseTypes();

			set< ::Uml::Class>::iterator i;
			for(i = bases.begin(); i != bases.end(); i++) 
			{
				if (find(classes.begin(), classes.end(), *i) != classes.end()) break;
			}
			if(i == bases.end()) break;  // this class had all its parents already printed
		}
		if(c == classes.end()) 
		{ // list is not empty, still we found no one to continue with
			throw udm_exception("inheritance circularity detected");
		}

		good_inheritence_order.push_back(cl);

		// sort classes, putting classes from the same
		// namespace as "cl" to the beginning
		classes.erase(c);
		if (sort_by_namespace && !classes.empty() && has_namespaces)
		{
			::Uml::Namespace cl_ns = cl.parent_ns();
			if (cl_ns != classes.begin()->parent_ns())
				sort(classes.begin(), classes.end(), sortByActiveNamespace(cl_ns));
		}

		virtualchildren.insert(cltoclsmap::value_type(cl, set< ::Uml::Class>()));
			
		//filling up virtual base classes
		if( bases.size() == 0) 
		{
// Object should be virtual, if any of the descendants has an immediate baseclass,
// that is not derived from the current class   (only descendants with >=2 baseclasses are to be checked)
			set< ::Uml::Class> descs = Uml::DescendantClasses(cl);
			for(set< ::Uml::Class>::iterator i = descs.begin(); i != descs.end(); i++) 
			{
				if(*i == cl) continue;
				set< ::Uml::Class> dirancs = i->baseTypes();
				if(dirancs.size() > 1) 
				{
					for(set< ::Uml::Class>::iterator j = dirancs.begin(); j != dirancs.end(); j++) 
					{
						if (!gen.isDerivedFrom(*j, cl))
						{
							virtualbaseclasses.insert(cl);
							break;
						}
					}
				}
			}
		}
		else 
		{
			set< ::Uml::Class>::iterator h;
			for(h = bases.begin(); h != bases.end(); h++) 
			{
// the current base should be virtual, if any of the descendants has an immediate baseclass,
// that is derived from the current base, but not from the current class 
// (only descendants with >=2 baseclasses are to be checked)
				cltoclsmap::iterator actmapi = virtualchildren.find(*h);
				if(actmapi == virtualchildren.end()) 
				{
					throw udm_exception("internal error in class ordering");
				}
				set< ::Uml::Class> descs = Uml::DescendantClasses(cl);
				set< ::Uml::Class>::iterator i;
				for( i = descs.begin(); i != descs.end(); i++) 
				{
					if(*i == cl) continue;
					set< ::Uml::Class> dirancs = i->baseTypes();
					if(dirancs.size() > 1) 
					{
						for(set< ::Uml::Class>::iterator j = dirancs.begin(); j != dirancs.end(); j++) 
						{
							if (!gen.isDerivedFrom(*j, cl) && gen.isDerivedFrom(*j, *h))
							{
									actmapi->second.insert(cl);
							}
						}
					}
				}

				descs = Uml::AncestorClasses(cl);
				for(i = descs.begin(); i != descs.end(); i++) 
				{
					if(*i == cl) continue;
					if (actmapi->second.find(*i) != actmapi->second.end())
					{
						actmapi->second.insert(cl);
						break;
					}
				}
			}
		}

		int object_needed = 0, object_notneeded = 0;
		set< ::Uml::Class> ancestors = Uml::AncestorClasses(cl); 
		set< ::Uml::Class> additionaexplicitinitializers;

		for(set< ::Uml::Class>::iterator i = ancestors.begin(); i != ancestors.end(); i++) 
		{
			if(*i == cl) continue;

			if(set< ::Uml::Class>(i->baseTypes()).size() == 0) 
			{
				if(virtualbaseclasses.find(*i) != virtualbaseclasses.end()) object_needed++;
				else object_notneeded++;
			}
			if(bases.find(*i) != bases.end()) continue;

			cltoclsmap::iterator actmapi = virtualchildren.find(*i);
			set< ::Uml::Class> subtypes = i->subTypes();
			int found_inset = 0, found_notinset = 0;
			for(set< ::Uml::Class>::iterator p = subtypes.begin(); p != subtypes.end(); p++) 
			{
				if(!gen.isDerivedFrom(cl, *p)) continue;
				if(actmapi->second.find(*p) != actmapi->second.end()) found_inset++;
				else found_notinset++;
			}
			if(found_notinset > 1 || (found_inset && found_notinset))
			{
				throw udm_exception(string("Internal error resolving inheritance from class ") + string(i->name()) + " to " + string(cl.name()));
			}
			else if(found_inset) 
			{
					additionaexplicitinitializers.insert(*i);
			}
		}
		if(object_notneeded > 1 || (object_needed && object_notneeded)) 
		{ 
				throw udm_exception("Internal error resolving inheritance from class Object to " + string(cl.name()));
		}
			
		cltoclsmap::value_type item1(cl, additionaexplicitinitializers);
		cltoboolmap::value_type item2(cl, (object_needed!=0));

		pair<cltoclsmap::iterator, bool> res1 = explicitinits.insert(item1);
		pair<cltoboolmap::iterator, bool> res2 = objectneeded_map.insert(item2);

		if (!res1.second || !res2.second)
			throw udm_exception(string("Error generating the explicit initializer lists for class:") + (string)cl.name());
	}
}

string InheritanceSolver::getAncestorList(const ::Uml::Class &cl) const
{
	string ret;
	typedef set< ::Uml::Class, name_less<Uml::Class> > SortedClasses;
	name_less< ::Uml::Class> sort;
	SortedClasses bases = cl.baseTypes_sorted<name_less<Uml::Class> >(sort);
	if( bases.size() == 0) 
	{
		if (virtualbaseclasses.find(cl) != virtualbaseclasses.end()) ret+= " virtual ";
		ret += "public ::Udm::Object";
	}
	else 
	{
		string sep = "";
		SortedClasses::iterator h;
		for(h = bases.begin(); h != bases.end(); h++) 
		{
			cltoclsmap::const_iterator actmapi = virtualchildren.find(*h);
			if(actmapi == virtualchildren.end()) 
			{
				throw udm_exception("internal error in class ordering");
			}
			if (actmapi->second.find(cl) != actmapi->second.end())
			{
				ret += sep;
				ret += " virtual ";
				sep = "";
			};
			if(*h != cl) 
			{
				ret += sep;
				ret += " public ";
				ret += UmlClassCPPName(*h, cl.parent_ns());
				sep = ", ";
			}
		}
	}

	return ret;
}

string InheritanceSolver::getInitializers(const ::Uml::Class &cl, const string& argument) const
{
	set< ::Uml::Class> bases = cl.baseTypes();
	string ret;

	if( bases.size() == 0 ) 
	{
		ret += "UDM_OBJECT(";
		ret += argument;
		ret += ")";
	}
	else 
	{
		string sep = "";
		set< ::Uml::Class>::iterator i;
		for(i = bases.begin(); i != bases.end(); i++) 
		{
			ret += sep;
			ret += UmlClassCPPName(*i, cl.parent_ns());
			ret += "(";
			ret += argument;
			ret += ")";

			sep = ",";
		}
	}

	cltoclsmap::const_iterator ei_i = explicitinits.find(cl);
	if (ei_i == explicitinits.end())
		throw udm_exception("InheritanceSolver error");

	for(set< ::Uml::Class>::const_iterator i = ei_i->second.begin(); i != ei_i->second.end(); i++) 
	{
		ret += ", ";
		ret += UmlClassCPPName(*i, cl.parent_ns());
		ret += "(";
		ret += argument;
		ret += ")";
	}

	cltoboolmap::const_iterator on_i = objectneeded_map.find(cl);
	if (on_i == objectneeded_map.end())
		throw udm_exception("InheritanceSolver error");

	if(on_i->second) 
	{
		ret += ", ";
		ret += "UDM_OBJECT(";
		ret += argument;
		ret += ")";
	}

	return ret;
}

InheritanceSolver::sortByActiveNamespace::sortByActiveNamespace(const ::Uml::Namespace &ns) : active_ns(ns) {}

bool InheritanceSolver::sortByActiveNamespace::operator()(const ::Uml::Class &a, const ::Uml::Class &b) const
{
	::Uml::Namespace a_ns = a.parent_ns();
	::Uml::Namespace b_ns = b.parent_ns();
	if (a_ns == active_ns && b_ns != active_ns)
		return true;
	if (a_ns != active_ns && b_ns == active_ns)
		return false;
	return a_ns < b_ns;
}


int UdmGen::mpl_seq_max_size;

vector<boost::format> UdmGen::HPreamble(const string &fname) const
{
	string hname("MOBIES_" + fname + "_H");
	boost::to_upper(hname);

	vector<boost::format> r;

    r.push_back( boost::format("#pragma once\n\
#ifndef %1%\n\
#define %1%\n\
\n\
// header file %2%.h generated from diagram %3%\n\
// generated with Udm version %4%.%5% on %6%\n\
\n\
#include <UdmBase.h>\n\
\n\
#if !defined(UDM_VERSION_MAJOR) || !defined(UDM_VERSION_MINOR)\n\
#    error \"Udm headers too old, they do not define UDM_VERSION\"\n\
#elif UDM_VERSION_MAJOR < %4%\n\
#    error \"Udm headers too old, minimum version required %4%.%5%\"\n\
#elif UDM_VERSION_MAJOR == %4% && UDM_VERSION_MINOR < %5%\n\
#    error \"Udm headers too old, minimum version required %4%.%5%\"\n\
#endif\n\
\n\
#include <Uml.h>\n\
\n\
")
		% hname
		% fname
		% (string) diagram.name()
		% UDM_VERSION_MAJOR
		% UDM_VERSION_MINOR
		% (opts.rec_tstamp ? GetTime().c_str() : "") );

	if (opts.mode == UdmOpts::CXX_GENERIC) {
		if (mpl_seq_max_size > BOOST_MPL_LIMIT_VECTOR_SIZE) {
			// find the nearest upper multiple of ten
			int new_limit = mpl_seq_max_size;
			if (new_limit % 10)
				new_limit = new_limit - new_limit % 10 + 10;

			r.push_back( boost::format("\
#define BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS\n\
#define BOOST_MPL_LIMIT_VECTOR_SIZE %1%\n\
")
				% new_limit );
		}

		r.push_back( boost::format("\
#include <UdmMPL.h>\n") );
	}

	r.push_back( boost::format("\
\n\
#ifdef min\n\
#undef min\n\
#endif\n\
\n\
#ifdef max\n\
#undef max\n\
#endif\n\
\n") );

	return r;
}


vector<boost::format> UdmGen::CPPPreamble(const string &fname, const ::Uml::Diagram &cross_dgr) const
{
	vector<boost::format> r;

	r.push_back( boost::format("\
// cpp (meta datanetwork format) source file %1%.cpp\n\
// generated from diagram %2%\n\
// generated on %3%\n\
\n\
#include \"%1%.h\"\n\
#include <UmlExt.h>\n\
#include <UdmStatic.h>\n\
")
			% fname
			% (string) diagram.name()
			% (opts.rec_tstamp ? GetTime().c_str() : "") );

	if (opts.integrate_xsd)
	{
		r.push_back( boost::format("#include <UdmDom.h>\n\
#include \"%1%_xsd.h\"") % (string) diagram.name() );

		::Uml::DiagramNamespaces nses(diagram);
		for (::Uml::DiagramNamespaces::iterator nses_i = nses.begin(); nses_i != nses.end(); nses_i++)
			r.push_back( boost::format("#include \"%1%_xsd.h\"") % NameToFilename(nses_i->getPath2("_")) );
	}

	if (!opts.ns_map.empty() && !opts.integrate_xsd)
		r.push_back( boost::format("#include <UdmDom.h>") );

	r.push_back( boost::format("using namespace std;") );
	r.push_back( boost::format("") );

	if (cross_dgr && cross_dgr != diagram)
		r.push_back( boost::format("\
// cross-package metainformation header file\n\
#include \"%1%.h\"\n\
")
				% (string) cross_dgr.name() );

	return r;
}


vector<boost::format> UdmGen::CPPURIMapping() const
{
	vector<boost::format> r;

	if (!opts.ns_map.empty())
	{
		r.push_back( boost::format("\t// URI namespace mapping for dom backend, udm was invoked with switch -u") );

		map<string, string>::const_iterator it =  opts.ns_map.begin();
		for (; it != opts.ns_map.end(); ++it)
		{
			const std::string& ns = it->first;
			const std::string& uri = it->second;

			r.push_back( boost::format("\t::UdmDom::AddURIToUMLNamespaceMapping(\"%1%\", \"%2%\", \"%3%_%4%.xsd\");") % uri % ns % (string) diagram.name() % UdmUtil::replace_delimiter(ns, "::", "_") );
		}
	}

	return r;
}


vector<boost::format> UdmGen::CPPXsdStorage() const
{
	vector<boost::format> r;

	if (opts.integrate_xsd)
	{
		::Uml::DiagramNamespaces nses(diagram);
		for (::Uml::DiagramNamespaces::iterator nses_i = nses.begin(); nses_i != nses.end(); ++nses_i)
		{
			const ::Uml::Namespace& ns = *nses_i;
			const std::string& nsn = NameToFilename(ns.getPath2("_"));
			std::string infname(opts.out_dir + nsn + std::string(".xsd"));

			File2Code f2c(opts.out_dir, nsn + std::string("_xsd"), infname, File2Code::CPP);
			f2c.gen();
			r.push_back( boost::format("\t::UdmDom::str_xsd_storage::StoreXsd(\"%1%.xsd\", %1%_xsd::getString());") % nsn );
		}

		const string& dgrn = diagram.name();
		string infname(opts.out_dir + dgrn + ".xsd");
		File2Code f2c(opts.out_dir, dgrn + "_xsd", infname, File2Code::CPP);
		f2c.gen();
		r.push_back( boost::format("\tUdmDom::str_xsd_storage::StoreXsd(\"%1%.xsd\", %1%_xsd::getString());") % dgrn );
	}

	return r;
}


boost::format UdmGen::HPostamble(const string &fname) const
{
	string hname("MOBIES_" + fname + "_H");
	boost::to_upper(hname);

	return boost::format("#endif // %1%\n") % hname;
}

boost::format UdmGen::HExport(const string &fname) const
{
	string dgr_fname = NameToFilename(diagram.name());

	string hname = "MOBIES_" + fname + "_export_H";

	return boost::format("#ifndef %1%\n\
#	define %1%\n\
#	ifndef %2%\n\
#		if defined (%3%) && (%3% == 1)\n\
#			if defined (%4%)\n\
#				define %2%__declspec(dllexport)\n\
#			else\n\
#				define %2%__declspec(dllimport)\n\
#			endif /* %4% */\n\
#		else\n\
#			define %2%\n\
#		endif /* %3% */\n\
#	endif /* ifndef %2% */\n\
#endif /* %1% */\n\
")
		% hname
		% opts.macro
		% string(dgr_fname + "_HAS_DLL")
		% string(dgr_fname + "_EXPORT");

}

boost::format UdmGen::HIncludeExport(const string &fname) const
{
	if (opts.macro.size())
		return boost::format("#include \"%1%_export.h\"\n\n") % fname;
	else
		return boost::format("");
}

void UdmGen::GenHExport(const string &filename)
{
	if (opts.macro.size())
	{
		string exp_filename = filename + "_export.h";

		ofstream out_export;
		out_export.open( (opts.out_dir + exp_filename).c_str() );
		if (!out_export.good()) throw udm_exception("Error opening for write " + opts.out_dir + exp_filename);

		out_export << HExport(filename);
		out_export.close();
	}
}

void UdmGen::Generate(const ::Uml::Diagram &cross_dgr, const string &filename)
{
	DiagramGen dgen(diagram, *this);
	dgen.Process(cross_dgr);


	// diagram.h source
	ofstream out_h;

	out_h.open( (opts.out_dir + filename + ".h").c_str() );
	if (!out_h.good()) throw udm_exception("Error opening for write " + opts.out_dir + filename + ".h");

	vector<boost::format> r1 = HPreamble(filename);
	for (vector<boost::format>::const_iterator i = r1.begin(); i != r1.end(); i++)
		out_h << *i;

	out_h << HIncludeExport(filename);

	dgen.OutH(out_h);

	out_h << HPostamble(filename);
	out_h.close();


	// diagram.cpp source
	ofstream out_cpp;
	out_cpp.open( (opts.out_dir + filename + ".cpp").c_str() );
	if (!out_cpp.good()) throw udm_exception("Error opening for write " + opts.out_dir + filename + ".cpp");

	vector<boost::format> r2 = CPPPreamble(filename, cross_dgr);
	for (vector<boost::format>::const_iterator i = r2.begin(); i != r2.end(); i++)
		out_cpp << *i << endl;

	dgen.OutCPP(out_cpp);

	out_cpp.close();


	// diagram-export.h source
	GenHExport(filename);
}

void UdmGen::SetMPLSequenceMaxSize(int size)
{
	if (size > mpl_seq_max_size)
		mpl_seq_max_size = size;
}

void OutFmts(ostream &out, const string &idt, const vector<boost::format> &v, bool semicolon)
{
	if (v.size())
	{
		for (vector<boost::format>::const_iterator i = v.begin(); i != v.end(); i++)
		{
			out << idt << *i;
			if (semicolon)
				out << ";";
			out << endl;
		}
		out << endl;
	}
}

void OutVecFmts(ostream &out, const string &idt, const vector< vector<boost::format> > &v, const string &label)
{
	if (v.size())
	{
		if (label.length())
			out << idt << "// " << label << endl;
		for (vector< vector<boost::format> >::const_iterator i = v.begin(); i != v.end(); i++)
		{
			out << idt << "{" << endl;
			OutFmts(out, idt + "\t", *i);
			out << idt << "}" << endl;
		}
		out << endl;
	}
}


} // namespace UdmCPPGen
