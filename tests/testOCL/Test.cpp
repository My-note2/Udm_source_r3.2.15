
#include <UdmBase.h>





#include  <UdmBase.h>
#include  <UdmDom.h>
#include  <sstream>
#include  <fstream>
#include <UdmOcl.h>
#include "SBML2.h"
#include <stdexcept>
#include "Test.h"

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(UdmTests::Test);

/*
#include  "sbml_xsd.h"
#include "math_xsd.h"
*/
using namespace ::SBML2::sbml;
using namespace ::SBML2::math;
using namespace ::Udm;
//==================================
void UdmTests::Test::readSBML(const std::string& fname)
{
 	std::string   sbml_xsdName("SBML2_sbml.xsd"); 
//  std::string sxsd(sbml_xsd::getString());
/*
  UdmDom::str_xsd_storage::StoreXsd(sbml_xsdName , sxsd);
  UdmDom::str_xsd_storage::StoreXsd("math.xsd" , sbml_xsd::getString());
*/

  std::ifstream in(fname.c_str());
  if (!in.good())
    throw std::runtime_error("Could not open " + fname);

  std::stringstream ins;
  ins << in.rdbuf();

  std::string xml(ins.str());


  UdmDom::DomDataNetwork  ind(::SBML2::diagram);
  ind.OpenExistingFromString(xml,sbml_xsdName, Udm::CHANGES_PERSIST_ALWAYS);
  ind.CloseNoUpdate();

}


//==================================
void UdmTests::Test::readTest()
{
 	std::string   sbml_xsdName("SBML2_sbml.xsd"); 
//  std::string sxsd(sbml_xsd::getString());

//  UdmDom::str_xsd_storage::StoreXsd(sbml_xsdName , sxsd);

  std::ifstream in("test.xml");
  if (!in.good())
    throw std::runtime_error("Could not open test.xml");

  std::stringstream ins;
  ins << in.rdbuf();

 	//std::ostringstream oss;
  //executeTransform(ins,oss, getSBMLTrafoxslt().c_str());
 	//fromSBML(ins, oss);
 	std::string xml = ins.str();


  UdmDom::DomDataNetwork  ind(::SBML2::diagram);
  ind.OpenExistingFromString(xml,sbml_xsdName, Udm::CHANGES_PERSIST_ALWAYS);
  ind.CloseNoUpdate();

}

//==================================
void UdmTests::Test::selfTest()
{

  std::string fname("test.xml");

  UdmDom::DomDataNetwork  out(::SBML2::diagram);
	
 	std::string   sbml_xsdName("SBML2_sbml.xsd"); 
//  std::string sxsd(sbml_xsd::getString());

//  UdmDom::str_xsd_storage::StoreXsd(sbml_xsdName , sxsd);


	out.CreateNew(fname.c_str(), 
		sbml_xsdName,
    sbml::meta, Udm::CHANGES_PERSIST_ALWAYS);


  


//   std::ofstream oo("SBML2.xsd");
//   oo << SBML2xsd;
//   oo.close();
// 
// 	UdmDom::DomDataNetwork  out(diagram);
// 	out.CreateNewToString(sbml_xsdName, sbml::meta, Udm::CHANGES_PERSIST_ALWAYS);

  sbml sbml = sbml::Cast(out.GetRootObject());


	model model = model::Create(sbml);
 	listOfSpecies listOfSpecies  = listOfSpecies::Create(model);
  ::SBML2::sbml::species species0  = ::SBML2::sbml::species::Create(listOfSpecies);
	species0.id() = "species0";
  species0.annotation() = "annotation0";

  ::SBML2::sbml::species species1  = ::SBML2::sbml::species::Create(listOfSpecies);
	species0.id() = "species0";
	species1.annotation() = "annotation1";



	listOfReactions listOfReactions  = listOfReactions::Create(model);
	reaction reaction  = reaction::Create(listOfReactions);

  kineticLaw kineticLaw = kineticLaw::Create(reaction);
  math math = math::Create(kineticLaw);
  apply apply = apply::Create(math);
  times times = times::Create(apply);
  //apply.ci() = "a";
  //minus minus = minus::Create(apply);

	listOfReactants listOfReactants  = listOfReactants::Create(reaction);

	speciesReference speciesReference0  = speciesReference::Create(listOfReactants);
	speciesReference0.species() = "species0";
	speciesReference speciesReference1  = speciesReference::Create(listOfReactants);
	speciesReference1.species() = "species1";


  listOfRules listOfRules  = listOfRules::Create(model);
 	rule rule  = rule::Create(listOfRules);
 


  out.CloseWithUpdate();
//   std::string fname("test.xml");
//   std::ofstream of(fname.c_str());
//   std::string str = out.Str();
//   of << out.Str();
//   of << std::ends;
//   of.close();
  



//   std::ifstream is(fname.c_str());
//   
//   std::string xml0;
// 	while(!std::getline(is, xml0, '|').eof())
//  				std::getline(is, xml0);
// 
//   {
//   std::string fname("test1.xml");
//   std::ofstream of(fname.c_str());
//   of << xml0;
//   of.close();
//   }
//  	UdmDom::DomDataNetwork  in(diagram);  
//   in.OpenExistingFromString(xml0,sbml_xsdName);
//   in.CloseNoUpdate();

  {
    UdmDom::DomDataNetwork  in(::SBML2::diagram);
  in.OpenExisting("test.xml",sbml_xsdName, Udm::CHANGES_PERSIST_ALWAYS);
  in.CloseNoUpdate();
  }

}

//==================================

void UdmTests::Test::evaluate(const ::Uml::Diagram& metaDiagram,
              const Udm::Object& objContext,
              std::ostream& out)
{

	Ocl::SErrorNotification en(false, Ocl::ENT_ALL) ;

	Ocl::Initialize( metaDiagram, en );

	Ocl::Evaluator eval(objContext);
	Ocl::SEvaluationOptions options;
	options.sErrorNotification = en;

	try
	{
		Ocl::EEvaluationResult eResult = eval.Check(options);
	}
	catch ( const udm_exception& ex )
	{
		CPPUNIT_ASSERT_MESSAGE(ex.what(), 0);
	}

	Ocl::UnInitialize( metaDiagram );
}


//==================================
void UdmTests::Test::read(const std::string& fname,
              std::ostream& out)
{
  std::stringstream ins;

  std::ostringstream tmp;
  std::ifstream in(fname.c_str());
  if (!in.good())
    throw std::runtime_error("Could not open " + fname);

  ins << in.rdbuf();

 	//std::ostringstream oss;
  //executeTransform(ins,oss, getSBMLTrafoxslt().c_str());
 	//fromSBML(ins, oss);
 	std::string xml = ins.str();

   std::ofstream outt("test_out.xml");
   outt << xml;
   outt.close();

 	std::string   sbml_xsdName("SBML2_sbml.xsd");

  //UdmDom::str_xsd_storage::StoreXsd(sbml_xsdName , sbml_xsd::getString());
  //UdmDom::str_xsd_storage::StoreXsd("math.xsd" , math_xsd::getString());


  UdmDom::DomDataNetwork  indn(::SBML2::diagram);
	indn.OpenExistingFromString(xml,sbml_xsdName, Udm::CHANGES_PERSIST_ALWAYS);



	sbml irf = sbml::Cast(indn.GetRootObject());

  evaluate(*(::SBML2::diagram.dgr), irf, out);

 
}
//==================================
void UdmTests::Test::test()
{

  //readSBML("test.xml");
//  readSBML("versionCheck.xml");
  //selfTest();
  
  //std::stringstream str;
  read("versionCheck.xml", cout);
  
  
}
