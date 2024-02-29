import sys
import os
import os.path
import unittest

this_dir = os.path.dirname(os.path.abspath(__file__))
os.chdir(this_dir)

from sys import platform as _platform
if _platform == "linux" or _platform == "linux2":
    #linux
    if os.environ.has_key("UDM_PATH"):
        sys.path.append(os.path.join(os.environ["UDM_PATH"], "bin"))
    else:
        sys.path.append(os.path.join(os.path.dirname(__file__), "../../bin/"))


elif _platform == "darwin":
    #darwin
    if os.environ.has_key("UDM_PATH"):
        sys.path.append(os.path.join(os.environ["UDM_PATH"], "lib"))
    else:
        sys.path.append(os.path.join(os.path.dirname(__file__), "../../lib/"))
elif _platform == "win32":
    # Windows...
    sys.path.append(r"C:\Program Files\ISIS\Udm\bin")
    if "UDM_PATH" in os.environ:
        sys.path.append(os.path.join(os.environ["UDM_PATH"], "bin"))

try:
    import udm
    # import udmd as udm
except ImportError:
    sys.stderr.write("Could not find udm Python module in " + repr(sys.path))
    raise
#print udm.__file__

def get_names(col):
    names = map(lambda x: x.name, col)
    names = sorted(names)
    return names


class TestUdmPython(unittest.TestCase):
    def __init__(self, test):
        unittest.TestCase.__init__(self, test)

    def testudmpython(self):
        # need to open meta DN since it isn't compiled in
        test_meta_dn= udm.SmartDataNetwork(udm.uml_diagram())
        test_meta_dn.open(r"UdmPythonTestMeta.xml", "")
        self.assertEquals(test_meta_dn.root.name, "UdmPythonTestMeta")
        test_meta = udm.map_uml_names(test_meta_dn.root)

        dn = udm.SmartDataNetwork(test_meta_dn.root)
        if _platform == "linux" or _platform == "linux2" or _platform == "darwin":
            dn.open(r"UdmPythonTest.xml", "")
        else:
            dn.open(r"UdmPythonTestModel.mga", "")

        container = dn.root.children()[0]
        self.assertTrue(container)
        # get children by type
        self.assertEquals(get_names(container.children(child_type=test_meta.AtomB)), [ "cmproleA1", "cmproleA2", "cmproleB1" ]) # also accepts child_role, parent_role
        # get children by child's composition role
        self.assertEquals(get_names(container.cmproleA_role_children), [ "cmproleA1", "cmproleA2" ])
        self.assertEquals(container.children(child_role="cmproleA"), container.cmproleA_role_children)
        self.assertEquals(get_names(container.cmproleB_role_children), [ "cmproleB1"] )
        self.assertEquals(container.type, test_meta.Container)
        def first(container): return next(container.__iter__())
        cmproleA1 = first(filter(lambda x: x.name =="cmproleA1", container.children()))
        cmproleA2 = first(filter(lambda x: x.name =="cmproleA2", container.children()))
        self.assertEquals(cmproleA1.dstConnection, [cmproleA2])
        self.assertEquals(cmproleA1.adjacent(dst_role="dstConnection"), [cmproleA2])
        self.assertEquals(cmproleA1.adjacent(src_role="srcConnection"), [cmproleA2])
        self.assertEquals(cmproleA2.adjacent(dst_role="dstConnection"), [])
        self.assertEquals(cmproleA2.adjacent(src_role="srcConnection"), [])
        AtomA1 = first(filter(lambda x: x.name =="AtomA1", container.children()))
        self.assertEquals(AtomA1.boolattr, True)
        self.assertEquals(AtomA1.stringattr, "teststring")
        self.assertEquals(AtomA1.intattr, 42)
        AtomA1.intattr = 60
        self.assertEquals(AtomA1.intattr, 60)
        AtomA1.stringattr = "test123"
        self.assertEquals(AtomA1.stringattr, "test123")
        unicode_teststring = u'\xc1rv\xedzt\u0171r\u0151 t\xfck\xf6rf\xfar\xf3g\xe9p\U0001d11e\u0393\u03b1\u03b6\u03ad\u03b5\u03c2 \u03ba\u03b1\u1f76 \u03bc\u03c5\u03c1\u03c4\u03b9\u1f72\u03c2 \u03b4\u1f72\u03bd \u03b8\u1f70 \u03b2\u03c1\u1ff6 \u03c0\u03b9\u1f70 \u03c3\u03c4\u1f78 \u03c7\u03c1\u03c5\u03c3\u03b1\u03c6\u1f76 \u03be\u03ad\u03c6\u03c9\u03c4\u03bf\u0ca0_\u0ca0\u30a6\u30f0\u30ce\u30aa\u30af\u30e4\u30de \u30b1\u30d5\u30b3\u30a8\u30c6 \u30a2\u30b5\u30ad\u30e6\u30e1\u30df\u30b7 \u30f1\u30d2\u30e2\u30bb\u30b9\u30f3\u0421\u044a\u0435\u0448\u044c \u0436\u0435 \u0435\u0449\u0451 \u044d\u0442\u0438\u0445 \u043c\u044f\u0433\u043a\u0438\u0445 \u0444\u0440\u0430\u043d\u0446\u0443\u0437\u0441\u043a\u0438\u0445 \u0431\u0443\u043b\u043e\u043a \u0434\u0430 \u0432\u044b\u043f\u0435\u0439 \u0447\u0430\u044e'
        AtomA1.stringattr = unicode_teststring
        self.assertEquals(AtomA1.stringattr, unicode_teststring)
        
        
        # trying to access nonexistant attribute raises an exception
        try:
            self.assertFalse(container.nonexistantattribute)
        except RuntimeError as e:
            # the error message should include the type's name and attribute name
            self.assertTrue(str(e).find("Container") != -1)
            self.assertTrue(str(e).find("nonexistantattribute") != -1)
        else:
            self.fail("Expected an exception")

        uml = udm.map_uml_names(udm.uml_diagram())
        for o in udm.uml_diagram().children():
            if get_names(o.children(child_type=uml.CompositionChildRole)) == ['namespaces'] and get_names(o.children(child_type=uml.CompositionParentRole)) == ['parent']:
                break
        else:
            self.fail()
        namespaces = o.children(child_type=uml.CompositionChildRole)[0] # i.e. CompositionChildRole in Composition Diagram<>---Namespace
        self.assertEquals(namespaces.min, 0)
        self.assertEquals(namespaces.max, -1)
        self.assertTrue(namespaces.isNavigable)
        self.assertEquals(namespaces.target, uml.Namespace)
        self.assertEquals(namespaces.adjacent(dst_role="target"), uml.Namespace)
        self.assertEquals(namespaces.adjacent(dst_role="target", src_role="childRoles"), uml.Namespace)
        # if we don't specify dst_role, adjacent() returns a list
        self.assertEquals(namespaces.adjacent(), [uml.Namespace])
        self.assertEquals(namespaces.adjacent(src_role="childRoles"), [uml.Namespace])

        self.assertEquals(get_names(test_meta_dn.root.children()[0].adjacent(dst_role="subTypes")), []) # also accepts dst_role, association_class
        self.assertTrue(test_meta_dn.root.children()[0].name)
        self.assertTrue(test_meta_dn.root.children()[0].stereotype)
        self.assertEqual(test_meta.AtomA.name, "AtomA")
        self.assertEquals(test_meta.AtomA, test_meta.AtomA)
        self.assertNotEqual(test_meta.AtomA, test_meta.AtomB)
        self.assertNotEqual(test_meta_dn.root, udm.uml_diagram())
        self.assertNotEqual(test_meta_dn, udm.uml_diagram()) # disparate types
        self.assertNotEqual(udm.uml_diagram(), test_meta_dn) # disparate types
        self.assertTrue(test_meta.AtomA.__hash__() > 0)
        self.assertTrue(test_meta.AtomA)

        #KMS: BUG: childRole_role_children should return a singleton, not a list
        self.assertEquals(test_meta.AtomA.childRoles[0].parent.childRole_role_children[0].target, test_meta.AtomA)

        dn.close_no_update()
        test_meta_dn.close_no_update()

class TestUdmBackwardsCompat(unittest.TestCase):
    def testUdmDll(self):
        if _platform != "win32":
            return
        import subprocess
        import os.path
        pf86 = os.environ.get('ProgramFiles(x86)', os.environ.get('ProgramFiles'))
        
        with open(os.path.join(this_dir, 'UdmDll_VS10.lib.exports'), 'wb') as out_file:
            env = dict(os.environ)
            env['PATH'] += r';' + pf86 + r'\Microsoft Visual Studio 10.0\Common7\IDE'
            subprocess.check_call([pf86 + r'\Microsoft Visual Studio 10.0\VC\bin\dumpbin.exe', '/EXPORTS',
                os.path.join(this_dir, r'..\..\build\Win32\v100\Release\UdmDll_VS10.lib')], stdout=out_file, env=env)
        
        new_exports = {}
        with open(os.path.join(this_dir, 'UdmDll_VS10.lib.exports'), 'r') as out_file:
            for line in out_file:
                new_exports[line.rstrip('\r\n')] = 1
                
        with open(os.path.join(this_dir, 'UdmDll_VS10.lib.3.2.12.exports'), 'r') as existing_file:
            for lineno, existing_line in enumerate(existing_file):
                existing_line = existing_line.rstrip('\r\n')
                self.assertTrue(existing_line in new_exports, existing_line + " doesnt exist anymore. Line " + str(lineno))
                


if __name__ == '__main__':
    runner = unittest.TextTestRunner()
    runner.run(unittest.TestSuite([unittest.defaultTestLoader.loadTestsFromModule(sys.modules['__main__'])]))
