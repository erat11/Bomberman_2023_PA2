class gameObject
{
	public: 
		GameObject () {}
		GameObject ( const GameObject & x ) { *this = x; }
		GameObject ( int x, int y, char r, bool v ) : 
		coordinates ( make_pair ( x, y ),
		mapRepresentation ( r ),
		visibility ( v ) ) 
		{}
		virtual ~GameObject () {}
		virtual printer     () {}
		virtual destroy     () {}
	protected:
		pair<int, int> coordinates;
		char mapRepresentation;
		bool visibility;
};