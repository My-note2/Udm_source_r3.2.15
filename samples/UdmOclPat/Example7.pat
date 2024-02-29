<:$OrState::printContents( indent : ocl::String ) : bool defmethod : 
{
    print(indent);
    :><OrState name="<: print(self.name); :>">\n<:
    self.compoundState.sortedBy(k | { k.name; })->forAll(o | { o.oclAsType(OrState).printContents( indent + "    " ); } );
    self.simpleState.sortedBy(k | { k.name; })->forAll(s | { 
	print(indent + "    " );
	:><SimpleState><:print(s.name);:></SimpleState>\n<:
    }); 
    print(indent);
    :></OrState>\n<:
}
$:>
<: 
open("Example7.xml", "o", ex7);
switch(ex7);
:>
<?xml version="1.0"?>

<StateChart name="<: print(self->name); :>">\n<:
  self.orState.sortedBy(k | { k.name; })->forAll(o | { o.printContents("    "); } );
:></StateChart>