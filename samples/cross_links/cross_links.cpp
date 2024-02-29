// cross_links.cpp : Defines the entry point for the console application.
//





#include <UdmBase.h>
#include "CD1.h"
#include "CD2.h"
#include "CD3.h"
#include "CL.h"
#include <UmlExt.h>
#include <functional>

UDM_USE_DOM

using namespace std;

int main_static(int argc, char * argv[])
{
	try
	{
		/*
			Create regular data networks
		*/
		Udm::SmartDataNetwork sm1(CD1::diagram);
		Udm::SmartDataNetwork sm2(CD2::diagram);
		Udm::SmartDataNetwork sm3(CD3::diagram);

		sm1.CreateNew("sm1.xml", "CD1", CD1::M1::meta, Udm::CHANGES_LOST_DEFAULT);
		sm2.CreateNew("sm2.xml", "CD2", CD2::M2::meta, Udm::CHANGES_LOST_DEFAULT);
		sm3.CreateNew("sm3.xml", "CD3", CD3::MM3::meta, Udm::CHANGES_LOST_DEFAULT);

		CD1::M1 m1 = CD1::M1::Cast(sm1.GetRootObject());
		CD2::M2 m2 = CD2::M2::Cast(sm2.GetRootObject());
		CD3::MM3 m3 = CD3::MM3::Cast(sm3.GetRootObject());

		/*
			Create a static udm project which does not offer
			any persistence services for the cross-links,
			but allowes to use such links runtime

			The StaticUdmProject is being constructed from 
			existing datanetworks through StaticDataNetworkSpecifiers
		*/
		Udm::StaticDataNetworkSpecifier snds_1("sm1.mem", &sm1);
		Udm::StaticDataNetworkSpecifier snds_2("sm2.mem", &sm2);
		Udm::StaticDataNetworkSpecifier snds_3("sm3.mem", &sm3);

		vector<Udm::StaticDataNetworkSpecifier> vsnds;
		vsnds.push_back(snds_1);
		vsnds.push_back(snds_2);
		vsnds.push_back(snds_3);

		Udm::StaticUdmProject sup(vsnds, CL::diagram);

		/*
			Finnally set a cross-package link among existing regular
			datanetworks.
		*/

		m2.dst() = m1;
		set<CD1::M1> a; a.insert(m1);
		m3.src() = a;

		sup.Close();
	}
	catch (udm_exception &e)
	{
		cout << e.what() << endl;
	}

	return 0;
};
int main(int argc, char* argv[])
{
	try 
	{
		{
			Udm::UdmProject pr;

			//specifying instance data networks which will be present in the project
			Udm::DataNetworkSpecifier cd1_dn("CD1_inst.xml", "CD1", CD1::M1::meta);
			Udm::DataNetworkSpecifier cd2_dn("CD2_inst.xml", "CD2", CD2::M2::meta);
			Udm::DataNetworkSpecifier cd3_dn("CD3_inst.xml", "CD3", CD3::MM3::meta);

			//putting them in a vector
			vector<Udm::DataNetworkSpecifier> dnsvec;
			dnsvec.push_back(cd1_dn);
			dnsvec.push_back(cd2_dn);
			dnsvec.push_back(cd3_dn);

			//creating a Udm Project
			pr.CreateNew("sample.udm", dnsvec,CL::diagram);

			//obtaining handles to the datanetworks which were created
			Udm::DataNetwork& cd1 = pr.GetDataNetwork("CD1_inst.xml");
			Udm::DataNetwork& cd2 = pr.GetDataNetwork("CD2_inst.xml");
			Udm::DataNetwork& cd3 = pr.GetDataNetwork("CD3_inst.xml");

			//obtaining handles to the root objects (this is normal - existing - Udm operation)
			CD1::M1 m1 = CD1::M1::Cast(cd1.GetRootObject());
			CD2::M2 m2 = CD2::M2::Cast(cd2.GetRootObject());
			CD3::MM3 m3 = CD3::MM3::Cast(cd3.GetRootObject());

			/*
			//assoc. class based link - setting up by setting the ends 
			
			m1.src_end() = m2;
			m1.dst_end() = m3;


			//test
			CD2::M2 m2_too = m1.src_end();
			if (m2 != m2_too)
				throw udm_exception("Test failed!");

			CD3::M3 m3_too = m1.dst_end();
			if (m3 != m3_too)
				throw udm_exception("Test failed!");

			
			*/
			//assoc. class based link - setting up by setting the links from the ends

			
			set<CD1::M1> a;
			m2.dst() = m1;
			 a.insert(m1);
			m3.src() = a;
			

			//test

			CD1::M1 m1_too = m2.dst();
			if (m1 != m1_too)
				throw udm_exception("Test failed!");

			set<CD1::M1> a_too = m3.src();
			if (a_too.size() != a.size()) 
				throw udm_exception("Test failed!");
			if (*(a_too.begin()) != m1)
				throw udm_exception("Test failed!");


			Udm::Object::AssociationInfo ai(&Udm::_null);
			ai.strSrcRoleName = "b";
			ai.strDstRoleName = "a";

			m1.CreateLink(m3, ai);


			set<CD1::M1, greater_equal<CD1::M1> > a_tooo = m3.src_sorted(greater_equal<CD1::M1>());
			
			
/*			
			//simple links
			//setting a cross-network  simple link
			set<CD2::M2> s; s.insert(m2);
			m1.dst() = s;

			//getting a cross-network simple link
			set<CD2::M2> ss = m1.dst();
*/			


			set<Uml::AssociationRole> asr = Uml::AncestorCrossAssociationRoles(CD3::MM3::meta, *CL::diagram.dgr);
			cout << asr.size() << endl;




		}
		{
			Udm::UdmProject pr;
			pr.OpenExisting("sample.udm");

		}
		main_static(argc, argv);

		return 0;
	}


	catch(udm_exception &e)
	{
		cout << e.what() << endl;
		return 11;
	};

	return 0;

}
