#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <deque>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <iterator>
#include <functional>
using namespace std;

class CDate
{
  public:
    //---------------------------------------------------------------------------------------------
                             CDate                         ( int               y,
                                                             int               m,
                                                             int               d )
      : m_Year ( y ),
        m_Month ( m ),
        m_Day ( d )
    {
    }
    //---------------------------------------------------------------------------------------------
    int                      compare                       ( const CDate     & x ) const
    {
      if ( m_Year != x . m_Year )
        return m_Year - x . m_Year;
      if ( m_Month != x . m_Month )
        return m_Month - x . m_Month;
      return m_Day - x . m_Day;
    }
    //---------------------------------------------------------------------------------------------
    int                      year                          ( void ) const 
    {
      return m_Year;
    }
    //---------------------------------------------------------------------------------------------
    int                      month                         ( void ) const 
    {
      return m_Month;
    }
    //---------------------------------------------------------------------------------------------
    int                      day                           ( void ) const 
    {
      return m_Day;
    }
    //---------------------------------------------------------------------------------------------
    friend ostream         & operator <<                   ( ostream         & os,
                                                             const CDate     & x )
    {
      char oldFill = os . fill ();
      return os << setfill ( '0' ) << setw ( 4 ) << x . m_Year << "-" 
                                   << setw ( 2 ) << static_cast<int> ( x . m_Month ) << "-" 
                                   << setw ( 2 ) << static_cast<int> ( x . m_Day ) 
                << setfill ( oldFill );
    }
    //---------------------------------------------------------------------------------------------
  private:
    int16_t                  m_Year;
    int8_t                   m_Month;
    int8_t                   m_Day;
};
#endif /* __PROGTEST__ */

string standardize ( const string & name ) 
{
  /* Removes duplicate whitespace, converts to lower, removes
   * beginning and ending spaces
   */
    string ret;
    unsigned int i = 0;
    while ( isspace ( name[i] ) ) i++;
    for ( ; i < name.length(); ++i )
    {
        if ( isspace ( name[i] ) )
        {
          char x = name[i];
          while ( isspace ( name[i] ) ) 
          {
            i++;
            if ( i == name.length ( ) ) return ret;
          }
          ret += tolower ( x );
        }
        ret += tolower ( name[i] );
    }
    return ret;
}

class CDateWrapper
{
  /* This class copies the immutable CDate class's data, 
   * and reimplements it to be comparable with operators
   */
  public:
    CDateWrapper ( ) { }
    CDateWrapper ( int y, int m, int d ) : m_Year ( y ), m_Month ( m ), m_Day ( d ) {}
    bool operator == ( const CDateWrapper & s ) const { return this->get().compare( s.get() ) == 0; }
    bool operator != ( const CDateWrapper & s ) const { return this->get().compare( s.get() ) != 0; }
    bool operator >  ( const CDateWrapper & s ) const { return this->get().compare( s.get() ) >  0; }
    bool operator <  ( const CDateWrapper & s ) const { return this->get().compare( s.get() ) <  0; }
    CDate get ( ) const { return CDate ( m_Year, m_Month, m_Day ); }
  private:
    int16_t m_Year;
    int8_t m_Month;
    int8_t m_Day;
};

class CInvoice
{
  public:
    CInvoice () {}
    CInvoice ( const CDate & date, const string & seller, const string & buyer, unsigned int amount, double vat ) :
    m_wdate ( date.year ( ), date.month ( ), date.day ( ) ), m_seller ( seller ),
    m_buyer ( buyer ), m_amount ( amount ), m_vat ( vat ) 
    { 
      m_seller_lower = toLowerCaseWithSpaces ( seller );
      m_seller_standard = standardize ( seller );
      m_buyer_lower = toLowerCaseWithSpaces ( buyer );
      m_buyer_standard = standardize ( buyer );
      m_order = -1;
    }
    bool operator == ( const CInvoice & s ) const { return this->compare( s ) == 0; }
    bool operator != ( const CInvoice & s ) const { return this->compare( s ) != 0; }
    bool operator >  ( const CInvoice & s ) const { return this->compare( s ) >  0; }
    bool operator <  ( const CInvoice & s ) const { return this->compare( s ) <  0; }
    CInvoice ( const CInvoice & i, int order, const string & s, const string & b ) : m_wdate ( i.wdate() ), m_seller ( i.seller() ),
    m_buyer ( i.buyer() ), m_amount ( i.amount()), m_order ( order ), m_vat ( i.vat() )
    {
      setString ( s, m_seller );
      setString ( b, m_buyer );
      m_seller_lower = toLowerCaseWithSpaces ( m_seller );
      m_seller_standard = standardize ( m_seller );
      m_buyer_lower = toLowerCaseWithSpaces ( m_buyer );
      m_buyer_standard = standardize ( m_buyer );
    }
    int compare ( const CInvoice & s ) const
    {
      /* Default comparison order for std::set is :
       * date, seller, buyer, amount, vat
       */
      if ( dateComp ( s ) != 0 ) return dateComp ( s );
      if ( s.sellerF().compare( sellerF() ) != 0 ) return s.sellerF().compare( sellerF() );
      if ( s.buyerF().compare( buyerF() ) != 0 ) return s.buyerF().compare( buyerF() );
      if ( (int)(s.amount() - amount()) != 0  ) return (int)(s.amount() - amount());
      if ( s.vat() - vat() != 0 ) return s.vat() - vat();
      return 0;
    }
    int dateComp ( const CInvoice & s ) const
    {
      if ( s.wdate ( ) > m_wdate ) return -1;
      if ( s.wdate ( ) < m_wdate ) return  1;
      return 0;
    }
    CDate date ( void ) const { return m_wdate.get(); }
    CDateWrapper wdate ( void ) const { return m_wdate; }
    string seller ( void ) const { return m_seller; }
    string buyer ( void ) const { return m_buyer; }
    string sellerF ( void ) const { return m_seller_standard; }
    string buyerF ( void ) const { return m_buyer_standard; }
    string lowerS ( void ) const { return m_seller_lower; }
    string lowerB ( void ) const { return m_buyer_lower; }
    void setString ( const string & s, string & x ) { x = s; }
    unsigned int amount ( void ) const { return m_amount; }
    int order ( void ) const { return m_order; }
    double vat ( void ) const { return m_vat; }
    void print () const
    {
      cout << date() << " " << lowerS() << " " << lowerB()
      << " " << m_amount << " " << m_vat << " " << m_order << endl; 
    }
  private:
    // m_order represents the order in which the invoices are inserted into CVATRegiste class
    CDateWrapper m_wdate;
    string m_seller, m_seller_lower, m_seller_standard, m_buyer, m_buyer_lower, m_buyer_standard;
    unsigned int m_amount, m_order;
    double m_vat;
    string toLowerCaseWithSpaces ( const string & input ) 
    {
      //preserves space duplicates
      string result;
      for (char c : input) {
          if ( ! isspace( c ) ) result += tolower(c);
          else result += c;
      }
      return result;
    }
};

class CSortOpt
{
  public:
    static const int BY_DATE   = 0;
    static const int BY_BUYER  = 1;
    static const int BY_SELLER = 2;
    static const int BY_AMOUNT = 3;
    static const int BY_VAT    = 4;
    static const int BYORDER = 5;
    CSortOpt ( void ) {}
    CSortOpt & addKey ( int key, bool ascending = true )
    {
      keys.push_back( make_pair( key, ascending ) );
      return *this;
    }
    vector<pair<int, bool>> getKey() const { return keys; }
  private:
    vector<pair<int, bool>> keys;
};

class CVATRegister
{
  public:
    CVATRegister ( void ) { size = 0; }
    bool registerCompany(const string & name)
    {
        if ( name.length() == 0 ) return false;
        string standard = standardize ( name ); 
        if ( companies.find( standard ) != companies.end() ) return false;
        companies[standard] = name;
        return true;
    }

    bool addInvoice ( const CInvoice & x, int state )
    {
        auto tmpS = companies.find( x.sellerF() );
        auto tmpB = companies.find( x.buyerF() );
        auto tmpI = invoices.find ( x );
        if ( tmpS == companies.end() || tmpB == companies.end() || x.sellerF() == x.buyerF() ) return false;
        if ( tmpI != invoices.end() )
        {
          if ( tmpI->second == state ) return false;
          invoices[x] = 2;
        }
        else 
        {
          invoices[CInvoice ( x, size++, tmpS->second, tmpB->second )] = state; 
          compInv[ tmpS->second ].push_back ( CInvoice ( x, size++, tmpS->second, tmpB->second )  );
          compInv[ tmpB->second ].push_back ( CInvoice ( x, size++, tmpS->second, tmpB->second )  );
        }
        return true;
    }

    bool removeInvoice( const CInvoice & x, int state )
    {
        auto tmpI = invoices.find ( x );
        if ( tmpI == invoices.end() ) return false;
        if ( tmpI->second == 2 ) invoices[x] = !state;
        else if ( tmpI->second != state ) return false;
        else invoices.erase ( x );
        return true;
    }

    bool addIssued ( const CInvoice & x ) { return addInvoice ( x, 0 ); }
    bool addAccepted ( const CInvoice & x ) { return addInvoice ( x, 1 ); }
    bool delIssued ( const CInvoice & x ) { return removeInvoice ( x, 0 ); }
    bool delAccepted ( const CInvoice & x ) { return removeInvoice ( x, 1 ); }
    list<CInvoice> unmatched ( const string & company, const CSortOpt & sortBy ) const
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
    }
    void print ( ) const
    {
      for ( auto & x : invoices ) x.first.print();
    }
  private:
    int size;
    map<CInvoice, int> invoices; //int = 0 : issued, int = 1 : accepted ,int = 2 : paired ( issued && accepted )
    map<string, vector<CInvoice>> compInv;
    map<string, string>companies; // pair.first : original name from registerCompany, pair.second : after standardization
    int compareBy ( const CInvoice & i, const CInvoice & x, const pair<int, bool> & a ) const
    {
      //this function compares two invoices by a, which is a pair from sortBy.keys
      switch(a.first)
      {
        case(0): return a.second ? i.dateComp(x) : x.dateComp(i);
        case(1): return a.second ? i.lowerB().compare( x.lowerB() ) : x.lowerB().compare( i.lowerB() );
        case(2): return a.second ? i.lowerS().compare( x.lowerS() ) : x.lowerS().compare( i.lowerS() );
        case(3): return a.second ? i.amount() - x.amount() : x.amount() - i.amount();
        case(4): return a.second ? i.vat() - x.vat() : x.vat() - i.vat();
        default: return x.order() - i.order();
      }
      return 0;
    }
  };

#ifndef __PROGTEST__
bool equalLists ( const list<CInvoice> & a, const list<CInvoice> & b )
{
  if ( a.size ( ) != b.size ( ) ) return false;
  auto it1 = a.begin ( );
  auto it2 = b.begin ( );
  while ( true ) 
  {
    if ( it1 == a.end() ) break;
    if ( it1->buyer() != it2->buyer() || it1->seller() != it2->seller() || 
        it1->date().compare( it2->date() )||  it1->amount() != it2->amount() || 
        it1->vat() != it2->vat()
       ) return false;
    ++it1;
    ++it2;
  }
  return true;
}


int main ( void )
{
  CVATRegister r;
  assert ( r . registerCompany ( "first Company" ) );
  assert ( r . registerCompany ( "Second     Company" ) );
  assert ( r . registerCompany ( "ThirdCompany, Ltd." ) );
  assert ( r . registerCompany ( "Third Company, Ltd." ) );
  assert ( !r . registerCompany ( "Third Company, Ltd." ) );
  assert ( !r . registerCompany ( " Third  Company,  Ltd.  " ) );
  assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company ", 100, 20 ) ) );
  assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 2 ), "FirSt Company", "Second Company ", 200, 30 ) ) );
  assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company ", 100, 30 ) ) );
  assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company ", 300, 30 ) ) );
  assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", " Third  Company,  Ltd.   ", 200, 30 ) ) );
  assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "Second Company ", "First Company", 300, 30 ) ) );
  assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "Third  Company,  Ltd.", "  Second    COMPANY ", 400, 34 ) ) );
  assert ( !r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company ", 300, 30 ) ) );
  assert ( !r . addIssued ( CInvoice ( CDate ( 2000, 1, 4 ), "First Company", "First   Company", 200, 30 ) ) );
  assert ( !r . addIssued ( CInvoice ( CDate ( 2000, 1, 4 ), "Another Company", "First   Company", 200, 30 ) ) );
  assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () 
    . addKey ( CSortOpt::BY_SELLER, true ) 
    . addKey ( CSortOpt::BY_BUYER, false ) 
    . addKey ( CSortOpt::BY_DATE, false ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 )
           } ) );
  assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_DATE, true ) . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 )
           } ) );
  assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_VAT, true ) . addKey ( CSortOpt::BY_AMOUNT, true ) . addKey ( CSortOpt::BY_DATE, true ) . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 )
           } ) );
  assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 )
           } ) );
  assert ( equalLists ( r . unmatched ( "second company", CSortOpt () . addKey ( CSortOpt::BY_DATE, false ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Third Company, Ltd.", "Second     Company", 400, 34.000000 )
           } ) );
  assert ( equalLists ( r . unmatched ( "last company", CSortOpt () . addKey ( CSortOpt::BY_VAT, true ) ),
           list<CInvoice>
           {
           } ) );
  assert ( r . addAccepted ( CInvoice ( CDate ( 2000, 1, 2 ), "First Company", "Second Company ", 200, 30 ) ) );
  assert ( r . addAccepted ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", " Third  Company,  Ltd.   ", 200, 30 ) ) );
  assert ( r . addAccepted ( CInvoice ( CDate ( 2000, 1, 1 ), "Second company ", "First Company", 300, 32 ) ) );
  assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) . addKey ( CSortOpt::BY_DATE, true ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 32.000000 )
           } ) );
  assert ( !r . delIssued ( CInvoice ( CDate ( 2001, 1, 1 ), "First Company", "Second Company ", 200, 30 ) ) );
  assert ( !r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "A First Company", "Second Company ", 200, 30 ) ) );
  assert ( !r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Hand", 200, 30 ) ) );
  assert ( !r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company", 1200, 30 ) ) );
  assert ( !r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company", 200, 130 ) ) );
  assert ( r . delIssued ( CInvoice ( CDate ( 2000, 1, 2 ), "First Company", "Second Company", 200, 30 ) ) );
  assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () 
  . addKey ( CSortOpt::BY_SELLER, true ) 
  . addKey ( CSortOpt::BY_BUYER, true ) 
  . addKey ( CSortOpt::BY_DATE, true ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 32.000000 )
           } ) );
  assert ( r . delAccepted ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", " Third  Company,  Ltd.   ", 200, 30 ) ) );
  assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) . addKey ( CSortOpt::BY_DATE, true ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 32.000000 )
           } ) );
  assert ( r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", " Third  Company,  Ltd.   ", 200, 30 ) ) );
  assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) . addKey ( CSortOpt::BY_DATE, true ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 32.000000 )
           } ) );
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
