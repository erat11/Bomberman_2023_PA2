#include "GameObject.h"
#include <chrono>
#include <random>

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
		static int dropChance;
		void setDropChance ( int chance ) { dropChance = chance; }
		virtual Wall * clone () const override  { return new Wall ( *this ); }
};

class Explosion : public Wall
{
	public:
		Explosion ( char w ) : Wall ( '@' ) 
		{
			done = false;
			previous = w;
			start = chrono::high_resolution_clock::now();
			ttl = 1;
			type = 10;
		}
		void ready()
		{
			auto now = chrono::high_resolution_clock::now();
			int z = chrono::duration_cast<chrono::seconds>(now - start).count();
			if ( z > ttl ) 
			{ 
				if ( ( ( previous == '~' || previous == '#' ) && rand() % 100 < dropChance ) || ( previous == '$' && rand() % 100 < dropChance * 3 ) )
				{
					hp = 1; mapRep = '!'; 
				}
				else
				{
					hp = 0; mapRep = ' '; 
				}
				done = true;
			}
		}
		void update () override 
		{
			if ( !done )
			{
				auto now = chrono::high_resolution_clock::now();
				auto z = chrono::duration_cast<chrono::seconds>(now - start).count();
				if ( z > 0.5 ) mapRep = '.';
				ready();
			}
		}
		Explosion * clone () const override  { return new Explosion ( *this ); }
	private:
		bool done;
		chrono::system_clock::time_point start;
		double ttl;
		char previous;
};