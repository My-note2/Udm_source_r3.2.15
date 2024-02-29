// UdmVizDump.cpp: implementation of the UdmVizDump class.
//
//////////////////////////////////////////////////////////////////////

#include "UdmVizDump.h"
#include "UmlExt.h"
#include "UdmUtil.h"

using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

UdmVizDump::UdmVizDump()
{
	bAggregate=false;
	bLinks=true;

}

UdmVizDump::~UdmVizDump()
{

}


/*digraph g {
graph [
rankdir = "LR"
];
node [
fontsize = "12"
shape = "ellipse"
];
edge [
];
*/
void UdmVizDump::Dump(Udm::SmartDataNetwork *sdnBackend)
{
	try 
	{

		cout<<"digraph g {"<<endl;
		cout<<"graph ["<<endl;
		cout<<"rankdir = \"LR\""<<endl;
		cout<<"];"<<endl;
		cout<<"node ["<<endl;
		cout<<"fontsize = \"12\""<<endl;
		cout<<"shape = \"ellipse\""<<endl;
		cout<<"];"<<endl;
		cout<<"edge ["<<endl;
		cout<<"];"<<endl;


		Udm::Object ob=sdnBackend->GetRootObject();		
		VisitObjects(ob);	
		if(bLinks)
			VisitLinks(ob.__impl());

		cout<<"}"<<endl;
	} 
	catch(udm_exception &exc) 
	{
		cerr<<"Exception occurred: "<<exc.what()<<endl;
	}
}

void UdmVizDump::VisitObjects(Udm::Object ob, int nDepthLevel) 
{

	// Displaying attributes
	cout<<endl;
	ExtractAttributes(ob, nDepthLevel);
		

	vector<Udm::ObjectImpl*> a = ob.__impl()->getChildren(NULL,NULL);
	
	vector<Udm::ObjectImpl*>::const_iterator i = a.begin();
	while( i != a.end() )
	{
		Udm::Object childObject(*i);

		VisitObjects(childObject, ++nDepthLevel);
		if(bAggregate)
			cout<<'\"'<<ob.uniqueId()<<'\"'<<" -> "<<'\"'<<childObject.uniqueId()<<'\"'<<" [\nid = "<<childObject.uniqueId()<<"\narrowtail=\"diamond\"\n\n];"<<endl;
		++i;
	}
	
	
}


/* Udm objects with attributes */
//"object1" [
//label = "<f0>Name| <f1>Attribute1=value1 | <f2>Attribute2=value2 |Attribute3=value3"
//shape = "record"
//];


void UdmVizDump::ExtractAttributes(Udm::Object ob, int nDepthLevel)
{
	// Getting Attributes from meta
	::Uml::Class cls= ob.type();
	long ObjectID=ob.uniqueId();
	
	cout<<'\"'<<ObjectID<<'\"'<<"["<<endl;
	
	int i=0;
	string name=UdmUtil::ExtractName(ob);
	string umlName=name+":"+(string)cls.name();
	cout<<"label="<<"\"<"<<'f'<<i++<<">"<<umlName;


	set< ::Uml::Attribute> attrs=cls.attributes();	
	
	// Adding parent attributes
	set< ::Uml::Attribute> aattrs=Uml::AncestorAttributes(cls);
	attrs.insert(aattrs.begin(),aattrs.end());
	
	
	for(set< ::Uml::Attribute>::iterator ai = attrs.begin();ai != attrs.end(); ai++) 
	{
		if(string(ai->type())=="String")
		{
			if(string(ai->name())=="name") continue;
			

			string value=ob.getStringAttr(*ai);
			if(value.empty())value="(empty string)";

			cout<<" | <"<<'f'<<i<<">"<<string(ai->name())<<": "<<value;
			
		}
		else if(string(ai->type())=="Integer")
		{
			cout<<" | <"<<'f'<<i<<">"<<string(ai->name())<<": "<<ob.getIntegerAttr(*ai);

		}
		else if(string(ai->type())=="Boolean")
		{
			cout<<" | <"<<'f'<<i<<">"<<string(ai->name())<<": "<<ob.getBooleanAttr(*ai)<<ob.getBooleanAttr(*ai);

		}
		else if(string(ai->type())=="Real")
		{
			cout<<" | <"<<'f'<<i<<">"<<string(ai->name())<<": "<<ob.getRealAttr(*ai)<<ob.getRealAttr(*ai);
			
		}
		i++;
	}
	cout<<'\"'<<endl;
	cout<<"shape = \"record\"\n];"<<endl;
}



int UdmVizDump::VisitLinks(Udm::ObjectImpl* p_srcObjectImpl)
{
	
	using namespace Udm;

	int ret=0;

	::Uml::Class srcClass= p_srcObjectImpl->type();
	set< ::Uml::Class> ancestorClasses=::Uml::AncestorClasses(srcClass);

	for(set< ::Uml::Class>::iterator p_currClass=ancestorClasses.begin();
					p_currClass!=ancestorClasses.end(); p_currClass++)
	{
		// Getting the association roles and iterating through them
		set< ::Uml::AssociationRole> assocRoles=p_currClass->associationRoles();
		for(set< ::Uml::AssociationRole>::iterator p_currAssocRole=assocRoles.begin();
							p_currAssocRole!=assocRoles.end();p_currAssocRole++)
		{
			::Uml::Class assocClass=::Uml::Association(p_currAssocRole->parent()).assocClass();

			bool isAssocClass=assocClass?true:false;

			vector<ObjectImpl*>dstPeers=p_srcObjectImpl->\
				getAssociation(::Uml::theOther(*p_currAssocRole),\
				isAssocClass?Udm::CLASSFROMTARGET : Udm::TARGETFROMPEER);
			

			for(vector<ObjectImpl*>::iterator p_currDstPeer=dstPeers.begin();
								p_currDstPeer!=dstPeers.end(); p_currDstPeer++)
			{
				Udm::Object dstObject(*p_currDstPeer);

				// If we are in an association class
				if(isAssocClass)
				{
					if( objectIDSet.insert(dstObject.uniqueId()).second==false )
					{
						continue;
					}

					// Get target peer					
					vector<ObjectImpl*> targetPeers =dstObject.__impl()->getAssociation(::Uml::theOther(*p_currAssocRole),Udm::TARGETFROMCLASS);
					ObjectImpl* p_targetObjImpl=*targetPeers.begin();

					// Creating virtual node
					cout<<'\"'<<p_srcObjectImpl->uniqueId()<<p_targetObjImpl->uniqueId()<<'\"';
					cout<<"[shape=circle,width=.01,height=.01,label=\"\"];\n\n";


					// Form virtual node to assoc class
					cout<<'\"'<<dstObject.uniqueId()<<'\"'<<" -> "<<'\"';
					cout<<p_srcObjectImpl->uniqueId()<<p_targetObjImpl->uniqueId()<<'\"';
					cout<<" [\n weight=4 arrowhead=\"none\" style=\"dotted\"\n];"<<endl;


		
/*
yourvirtualnode [shape=circle,width=.01,height=.01,label=""];
a -> yourvirtualnode [arrowhead=none]
yourvirtualnode -> b

 */	

/*
					cout<<'\"'<<p_srcObjectImpl->uniqueId()<<'\"'<<":f0 -> "<<'\"';
					cout<<p_targetObjImpl->uniqueId()<<'\"';
					cout<<":f0 [\n arrowhead=\"none\" headlabel=\"";
					cout<<(string)(*p_currAssocRole).name() <<"\" taillabel=\"";
					cout<<(string)theOther(*p_currAssocRole).name() <<"\"\n];"<<endl;
					*/
					
					// From source to virtual node
					cout<<'\"'<<p_srcObjectImpl->uniqueId()<<'\"'<<" -> "<<'\"';
					cout<<p_srcObjectImpl->uniqueId()<<p_targetObjImpl->uniqueId()<<'\"';
					cout<<" [\n weight=3 arrowhead=\"none\" taillabel=\"";
					cout<<(string)(*p_currAssocRole).name() <<"\"\n];"<<endl;


					// From virtual node to target

					cout<<'\"'<<p_srcObjectImpl->uniqueId()<<p_targetObjImpl->uniqueId()<<'\"'<<" -> "<<'\"';
					cout<<p_targetObjImpl->uniqueId()<<'\"';
					cout<<" [\n weight=3 arrowhead=\"none\" headlabel=\"";
					cout<<(string)::Uml::theOther(*p_currAssocRole).name() <<"\"\n];"<<endl;
					

				}
				else
				{
					cout<<'\"'<<p_srcObjectImpl->uniqueId()<<'\"'\
						<<" -> "<<'\"'<<dstObject.uniqueId()<<'\"'\
						<<" [\n arrowhead=\"none\" headlabel=\""\
						<<(*p_currAssocRole).name() <<"\" taillabel=\""<<::Uml::theOther(*p_currAssocRole).name() \
						<<"\"\n];"<<endl;
				}

				
			}
			
		}
	}


	// Traversing the source object network via the containtment hierarchy
	vector<ObjectImpl*>children=p_srcObjectImpl->getChildren(NULL,NULL);
	for(vector<ObjectImpl*>::iterator p_currImpl=children.begin();
		p_currImpl!=children.end();p_currImpl++)
	{
		ObjectImpl* p_srcChild=*p_currImpl;



		/////////////// Recursive Call to Itself //////////////////
		ret=VisitLinks(p_srcChild);
		///////////////////////////////////////////////////////////

		p_srcChild->release();

		if(ret)return ret;
	}	
			
	return ret;

}
