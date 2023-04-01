#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <ctime>
#include <climits>
#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */

// uncomment if your code implements initializer lists
// #define EXTENDED_SYNTAX

class CRangeList;

class CRange
{
	public:
		CRange ( long long l, long long h ) : l(l), h(h) { if ( l > h ) throw std::logic_error("Nespravny vstup\n"); }
		long long low () const { return l; }
		long long high () const { return h; }
		void setL ( long long l ) { this->l = l; }
		void setH ( long long h ) { this->h = h; }
		bool includes ( const CRange & r ) const { if ( l <= r.low() && h >= r.high() ) return true; return false; }
		bool operator == ( const CRange & r ) const { return ( r.h == h && r.l == l ); }
		bool operator != ( const CRange & r ) const { return !( r.h == h && r.l == l ); }
		friend CRangeList operator + ( const CRange & r1, const CRange & r2 );
		friend CRangeList operator - ( const CRange & r1, const CRange & r2 );
	private:
		long long l, h;
};

class CRangeList
{
	public:
		CRangeList () {}
		~CRangeList () {}
		CRangeList ( const CRangeList & list ) { l = list.l; }
		CRangeList ( const CRange & list ) { l.push_back(list); }
		CRangeList & operator = ( const CRangeList & list ) { l = list.l; return *this; }
		CRangeList & operator = ( const CRange & list ) { l.push_back( list ); return *this; }
		bool includes ( long long s ) const
		{
			for ( auto & x : l ) if ( s >= x.low() && s <= x.high() ) return true;
			return false;
		}
		bool includes ( const CRange & r ) const
		{
			for ( auto & x : l ) if ( r.low() >= x.low() && r.high() <= x.high() ) return true;
			return false;
		}
		int intersects ( const CRange & r1, const CRange & r2 ) const
		{
			if ( r1.includes( r2 ) ) return -1;
			if ( r2.includes ( r1 ) ) return -2;
			if ( r1.low() <= r2.low() + !( r2.low() == LLONG_MAX ) && r2.low() <= r1.high() + !( r1.high() == LLONG_MAX ) && r1.high() <= r2.high() + !( r2.high() == LLONG_MAX ) )  return 1;
			if ( r2.low() <= r1.low() + !( r1.low() == LLONG_MAX ) && r1.low() <= r2.high() + !( r2.high() == LLONG_MAX ) && r2.high() <= r1.high() + !( r1.high() == LLONG_MAX )  ) return 2;
			if ( r1.high() < r2.low() ) return -3;
			return 0;
		}
		CRangeList & operator + ( const CRange & r ) { return *this += r; }
		CRangeList & operator - ( const CRange & r ) { return *this -= r; }
		CRangeList & operator += ( CRange r )
		{
			int i = 0;
			for ( auto & x : l )
			{
				int q = intersects ( x, r );
				if ( q == -1 ) return *this; 
				if ( q == -2 )  {  l.erase( l.begin() + i );  continue; }
				if ( q == 1 ) 
				{ 
					x.setH( r.high() );
					r = x;
					l.erase( l.begin() + i );
					return *this += r;
				}
				if ( q == 2 ) 
				{ 
					x.setL( r.low() ); 
					r = x;
					l.erase( l.begin() + i );
					return *this += r; 
				}
				if ( q == -3 ) i++;
				else break;
			}
			l.insert( l.begin() + i, r );
			return *this;
		}
		CRangeList & operator += ( const CRangeList & r ) { for ( auto & x : r.getList() ) *this += x; return *this; }
		CRangeList & operator -= ( const CRangeList & r ) { for ( auto & x : r.getList() ) *this -= x; return *this; }
		CRangeList & operator -= ( const CRange & r )
		{
			size_t i = 0;
			for ( auto & x : l )
			{
				if ( r.high() == LLONG_MAX && x.high() == LLONG_MAX ) x.setH( LLONG_MAX - 1 );
				if ( r.low() == LLONG_MIN && x.low() == LLONG_MIN ) x.setL( LLONG_MIN + 1 );
				if ( i == l.size() ) break;
				if ( x == r ) { l.erase ( l.begin() + i ); return *this; }
				int q = intersects ( x, r );
				if ( q == -1 )
				{
					long long old = x.high();
					x.setH( r.low() - !( r.low() == LLONG_MIN ) );
					if ( r.high() + !( r.high() == LLONG_MAX ) <= old ) l.insert ( l.begin() + i + 1, CRange ( r.high() + !( r.high() == LLONG_MAX ), old ) );
					return *this;
				}
				if ( q == -2 )
				{
					l.erase ( l.begin() + i );
					return *this -= r;
				}
				if ( q == 1 ) 
				{
					x.setH( r.low() - !( r.low() == LLONG_MIN ) );
					if ( i + 1 != l.size() ) { i++; continue; }
					return *this;
				}
				if ( q == 2 ) 
				{ 
					x.setL( r.high() + !( r.high() == LLONG_MAX ) );
					if ( i + 1 != l.size() ) { i++; continue; }
					return *this;
				}
				if ( x.high() < r.low() ) i++;
				else break;
			}
			return *this;
		}
		vector<CRange> getList () const { return l; }
		bool operator == ( const CRangeList & list ) const { return ( l == list.getList() ); }
		bool operator != ( const CRangeList & list ) const { return ( l != list.getList() ); }
		friend ostream& operator<<(ostream & os, const CRangeList & t);
	private:
		vector<CRange> l;
};

ostream & operator<< (ostream& os, const CRangeList & t)
{
	os << '{';
	int i = 0;
    for ( auto & x : t.getList() )
    {
    	if ( i != 0 ) os << ',';
    	if ( x.low() == x.high() )
    		os << x.low();
    	else
    		os << '<' << x.low() << ".." << x.high() << '>';
    	i++;
    }
    os << '}';
    return os;
}

CRangeList operator + ( const CRange & r1, const CRange & r2 )
{
	CRangeList list( r1 );
	list += r2;
	return list;
}

CRangeList operator - ( const CRange & r1, const CRange & r2 )
{
	CRangeList list ( r1 );
	list -= r2;
	return list;
}


#ifndef __PROGTEST__
string toString ( const CRangeList& x )
{
  ostringstream oss;
  oss << x;
  return oss . str ();
}

int main ( void )
{
CRangeList a, b, c;

assert ( sizeof ( CRange ) <= 2 * sizeof ( long long ) );
a = CRange ( 5, 10 );
a += CRange ( 25, 100 );
assert ( toString ( a ) == "{<5..10>,<25..100>}" );
a += CRange ( -5, 0 );
a += CRange ( 8, 50 );
assert ( toString ( a ) == "{<-5..0>,<5..100>}" );
a += CRange ( 101, 105 ) + CRange ( 120, 150 ) + CRange ( 160, 180 ) + CRange ( 190, 210 );
assert ( toString ( a ) == "{<-5..0>,<5..105>,<120..150>,<160..180>,<190..210>}" );
a += CRange ( 106, 119 ) + CRange ( 152, 158 );
assert ( toString ( a ) == "{<-5..0>,<5..150>,<152..158>,<160..180>,<190..210>}" );
a += CRange ( -3, 170 );
a += CRange ( -30, 1000 );
assert ( toString ( a ) == "{<-30..1000>}" );
b = CRange ( -500, -300 ) + CRange ( 2000, 3000 ) + CRange ( 700, 1001 );
a += b;
assert ( toString ( a ) == "{<-500..-300>,<-30..1001>,<2000..3000>}" );
a -= CRange ( -400, -400 );
assert ( toString ( a ) == "{<-500..-401>,<-399..-300>,<-30..1001>,<2000..3000>}" );
a -= CRange ( 10, 20 ) + CRange ( 900, 2500 ) + CRange ( 30, 40 ) + CRange ( 10000, 20000 );
assert ( toString ( a ) == "{<-500..-401>,<-399..-300>,<-30..9>,<21..29>,<41..899>,<2501..3000>}" );
try
{
	a += CRange ( 15, 18 ) + CRange ( 10, 0 ) + CRange ( 35, 38 );
	assert ( "Exception not thrown" == nullptr );
}
catch ( const std::logic_error & e )
{

}
catch ( ... )
{
	assert ( "Invalid exception thrown" == nullptr );
}
assert ( toString ( a ) == "{<-500..-401>,<-399..-300>,<-30..9>,<21..29>,<41..899>,<2501..3000>}" );
b = a;
assert ( a == b );
assert ( !( a != b ) );
b += CRange ( 2600, 2700 );
assert ( toString ( b ) == "{<-500..-401>,<-399..-300>,<-30..9>,<21..29>,<41..899>,<2501..3000>}" );
assert ( a == b );
assert ( !( a != b ) );
b += CRange ( 15, 15 );
assert ( toString ( b ) == "{<-500..-401>,<-399..-300>,<-30..9>,15,<21..29>,<41..899>,<2501..3000>}" );
assert ( !( a == b ) );
assert ( a != b );
assert ( b . includes ( 15 ) );
assert ( b . includes ( 2900 ) );
assert ( b . includes ( CRange ( 15, 15 ) ) );
assert ( b . includes ( CRange ( -350, -350 ) ) );
assert ( b . includes ( CRange ( 100, 200 ) ) );
assert ( !b . includes ( CRange ( 800, 900 ) ) );
assert ( !b . includes ( CRange ( -1000, -450 ) ) );
assert ( !b . includes ( CRange ( 0, 500 ) ) );
a += CRange ( -10000, 10000 ) + CRange ( 10000000, 1000000000 );
assert ( toString ( a ) == "{<-10000..10000>,<10000000..1000000000>}" );
b += a;
assert ( toString ( b ) == "{<-10000..10000>,<10000000..1000000000>}" );
b -= a;
assert ( toString ( b ) == "{}" );
b += CRange ( 0, 100 ) + CRange ( 200, 300 ) - CRange ( 150, 250 ) + CRange ( 160, 180 ) - CRange ( 170, 170 );
assert ( toString ( b ) == "{<0..100>,<160..169>,<171..180>,<251..300>}" );
b -= CRange ( 10, 90 ) - CRange ( 20, 30 ) - CRange ( 40, 50 ) - CRange ( 60, 90 ) + CRange ( 70, 80 );
assert ( toString ( b ) == "{<0..9>,<20..30>,<40..50>,<60..69>,<81..100>,<160..169>,<171..180>,<251..300>}" );
c = CRange ( LLONG_MIN, LLONG_MAX );
c -= CRange ( LLONG_MIN + 1, LLONG_MAX );
assert ( toString ( c ) == "{-9223372036854775808}" );
/*#ifdef EXTENDED_SYNTAX
CRangeList x { { 5, 20 }, { 150, 200 }, { -9, 12 }, { 48, 93 } };
assert ( toString ( x ) == "{<-9..20>,<48..93>,<150..200>}" );
ostringstream oss;
oss << setfill ( '=' ) << hex << left;
for ( const auto & v : x + CRange ( -100, -100 ) )
oss << v << endl;
oss << setw ( 10 ) << 1024;
assert ( oss . str () == "-100\n<-9..20>\n<48..93>\n<150..200>\n400=======" );
#endif  EXTENDED_SYNTAX */
return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
