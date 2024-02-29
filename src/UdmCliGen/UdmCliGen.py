import sys
import os
import os.path
sys.path.append(os.path.join(os.environ['UDM_PATH'], "bin"))
import udm
import common

if os.environ.has_key("UDM_3RDPARTY_PATH"):
    sys.path.append(os.path.join(os.environ["UDM_3RDPARTY_PATH"], r"Cheetah-2.4.4\build\lib.win32-2.6"))

from Cheetah.Template import Template

udm.Object.__nonzero__ = lambda(self) : self.id != 0

def get_template(name, **kwargs):
    if hasattr(sys, "frozen"): #i.e. .exe generated by py2exe
        __import__(name)
        return getattr(sys.modules[name], name)(**kwargs)
    else:
        return Template(file=os.path.join(os.path.dirname(os.path.realpath(__file__ )), name+'.tmpl'), **kwargs)
   

if __name__ == "__main__":
    from optparse import OptionParser
    parser = OptionParser(usage="usage: %prog [options] input-uml.xml")
    parser.add_option("-o", "--output", dest="output", help="File or directory to output to.")
    parser.add_option("--impl_namespace", dest="impl_namespace")
    parser.add_option("--interface_namespace", dest="interface_namespace")
    (options, args) = parser.parse_args()
    if len(args) != 1:
        print parser.print_help()
        sys.exit()
    if options.impl_namespace:
        common.impl_namespace = options.impl_namespace + "."
    if options.interface_namespace:
        common.interface_namespace = options.interface_namespace + "."
    
    uml = udm.map_uml_names(udm.uml_diagram())
    dn = udm.SmartDataNetwork(udm.uml_diagram())
    dn.open(args[0], "")

    if options.output:
        if os.path.isdir(options.output):
            output = open(os.path.join(options.output, dn.root.name + ".cs"), "w")
        else:
            output = open(options.output, "w")
        # TODO: close this
    else:
        output = sys.stdout

    output.write("#pragma warning disable 0108\n"); # disable "'member1' hides inherited member 'member2'. Use the new keyword if hiding was intended."

    def get_classes(dn):
        classes = []
        classes.extend(dn.root.children(child_type=uml.Class))
        import collections
        q = collections.deque()
        q.extend(dn.root.children(child_type=uml.Namespace))
        while q:
            namespace = q.pop()
            q.extend(namespace.children(child_type=uml.Namespace))
            classes.extend(namespace.children(child_type=uml.Class))
        return classes
    
    for child in get_classes(dn):
        searchList = {'c': child, 'namespace': common.interface_namespace + common.get_path(child.parent), 'uml': uml, 'diagram_name': dn.root.name}
        t = get_template("Interface", searchList=[searchList])
        output.write(str(t))

        searchList = {'c': child, 'namespace': common.impl_namespace + common.get_path(child.parent), 'uml': uml, 'diagram_name': dn.root.name, 'root': dn.root}
        t = get_template("Implementation", searchList=[searchList])
        output.write(str(t))

    searchList = {'root': dn.root}
    t = get_template("Initialize", searchList=[searchList])
    output.write(str(t))
