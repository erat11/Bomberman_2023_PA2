#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <typeinfo>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <list>
#include <string>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <memory>
#include <algorithm>
#include <type_traits>
using namespace std;

class CRect 
{
  public:
    CRect ( double x, double y, double w, double h )
    : m_X ( x ), m_Y ( y ), m_W ( w ), m_H ( h ) { }
    friend ostream & operator << ( ostream & os, const CRect & x )
    {
      return os << '(' << x . m_X << ',' << x . m_Y << ',' << x . m_W << ',' << x . m_H << ')';
    }
    double m_X;
    double m_Y;
    double m_W;
    double m_H;
};
#endif /* __PROGTEST__ */

class CParent
{
  public:
    CParent ( int i, const string & n, const CRect & r, int type ) : id ( i ), name ( n ), pos ( r ), type ( type ) {}
    friend ostream & operator << ( ostream & os, const CParent & x )
    {
      x.print( os );
      return os;
    }
    virtual void print( ostream & os ) const 
    {
      string out;
      switch ( getType() )
      {
        case 1:{ out = "Button"; break; }
        case 2:{ out = "Input"; break; }
        case 3:{ out = "Label"; break; }
      }
      os << '[' << getId() << "] "<< out << " \"" << getName() << "\" " << getPos() << "\n";
    }
    CParent () : id(-1), name("JON"), pos ( CRect ( 0,0,0,0 ) ), type(1) {}
    CParent ( const CParent & x ) : id ( x.getId() ), name ( x.getName() ), 
    pos ( CRect ( x.getPos().m_X, x.getPos().m_Y, x.getPos().m_W, x.getPos().m_H ) ), type ( x.getType() ) {}
    virtual CParent & operator = ( const CParent & x )
    {
      id = x.getId();
      name = x.getName();
      pos = CRect ( x.getPos().m_X, x.getPos().m_Y, x.getPos().m_W, x.getPos().m_H );
      type = x.getType();
      return *this;
    }
    virtual CParent & operator * ( ) { return *this; }
    virtual ~CParent () {}
    string getName () const
    {
      return name;
    }
    int getId () const
    {
      return id;
    }
    CRect getPos () const
    {
      return pos;
    }
    int getType () const
    {
      return type;
    }
    void setPos ( const CRect & x )
    {
      pos = CRect ( x.m_X, x.m_Y, x.m_W, x.m_H );
    }
  protected:
    int id;
    string name;
    CRect pos;
    int type; // 0 window, 1 button, 2 input, 3 label, 4 combobox
};

class CComboBox : public CParent
{
  public:
    CComboBox ( int id, const CRect & relPos ) : CParent ( id, "placeholder"s, relPos, 4 ) { idx = 0; }
    CComboBox ( const CComboBox & x )
    {
      *this = x;
    }
    CComboBox & add ( const string & x )
    {
      offer.push_back ( x );
      return *this;
    }
    CParent & operator = ( const CParent & x ) override
    {
      id = x.getId();
      pos = CRect ( x.getPos().m_X, x.getPos().m_Y, x.getPos().m_W, x.getPos().m_H );
      type = x.getType();
      if ( type == 4 )
      {
        auto y = dynamic_cast<const CComboBox &>(x);
        offer = y.get();
        idx = y.getSelected();
      }
      return *this;
    }
    friend ostream & operator << ( ostream & os, const CComboBox & x )
    {
      x.print( os );
      return os;
    }    
    void print( ostream & os ) const override
    {
      os << '[' << getId() << "] ComboBox " << getPos() << "\n";
      for ( unsigned int i = 0; i < offer.size(); ++i )
        if ( i == idx )
          os <<"+->" << offer[i] << "<" << endl;
        else
          os <<"+- " << offer[i] << endl;
    }                                                         
    void setSelected ( unsigned int x )
    {
      idx = x;
    }
    unsigned int getSelected () const
    {
      return idx;
    }
    vector<string> get() const
    {
      return offer;
    }
  private:
    vector<string> offer;
    unsigned int idx;
};

class CInput : public CParent
{
  public:
    CInput ( int id, const CRect & relPos, const string & value ) : CParent ( id, value, relPos, 2 ) { setValue( value ); }
    void setValue ( const string & x )
    {
      name = x;
    } 
    string getValue ( void ) const
    {
      return name;
    }
};

class CWindow : public CParent
{
  public:
    CWindow ( int id, const string & title, const CRect & absPos ) : CParent ( id, title, absPos, 0 ) {}
    CWindow ( const CWindow & x )
    {
      id = x.getId();
      name = x.getName();
      pos = CRect ( x.getPos().m_X, x.getPos().m_Y, x.getPos().m_W, x.getPos().m_H );
      type = x.getType();
      for ( auto & ptr : x.get() ) 
        this->add( *ptr );
    }
    CWindow & add(const CParent & x)
    {
        CRect tmp = x.getPos();
        CRect n ( x.getPos().m_X, x.getPos().m_Y, x.getPos().m_W, x.getPos().m_H );
        if ( x.getPos().m_X < 1 )
          n = CRect(tmp.m_X * pos.m_W + pos.m_X, tmp.m_Y * pos.m_H + pos.m_Y, tmp.m_W * pos.m_W, tmp.m_H * pos.m_H);
        if ( x.getType() == 4 ) 
        {
          count++;
          CComboBox * a = new CComboBox ( x.getId(), n );
          const CComboBox & b = dynamic_cast<const CComboBox &> ( x );
          for ( auto & c : b.get() )
            a->add( c );
          op.push_back( a );
        }
        else if ( x.getType() == 2 )
        {
          CInput * a = new CInput ( x.getId(), n, x.getName() );
          const CInput & b = dynamic_cast<const CInput &> ( x );
          a->setValue( b.getValue() );
          op.push_back( a );
        }
        else op.push_back( new CParent(x.getId(), x.getName(), n, x.getType()) );
        return *this;
    }
    friend ostream & operator << ( ostream & os, const CWindow & x )
    {
      x.print( os );
      return os;
    }
    CParent & operator = ( const CParent & x ) override
    {
      id = x.getId();
      name = x.getName();
      pos = CRect ( x.getPos().m_X, x.getPos().m_Y, x.getPos().m_W, x.getPos().m_H );
      type = x.getType();
      if ( type == 0 )
      {
        auto y = dynamic_cast< const CWindow &> ( x );
        for ( auto & ptr : y.get() ) 
          this->add( *ptr );
      }
      return *this;
    }
    CParent & operator * ( ) override
    { 
      return *this;
    }
    void print( ostream & os ) const override
    {
      int c = 0;
      os << '[' << getId() << "] Window \"" << getName() << "\" " << getPos() << "\n";
      string s = "   ";
      for ( auto y : get() )
        if ( y->getType() != 4 ) os << "+- " << *y;
        else
        {
          auto z = dynamic_cast< CComboBox & >(*y);
          os << "+- [" << z.getId() << "] ComboBox " << z.getPos() << "\n";
          for ( unsigned int i = 0; i < z.get().size(); ++i )
          {
            if ( count == 1 )
            {
              if ( i == z.getSelected() ) os <<"   +->" << z.get()[i] << "<" << endl;
              else os <<"   +- " << z.get()[i] << endl;
            }
            else
            {
              if ( i == z.getSelected() && !c )
                os <<"|  +->" << z.get()[i] << "<" << endl;
              else if ( i != z.getSelected() && !c )
                os <<"|  +- " << z.get()[i] << endl;
              else if ( i == z.getSelected() && c )
                os <<"   +->" << z.get()[i] << "<" << endl;
              else
                os <<"   +- " << z.get()[i] << endl;
            }
          }
          c++;
        }
    }  
    CParent * search ( int f )
    {
      for ( auto & x : op )
      {
        if ( x->getId() == f ) return x; 
      }
      return nullptr;
    }
    void setPosition ( const CRect & np )
    {
      CRect oldPos ( pos.m_X, pos.m_Y, pos.m_W, pos.m_H );
      pos = CRect ( np.m_X, np.m_Y, np.m_W, np.m_H );
      for ( auto & x : op )
      {
        x->setPos ( CRect ( ceil ( ( ( x->getPos().m_X - oldPos.m_X ) / oldPos.m_W ) * np.m_W + np.m_X ), 
                            ceil ( ( ( x->getPos().m_Y - oldPos.m_Y ) / oldPos.m_H ) * np.m_H + np.m_Y ),
                            ceil ( ( x->getPos().m_W / oldPos.m_W ) * np.m_W ),  
                            ceil ( ( x->getPos().m_H / oldPos.m_W ) * np.m_W )
                          )   
                  );
        //tmp.m_X * pos.m_W + pos.m_X, tmp.m_Y * pos.m_H + pos.m_Y, tmp.m_W * pos.m_W, tmp.m_H * pos.m_H
      }
    }
    vector<CParent*> get() const
    {
      return op;
    }
    ~CWindow (  ) override
    {
      if ( op.size() )
        for ( auto y : op )
          delete y;
    }
  private:
    vector <CParent*> op;
    int count = 0;
};

class CButton : public CParent
{
  public:
    CButton ( int id, const CRect & relPos, const string & name ) : CParent ( id, name, relPos, 1 ) {}
};

class CLabel : public CParent
{
  public:
    CLabel ( int id, const CRect & relPos, const string & label ) : CParent ( id, label, relPos, 3 ) {}
};

// output operators

#ifndef __PROGTEST__
template <typename _T>
string toString ( const _T & x )
{
  ostringstream oss;
  oss << x;
  return oss . str ();
}

int main ( void )
{
  assert ( sizeof ( CButton ) - sizeof ( string ) < sizeof ( CComboBox ) - sizeof ( vector<string> ) );
  assert ( sizeof ( CInput ) - sizeof ( string ) < sizeof ( CComboBox ) - sizeof ( vector<string> ) );
  assert ( sizeof ( CLabel ) - sizeof ( string ) < sizeof ( CComboBox ) - sizeof ( vector<string> ) );
  CWindow a ( 0, "Sample window", CRect ( 10, 10, 600, 480 ) );
  a . add ( CButton ( 1, CRect ( 0.1, 0.8, 0.3, 0.1 ), "Ok" ) ) . add ( CButton ( 2, CRect ( 0.6, 0.8, 0.3, 0.1 ), "Cancel" ) );
  a . add ( CLabel ( 10, CRect ( 0.1, 0.1, 0.2, 0.1 ), "Username:" ) );
  a . add ( CInput ( 11, CRect ( 0.4, 0.1, 0.5, 0.1 ), "chucknorris" ) );
  a . add ( CComboBox ( 20, CRect ( 0.1, 0.3, 0.8, 0.1 ) ) . add ( "Karate" ) . add ( "Judo" ) . add ( "Box" ) . add ( "Progtest" ) );
  //cout << toString ( a );
  assert ( toString ( a ) ==
    "[0] Window \"Sample window\" (10,10,600,480)\n"
    "+- [1] Button \"Ok\" (70,394,180,48)\n"
    "+- [2] Button \"Cancel\" (370,394,180,48)\n"
    "+- [10] Label \"Username:\" (70,58,120,48)\n"
    "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
    "+- [20] ComboBox (70,154,480,48)\n"
    "   +->Karate<\n"
    "   +- Judo\n"
    "   +- Box\n"
    "   +- Progtest\n" );
  CWindow b = a;
  assert ( toString ( b ) ==
    "[0] Window \"Sample window\" (10,10,600,480)\n"
    "+- [1] Button \"Ok\" (70,394,180,48)\n"
    "+- [2] Button \"Cancel\" (370,394,180,48)\n"
    "+- [10] Label \"Username:\" (70,58,120,48)\n"
    "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
    "+- [20] ComboBox (70,154,480,48)\n"
    "   +->Karate<\n"
    "   +- Judo\n"
    "   +- Box\n"
    "   +- Progtest\n" );
  assert ( toString ( *b . search ( 20 ) ) ==
    "[20] ComboBox (70,154,480,48)\n"
    "+->Karate<\n"
    "+- Judo\n"
    "+- Box\n"
    "+- Progtest\n" );
  assert ( dynamic_cast<CComboBox &> ( *b . search ( 20 ) ) . getSelected () == 0 );
  dynamic_cast<CComboBox &> ( *b . search ( 20 ) ) . setSelected ( 3 );
  assert ( dynamic_cast<CInput &> ( *b . search ( 11 ) ) . getValue () == "chucknorris" );
  dynamic_cast<CInput &> ( *b . search ( 11 ) ) . setValue ( "chucknorris@fit.cvut.cz" );
  b . add ( CComboBox ( 21, CRect ( 0.1, 0.5, 0.8, 0.1 ) ) . add ( "PA2" ) . add ( "OSY" ) . add ( "Both" ) );
  assert ( toString ( b ) ==
    "[0] Window \"Sample window\" (10,10,600,480)\n"
    "+- [1] Button \"Ok\" (70,394,180,48)\n"
    "+- [2] Button \"Cancel\" (370,394,180,48)\n"
    "+- [10] Label \"Username:\" (70,58,120,48)\n"
    "+- [11] Input \"chucknorris@fit.cvut.cz\" (250,58,300,48)\n"
    "+- [20] ComboBox (70,154,480,48)\n"
    "|  +- Karate\n"
    "|  +- Judo\n"
    "|  +- Box\n"
    "|  +->Progtest<\n"
    "+- [21] ComboBox (70,250,480,48)\n"
    "   +->PA2<\n"
    "   +- OSY\n"
    "   +- Both\n" );
  assert ( toString ( a ) ==
    "[0] Window \"Sample window\" (10,10,600,480)\n"
    "+- [1] Button \"Ok\" (70,394,180,48)\n"
    "+- [2] Button \"Cancel\" (370,394,180,48)\n"
    "+- [10] Label \"Username:\" (70,58,120,48)\n"
    "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
    "+- [20] ComboBox (70,154,480,48)\n"
    "   +->Karate<\n"
    "   +- Judo\n"
    "   +- Box\n"
    "   +- Progtest\n" );
  b . setPosition ( CRect ( 20, 30, 640, 520 ) );
  assert ( toString ( b ) ==
    "[0] Window \"Sample window\" (20,30,640,520)\n"
    "+- [1] Button \"Ok\" (84,446,192,52)\n"
    "+- [2] Button \"Cancel\" (404,446,192,52)\n"
    "+- [10] Label \"Username:\" (84,82,128,52)\n"
    "+- [11] Input \"chucknorris@fit.cvut.cz\" (276,82,320,52)\n"
    "+- [20] ComboBox (84,186,512,52)\n"
    "|  +- Karate\n"
    "|  +- Judo\n"
    "|  +- Box\n"
    "|  +->Progtest<\n"
    "+- [21] ComboBox (84,290,512,52)\n"
    "   +->PA2<\n"
    "   +- OSY\n"
    "   +- Both\n" );
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
