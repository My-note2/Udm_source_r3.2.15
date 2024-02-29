
This folder contains some samples and instructions to get you started with using UdmOclPat.
UdmOclPat takes a datanetwork and a "patternfile" with plain text interspersed with some special instructions
to traverse the datanetwork and generate a text file. It is ideal for generating XML files or textual config files 
from models, that is easy to modify and update, without requiring recompilation.

Usage:

>UdmOclPat <datanetwork> <umldiagram> <patternfile>

UdmOclPat is run from the command prompt as shown above. 
<datanetwork> represents the ".mga" or ".xml" data network file to be traversed.
<umldiagram> represents the UDM generated UML-XML representing the meta of the datanetwork
		(please refer the UDM documentation to generate this file)
<patternfile> represents the pattern file which describes the desired output

The output is sent to stdout by default, and can be directed to one or more files from within the pattern file.

Files:
StateChartGMEMeta.xme: 		This contains the GME meta for the StateChart paradigm. 
				If you do not have this paradigm registered already, 
				please import this file in GME and register the paradigm.
StateChartUMLMeta.xme: 		This is the meta in the UML paradigm, and is necessary for UDM
				(This can also be generated from GME using the MetaGME2UML tool)
StateChart.xml: 		You can import the UML meta in GME and run the UML2XML interpreter to get this file.
				This is required as one of the parameters for running UdmOclPat.
stateClasses.xme, 
nestedStateClasses.xme: 	These are some sample models for running the examples.
				Alternately, you can create your own models to test the examples.
				Import these in GME and save as ".mga" to run the examples.

Example 1
=========
This example generates a text file with a predefined text. 
This just checks if the application is working correctly.
To run this, type the command:

>UdmOclPat stateClasses.mga StateChart.xml Example1.pat

(use the above format to test subsequent examples also).
This example shows how to mix plain text and special commands, how to direct output to files, 
and the usage of the "print" statement to print data to the output.
(you can open the *.pat examples in notepad to view the source)

Output is sent to stdout by default, or the current selected output stream.
All plain text is sent directly to the output.
Special instructions are separated from plain text, by enclosing them in the tags "<:" and ":>"
Any text within these tags is processed. This may or may not generate textual output, or may result in an error.
The language used for the special instructions is a modified form of OCL.
(Please refer to the UdmOclPat documentation for more detailed information).

Example 2
=========
This example simply prints the name of the root folder in the model.
This checks if you have the correct UDM and GME versions for running UdmOclPat.

Example 3
=========
This example iterates over the models in the root folder and prints their names.

Example 4
=========
This goes one step further, and iterates over the models contained within the models in the root folder.

Example 5
=========
Same as Example 4, but it generates the output in XML format instead.
This shows a typical usage scenario of generating an XML file from a model, which can be fed to other applications

Example 6
=========
This shows the usage of the "sortedBy" function.

Example 7
=========
This shows how to define your own methods for objects.
This demonstrates a recursive method that can be used to traverse data networks of arbitrary depth.
Try this example with "nestedStateClasses.mga".

State Chart HTML Demo Example
=============================
This is a detailed example that shows how to generate multiple files from models.
It demonstrates how we can generate a full fledged application using UdmOclPat.
>UdmOclPat stateClasses.mga StateChart.xml stateChartHTMLDemo.pat
This will generate some HTML files.
Open "statedemo.html" in a browser. 
This shows a javascript powered interactive simulation of the statechart model.