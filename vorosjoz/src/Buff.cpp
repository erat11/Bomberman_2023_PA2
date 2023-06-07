#include "Buff.h"


Buff::Buff () { mapRep = '?'; type = 8; name = "Buff"; duration = 0; }
void Buff::activate ( PlayerAttributes & plAttr, vector<vector<GameObject*>> & gameMap ) {}
Buff::~Buff() {}
string Buff::getName() const { return name; }
Buff * Buff::clone () const { return new Buff ( *this ); }
bool Buff::done() const { return false; }
bool Buff::isActive() const { return false; }
double Buff::timer() const { return 0; }
double Buff::getDuration() const { return duration; }
