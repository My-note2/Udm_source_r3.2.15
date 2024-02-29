// test_ns2.cpp : Defines the entry point for the console application.
//

#include "CHost.h"

void writeTest(const std::string& fname)
{

	Udm::SmartDataNetwork  out(CHost::diagram);
	out.CreateNew(fname.c_str(), "CHost", CHost::RootFolder::meta, Udm::CHANGES_PERSIST_ALWAYS);

	CHost::RootFolder rf = CHost::RootFolder::Cast( out.GetRootObject() );
	rf.name() = "hello, this is rf!";

	CHost::C::CModel cm = CHost::C::CModel::Create(rf);
	cm.name() = "C::CModel obj";

	CHost::A::DAtom d = CHost::A::DAtom::Create(cm);
	d.name() = "A::DAtom obj";

	CHost::C::C2Model c2m = CHost::C::C2Model::Create(rf);
	
	CHost::A::AModel a2 = CHost::A::AModel::Create(c2m, CHost::C::C2Model::meta_Role1);
	CHost::A::AModel a3 = CHost::A::AModel::Create(c2m, CHost::C::C2Model::meta_Role2);

	std::set<CHost::A::AModel> role1s = (std::set<CHost::A::AModel>)c2m.Role1();
	if (role1s.size() == 0)
		throw udm_exception("role1s is empty");
	std::set<CHost::A::AModel> role2s = (std::set<CHost::A::AModel>)c2m.Role2();
	if (role2s.size() == 0)
		throw udm_exception("role2s is empty");

	out.CloseWithUpdate();
}

//==========================================
void readTest(const std::string& fname)
{

	Udm::SmartDataNetwork  out(CHost::diagram);
	out.OpenExisting(fname.c_str(), "", Udm::CHANGES_PERSIST_ALWAYS);

	out.CloseNoUpdate();
}
//=========================================

int main()
{
	try
	{	
		std::string fn;
#ifdef _WIN32
		fn = "test1.mga";
		writeTest(fn);
		readTest(fn);
#endif
		fn = "test1.mem";
		writeTest(fn);
		readTest(fn);

		fn = "test1.xml";
		writeTest(fn);
		readTest(fn);
		
		
	}
	catch(udm_exception& e)
	{
	    std::cout << e.what();
	    return 1;
	}
		cout << "OK!" << endl;
	return 0;
}

