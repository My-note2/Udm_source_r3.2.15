// BackendDump.cpp: implementation of the CBackendDump class.
//
//////////////////////////////////////////////////////////////////////

#include <cstdio>


#include "BackendDump.h"

using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBackendDump::CBackendDump(ostream* an_os)
{
	dump=an_os;
}

CBackendDump::~CBackendDump()
{
	dump=NULL;
}


void CBackendDump::ExtractAttributes(Udm::Object ob, int nDepthLevel)
{
	// Getting Attributes from meta
	::Uml::Class cls= ob.type();
	long ObjectID=ob.uniqueId();
	*dump<<"Object '"<< UdmUtil::ExtractName(ob, "name")<<"["<<ObjectID<<"]"<<"("<<nDepthLevel<<")"<< "' of type '"<<cls.getPath2("::", false)<<"'" << endl;

	Udm::Object archetype = ob.archetype();
	if (archetype) {
		*dump << "\t";
		if (ob.hasRealArchetype())
			*dump << "explicit ";
		if (ob.isInstance())
			*dump << "instance of '" << UdmUtil::ExtractName(archetype) << "[" << archetype.uniqueId() << "]'" << endl;
		else if (ob.isSubtype())
			*dump << "subtype of '" << UdmUtil::ExtractName(archetype) << "[" << archetype.uniqueId() << "]'" << endl;
	}

	*dump<<"attributes:"<<endl;
	

	set< ::Uml::Attribute> attrs=cls.attributes();	
	
	// Adding parent attributes
	set< ::Uml::Attribute> aattrs=Uml::AncestorAttributes(cls);
	attrs.insert(aattrs.begin(),aattrs.end());

	
	
	for(set< ::Uml::Attribute>::iterator ai = attrs.begin();ai != attrs.end(); ai++) 
	{
		if(string(ai->type())=="String")
		{
			*dump<<"  "<<string(ai->name()).c_str()<<": ";

			string value=ob.getStringAttr(*ai);
			if(value.empty())value="<empty string>";
			*dump<<value.c_str()<<endl;
			
		}
		else if(string(ai->type())=="Integer")
		{
			*dump<<"  "<<string(ai->name())<<": ";
			
			*dump << (long)ob.getIntegerAttr(*ai);
			*dump << endl;

		}
		else if(string(ai->type())=="Boolean")
		{
			*dump<<"  "<<string(ai->name())<<": ";
			
			*dump<<ob.getBooleanAttr(*ai)<<endl;

		}
		else if(string(ai->type())=="Real")
		{
			*dump<<"  "<<string(ai->name())<<": ";
			
			*dump<<ob.getRealAttr(*ai)<<endl;

		}

	}
}



void CBackendDump::DumpNamespaces(const ::Uml::Diagram &dgr)
{
	*dump<<endl<<endl<<"Meta Namespaces"<<endl;
	*dump<<"____________"<<endl;

	::Uml::DiagramNamespaces metanses(dgr);
	for(::Uml::DiagramNamespaces::iterator i = metanses.begin(); i != metanses.end(); i++) 
	{
		*dump<<" "<< i->getPath2("::", false) << endl;
	}
}

void CBackendDump::DumpClasses(const ::Uml::Diagram &dgr)
{
	*dump<<endl<<endl<<"Meta Classes"<<endl;
	*dump<<"____________"<<endl;

	::Uml::DiagramClasses metaclasses(dgr);
	for(::Uml::DiagramClasses::iterator ii = metaclasses.begin(); ii != metaclasses.end(); ii++) 
	{
		*dump<<" "<< ii->getPath2("::", false) << endl;
	}
}

void CBackendDump::DumpCompositions(const ::Uml::Diagram &dgr)
{
	*dump<<endl<<endl<<"Meta Compositions"<<endl;
	*dump<<"_________________"<<endl;

	::Uml::DiagramCompositions metacomps(dgr);
	for(::Uml::DiagramCompositions::iterator iii = metacomps.begin(); iii != metacomps.end(); iii++) 
	{
		*dump<<endl<<"  "<< iii->getPath2("::", false) <<endl;
		*dump<<"    "<<"ParentRole"<<endl;
			

		::Uml::CompositionParentRole parentRole=iii->parentRole();
		*dump<<"      "<<string(parentRole.name())<<endl;


		string strTmp="IsNavigable: ";
		parentRole.isNavigable()?strTmp+="true":strTmp+="False";
		*dump<<"      "<<strTmp<<endl;

		*dump<<"    "<<"Child Role"<<endl;
			

		::Uml::CompositionChildRole childRole=iii->childRole();
		strTmp=childRole.name();
		*dump<<"      "<<strTmp<<endl;
			

		strTmp="IsNavigable: ";
		childRole.isNavigable()?strTmp+="true":strTmp+="False";
		*dump<<"      "<<strTmp<<endl;

		__int64 minm=childRole.min(),maxm=childRole.max();

		char strMax[40];
		char strTmp2[255];

		if(maxm==-1)
			sprintf(strMax,"%s","*");
		else
		{
#ifdef _WIN32
			sprintf(strMax,"%I64d",maxm);
#else
			sprintf(strMax,"%lld",maxm);
#endif
		}

		if(minm==maxm)
		{
#ifdef _WIN32
			sprintf(strTmp2,"Multiplicity: %I64d", minm);				
#else			
			sprintf(strTmp2,"Multiplicity: %lld", minm);				
#endif							
		}
		else
		{
#ifdef _WIN32
			sprintf(strTmp2,"Multiplicity: %I64d..%s", minm,strMax);				
#else
			sprintf(strTmp2,"Multiplicity: %lld..%s", minm,strMax);				
#endif
		}

		*dump<<"      "<<strTmp2<<endl;
			
	}
}

void CBackendDump::DumpAssociations(const ::Uml::Diagram &dgr)
{
	*dump<<endl<<endl<<"Meta Associations"<<endl;
	*dump<<"_________________"<<endl;

	::Uml::DiagramAssociations metaas(dgr);
	for(::Uml::DiagramAssociations::iterator i = metaas.begin();i != metaas.end(); i++) 
	{
		*dump<<endl<<"  "<< i->getPath2("::", false) <<endl;

		set< ::Uml::AssociationRole> metaroles=i->roles();
		for(set< ::Uml::AssociationRole>::iterator ir=metaroles.begin();ir!=metaroles.end();ir++)
		{
			string strTmp=ir->name();

			*dump<<"    "<<strTmp<<endl;

			strTmp="IsNavigable: ";
			ir->isNavigable()?strTmp+="true":strTmp+="False";

			*dump<<"      "<<strTmp<<endl;
				
			strTmp="IsPrimary: ";
			ir->isPrimary()?strTmp+="true":strTmp+="False";
			*dump<<"      "<<strTmp<<endl;

			__int64 minm=ir->min(),maxm=ir->max();
			char strMax[40];
			char strTmp2[255];

			if(maxm==-1)
				sprintf(strMax,"%s","*");
			else
			{
#ifdef _WIN32
				sprintf(strMax,"%I64d",maxm);
#else
				sprintf(strMax,"%lld",maxm);
#endif
			}

			if(minm==maxm)
			{
#ifdef _WIN32
				sprintf(strTmp2,"Multiplicity: %I64d", minm);				
#else
				sprintf(strTmp2,"Multiplicity: %lld", minm);
#endif
					
			}
			else
			{
#ifdef _WIN32
				sprintf(strTmp2,"Multiplicity: %I64d..%s", minm,strMax);				
#else
				sprintf(strTmp2,"Multiplicity: %lld..%s", minm,strMax);				
#endif
			}

			*dump<<"      "<<strTmp2<<endl;

		}
		
	}
}

void CBackendDump::Dump(Udm::SmartDataNetwork *sdnBackend)
{
	try 
	{
	
		//____________________________Meta Information________________________________________
		*dump<<"META INFORMATION"<<endl;
		*dump<<"________________"<<endl;

		::Uml::Diagram dgr=sdnBackend->GetRootMeta();
		
		// Namespaces
		DumpNamespaces(dgr);
		
		// Classes
		DumpClasses(dgr);
		
		// Compositions
		DumpCompositions(dgr);

		// Associations
		DumpAssociations(dgr);
		

		// ____________________________ Object Network Information __________________________
		*dump<<endl<<endl<<"OBJECT NETWORK INFORMATION"<<endl;
		*dump<<"__________________________"<<endl;
		Udm::Object ob=sdnBackend->GetRootObject();

		*dump<<endl<<endl<<"Objects"<<endl;
		*dump<<"_______"<<endl;
		VisitObjects(ob);		

		//Links
		*dump<<endl<<endl<<"Links"<<endl;
		*dump<<"_____"<<endl;
		VisitLinks(ob);

		*dump<<endl<<"Successful dump."<<endl;


	} 
	catch(udm_exception &exc) 
	{
		*dump<<"Exception occurred: "<<exc.what()<<endl;
	}

}


void CBackendDump::ExtractLinks(Udm::Object ob)
{
	::Uml::Class cls= ob.type();				

	set< ::Uml::AssociationRole> roles=cls.associationRoles();
	set< ::Uml::AssociationRole> aroles=Uml::AncestorAssociationRoles(cls);


	roles.insert(aroles.begin(),aroles.end());


	for(set< ::Uml::AssociationRole>::iterator ri = roles.begin();ri != roles.end(); ri++) 
	{

		set<Udm::Object> os=ob.getAssociation(Uml::theOther(*ri));

		if(os.size()>0)
		{
			*dump<<endl<<"Links from object '"<<UdmUtil::ExtractName(ob, "name")<<"["<<ob.uniqueId()<<"]' to"<<endl;
			for(set<Udm::Object>::iterator asi=os.begin();asi!=os.end();asi++)
			{
				*dump<<"   '"<<UdmUtil::ExtractName(*asi, "name")<<"["<<asi->uniqueId()<<"]',"<<endl;
				*dump<<"   "<<" with source role '"<<string(ri->name())<<"',"<<endl; 
				*dump<<"   "<<" and with destination role '" <<string(Uml::theOther(*ri).name())<<"'"<<endl;
			}
		}		
	}
}



void CBackendDump::VisitObjects(Udm::Object ob, int nDepthLevel) 
{

	// Displaying attributes
	*dump<<endl;
	ExtractAttributes(ob, nDepthLevel);
	
	nDepthLevel++;

	vector<Udm::ObjectImpl*> a = ob.__impl()->getChildren(NULL,NULL);

	vector<Udm::ObjectImpl*>::const_iterator i = a.begin();
	while( i != a.end() )
	{
		VisitObjects(*i, nDepthLevel);

		++i;
	}
	
}

void CBackendDump::VisitLinks(Udm::Object ob) 
{
		
	ExtractLinks(ob);

		vector<Udm::ObjectImpl*> a = ob.__impl()->getChildren(NULL,NULL);
		
		vector<Udm::ObjectImpl*>::const_iterator i = a.begin();
		while( i != a.end() )
		{
			VisitLinks(*i);
			++i;
		}
	
}
