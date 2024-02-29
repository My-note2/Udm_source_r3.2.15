def indent(indent_tabs=0):
	indent = ""
	for i in range(0,indent_tabs):
		indent += "\t";
	return indent

class UdmPython(object):

	def __init__(self, impl):
		if isinstance(impl, UdmPython):
			#"copy constructor"... 
			self.__dict__['impl'] = impl.impl
			self.__dict__['id'] = impl.impl.id
			self.__dict__['indent'] = 0
		else:
			#construction from udm.Object
			self.__dict__['impl'] = impl
			self.__dict__['id'] = impl.id
			self.__dict__['indent'] = 0

	def is_lib_root(self):
		return self.impl.is_lib_root
	def is_lib_object(self):
		return self.impl.is_lib_object

	def _type(self):
		import Uml
		return Uml.Class.cast(self.impl.type)
	def _get_parent(self, parent_role=None):
		#parent_role could be either Udm.Object or UdmPython (or descendants: it is expected to be Uml.CompositionChildRole)
		parent_role_impl = None
		if isinstance(parent_role, UdmPython):
			parent_role_impl = parent_role.impl
		else:
			parent_role_impl = parent_role
		return UdmPython(self.impl.getParent(parent_role_impl))
		
	def _get_children(self,child_role=None, child_type=None):
		#child_type could be either Udm.Object or UdmPython (or descendants: it is expected to be Uml.Class)
		#child_role could be either Udm.Object or UdmPython (or descendants: it is expected to be Uml.CompositionChildRole)
		#if role is given, return only those children that have that role (ignore kind)
		#else if kind is not null, return all children which are compatible with kind
		#else if kind is null, return all children


		ret = []
		child_type_impl = None
		child_role_impl = None
		
		if isinstance(child_type, UdmPython):
			child_type_impl = child_type.impl
		else:
			child_type_impl = child_type;

		if isinstance(child_role, UdmPython):
			child_role_impl = child_role.impl
		else:
			child_role_impl = child_role;

		for child in self.impl.getChildren(child_role_impl, child_type_impl):
			ret.append(UdmPython(child))
		return ret

	def _get_associations(self, assoc_role, mode):
		#assoc_role could be either Udm.Object or UdmPython (or descendants)
		ret = []
		assoc_role_impl = None
		if isinstance(assoc_role, UdmPython):
			assoc_role_impl = assoc_role.impl
		else:
			assoc_role_impl = assoc_role
		for peer in self.impl.getAssociation(assoc_role_impl, mode):
			ret.append(UdmPython(peer))
		return ret

	def __eq__(self, other):
		return self.__dict__['impl'] == other.__dict__['impl']
	def __ne__(self, other):
		return self.__dict__['impl'] != other.__dict__['impl']
	def __repr__(self):
		return self.impl.__repr__()
	def setIndent(self,i):
		self.__dict__['indent'] = i

	def __str__(self):
		import udm	
		if self.impl == udm.null:
			 return self.impl.__str__()
		import Uml
		line = str()
		cl = Uml.Class(self.impl.type)
		indent_tabs = self.__dict__['indent']
		line = indent(indent_tabs) + "--------------------------------------------------------------------" + "\n"
		line += indent(indent_tabs) + "Object's type:"  + cl.name + "\n"
		line += indent(indent_tabs) + self.impl.__repr__() + "\n"
		#attrs= cl.getAttributeChildren()
		attrs= cl.attributes()
		for attr in attrs:
			attr_value = self.impl.get_attribute(attr.impl)
			if attr.max == 1 or attr.max == 0:
				if attr.type == "Integer":
					line +=  indent(indent_tabs) + "attribute name: " + attr.name + ", value: %d " % (attr_value) +"\n"
				elif attr.type == "Real":
					line += indent(indent_tabs) + "attribute name: " + attr.name + ", value: %f " % (attr_value) + "\n"
				elif attr.type == "Boolean":
					boolstr = "true" if attr_value else "false"
					line += indent(indent_tabs) + "attribute name: " + attr.name + ", value:" + boolstr + "\n"
                		elif attr.type == "String":
					line += indent(indent_tabs) + "attribute name: " + attr.name + ", value:" + attr_value + "\n"
            		else:
				a_attr = ArrayAttr(self, attr)
				a_attr.setIndent(indent_tabs)
				line += a_attr.__str__() + "\n"
		return line

	def __getattr__(self, name):
		return self.impl.__getattr__(name)
	def __setattr__(self, name, value):
		self.impl.__setattr__(name, value)
	def __nonzero__(self):
		return self.impl.__nonzero__();

class ArrayAttr(object):
	@property
	def array(self):
		attr_value=self.obj.get_attribute(self.attr.impl)
		return ArrayAttr.escaped_split(str(attr_value), ';')

	@array.setter
	def array(self, val):
		if type(val) != type([]):
			raise Exception("ArrayAttr.array.setter: expected array")
		new_arr = []
		for val_item in val:
			new_arr.append(str(val_item))#this will work for any type
		arr_str = ArrayAttr.escaped_combine(new_arr, ';')
        	self.obj.set_attribute(self.attr.impl, arr_str)

	@staticmethod
	def escaped_split(s, delim):
		ret = []
		current = []
		itr = iter(s)
		for ch in itr:
			if ch == '\\':
				try:
                			# skip the next character; it has been escaped!
                			#current.append('\\')
                			current.append(itr.next())
            			except StopIteration:
                			pass
        		elif ch == delim:
            			# split! (add current to the list and reset it)
            			ret.append(''.join(current))
            			current = []
        		else:
            			current.append(ch)
    		ret.append(''.join(current))
    		return ret
	@staticmethod
	def escaped_combine(s, delim):
		ret = ""
		first = True
		for s_i in s:
			if not first: ret += delim
			first = False
			itr = iter(s_i)
			for ch in itr:
				if ch == '\\' or ch == delim:
					ret += '\\'
				ret += ch
		return ret;


	def __init__(self, obj, attr):
		import Uml
		if attr.max==0 or attr.max == 1:
			raise ("Attribute is not an array")
		
		self.attr = attr
		self.obj = obj.impl
		self.indent = 0

	def setIndent(self,i):
		self.indent = i

	def __str__(self):
		import Uml
		lines = indent(self.indent) + "Array attribute name: " + self.attr.name + ", type:" + self.attr.type
		lines += "\n"
		attr_value=self.obj.get_attribute(self.attr.impl)
		str_array = ArrayAttr.escaped_split(str(attr_value), ';')
		i=0
		for str_i in str_array:
			lines += indent(self.indent) + self.attr.name + "[" + str(i) + "] =" + str_i
			lines +="\n"
			i = i + 1
		return lines

	def __getitem__(self,index):
		attr_value=self.obj.get_attribute(self.attr.impl)
		str_array = ArrayAttr.escaped_split(str(attr_value), ';')
		if index > len(str_array):
			raise Exception("Index out of array boundaries")
		return str_array[index]

	def __setitem__(self, index, value):

		attr_value=self.obj.get_attribute(self.attr.impl)
		str_array = ArrayAttr.escaped_split(str(attr_value), ';')
		if index > len(str_array):
			raise Exception("Index out of array boundaries")
		str_array_new = []
		i=0;
		for str_array_i in str_array:
			if i == index:
				str_array_new.append(str(value))
			else:
				str_array_new.append(str_array_i)
			i=i+1
		arr_str = ArrayAttr.escaped_combine(str_array_new, ';')
        	self.obj.set_attribute(self.attr.impl, arr_str)


class StrArrayAttr(ArrayAttr):

	def __init__(self, obj, attr):
		import Uml
		if attr.type != "String":
			raise Exception("Attribute is not of type String")
		super(StrArrayAttr, self).__init__(obj, attr)


			
class IntArrayAttr(ArrayAttr):
	@property
	def array(self):
		str_array = super(IntArrayAttr, IntArrayAttr).array.fget(self)
		ret = []
		for str_i in str_array:
			ret.append(int(str_i))
		return ret

	def __init__(self, obj, attr):
		import Uml
		if attr.type != "Integer":
			raise ("Attribute is not of type Integer")
		super(StrArrayAttr, self).__init__(obj, attr)

	def __getitem__(self,index):
		return int(super(IntArrayAttr, self).__getitem__(index))


class RealArrayAttr(ArrayAttr):
	@property
	def array(self):
		str_array = super(IntArrayAttr, IntArrayAttr).array.fget(self)
		ret = []
		for str_i in str_array:
			ret.append(float(str_i))
		return ret

	def __init__(self, obj, attr):
		import Uml
		if attr.type != "Real":
			raise ("Attribute is not of type Real")
		super(StrArrayAttr, self).__init__(obj, attr)

	def __getitem__(self,index):
		return float(super(RealArrayAttr, self).__getitem__(index))



class BooleanArrayAttr(ArrayAttr):
	@property
	def array(self):
		str_array = super(IntArrayAttr, IntArrayAttr).array.fget(self)
		ret = []
		for str_i in str_array:
			ret.append(lower(str_i) == "true")
		return ret
	
	def __init__(self, obj, attr):
		import Uml
		if attr.type != "Boolean":
			raise ("Attribute is not of type Boolean")
		super(StrArrayAttr, self).__init__(obj, attr)

	def __getitem__(self,index):
		value = super(RealArrayAttr, self).__getitem__(index)
		return value.lower()=="true" 
			
		
def GetUmlAttributeByName(cl, name):
	import udm
	import Uml
	#attrs = cl.getAttributeChildren()
	attrs = cl.attributes()
	for attr in attrs:
		if attr.name == name:
			return attr
	return Uml.Attribute(udm.null)

def GetUmlClassByName(dgr, name):
	import udm
	import Uml
	#classes = dgr.getClassChildren()
	classes = dgr.Class_kind_children()
	for cl in classes:
		if cl.name == name:
			return cl
	return Uml.Class(udm.null)	

def GetUmlChildRoleByTypesAndRolenames(child_type, parent_type, crole_name, prole_name):
	import udm
	import Uml
	#child_type is expected to be Uml.Class
	#parent_type is expected to be Uml.Class
	#crole_name and prole_name are expected to be strings

	return Uml.CompositionChildRole(udm.InitChildRole(child_type.impl, parent_type.impl, crole_name, prole_name))

def GetUmlParentRoleByTypesAndRolenames(child_type, parent_type, crole_name, prole_name):
	import udm
	import Uml
	#child_type is expected to be Uml.Class
	#parent_type is expected to be Uml.Class
	#crole_name and prole_name are expected to be strings

	return Uml.CompositionParentRole(udm.InitParentRole(child_type.impl, parent_type.impl, crole_name, prole_name))
	
def GetUmlAssocRoleByPeerAndRole(my_type, peer_type, arole_name):
	import udm
	import Uml
	#my_type is expected to be Uml.Class
	#peer_type is expected to be Uml.Class
	#arole_name is expected to be string

	return Uml.AssociationRole(udm.GetUmlAssocRoleByPeerAndRole(my_type.impl, peer_type.impl, arole_name))

def GetUmlAssocRoleByAClassAndRole(my_type, aclass_type, arole_name):
	import udm
	import Uml
	#my_type is expected to be Uml.Class
	#aclass_type is expected to be Uml.Class
	#arole_name is string

	return Uml.AssociationRole(udm.GetUmlAssocRoleByAClass(my_type.impl, aclass_type.impl, arole_name))



