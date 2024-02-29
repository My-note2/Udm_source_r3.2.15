<: 
open("Example6.txt", "o", ex6);
switch(ex6);
:>

Example 6
=========

The root folder contains:
<:
  self.orState->forAll(o | { 
    print(o.name); :>\n<:
    print(o.name + " contains: ");
    o.compoundState->forAll(c | { 
      :>\n\t<: print(c.name); 
      c.oclAsType(OrState).simpleState->sortedBy(n | {n.name;})->forAll(s | { 
      	:>\n\t\t<: print(s.name); 
      });
    } );
  } );
:>

