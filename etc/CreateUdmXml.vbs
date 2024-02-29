' CreateUdmXml.vbs <mga_file>
'	It creates either a <package_name>_udm.xml or a <Root Folder name>_udm.udm  
'	file in the working directory, where the script was invoked. All erors are dismissed silently. 
'	If any error happens, an output file is not created.
'
' Arguments:
'	1. <mga_file>: GME model in the UML paradigm 
'
mga_file= WScript.arguments.item( 0)
Set oMGAProj= CreateObject( "Mga.MgaProject")
oMGAProj.Open "MGA=" & mga_file
'MsgBox mga_file
Set launcher= CreateObject( "Mga.MgaLauncher")
launcher.RunComponent "MGA.Interpreter.UML2XML", oMGAProj, Nothing, Nothing, CLng( 128)
oMGAProj.Close

