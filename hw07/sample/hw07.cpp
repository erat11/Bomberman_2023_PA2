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

/*list<CInvoice> unmatched ( const string & company, const CSortOpt & sortBy ) const
    {
      auto myComparisonFunction = [this, sortBy](const CInvoice &a, const CInvoice &b ) 
      {
        for ( const auto & x : sortBy.getKey()) 
        {
          int q = compareBy( a, b, x );
          if ( q < 0 ) return true;
          if ( q > 0 ) return false;
        }
        if ( a.order() < b.order() ) return true;
        return false;
      };
      set<CInvoice, decltype(myComparisonFunction)> ret(myComparisonFunction);
      string standComp = standardize ( company );
      for ( const auto & x : invoices ) if ( ( x.first.buyerF() == standComp || x.first.sellerF() == standComp ) && x.second != 2 )
        ret.emplace ( x.first );
      return list<CInvoice> ( ret.begin(), ret.end() );
    }*/

/*tento typ popisuje výsledek zápasu. Pro datový typ M_ je garantováno, že je přesouvatelný,
 kopírovatelný a zrušitelný (CopyConstructibe, MoveConstructible, CopyAssignable, MoveAssignable
  a Destructible). Další vlastnosti typu nejsou garantované, speciálně, není garantované, že typ
   M_ má implicitní konstruktor (nemusí být DefaultConstructible). */

template <typename M_>
class CContest
{
  public:
    class Result
    {
      public:
        Result ( const string & a, const string & b, const M_ & r ) : a ( a ), b ( b ), r ( r ) {}
        bool operator == ( const Result & x ) const
        {
          return ( a == x.a && b == x.b ) || ( a == x.b && b == x.a );
        }
        string a, b;
        M_ r;
    };
    CContest () {}
    ~CContest () {}
    CContest & addMatch ( const string & contestant1, const string & contestant2, const M_ & result )
    {
      Result a ( contestant1, contestant2, result );
      for ( const auto & x : storage ) if ( x == a ) throw logic_error("result match");
      storage.push_back ( a );
      return *this;
    }
    bool isOrdered ( bool (*comparator)(M_, M_) )
    {
      set<M_> visited;
      queue<M_> q;
      q.push(results[0].r);
      visited.insert(results[0].r);
      while ( !q.empty() ) 
      {
        M_ current = q.front();
        q.pop();
        // Find all Result objects that have the current M_ object
        vector<Result> currentResults;
        for (const Result & result : results)  
            if (result.r == current) currentResults.push_back(result);
        // Check if the M_ objects in the currentResult vector are ordered based on the provided comparator
        for (size_t i = 0; i < currentResults.size() - 1; i++) 
            if (!comparator(currentResults[i].r, currentResults[i+1].r)) return false;
        // Add all unvisited M_ objects to the queue and mark them as visited
        for (const Result& result : currentResults) 
          if (visited.find(result.r) == visited.end()) 
          {
              q.push(result.r);
              visited.insert(result.r);
          }
        return true; // the vector is sortable
      }
    }
    // results ( comparator )
  private:
    vector<Result> storage;
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
    
  
  /*assert ( ! x . isOrdered ( HigherScore ) );
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
  }*/
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
