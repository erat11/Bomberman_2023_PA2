#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <sys/time.h>
using namespace std;


// A C++ 11 compiler automatically adds move cons/op= if
// there is no copy cons/op= in the class.
// If there is a custom copy cons/op= in a class, the compiler assumes
// the class needs some special handling of its internal structures,
// thus it does not add the move cons/op=. This may be a significant slowdown.


class CStr
{
  public:
                             CStr                ( const char * Str = "" );
    explicit                 CStr                ( double X );
    explicit                 CStr                ( int X );
                             CStr                ( const CStr & x );
                             CStr                ( CStr && x ) noexcept;
                            ~CStr                ( void ) noexcept;
    CStr                   & operator =          ( const CStr & x );
    CStr                   & operator =          ( CStr && x );
    int                      Length              ( void ) const;
    CStr                   & operator +=         ( const CStr & x );
    CStr                   & operator +=         ( const char * x );
    CStr                   & operator +=         ( int x );
    CStr                   & operator +=         ( double x );
                             operator const char * ( void ) const;
    char                   & operator []         ( int idx );

  private:
    int                      m_Len;
    int                      m_Max;
    char                   * m_Data;

    void                     append              ( const char * src, int srclen );

    friend CStr              operator +          ( const CStr & a, const CStr & b );
    friend std::ostream    & operator <<         ( std::ostream & os, const CStr & x );
};
CStr               operator +                    ( const CStr & a, const CStr & b );
std::ostream     & operator <<                   ( std::ostream & os, const CStr & x );



//=================================================================================================
// 1-st implementation: no copy cons/op=, thus the compiler adds automatically generated
// CPerson1 copy/move cons/op=
class CPerson1
{
  public:
    //---------------------------------------------------------------------------------------------
                             CPerson1            ( const CStr & name = "",
                                                   const CStr & surname = "",
                                                   const CStr & optMaidenName = "" )
      : m_Name ( name ), 
        m_Surname ( surname ), 
        m_MaidenName ( optMaidenName )
    {
    }
    //---------------------------------------------------------------------------------------------
                             CPerson1            ( CStr && name,
                                                   CStr && surname,
                                                   CStr && optMaidenName )
      : m_Name ( move ( name ) ), 
        m_Surname ( move ( surname ) ), 
        m_MaidenName ( move ( optMaidenName ) )
    {
    }
    //---------------------------------------------------------------------------------------------
  private:
    CStr                     m_Name;
    CStr                     m_Surname;
    CStr                     m_MaidenName;
};


//=================================================================================================
// 2-nd implementation: we add the optional maiden name in the form of a pointer (nullptr/valid).
// Thus we have to implement copy cons/op=.
// But this means the compiler will not add the move cons/op=, resulting in slow move operations.
class CPerson2
{
  public:
    //---------------------------------------------------------------------------------------------
                             CPerson2            ( const CStr & name = "",
                                                   const CStr & surname = "",
                                                   CStr * optMaidenName = nullptr )
      : m_Name ( name ), 
        m_Surname ( surname ), 
        m_MaidenName ( optMaidenName )
    {
    }
    //---------------------------------------------------------------------------------------------
                             CPerson2            ( CStr && name,
                                                   CStr && surname,
                                                   CStr * optMaidenName = nullptr )
      : m_Name ( move ( name ) ), 
        m_Surname ( move ( surname ) ), 
        m_MaidenName ( optMaidenName )
    {
    }
    //---------------------------------------------------------------------------------------------
                             CPerson2            ( const CPerson2 & src )
      : m_Name ( src . m_Name ), 
        m_Surname ( src . m_Surname ),
        m_MaidenName ( src . m_MaidenName ? new CStr ( * src . m_MaidenName ) : nullptr )
    {
    }
    //---------------------------------------------------------------------------------------------
                            ~CPerson2            ( void )
    {
      delete m_MaidenName;
    }
    //---------------------------------------------------------------------------------------------
    CPerson2               & operator =          ( const CPerson2 & src )
    {
      if ( this == &src ) 
        return *this;
      m_Name = src . m_Name;
      m_Surname = src . m_Surname;
      delete m_MaidenName;
      m_MaidenName = src . m_MaidenName ? new CStr ( * src . m_MaidenName ) : nullptr;
      return *this;
    }
    //---------------------------------------------------------------------------------------------
  private:
    CStr                     m_Name;
    CStr                     m_Surname;
    CStr                   * m_MaidenName;
};
//=================================================================================================
// 3-rd implementation: we add move cons/op=, but we do it in a wrong way, thus the compiler
// will not actually use move cons/op= for the fields. Again, the speed will suffer.
class CPerson3
{
  public:
    //---------------------------------------------------------------------------------------------
                             CPerson3            ( const CStr & name = "",
                                                   const CStr & surname = "",
                                                   CStr * optMaidenName = nullptr )
      : m_Name ( name ), 
        m_Surname ( surname ), 
        m_MaidenName ( optMaidenName )
    {
    }
    //---------------------------------------------------------------------------------------------
                             CPerson3            ( CStr && name,
                                                   CStr && surname,
                                                   CStr * optMaidenName = nullptr )
      : m_Name ( move ( name ) ), 
        m_Surname ( move ( surname ) ), 
        m_MaidenName ( optMaidenName )
    {
    }
    //---------------------------------------------------------------------------------------------
                             CPerson3            ( const CPerson3 & src )
      : m_Name ( src . m_Name ), 
        m_Surname ( src . m_Surname ),
        m_MaidenName ( src . m_MaidenName ? new CStr ( * src . m_MaidenName ) : nullptr )
    {
    }
    //---------------------------------------------------------------------------------------------
                             CPerson3            ( CPerson3 && src )
      : m_Name ( src . m_Name ), 
        m_Surname ( src . m_Surname ),
        m_MaidenName ( src . m_MaidenName )
      // src . m_Name / src . m_Surname is not a r-value reference, the compiler does not use move copy cons
    {
      src . m_MaidenName = nullptr;
    }
    //---------------------------------------------------------------------------------------------
                            ~CPerson3            ( void )
    {
      delete m_MaidenName;
    }
    //---------------------------------------------------------------------------------------------
    CPerson3               & operator =          ( const CPerson3 & src )
    {
      if ( this == &src ) 
        return *this;
      m_Name = src . m_Name;
      m_Surname = src . m_Surname;
      delete m_MaidenName;
      m_MaidenName = src . m_MaidenName ? new CStr ( * src . m_MaidenName ) : nullptr;
      return *this;
    }
    //---------------------------------------------------------------------------------------------
    CPerson3               & operator =          ( CPerson3 && src )
    {
      swap ( m_Name, src . m_Name );
      swap ( m_Surname, src . m_Surname );
      swap ( m_MaidenName, src . m_MaidenName );
      return *this;
    }
    //---------------------------------------------------------------------------------------------
  private:
    CStr                     m_Name;
    CStr                     m_Surname;
    CStr                   * m_MaidenName;
};
//=================================================================================================
// 4-th implementation: we add move cons/op= and we do it the correct way (note the move() func calls).
class CPerson4
 {
   public:
    //---------------------------------------------------------------------------------------------
                             CPerson4            ( const CStr & name = "",
                                                   const CStr & surname = "",
                                                   CStr * optMaidenName = nullptr )
      : m_Name ( name ), 
        m_Surname ( surname ), 
        m_MaidenName ( optMaidenName )
    {
    }
    //---------------------------------------------------------------------------------------------
                             CPerson4            ( CStr && name,
                                                   CStr && surname,
                                                   CStr * optMaidenName = nullptr )
      : m_Name ( move ( name ) ), 
        m_Surname ( move ( surname ) ), 
        m_MaidenName ( optMaidenName )
    {
    }
    //---------------------------------------------------------------------------------------------
                             CPerson4            ( const CPerson4 & src )
      : m_Name ( src . m_Name ), 
        m_Surname ( src . m_Surname ),
        m_MaidenName ( src . m_MaidenName ? new CStr ( * src . m_MaidenName ) : nullptr )
    {
    }
    //---------------------------------------------------------------------------------------------
                             CPerson4            ( CPerson4 && src )
      : m_Name ( move ( src . m_Name ) ), 
        m_Surname ( move ( src . m_Surname ) ),
        m_MaidenName ( src . m_MaidenName )
    {
      src . m_MaidenName = nullptr;
    }
    //---------------------------------------------------------------------------------------------
                            ~CPerson4            ( void )
    {
      delete m_MaidenName;
    }
    //---------------------------------------------------------------------------------------------
    CPerson4               & operator =          ( const CPerson4 & src )
    {
      if ( this == &src ) 
        return *this;
      m_Name = src . m_Name;
      m_Surname = src . m_Surname;
      delete m_MaidenName;
      m_MaidenName = src . m_MaidenName ? new CStr ( * src . m_MaidenName ) : nullptr;
      return *this;
    }
    //---------------------------------------------------------------------------------------------
    CPerson4               & operator =          ( CPerson4 && src )
    {
      swap ( m_Name, src . m_Name );
      swap ( m_Surname, src . m_Surname );
      swap ( m_MaidenName, src . m_MaidenName );
      return *this;
    }
    //---------------------------------------------------------------------------------------------
  private:
    CStr                     m_Name;
    CStr                     m_Surname;
    CStr                   * m_MaidenName;
 };
//=================================================================================================
// 5-th implementation: we add noexcept declaration to the move cons/op=/destructor. This declaration informs
// STL containers that move can be safely used when copying the contents into a bigger container. Thus
// push_back can safely use move instead of expensive copy / delete
class CPerson5
{
  public:
    //---------------------------------------------------------------------------------------------
                             CPerson5            ( const CStr & name = "",
                                                   const CStr & surname = "",
                                                   CStr * optMaidenName = nullptr )
      : m_Name ( name ), 
        m_Surname ( surname ), 
        m_MaidenName ( optMaidenName )
    {
    }
    //---------------------------------------------------------------------------------------------
                             CPerson5            ( CStr && name,
                                                   CStr && surname,
                                                   CStr * optMaidenName = nullptr )
      : m_Name ( move ( name ) ), 
        m_Surname ( move ( surname ) ), 
        m_MaidenName ( optMaidenName )
    {
    }
    //---------------------------------------------------------------------------------------------
                             CPerson5            ( const CPerson5 & src )
      : m_Name ( src . m_Name ), 
        m_Surname ( src . m_Surname ),
        m_MaidenName ( src . m_MaidenName ? new CStr ( * src . m_MaidenName ) : nullptr )
    {
    }
    //---------------------------------------------------------------------------------------------
                             CPerson5            ( CPerson5 && src ) noexcept
      : m_Name ( move ( src . m_Name ) ), 
        m_Surname ( move ( src . m_Surname ) ),
        m_MaidenName ( src . m_MaidenName )
    {
      src . m_MaidenName = nullptr;
    }
    //---------------------------------------------------------------------------------------------
                            ~CPerson5            ( void ) noexcept
    {
      delete m_MaidenName;
    }
    //---------------------------------------------------------------------------------------------
    CPerson5               & operator =          ( const CPerson5 & src )
    {
      if ( this == &src ) 
        return *this;
      m_Name = src . m_Name;
      m_Surname = src . m_Surname;
      delete m_MaidenName;
      m_MaidenName = src . m_MaidenName ? new CStr ( * src . m_MaidenName ) : nullptr;
      return *this;
    }
    //---------------------------------------------------------------------------------------------
    CPerson5               & operator =          ( CPerson5 && src )
    {
      swap ( m_Name, src . m_Name );
      swap ( m_Surname, src . m_Surname );
      swap ( m_MaidenName, src . m_MaidenName );
      return *this;
    }
    //---------------------------------------------------------------------------------------------
  private:
    CStr                     m_Name;
    CStr                     m_Surname;
    CStr                   * m_MaidenName;
};
//=================================================================================================
                   CStr::CStr                    ( const char * Str )
  : m_Len ( strlen ( Str ) ),
    m_Max ( m_Len + 1 ),
    m_Data ( new char[m_Max] )
{
  strncpy ( m_Data, Str, m_Max );
}
//-------------------------------------------------------------------------------------------------
                   CStr::CStr                    ( double X )
   : m_Len ( 0 ),
     m_Max ( 20 ),
     m_Data ( new char [m_Max] )
{
 *m_Data    = '\0';
  operator += ( X );
}
//-------------------------------------------------------------------------------------------------
                   CStr::CStr                    ( int X )
                     : m_Len ( 0 ),
                       m_Max ( 20 ),
                       m_Data ( new char [m_Max] )
{
 *m_Data    = '\0';
  operator += ( X );
}
//-------------------------------------------------------------------------------------------------
                   CStr::CStr                    ( const CStr & src )
                     : m_Len ( src . m_Len ),
                       m_Max ( src . m_Max ),
                       m_Data ( new char [m_Max] )
{
  memcpy    ( m_Data, src . m_Data, m_Len + 1 );
}
//-------------------------------------------------------------------------------------------------
/** move constructor is invoked when a new object is created as a copy of some existing object x,
 * where it is clear x will be destroyed after the copy. Thus, it is save to grab the contents of
 * x and use it in the newly constructed object. The only consideration is to leave x in some
 * defined state, such that the destruction of x does not crash/corrupt data. In our case we
 * replace the data with nullptr (thus the subsequent destructor is a no-operation).
 */
                   CStr::CStr                    ( CStr && src ) noexcept
  : m_Len ( src . m_Len ),
    m_Max ( src . m_Max ),
    m_Data ( src . m_Data )
{
  src . m_Data = nullptr;
}
//-------------------------------------------------------------------------------------------------
                   CStr::~CStr                   ( void ) noexcept
{
  delete [] m_Data;
}
//-------------------------------------------------------------------------------------------------
CStr             & CStr::operator =              ( const CStr & x )
{
  if ( &x != this )
  {
    delete [] m_Data;
    m_Max   = x . m_Max;
    m_Len   = x . m_Len;
    m_Data  = new char [m_Max];
    memcpy  ( m_Data, x . m_Data, m_Len + 1 );
  }
  return *this;
}
//-------------------------------------------------------------------------------------------------
// move op = is similar to move constructor
CStr             & CStr::operator =              ( CStr && x )
{
  // just swap the contents. Thus the destruction of x indeed releases the content previously
  // stored in this
  swap ( x . m_Data, m_Data );
  swap ( x . m_Max, m_Max );
  swap ( x . m_Len, m_Len );
  return *this;
}
//-------------------------------------------------------------------------------------------------
int                CStr::Length                  ( void ) const
{
  return m_Len;
}
//-------------------------------------------------------------------------------------------------
CStr             & CStr::operator +=             ( const CStr & x )
{
  append ( x . m_Data, x . m_Len );
  return *this;
}
//-------------------------------------------------------------------------------------------------
CStr             & CStr::operator +=             ( const char * x )
{
  append ( x, strlen ( x ) );
  return *this;
}
//-------------------------------------------------------------------------------------------------
CStr             & CStr::operator +=             ( int x )
{
  char tmp [20];

  snprintf  ( tmp, sizeof ( tmp ), "%d", x );   // will not use ostringstream
  append    ( tmp, strlen ( tmp ) );
  return    *this;
}
//-------------------------------------------------------------------------------------------------
CStr             & CStr::operator +=             ( double x )
{
  char tmp [20];

  snprintf ( tmp, sizeof ( tmp ), "%f", x );
  append ( tmp, strlen ( tmp ) );
  return *this;
}
//-------------------------------------------------------------------------------------------------
                   CStr::operator const char *   ( void ) const
{
  return m_Data;
}
//-------------------------------------------------------------------------------------------------
char             & CStr::operator []             ( int idx )
{
  if ( idx < 0 || idx >= m_Len )
    throw "index out of range";

  return m_Data[idx];
}
//-------------------------------------------------------------------------------------------------
void              CStr::append                   ( const char * src, int srclen )
{
  if ( m_Len + srclen + 1 >= m_Max )
  {
    m_Max = m_Len + srclen + 1;
    char * tmp = new char [m_Max];
    memcpy ( tmp, m_Data, m_Len );
    memcpy ( tmp + m_Len, src, srclen );
    delete [] m_Data; // safe even when m_Data == src
    m_Data = tmp;
  }
  else
    memcpy         ( m_Data + m_Len, src, srclen );
  m_Len         += srclen;
  m_Data[m_Len]  = 0;
}
//-------------------------------------------------------------------------------------------------
CStr              operator +                     ( const CStr & a, const CStr & b )
{
  CStr res ( a );
  res += b;
  return ( res );
}
//-------------------------------------------------------------------------------------------------
std::ostream    & operator <<                    ( std::ostream & os, const CStr & x )
{
  os << x . m_Data;
  return os;
}
//=================================================================================================
double             getTimeStamp                  ( void )
{
  struct timeval tv;
  gettimeofday ( &tv, nullptr );
  return tv . tv_sec + tv . tv_usec / 1000000.0;
}
//-------------------------------------------------------------------------------------------------
CStr               randomString                  ( const char * randStr, int len )
{
  int x = (int)(0.5 * rand () * len / RAND_MAX);
  return CStr ( randStr + x );
}
//=================================================================================================
template <typename T>
void               test                          ( const char * testName,
                                                   int          testSize,
                                                   const char * randStr,
                                                   int          randLen )
{
  vector<T> vec;
  double timeSt;

  cout <<  "Test: " << testName << endl;
  timeSt = getTimeStamp ();
  for ( int i = 0; i < testSize; i ++ )
    vec . push_back ( T ( randomString ( randStr, randLen ), randomString ( randStr, randLen ) ) );
  cout << "push back: " << testSize << " objects: " << ( getTimeStamp () - timeSt ) << " sec" << endl;


  timeSt = getTimeStamp ();
  for ( int i = 0, j = testSize - 1; i < j; i ++, j -- )
    swap ( vec[i], vec[j] );
  cout << "op=: " << testSize / 2 << " objects: " << ( getTimeStamp () - timeSt ) << " sec" << endl << endl;
}
//-------------------------------------------------------------------------------------------------
int                main                          ( int argc, char * argv [] )
{
  const int RAND_LEN = 30000;
  char  randSeed [RAND_LEN];

  for ( int i = 0; i < RAND_LEN - 1; i ++ )
    randSeed[i] = 'a' + (int)(rand () * 26.0 / RAND_MAX );
  randSeed[RAND_LEN - 1] = 0;

  const int ARRAY_SIZE = 30000;
  test<CPerson1> ( "CPerson1 (auto gen move)", ARRAY_SIZE, randSeed, RAND_LEN );
  test<CPerson2> ( "CPerson2 (no move cons/op=)", ARRAY_SIZE, randSeed, RAND_LEN );
  test<CPerson3> ( "CPerson3 (wrong move cons)", ARRAY_SIZE, randSeed, RAND_LEN );
  test<CPerson4> ( "CPerson4 (correct move cons)", ARRAY_SIZE, randSeed, RAND_LEN );
  test<CPerson5> ( "CPerson5 (correct move + noexcept)", ARRAY_SIZE, randSeed, RAND_LEN );

  return 0;
}
//-------------------------------------------------------------------------------------------------
