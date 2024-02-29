Statechart demo using UdmPat
Output is in file statedemo.html

<:
self.orState->forAll( oo | { 
	oo.compoundState->forAll( o | { 
	open(o.name + ".html", "o", stateFile);
	switch(stateFile);
:>	
<!--
*******************************************************
Interactive HTML system generated from state chart model 
using UdmOclPat

Author :	Ananth
Date   :	7 Nov, 2003
*******************************************************
-->

<html>
<title>State Machine Implementation - <: print(o.name); :></title>
<script language="javascript">
var curState = "";

function disableAll()
{
<:		o.oclAsType(OrState).transition->collect( t | { t.Trigger; } ).asSet()->forAll(t | {
			:>\tfrmTriggers.<: print(t); :>.disabled = true;\n<: 
		});
:>\
}



function setCurState(state)
{
	disableAll();
	curState = state;
	d1.innerText = state;
<:
	o.oclAsType(OrState).simpleState->forAll( s | {
	:>\tif( curState == "<: print(s.name); :>" ) {\n<:
		s.transition[srcTransition]->forAll(tr | {
			:>\t\tfrmTriggers.<: print(tr.Trigger); :>.disabled = false;\n<:
		});
		:>\t}\n<:
	});
:>\
}


function activateTrigger(trigger)
{
<:
	o.oclAsType(OrState).simpleState->forAll( s | {
	:>\tif( curState == "<: print(s.name); :>" ) {\n<:
		s.transition[srcTransition]->forAll(tr | {
			:>\t\tif(trigger == "<: print(tr.Trigger); :>") setCurState("<: print(tr.dstTransition.name); :>");\n<:
		});
		:>\t}\n<:
	});
:>\	
}
</script>

<body onload="javascript: setCurState('S1');">
<h2><: print(o.name); :></h2>
<b>Current State:
<h1><div id="d1"></div></h1>
<br>
<form name="frmTriggers">
Available Triggers:<br>
<:
	o.oclAsType(OrState).transition->collect( t | { t.Trigger; } ).asSet()->forAll(t | {
		:><input type="button" name="<: print(t); :>" value="<: print(t); :>" disabled=true onclick="activateTrigger('<: print(t); :>')">\n<:
	});
:>\
</form>
</b><br>
<br>
<a href="statedemo.html">back</a>
<body>
</html>

<: 
 });
}); 
:>

<: open("statedemo.html", "o", main); :>
<: switch(main); :>
<!--
*******************************************************
Interactive HTML system generated from state chart model 
using UdmOclPat

Author :	Ananth
Date   :	7 Nov, 2003
*******************************************************
-->

<html>
<title>State Machine Implementation</title>
<body>
<h3>Select OR state: </h3>

<:
self.orState->forAll( oo | { 
	oo.compoundState->forAll( o | { 
		:><a href="<:print(o.name);:>.html"><:print(o.name);:></a><br>\n<:
	});
});
:>
</body>
</html>
