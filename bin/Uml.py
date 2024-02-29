"""Generated
"""

import sys
import Uml
from UdmPython import *

class GenericRole(UdmPython):
	"""Generated"""
	#isNavigable = "isNavigable";

	#name = "name";

	@staticmethod
	def cast(obj):
		"""Object cast"""
		return GenericRole(obj)
	
	def type(self):
		"""returning the type of object (Uml.Class)"""
		return Uml.Class.cast(self.meta)
	
	# Associations 

class CompositionParentRole(UdmPython):
	"""Generated"""
	@staticmethod
	def cast(obj):
		"""Object cast"""
		return CompositionParentRole(obj)
	
	def type(self):
		"""returning the type of object (Uml.Class)"""
		return Uml.Class.cast(self.meta)
	
	# Associations 

	"""
	 Association with role name <code>target</code>.
	"""

	def settarget(self, a): 
		"""
		Sets the other end of the association with role name <code>target</code>.
		@param a The other end of the association
		"""
		pass

	def target(self):
		""" Returns the other end of the association with role name <code>target</code>.
		@return The other end of the association
		"""
		return Class.cast(self.impl.target)
	
class CompositionChildRole(UdmPython):
	"""Generated"""
	#max = "max";

	#min = "min";

	@staticmethod
	def cast(obj):
		"""Object cast"""
		return CompositionChildRole(obj)
	
	def type(self):
		"""returning the type of object (Uml.Class)"""
		return Uml.Class.cast(self.meta)
	
	# Associations 

	"""
	 Association with role name <code>target</code>.
	"""

	def settarget(self, a): 
		"""
		Sets the other end of the association with role name <code>target</code>.
		@param a The other end of the association
		"""
		pass

	def target(self):
		""" Returns the other end of the association with role name <code>target</code>.
		@return The other end of the association
		"""
		return Class.cast(self.impl.target)
	
class AssociationRole(UdmPython):
	"""Generated"""
	#max = "max";

	#min = "min";

	#isPrimary = "isPrimary";

	@staticmethod
	def cast(obj):
		"""Object cast"""
		return AssociationRole(obj)
	
	def type(self):
		"""returning the type of object (Uml.Class)"""
		return Uml.Class.cast(self.meta)
	
	# Associations 

	"""
	 Association with role name <code>target</code>.
	"""

	def settarget(self, a): 
		"""
		Sets the other end of the association with role name <code>target</code>.
		@param a The other end of the association
		"""
		pass

	def target(self):
		""" Returns the other end of the association with role name <code>target</code>.
		@return The other end of the association
		"""
		return Class.cast(self.impl.target)
	
class TaggedValue(UdmPython):
	"""Generated"""
	#value = "value";

	#name = "name";

	@staticmethod
	def cast(obj):
		"""Object cast"""
		return TaggedValue(obj)
	
	def type(self):
		"""returning the type of object (Uml.Class)"""
		return Uml.Class.cast(self.meta)
	
	# Associations 

class Class(UdmPython):
	"""Generated"""
	#isAbstract = "isAbstract";

	#stereotype = "stereotype";

	#name = "name";

	#from = "from";

	@staticmethod
	def cast(obj):
		"""Object cast"""
		return Class(obj)
	
	def type(self):
		"""returning the type of object (Uml.Class)"""
		return Uml.Class.cast(self.meta)
	
	def getTaggedValueChildren(self):
		"""
		Returns all the children of type <code>TaggedValue<code> of this container. 
		@return  The children in a lisst
		"""
		childs = self.impl.children(child_type=TaggedValue.meta)
		list = []
		for i in childs:
			list.append(TaggedValue(i))
		return list

	def getConstraintChildren(self):
		"""
		Returns all the children of type <code>Constraint<code> of this container. 
		@return  The children in a lisst
		"""
		childs = self.impl.children(child_type=Constraint.meta)
		list = []
		for i in childs:
			list.append(Constraint(i))
		return list

	def getAttributeChildren(self):
		"""
		Returns all the children of type <code>Attribute<code> of this container. 
		@return  The children in a lisst
		"""
		childs = self.impl.children(child_type=Attribute.meta)
		list = []
		for i in childs:
			list.append(Attribute(i))
		return list

	def getConstraintDefinitionChildren(self):
		"""
		Returns all the children of type <code>ConstraintDefinition<code> of this container. 
		@return  The children in a lisst
		"""
		childs = self.impl.children(child_type=ConstraintDefinition.meta)
		list = []
		for i in childs:
			list.append(ConstraintDefinition(i))
		return list

	# Associations 

	"""
	 Association with role name <code>baseTypes</code>.
	"""

	def setbaseTypes(self, a): 
		"""
		Sets the other ends of the association with role name <code>baseTypes</code>.
		@param a The other ends of the association
		"""
		pass

	def baseTypes(self):
		"""
		Returns the other ends of the association with role name <code>baseTypes</code>.
		@return The other ends of the association
		""" 
		list = []
		for i in self.impl.baseTypes:
			list.append(Class.cast(i))
		return list
	

	"""
	 Association with role name <code>subTypes</code>.
	"""

	def setsubTypes(self, a): 
		"""
		Sets the other ends of the association with role name <code>subTypes</code>.
		@param a The other ends of the association
		"""
		pass

	def subTypes(self):
		"""
		Returns the other ends of the association with role name <code>subTypes</code>.
		@return The other ends of the association
		""" 
		list = []
		for i in self.impl.subTypes:
			list.append(Class.cast(i))
		return list
	

	"""
	 Association with role name <code>association</code>.
	"""

	def setassociation(self, a): 
		"""
		Sets the other end of the association with role name <code>association</code>.
		@param a The other end of the association
		"""
		pass

	def association(self):
		""" Returns the other end of the association with role name <code>association</code>.
		@return The other end of the association
		"""
		return Association.cast(self.impl.association)
	
	"""
	 Association with role name <code>associationRoles</code>.
	"""

	def setassociationRoles(self, a): 
		"""
		Sets the other ends of the association with role name <code>associationRoles</code>.
		@param a The other ends of the association
		"""
		pass

	def associationRoles(self):
		"""
		Returns the other ends of the association with role name <code>associationRoles</code>.
		@return The other ends of the association
		""" 
		list = []
		for i in self.impl.associationRoles:
			list.append(AssociationRole.cast(i))
		return list
	

	"""
	 Association with role name <code>parentRoles</code>.
	"""

	def setparentRoles(self, a): 
		"""
		Sets the other ends of the association with role name <code>parentRoles</code>.
		@param a The other ends of the association
		"""
		pass

	def parentRoles(self):
		"""
		Returns the other ends of the association with role name <code>parentRoles</code>.
		@return The other ends of the association
		""" 
		list = []
		for i in self.impl.parentRoles:
			list.append(CompositionParentRole.cast(i))
		return list
	

	"""
	 Association with role name <code>childRoles</code>.
	"""

	def setchildRoles(self, a): 
		"""
		Sets the other ends of the association with role name <code>childRoles</code>.
		@param a The other ends of the association
		"""
		pass

	def childRoles(self):
		"""
		Returns the other ends of the association with role name <code>childRoles</code>.
		@return The other ends of the association
		""" 
		list = []
		for i in self.impl.childRoles:
			list.append(CompositionChildRole.cast(i))
		return list
	

class Constraint(UdmPython):
	"""Generated"""
	#description = "description";

	#name = "name";

	#expression = "expression";

	@staticmethod
	def cast(obj):
		"""Object cast"""
		return Constraint(obj)
	
	def type(self):
		"""returning the type of object (Uml.Class)"""
		return Uml.Class.cast(self.meta)
	
	# Associations 

class Attribute(UdmPython):
	"""Generated"""
	#name = "name";

	#type = "type";

	#defvalue = "defvalue";

	#min = "min";

	#max = "max";

	#visibility = "visibility";

	#ordered = "ordered";

	#nonpersistent = "nonpersistent";

	#registry = "registry";

	@staticmethod
	def cast(obj):
		"""Object cast"""
		return Attribute(obj)
	
	def type(self):
		"""returning the type of object (Uml.Class)"""
		return Uml.Class.cast(self.meta)
	
	# Associations 

class ConstraintDefinition(UdmPython):
	"""Generated"""
	#stereotype = "stereotype";

	#name = "name";

	#parameterList = "parameterList";

	#expression = "expression";

	#returnType = "returnType";

	@staticmethod
	def cast(obj):
		"""Object cast"""
		return ConstraintDefinition(obj)
	
	def type(self):
		"""returning the type of object (Uml.Class)"""
		return Uml.Class.cast(self.meta)
	
	# Associations 

class Association(UdmPython):
	"""Generated"""
	#nonpersistent = "nonpersistent";

	#name = "name";

	@staticmethod
	def cast(obj):
		"""Object cast"""
		return Association(obj)
	
	def type(self):
		"""returning the type of object (Uml.Class)"""
		return Uml.Class.cast(self.meta)
	
	def getAssociationRoleChildren(self):
		"""
		Returns all the children of type <code>AssociationRole<code> of this container. 
		@return  The children in a lisst
		"""
		childs = self.impl.children(child_type=AssociationRole.meta)
		list = []
		for i in childs:
			list.append(AssociationRole(i))
		return list

	def getTaggedValueChildren(self):
		"""
		Returns all the children of type <code>TaggedValue<code> of this container. 
		@return  The children in a lisst
		"""
		childs = self.impl.children(child_type=TaggedValue.meta)
		list = []
		for i in childs:
			list.append(TaggedValue(i))
		return list

	# Associations 

	"""
	 Association with role name <code>assocClass</code>.
	"""

	def setassocClass(self, a): 
		"""
		Sets the other end of the association with role name <code>assocClass</code>.
		@param a The other end of the association
		"""
		pass

	def assocClass(self):
		""" Returns the other end of the association with role name <code>assocClass</code>.
		@return The other end of the association
		"""
		return Class.cast(self.impl.assocClass)
	
class Namespace(UdmPython):
	"""Generated"""
	#name = "name";

	@staticmethod
	def cast(obj):
		"""Object cast"""
		return Namespace(obj)
	
	def type(self):
		"""returning the type of object (Uml.Class)"""
		return Uml.Class.cast(self.meta)
	
	def getClassChildren(self):
		"""
		Returns all the children of type <code>Class<code> of this container. 
		@return  The children in a lisst
		"""
		childs = self.impl.children(child_type=Class.meta)
		list = []
		for i in childs:
			list.append(Class(i))
		return list

	def getAssociationChildren(self):
		"""
		Returns all the children of type <code>Association<code> of this container. 
		@return  The children in a lisst
		"""
		childs = self.impl.children(child_type=Association.meta)
		list = []
		for i in childs:
			list.append(Association(i))
		return list

	def getNamespaceChildren(self):
		"""
		Returns all the children of type <code>Namespace<code> of this container. 
		@return  The children in a lisst
		"""
		childs = self.impl.children(child_type=Namespace.meta)
		list = []
		for i in childs:
			list.append(Namespace(i))
		return list

	def getCompositionChildren(self):
		"""
		Returns all the children of type <code>Composition<code> of this container. 
		@return  The children in a lisst
		"""
		childs = self.impl.children(child_type=Composition.meta)
		list = []
		for i in childs:
			list.append(Composition(i))
		return list

	# Associations 

class Composition(UdmPython):
	"""Generated"""
	#nonpersistent = "nonpersistent";

	#name = "name";

	@staticmethod
	def cast(obj):
		"""Object cast"""
		return Composition(obj)
	
	def type(self):
		"""returning the type of object (Uml.Class)"""
		return Uml.Class.cast(self.meta)
	
	def getCompositionParentRoleChild(self): 
		"""
		Return the child of type <code>CompositionParentRole<code> of this container. 
		@return  The child
		"""
		childs = self.impl.children(child_type=CompositionParentRole.meta)
		if len(childs) > 0:
			return CompositionParentRole(childs[0])
		else: 
			return None
	
	def getCompositionChildRoleChild(self): 
		"""
		Return the child of type <code>CompositionChildRole<code> of this container. 
		@return  The child
		"""
		childs = self.impl.children(child_type=CompositionChildRole.meta)
		if len(childs) > 0:
			return CompositionChildRole(childs[0])
		else: 
			return None
	
	# Associations 

class Diagram(UdmPython):
	"""Generated"""
	#version = "version";

	#name = "name";

	@staticmethod
	def cast(obj):
		"""Object cast"""
		return Diagram(obj)
	
	def type(self):
		"""returning the type of object (Uml.Class)"""
		return Uml.Class.cast(self.meta)
	
	def getClassChildren(self):
		"""
		Returns all the children of type <code>Class<code> of this container. 
		@return  The children in a lisst
		"""
		childs = self.impl.children(child_type=Class.meta)
		list = []
		for i in childs:
			list.append(Class(i))
		return list

	def getAssociationChildren(self):
		"""
		Returns all the children of type <code>Association<code> of this container. 
		@return  The children in a lisst
		"""
		childs = self.impl.children(child_type=Association.meta)
		list = []
		for i in childs:
			list.append(Association(i))
		return list

	def getNamespaceChildren(self):
		"""
		Returns all the children of type <code>Namespace<code> of this container. 
		@return  The children in a lisst
		"""
		childs = self.impl.children(child_type=Namespace.meta)
		list = []
		for i in childs:
			list.append(Namespace(i))
		return list

	def getCompositionChildren(self):
		"""
		Returns all the children of type <code>Composition<code> of this container. 
		@return  The children in a lisst
		"""
		childs = self.impl.children(child_type=Composition.meta)
		list = []
		for i in childs:
			list.append(Composition(i))
		return list

	# Associations 


def init_meta(meta_map):
	GenericRole.meta = meta_map.GenericRole
	CompositionParentRole.meta = meta_map.CompositionParentRole
	CompositionChildRole.meta = meta_map.CompositionChildRole
	AssociationRole.meta = meta_map.AssociationRole
	TaggedValue.meta = meta_map.TaggedValue
	Class.meta = meta_map.Class
	Constraint.meta = meta_map.Constraint
	Attribute.meta = meta_map.Attribute
	ConstraintDefinition.meta = meta_map.ConstraintDefinition
	Association.meta = meta_map.Association
	Namespace.meta = meta_map.Namespace
	Composition.meta = meta_map.Composition
	Diagram.meta = meta_map.Diagram


def initialize(meta_map, uml_meta_map):
	try:
		module_initialized
	except NameError:
		init_meta(meta_map)
		module_initialized = True
		if sys.modules[__name__] != Uml:
			Uml.initialize(uml_meta_map, uml_meta_map)

