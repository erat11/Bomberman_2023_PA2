#include "GameObject.h"
#include <chrono>

class Wall : public GameObject
{
	public:
		Wall () : GameObject( 0, ' ' ) { hp = 0; }
		Wall ( char r )
		{
			mapRep = r;
			if ( r == '|' || r == '-' ) hp = 666; 
			else if ( r == '#' )        hp =  6;  
			else if ( r == '~' || '$' ) hp =  2;   
			else                        hp =  1;    
		}
		~Wall () override {}
		virtual void update () override 
		{
			if ( hp < 0 )
			{
				hp = 0;
				mapRep = ' ';
			}
		}	
};

class Explosion : public Wall
{
	public:
		Explosion () : Wall ( '@' ) 
		{
			start = chrono::high_resolution_clock::now();
			ttl = 1;
			type = 10;
		}
		void ready()
		{
			auto now = chrono::high_resolution_clock::now();
			int z = chrono::duration_cast<chrono::seconds>(now - start).count();
			if ( z > ttl ) { hp = 0; mapRep = ' '; }
		}
		void update () override 
		{
			auto now = chrono::high_resolution_clock::now();
			auto z = chrono::duration_cast<chrono::seconds>(now - start).count();
			if ( z > 0.5 ) mapRep = '.';
			ready();
		}
	private:
		chrono::system_clock::time_point start;
		double ttl;
};