#include "Interface.h"

class StartScreen : public Interface
{
	public:
		StartScreen( const char * configFile ) : Interface ( configFile ) {}
};