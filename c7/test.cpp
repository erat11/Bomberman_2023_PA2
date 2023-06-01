#ifndef __PROGTEST__
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <numeric>
#include <vector>
#include <array>
#include <set>
#include <map>
#include <deque>
#include <queue>
#include <stack>
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
        if       ( t1.y  > this->y ) return  1;
        else if  ( t1.y  < this->y ) return -1;
        else if  ( t1.mo > this->mo )return  1;
        else if  ( t1.mo < this->mo )return -1;
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
  public:
    const CTimeStamp t;
    const int n1, n2;
    CContact ( const CTimeStamp t, const int n1, const int n2 ) : t(t), n1(n1), n2(n2) {}
};

class CEFaceMask
{
  public:
    CEFaceMask () {}
    CEFaceMask & addContact ( const CContact & contact )
    {
      vec.push_back ( contact );
      return *this;
    }
    vector<int> getSuperSpreaders ( const CTimeStamp & from, const CTimeStamp  & to )
    {
      map<int, int> myMap;
      for ( auto x : vec ) if ( x.n1 != x.n2 && x.t.compareCTS( from ) <= 0 && x.t.compareCTS( to ) >= 0 && x.n1 != x.n2  ) { myMap[x.n1]++; myMap[x.n2]++; }
      int max = 1;
      set<int> a;
      for ( auto & pair : myMap ) if ( pair.second > max ) { max = pair.second; a.clear(); a.insert(pair.first); }
                             else if ( pair.second == max ) { a.insert(pair.first); }
      vector<int> b(a.size());
      copy(a.begin(), a.end(), b.begin());
      return b;
    }
  private:
  vector<CContact> vec;
  bool inVector ( const vector<int> v, const int q ) const
  {
    for ( int x : v )
      if ( x == q )
        return true;
    return false;
  }
};

#ifndef __PROGTEST__
int main ()
{
  CEFaceMask test;

  test . addContact ( CContact ( CTimeStamp ( 2021, 1, 10, 12, 40, 10 ), 111111111, 222222222 ) );
  test . addContact ( CContact ( CTimeStamp ( 2021, 1, 12, 12, 40, 10 ), 333333333, 222222222 ) )
       . addContact ( CContact ( CTimeStamp ( 2021, 2, 14, 15, 30, 28 ), 222222222, 444444444 ) );
  test . addContact ( CContact ( CTimeStamp ( 2021, 2, 15, 18, 0, 0 ), 555555555, 444444444 ) );
  assert ( test . getSuperSpreaders ( CTimeStamp ( 2021, 1, 1, 0, 0, 0 ), CTimeStamp ( 2022, 1, 1, 0, 0, 0 ) ) == (vector<int> {222222222}) );
  test . addContact ( CContact ( CTimeStamp ( 2021, 3, 20, 18, 0, 0 ), 444444444, 666666666 ) );
  test . addContact ( CContact ( CTimeStamp ( 2021, 3, 25, 0, 0, 0 ), 111111111, 666666666 ) );
  assert ( test . getSuperSpreaders ( CTimeStamp ( 2021, 1, 1, 0, 0, 0 ), CTimeStamp ( 2022, 1, 1, 0, 0, 0 ) ) == (vector<int> {222222222, 444444444}) );
  return 0;
}
#endif /* __PROGTEST__ */
