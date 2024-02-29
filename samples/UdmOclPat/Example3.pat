
<: 
open("Example3.txt", "o", ex3);
switch(ex3);
:>

Example 3
=========

The root folder contains:
<:
  self.orState->forAll(o | { print(o.name); } );
:>
