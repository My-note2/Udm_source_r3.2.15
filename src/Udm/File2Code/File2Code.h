#ifndef File2Code_H
#define File2Code_H
#include <vector>
#include <map>
typedef std::vector<std::string> StrVec;
typedef std::map<std::string, StrVec> StrStrVecMap;

//============================
class File2Code
{
public:
  enum GenModeType{JAVA, CPP};

protected:
  std::string m_dest_dir;
  std::string m_fname;
  long  m_linenum;
  std::string m_infname;
  std::string m_packageName;
  GenModeType m_mode;
protected:
  
  void replace(std::string& out,
                        const std::string& str,
                        const std::string& s1,
                        const std::string& s2);

  void genJavaFiles(const StrStrVecMap& sm);
  void genMainJavaFile(std::ostream& out, const StrStrVecMap& sm);
  void genMainCppFile(std::ostream& out, const StrStrVecMap& sm);
  void genJavaHead(std::ostream& out, const std::string& name,const StrStrVecMap& sm = StrStrVecMap());
  void genJavaTail(std::ostream& out);
  void genCppHead(std::ostream& out);
  void genCppTail(std::ostream& out);
  void  generateMap(std::istream& in, StrStrVecMap& map);

public:
  File2Code(        const std::string& dest_dir, 
                    const std::string& fname, 
                    const std::string& infname, 
                    const GenModeType& mode,
                    const std::string& pn = "");
  void gen();
  ~File2Code(void);
};

#endif //File2Code
