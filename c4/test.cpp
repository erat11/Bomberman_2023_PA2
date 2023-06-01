#ifndef __PROGTEST__
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <array>
#include <cassert>
using namespace std;
#endif /* __PROGTEST__ */

class CTimeStamp
{
	int y, mo, d, h, mi, s;
	public:
  		CTimeStamp ( int y, int mo, int d, int h, int mi, int s ) : y(y), mo(mo), d(d), h(h), mi(mi), s(s) {}
  		int compareCTS ( const CTimeStamp t1 ) const
		{
			if 		 ( t1.y  > this->y ) return  1;
			else if  ( t1.y  < this->y ) return -1;
			else if  ( t1.mo > this->mo ) return  1;
			else if  ( t1.mo < this->mo ) return -1;
			else if  ( t1.d  > this->d ) return  1;
			else if  ( t1.d  < this->d ) return -1;
			else if  ( t1.h  > this->h ) return  1;
			else if  ( t1.h  < this->h ) return -1;
			else if  ( t1.mi > this->mi) return  1;
			else if  ( t1.mi < this->mi) return -1;
			else if  ( t1.s  > this->s ) return  1;
			else if  ( t1.s  < this->s ) return -1;
			return 0;
		}
};
class CContact
{
	const CTimeStamp t;
	const int n1, n2;
	public:
		CContact ( const CTimeStamp t, const int n1, const int n2 ) : t(t), n1(n1), n2(n2) {}
		int getN1() const {return n1;}
		int getN2() const {return n2;}
		CTimeStamp getTS() const { return t; }
};
class CEFaceMask
{
	vector<CContact> list;
	bool inVector ( const vector<int> v, const int q ) const
	{
		for ( int x : v )
			if ( x == q )
				return true;
		return false;
	}
  	public:
	  	CEFaceMask & addContact ( const CContact contact )
	  	{
	  		list.push_back ( contact );
	  		return *this;
	  	}
	  	vector<int> listContacts ( const int query ) const
	  	{
	  		vector<int> ret;
	  		for ( auto & x : list )
	  			if ( x.getN2() == x.getN1() ) continue;
	  			else if ( ( x.getN1() == query && !inVector ( ret, x.getN2() ) ) ) ret.push_back( x.getN2() );
	  			else if ( ( x.getN2() == query && !inVector ( ret, x.getN1() ) ) ) ret.push_back( x.getN1() );
	  		return ret;

	  	}
	  	vector<int> listContacts ( const int query, const CTimeStamp t1, const CTimeStamp t2 ) const
	  	{
	  		vector<int> ret;
	  		for ( auto & x : list )
	  		{
	  			if ( x.getN2() == x.getN1() ) continue;
	  			else if ( x.getTS().compareCTS ( t1 ) <= 0 && x.getTS().compareCTS ( t2 ) >= 0 && ( x.getN1() == query && !inVector ( ret, x.getN2() ) ) ) ret.push_back( x.getN2() );
	  			else if ( x.getTS().compareCTS ( t1 ) <= 0 && x.getTS().compareCTS ( t2 ) >= 0 && ( ( x.getN2() == query && !inVector ( ret, x.getN1() ) ) ) ) ret.push_back( x.getN1() );

	  		}
	  		return ret;
	  	}
};

#ifndef __PROGTEST__
int main ()
{
  CEFaceMask test;

  test . addContact ( CContact ( CTimeStamp ( 2021, 1, 10, 12, 40, 10 ), 123456789, 999888777 ) );
  test . addContact ( CContact ( CTimeStamp ( 2021, 1, 12, 12, 40, 10 ), 123456789, 111222333 ) )
       . addContact ( CContact ( CTimeStamp ( 2021, 2, 5, 15, 30, 28 ), 999888777, 555000222 ) );
  test . addContact ( CContact ( CTimeStamp ( 2021, 2, 21, 18, 0, 0 ), 123456789, 999888777 ) );
  test . addContact ( CContact ( CTimeStamp ( 2021, 1, 5, 18, 0, 0 ), 123456789, 456456456 ) );
  test . addContact ( CContact ( CTimeStamp ( 2021, 2, 1, 0, 0, 0 ), 123456789, 123456789 ) );
  assert ( test . listContacts ( 123456789 ) == (vector<int> {999888777, 111222333, 456456456}) );
  assert ( test . listContacts ( 999888777 ) == (vector<int> {123456789, 555000222}) );
  assert ( test . listContacts ( 191919191 ) == (vector<int> {}) );
  assert ( test . listContacts ( 123456789, CTimeStamp ( 2021, 1, 5, 18, 0, 0 ), CTimeStamp ( 2021, 2, 21, 18, 0, 0 ) ) == (vector<int> {999888777, 111222333, 456456456}) );
  assert ( test . listContacts ( 123456789, CTimeStamp ( 2021, 1, 5, 18, 0, 1 ), CTimeStamp ( 2021, 2, 21, 17, 59, 59 ) ) == (vector<int> {999888777, 111222333}) );
  assert ( test . listContacts ( 123456789, CTimeStamp ( 2021, 1, 10, 12, 41, 9 ), CTimeStamp ( 2021, 2, 21, 17, 59, 59 ) ) == (vector<int> {111222333}) );
  return 0;
}
#endif /* __PROGTEST__ */
