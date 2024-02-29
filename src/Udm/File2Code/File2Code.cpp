#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <limits>

#include "File2Code.h"

File2Code::File2Code(
                     const std::string& dest_dir,// the path of the target directory
                     const std::string& name, // the name of the generated source file
                    const std::string& infname, // the input xsd file
                    const GenModeType& mode,   // mode = java or cpp
                    const std::string& pn): // package name for java
  m_dest_dir(dest_dir),
  m_fname(name),
  m_infname (infname),
  m_mode(mode),
  m_packageName(pn),
  m_linenum(1000) // mondjon egy nagy valószínuséget: 2 (Give me a high probability)
{
}

File2Code::~File2Code(void)
{
}

//==================================
void File2Code::gen()
{
  std::ifstream in(m_infname.c_str());
  if (!in.good())
  {
    std::string err = std::string("Could not open '") + m_infname + "'";
    throw std::runtime_error(err);
  }



  if (m_mode == JAVA)
  {
    std::string ofn(( m_dest_dir + "//" + m_fname + ".java" ));
    std::ofstream out( ofn.c_str( ) );
    if ( !out.good() )
    {
        throw std::runtime_error("Error opening file for writing " + ofn);
    }
    StrStrVecMap sm;
    generateMap(in, sm);
  
    genJavaFiles(sm);
    genMainJavaFile(out, sm);
  }
  else
  if (m_mode == CPP)
  {

  
    std::string ofn(( m_dest_dir + "//" + m_fname + ".h" ));
    std::ofstream out( ofn.c_str( ) );
    if ( !out.good() )
    {
        throw std::runtime_error("Error opening file for writing " + ofn);
    }
    m_linenum = std::numeric_limits<long>::max();
    StrStrVecMap sm;
    generateMap(in, sm);
    genMainCppFile(out, sm);
  
  }
}
//==================================
void File2Code::genMainCppFile(std::ostream& out, const StrStrVecMap& sm)
{

  genCppHead(out);
  StrStrVecMap::const_iterator it = sm.begin();
  for(;it != sm.end(); ++it)
  {
    
    const StrVec& sv = it->second;
    StrVec::const_iterator itv = sv.begin();
    for(;itv != sv.end(); ++itv)
    {
      out << *itv << std::endl;
    }
  }
  genCppTail(out);
}


//==================================
void File2Code::genMainJavaFile(std::ostream& out, const StrStrVecMap& sm)
{
  genJavaHead(out, m_fname, sm);
  StrStrVecMap::const_iterator it = sm.begin();
  for(;it != sm.end(); ++it)
  {
      out << "\t\t\tstr += ";   
      out << it->first << ".getString();" << std::endl;
  }
  out << "\t\t}" << std::endl;
  out << "\t\treturn str;" << std::endl;
  out << "\t}" << std::endl;

  genJavaTail(out);

}

//==================================
void File2Code::genJavaFiles(const StrStrVecMap& sm)
{
  StrStrVecMap::const_iterator it = sm.begin();
  for(;it != sm.end(); ++it)
  {
    const std::string& name = it->first;
    std::string fname =  m_dest_dir + "//" + name + std::string(".java");
    std::ofstream out(fname.c_str());
    if (!out.good())
    {
      std::string err = fname + std::string(" could not open.");
      throw std::runtime_error(err);
    }
    genJavaHead(out, name);
    const StrVec& sv = it->second;
    StrVec::const_iterator itv = sv.begin();
    for(;itv != sv.end(); ++itv)
    {
      out << *itv << std::endl;
    }
    genJavaTail(out);
  }
}
//==================================
void File2Code::genJavaHead(std::ostream& out, const std::string& name, const StrStrVecMap& sm)
{
  if (!m_packageName.empty())
    out << "package " << m_packageName << ";" << std::endl << std::endl;

  StrStrVecMap::const_iterator it = sm.begin();
  for(;it != sm.end(); ++it)
  {
    out << std::endl << "import " << m_packageName << "." << it->first << ";";
  }
  out << std::endl ;
  out << std::endl ;

  out << "public class " << name << " {" << std::endl;
  out << "\tprivate static String str = new String("");" << std::endl << std::endl;
  out << "\tpublic static String getString()" << std::endl;
  out << "\t{" << std::endl;
  out << "\t\tif (str.length() == 0)" << std::endl;
  out << "\t\t{" << std::endl;

}
//==================================
void File2Code::genJavaTail(std::ostream& out)
{
    out << "}" << std::endl;
}

//==================================
void File2Code::genCppHead(std::ostream& out)
{
  
    out << "#ifndef " <<  m_fname << "_H" << std::endl;
    out << "#define " << m_fname << "_H" << std::endl;
    out << "#include <string>" << std::endl;
    out << "#pragma warning( disable : 4010)" << std::endl << std::endl;
    out << "namespace " << m_fname <<std::endl;
    out << "{" << std::endl;
    out << "const std::string& getString()" << std::endl;
    out << "{" << std::endl;
    out << "\tstatic std::string str;" << std::endl;
    out << "\tif (str.empty())" << std::endl;
    out << "\t{" << std::endl;
  
}

//==================================
void File2Code::genCppTail(std::ostream& out)
{
    out << "} //namespace" << std::endl;
    out << "#endif" << std::endl;
}
//==================================
void File2Code::replace(std::string& out,
                        const std::string& str,
                        const std::string& s1,
                        const std::string& s2)
{
    out.clear();
      std::string::size_type p = str.find_first_of(s1,0);
      if (p == std::string::npos)
        out += str;
      else
      {

        std::string::size_type oldp = 0;
        while(p != std::string::npos)
        {
          out += str.substr(oldp,p-oldp);
          out += s2;
          oldp = p + 1;
          p = str.find_first_of(s1, oldp);
        }
        out += str.substr(oldp,str.size() - oldp);
      }
}
//==================================
void  File2Code::generateMap(std::istream& in, StrStrVecMap& map)
{
    long fnum = 0;
    while(true)
    {
      std::stringstream ss;
      ss << m_fname << "_" << fnum++;
      StrVec& vec = map[ss.str()];
      
      std::string str;
      
      long linenum = 0;
      while(in.good() && (linenum++ < m_linenum))
      {
        std::stringstream out;
        std::getline(in, str);
        if (str.find("<!-- generated on ") != std::string::npos) {
          out << "//"; 
        }
        out << "\t\tstr +=";   
        out << "\"";   

        std::string tmp;
        replace(tmp, str, "\\", "\\\\");
        std::string tmp1;
        replace(tmp1, tmp, "\"", "\\\"");
        /*std::string tmp;
        replace(tmp, str, "\\&quot;", "\\\\&quot;");
        std::string tmp1;
        replace(tmp1, tmp, "\\n", "\\\\n");*/

	std::string::size_type b_eol = tmp1.find_last_not_of("\r\n");
        if (b_eol != std::string::npos)
          tmp1.erase(b_eol + 1);
        if (!tmp1.empty() && (tmp1.at(0) == '\r' || tmp1.at(0) == '\n'))
          tmp1.erase();

        out << tmp1;
        out << "\\n\"";  
        out << ";";
        vec.push_back(out.str());
      }
      vec.push_back("\t}");
      vec.push_back("\t\treturn str;");
      vec.push_back("}");
      
      if (!in.good())
       break;
    }
}
