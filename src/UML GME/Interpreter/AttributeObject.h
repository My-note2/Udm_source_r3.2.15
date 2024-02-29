#ifndef ATTRIBUTE_OBJECT_H
#define ATTRIBUTE_OBJECT_H

/*
CHANGELOG

  10/24/04	-	endre

		- Added a new type, 'Text'. It's the same as String but it gets persisted differently in DOM backend.
*/

#ifdef UML2XML
#include "Uml.h"
#include "UdmUtil.h"
#endif

//Visibility specifiers

class AttributeObjectException : public std::exception
{
	const std::string err;
public:
	AttributeObjectException (const std::string str) : err(str){}
	const char * what() { return err.c_str();};
};

//visibility stuff

#define VSC_PUBLIC		'+'
#define VSC_PROTECTED	'#'
#define VSC_PRIVATE		'-'
#define VSC_PACKAGE		'~'

#define VSS_PUBLIC		"public"
#define VSS_PROTECTED	"protected"
#define VSS_PRIVATE		"private"
#define VSS_PACKAGE		"package"

//volatile stuff
#define VSS_VOLATILE	"volatile"
#define VSS_REGISTER	"registry"		// register attributes will be stored as register values in GME backend

enum visibility
{
	Public = 0,
	Protected,
	Private,
	Package
};

//type stuff

#define TS_INTEGER		"Integer"
#define TS_REAL			"Real"
#define TS_BOOLEAN		"Boolean"
#define TS_STRING		"String"
#define TS_TEXT			"Text"			// same as String in GME and MEM backends
										// in DOM backend is persisted as the direct value of the node
enum type
{
	Integer = 0,
	Real,
	Boolean,
	String,
	Text
};

//order stuff
#define OS_ORDERED		"ordered"
#define OS_UNORDERED	"unordered"

//bool values
#define BV_TRUE			"true"
#define BV_FALSE		"false"




class AttributeObject
{

private:
	std::string UML_attribute; //UML attribute specifier notation, as a whole

	//required
	type t;
	std::string name;

	//optional
	CardinalityObject c_obj;
	visibility vis;
	
	//default value
	std::vector<bool> defval_bool;
	std::vector<std::string> defval_string;
	std::vector<long>	defval_int;
	std::vector<double> defval_real;

	//real def value
	long real_int;//the integer part of real values; real values are set in two steps (HACK!)
	bool real_int_set;

	//order
	bool ordered;

	//status
	bool type_set;
	bool name_set;

	//volatile
	bool nonpersistent;

	//register
	bool reg_val;

	static AttributeObject * theInstance;
	//cardinality check
	bool CheckCard() const;
	
	//type to str converters
	std::string strType();
	std::string strVisibility();



public:
	AttributeObject(std::string str);
	~AttributeObject();
//functions called by the parser
	void setName(const std::string str);
	void setType(const std::string str);
	void setOrder(const std::string str);
	void setVisib(const std::string str);
	void setVisibNp(const std::string str);
	void setVisib(const char c);
	void setVolReg(const std::string str);

	void addDefBool(const std::string str);
	void addDefNum(const std::string str);
	void addDefReal(const std::string str);
	void addDefStr(const std::string str);

	//what are these for ?
	/*
	isTypeBool() const;
	isTypeReal() const;
	isTypeString() const;
	*/
	bool CheckNum(const std::string str) const;

	//parse
	void parse();
	//get the Instance
	static AttributeObject * GetInstance();

	

#ifdef UML2XML
	void BuildUML(::Uml::Class &uml_cls);
#endif
	/*
		Get Methods Implemented By Paka
	*/

	inline bool IsOrdered() const { return ordered; }
	inline type GetType() const { return t; }
	inline bool IsSet() const { return c_obj.getmax() == -1 || c_obj.getmax() > 1; }
	inline std::string GetName() const { return name; }

};


/*
	string Validators for the parser
*/

bool isTypeSpec(const std::string str);
bool isOrderSpec(const std::string str);
bool isVisibSpec(const std::string str);
bool isVolatileSpec(const std::string str);
bool isBoolSpec(const std::string str);
bool isVisibNpSpec(const std::string str);


#endif
