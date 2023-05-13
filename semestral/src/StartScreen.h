#ifndef STARTSCREEN_H
#define STARTSCREEN_H

#include "Interface.h"

class StartScreen : public Interface
{
	public:
		StartScreen () : Interface () {}
		StartScreen( const char * configFile ) : Interface ( configFile ) {}
};

#endif