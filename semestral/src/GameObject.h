#include <utility>

using namespace std;

class GameObject
{
	public: 
		GameObject () {}
		GameObject ( int x, int y, char r, bool v, bool p ) : 
		coordinates ( make_pair ( x, y ) ),
		mapRepresentation ( r ),
		visibility ( v ),
		rigidity ( p ) 
		{}
		pair<int, int> getCoordinates () const { return coordinates; 	   }
		char getMapRepresentation 	  () const { return mapRepresentation; }
		bool getVisibility 			  () const { return visibility; 	   }
		bool getRigidity 			  () const { return visibility; 	   }

		virtual void mapDestroy () { visibility = false; }
		virtual ~GameObject 	 () {}
		virtual void printer     () {}
		virtual void destroy     () {}
	protected:
		pair<int, int> coordinates;
		char mapRepresentation;
		bool visibility, rigidity;
};