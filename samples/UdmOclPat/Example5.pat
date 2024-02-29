<: 
open("Example5.xml", "o", ex5);
switch(ex5);
:>
<?xml version="1.0"?>

<StateChart name="<: print(self->name); :>">
<:
  self.orState->forAll(o | { :>
    <OrState name="<: print(o.name); :>"> <:
    o.compoundState->forAll(c | { :>
      <OrState name="<: print(c.name); :>"> <:
      c.oclAsType(OrState).simpleState->forAll(s | { :>
      	<SimpleState name="<: print(s.name); :>"></SimpleState><:
      }); :>
      </OrState> <:
    } );:>
    </OrState><:
  } );
:>
</StateChart>