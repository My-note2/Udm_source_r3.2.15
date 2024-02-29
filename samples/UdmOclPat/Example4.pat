<: 
open("Example4.txt", "o", ex4);
switch(ex4);
:>
Example 4
=========

The root folder contains:
<:
  self.orState->forAll(o | { 
    print(o.name); :>\n<:
    print(o.name + " contains: ");
    o.compoundState->forAll(c | { 
      :>\n\t<: print(c.name); 
      c.oclAsType(OrState).simpleState->forAll(s | { 
      	:>\n\t\t<: print(s.name); 
      });
    } );
  } );
:>
