#ifndef CARDINALITY_OBJECT_H
#define CARDINALITY_OBJECT_H


class CardinalityObjectException : public std::exception
{
	const std::string err;
public:
	CardinalityObjectException (const std::string str) : err(str){}
	const char * what() const { return err.c_str();};
};

class CardinalityObject
{
	int min;
	int max;
	std::string UML_multiplicity;
	bool min_set;
	bool star_rcvd;
	static CardinalityObject * theInstance;

public:
	//constructor - to be used from AttributeObject
	CardinalityObject();
	//constructor - to parse in a string directly
	CardinalityObject(const std::string str);
	//functions called by the parser
	void int_rcv(const int i);
	void star_rcv();
	//getters 
	int getmin() const;
	int getmax() const;
	std::string text();

	//get the Instance
	static CardinalityObject * GetInstance();
	void ResetInstance();

};

#endif //CARDINALITY_OBJECT