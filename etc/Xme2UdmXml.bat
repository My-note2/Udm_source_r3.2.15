:: Xme2UdmXml <xme_infile> <mga_save_file>
::	Converts a GME exported xml file (.xme) to UDM Xml file (<package_name>_udm.xml)
::
:: Arguments:
::	1. <xme_infile>: GME model to import
::	2. <mga_save_file>: GME model save file name
::
call "%UDM_PATH%\etc\CreateMga.vbs" %1 %2 UML
call "%UDM_PATH%\etc\CreateUdmXml.vbs" %2

