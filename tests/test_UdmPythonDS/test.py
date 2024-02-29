import sys
sys.path.append('/Users/endre/trunk/lib')
import udm          # C++ UdmPhyton module
import UdmPython    # python wrapper
import Uml          # generated UML classes
import LampDiagram  # generated Domain Specific API

import unittest     # testing




def walk_hierarchy_udm_o(obj, indent_tabs = 0):
    #obj is expected to be Udm.Object
    #walks recursively the hierarchy
    
    if obj:
        
        up_o = UdmPython.UdmPython(obj)
        up_o.setIndent(indent_tabs)
        print up_o
        generic_children = obj.children()
        if generic_children:
            print UdmPython.indent(indent_tabs) + "I have found the following children at this level: %d" % (len(generic_children))
            for child in generic_children:
                walk_hierarchy_udm_o(child, indent_tabs+1)
    else:
        return

def walk_hierarchy_udmp(obj, indent_tabs = 0):
    #obj is expected to be UdmPython.UdmPython or any DS class
    #walks recursively the hierarchy
    
    if obj:
        
        obj.setIndent(indent_tabs)
        print obj
        generic_children = obj._get_children()
        if generic_children:
            print UdmPython.indent(indent_tabs) + "I have found the following children at this level: %d" % (len(generic_children))
            for child in generic_children:
                walk_hierarchy_udmp(child, indent_tabs+1)
    else:
        return



pdn = udm.SmartDataNetwork(udm.uml_diagram())
pdn.open(r"../../samples/LampDiagram_udm.xml","")
LampDiagram.initialize(pdn.root)  #for the moment, this takes as argument and Udm.Object and not Uml.Diagram



dn = udm.SmartDataNetwork(pdn.root)
dn.open(r"Lamp.mem","")


rf =LampDiagram.RootFolder(dn.root)

#walk_hierarchy_udm_o(dn.root)
#walk_hierarchy_udmp(UdmPython.UdmPython(dn.root))
walk_hierarchy_udmp(rf)


l_children = rf.Lamp_kind_children()

if l_children:
    print "I have found the following lamps: "
    for lamp in l_children:
        print lamp.ModelName
        print lamp.name
        print type(lamp)




lamp_1 = l_children[0]

print lamp_1.ModelName
lamp_1.ModelName = "test_python"
print lamp_1.ModelName
print lamp_1.Meta.name

print LampDiagram.Lamp.meta_ModelName
print Uml.Class.meta_name

o = lamp_1.ArrayStr()
print o
print o[2]
o[2] = "hello python"
print o
print o[2]
print lamp_1.ArrayStr()[3]

print o.array

o.array = ["alma", "korte", "szilva"]
print o.array
print len(o.array)
print o[2]

rf = lamp_1.parent
print isinstance(rf, LampDiagram.RootFolder)
#lamp_1.parent = rf

#print lamp1.parent == rf

bulbs = lamp_1.Bulb_kind_children()
for bulb in bulbs:
    print "----------------------------"
    print "Bulb :", type(bulb), bulb
    cl = bulb.dst()
    if len(cl):
        print "Controllink", type(cl[0]), cl[0]
        sw = cl[0].dst_end()
        print "switch", type(sw), sw
        cl1  = sw.src()
        if len(cl1):
        
            print "Controllink1", type(cl1[0]), cl1[0]
            print cl1[0] == cl[0]
            bulb1 = cl1[0].src_end()
            print "Bulb1", type(bulb1), bulb1
            print bulb == bulb1



print "Switch.meta_src: "  + LampDiagram.Switch.meta_src.name  #this should be src
print "Bulb.meta_dst: "  + LampDiagram.Bulb.meta_dst.name      #this should be dst

print "descendants of ElectricDevice are: "
classes = LampDiagram.ElectricDevice.Meta.subTypes()
for cl in classes:
    print cl.name
    print cl.baseTypes()[0] == LampDiagram.ElectricDevice.Meta



controllinks = lamp_1.ControlLink_kind_children()
for cl in controllinks:
    print "Controllink :" + cl.name
    bulb  = cl.src_end()
    switch  = cl.dst_end()

    print "this should be bulb!:"
    print bulb

    print "this should be switch!:"
    print switch




dn.close_no_update()
pdn.close_no_update()
