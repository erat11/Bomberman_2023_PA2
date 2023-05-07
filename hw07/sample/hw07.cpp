#ifndef __PROGTEST__
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <set>
#include <list>
#include <map>
#include <vector>
#include <queue>
#include <string>
#include <stack>
#include <queue>
#include <deque>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <functional>
#include <iterator>
#include <stdexcept>
using namespace std;
#endif /* __PROGTEST__ */

template <typename M_>
class CContest
{
  public:
    class Result
    {
      public:
        Result ( const string & a, const string & b, const M_ & r ) : a ( a ), b ( b ), r ( r ) {}
        bool operator < ( const Result & x ) const
        {
          if ( a < x.a ) return true;
          if ( a == x.a && b < x.b ) return true;
          return false;
        }
        bool operator == ( const Result & x ) const
        {
          return ( a == x.a && b == x.b ) || ( a == x.b && b == x.a );
        }
        string a, b;
        M_ r;
    };
    CContest () {}
    ~CContest () {}
    bool contains ( const Result & x )
    {
      for ( const auto & y : storage )
        if ( x == y ) return true;
      return false;
    }
    CContest & addMatch ( const string & contestant1, const string & contestant2, const M_ & result )
    {
      Result d ( contestant1, contestant2, result );
      if ( contains( d ) ) throw logic_error("result match");
      teams.insert( contestant1 );
      teams.insert( contestant2 );
      storage.push_back( d );
      return *this;
    }
    template <typename Comparator>
    void bfs( const string & s, Comparator comparator, map<string,set<string>> & tmpMap, unordered_map<string, bool> & visited ) const
    {
      visited[s] = true;
      for ( auto x : tmpMap[s] ) 
      {
        if ( !visited[x] ) bfs ( x, comparator, tmpMap, visited );
        tmpMap[s].insert( tmpMap[x].begin(), tmpMap[x].end() );
      }
    }
    template <typename Comparator>
    bool isOrderedWrapper ( Comparator comparator, map<string,set<string>> & tmpMap, unordered_map<string, bool> & visited, unsigned int & max ) const
    {
      if ( !storage.size() ) return true;
      for ( const auto &x : storage ) 
      {
        if ( comparator(x.r) > 0 ) tmpMap[x.a].insert(x.b);
        else if ( comparator( x.r ) < 0 ) tmpMap[x.b].insert( x.a );
      }
      for ( const auto & x : teams ) if ( !visited[x] ) bfs( x, comparator, tmpMap, visited );
      max = 0;
      for ( const auto & x : tmpMap )
      {
        if ( x.second.find(x.first) != x.second.end() ) return false;
        for ( const auto & y : tmpMap )
          if ( x.second == y.second && x.first != y.first ) return false;
        if ( max < x.second.size() ) max = x.second.size();
      }
      /*for ( auto & x : tmpMap )
        for ( auto & y : x.second )
          cout << x.first << " -> " << y << endl;
      cout << endl;*/
      return true;
    }

    template <typename Comparator>
    bool isOrdered ( Comparator comparator ) const
    {
      unsigned int max = 0;
      map<string,set<string>> tmpMap;
      unordered_map<string, bool> visited;
      return isOrderedWrapper( comparator, tmpMap, visited, max );
    }
    template <typename Comparator>
    list<string> topSort( Comparator comparator ) const
    {
      unsigned int max = 0;
      map<string,set<string>> tmpMap;
      unordered_map<string, bool> visited;
      if ( !isOrderedWrapper( comparator, tmpMap, visited, max ) ) throw logic_error("results are not ordered!");


      queue<string> q;
      unordered_map<string, int> inDegrees;
      for (const auto & x : tmpMap) 
      {
        inDegrees[x.first] = x.second.size();
        if ( max == x.second.size() ) q.push( x.first );
      }
      list<string> sorted;
      while ( !q.empty() )
      {
        string front = q.front();
        q.pop();
        sorted.push_back( front );

        vector<pair<string, int>> v;
        for ( const auto & y : tmpMap[front] ) v.push_back( make_pair( y, inDegrees[y] ) );
        stable_sort(v.begin(), v.end(), [](const auto& a, const auto& b) { return a.second > b.second; });
        for ( const auto & y : v )  { if ( inDegrees[y.first] != -1 ) { q.push( y.first ); inDegrees[y.first] = -1; } }
      }
      return sorted;
    }
    template <typename Comparator>
    list<string> results ( Comparator comparator ) const
    {
      return topSort( comparator );
    }
  private:
    vector<Result> storage;
    set<string> teams;
    unsigned int max;
};

#ifndef __PROGTEST__
struct CMatch
{
  public:
    CMatch ( int a, int b ) : m_A ( a ), m_B ( b ) { }
    int m_A;
    int m_B;
};

class HigherScoreThreshold
{
  public:
    HigherScoreThreshold ( int diffAtLeast ) : m_DiffAtLeast ( diffAtLeast ) {}
    int operator () ( const CMatch & x ) const
    {
      return ( x . m_A > x . m_B + m_DiffAtLeast ) - ( x . m_B > x . m_A + m_DiffAtLeast );
    }
  private:
    int m_DiffAtLeast;    
};

int HigherScore ( const CMatch & x )
{
  return ( x . m_A > x . m_B ) - ( x . m_B > x . m_A );
}

int main ( void )
{
  CContest<CMatch>  x;
  
  x . addMatch ( "C++", "Pascal", CMatch ( 10, 3 ) )
    . addMatch ( "C++", "Java", CMatch ( 8, 1 ) )
    . addMatch ( "Pascal", "Basic", CMatch ( 40, 0 ) )
    . addMatch ( "Java", "PHP", CMatch ( 6, 2 ) )
    . addMatch ( "Java", "Pascal", CMatch ( 7, 3 ) )
    . addMatch ( "PHP", "Basic", CMatch ( 10, 0 ) );
    
  
  assert ( ! x . isOrdered ( HigherScore ) );
  try
  {
    list<string> res = x . results ( HigherScore );
    assert ( "Exception missing!" == nullptr );
  }
  catch ( const logic_error & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown!" == nullptr );
  }

  x . addMatch ( "PHP", "Pascal", CMatch ( 3, 6 ) ); 

  assert ( x . isOrdered ( HigherScore ) );
  try
  {
    list<string> res = x . results ( HigherScore );
    assert ( ( res == list<string>{ "C++", "Java", "Pascal", "PHP", "Basic" } ) );
  }
  catch ( ... )
  {
    assert ( "Unexpected exception!" == nullptr );
  }

  
  assert ( ! x . isOrdered ( HigherScoreThreshold ( 3 ) ) );
  try
  {
    list<string> res = x . results ( HigherScoreThreshold ( 3 ) );
    assert ( "Exception missing!" == nullptr );
  }
  catch ( const logic_error & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown!" == nullptr );
  }
  
  assert ( x . isOrdered ( [] ( const CMatch & x )
  {
    return ( x . m_A < x . m_B ) - ( x . m_B < x . m_A ); 
  } ) );
  try
  {
    list<string> res = x . results ( [] ( const CMatch & x )
    {
      return ( x . m_A < x . m_B ) - ( x . m_B < x . m_A ); 
    } );
    assert ( ( res == list<string>{ "Basic", "PHP", "Pascal", "Java", "C++" } ) );
  }
  catch ( ... )
  {
    assert ( "Unexpected exception!" == nullptr );
  }
  
  CContest<bool>  y;
  
  y . addMatch ( "Python", "PHP", true )
    . addMatch ( "PHP", "Perl", true )
    . addMatch ( "Perl", "Bash", true )
    . addMatch ( "Bash", "JavaScript", true )
    . addMatch ( "JavaScript", "VBScript", true );
  
  assert ( y . isOrdered ( [] ( bool v )
  {
    return v ? 10 : - 10;
  } ) );
  try
  {
    list<string> res = y . results ( [] ( bool v )
    {
      return v ? 10 : - 10;
    });
    assert ( ( res == list<string>{ "Python", "PHP", "Perl", "Bash", "JavaScript", "VBScript" } ) );
  }
  catch ( ... )
  {
    assert ( "Unexpected exception!" == nullptr );
  }
    
  y . addMatch ( "PHP", "JavaScript", false );
  assert ( !y . isOrdered ( [] ( bool v )
  {
    return v ? 10 : - 10;
  } ) );
  try
  {
    list<string> res = y . results ( [] ( bool v )
    {
      return v ? 10 : - 10;
    } );
    assert ( "Exception missing!" == nullptr );
  }
  catch ( const logic_error & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown!" == nullptr );
  }
  
  try
  {
    y . addMatch ( "PHP", "JavaScript", false );
    assert ( "Exception missing!" == nullptr );
  }
  catch ( const logic_error & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown!" == nullptr );
  }
  
  try
  {
    y . addMatch ( "JavaScript", "PHP", true );
    assert ( "Exception missing!" == nullptr );
  }
  catch ( const logic_error & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown!" == nullptr );
  }
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
