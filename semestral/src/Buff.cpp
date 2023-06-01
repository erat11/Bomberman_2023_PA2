#include "Buff.h"


Buff::Buff () { mapRep = '?'; type = 8; name = "Buff"; }
void Buff::activate ( PlayerAttributes & plAttr ) 
{

}
Buff::~Buff() {}
string Buff::getName() const { return name; }
Buff * Buff::clone () const { return new Buff ( *this ); }
