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
                             CRect                         ( double            x,
                                                             double            y,
                                                             double            w,
                                                             double            h )
      : m_X ( x ),
        m_Y ( y ),
        m_W ( w ),
        m_H ( h )
    {
    }
    friend ostream         & operator <<                   ( ostream         & os,
                                                             const CRect     & x )
    {
      return os << '(' << x . m_X << ',' << x . m_Y << ',' << x . m_W << ',' << x . m_H << ')';
    }
    double                   m_X;
    double                   m_Y;
    double                   m_W;
    double                   m_H;
};
#endif /* __PROGTEST__ */

class CParent
{
  public:
    CParent ( int i, int type, const CRect & p, const CRect & r ) : id ( i ), type ( type ), pos ( make_pair( p, r ) ) {}
    CParent () : id(-1), type(1), pos ( make_pair( CRect ( 0,0,0,0 ), CRect ( 0,0,0,0 ) ) ) {}
    CParent * operator * (  ) { return this; }
    CParent ( const CParent & x ) : id ( x.getId() ) ,type ( x.getType() ), 
    pos ( x.getPos().first, x.getPos().second ) {}
    virtual CParent & operator = ( const CParent & x )
    {
      return *this;
    }
    friend ostream & operator << ( ostream & os, const CParent & x )
    {
      x.printer( os, vector<bool>(), true, true );
      return os;
    }
    virtual void destroy () {}
    virtual void printer ( ostream & os, vector<bool> ender, bool end, bool solo ) const {}   
    int getId () const { return id; }
    int getType ( ) const { return type; }
    void setId ( int i ) { id = i; }
    void setType ( int i ) { type = i; }
    pair<CRect, CRect> getPos () const { return pos; }
    void setPos ( const pair<CRect, CRect> & x ) { pos = make_pair ( x.first, x.second ); }
    virtual ~CParent() { this->destroy(); }
  protected:
    int id, type; // 0 window, 1 button, 2 input, 3 label, 4 combobox, 5 panel
    pair<CRect, CRect> pos; //parentPos, relativePos
};

void smallPrinter ( ostream & os, const CParent & x, const string & value, const string & type, bool c )
{
  CRect a ( x.getPos().first.m_W * x.getPos().second.m_X + x.getPos().first.m_X, x.getPos().first.m_H * x.getPos().second.m_Y 
  + x.getPos().first.m_Y,x.getPos().first.m_W * x.getPos().second.m_W, x.getPos().first.m_H * x.getPos().second.m_H);
  if ( c ) os << '[' << x.getId() << "] " << type << " \"" << value << "\" " << a << "\n";
  else os << '[' << x.getId() << "] " << type << " " << a << "\n";
}

void copyPaste ( CParent & des, const CParent & sou )
{
  des.setId( sou.getId() );
  des.setType( sou.getType() );
  des.setPos( make_pair ( sou.getPos().first, sou.getPos().second ) );
}

class CComboBox : public CParent
{
  public:
    CComboBox ( int id, const CRect & relPos ) : CParent ( id, 4, relPos, relPos ) { idx = 0; }
    CComboBox ( int id, const CRect & parPos, const CRect & relPos ) : CParent ( id, 4, parPos, relPos ) { idx = 0; }
    CComboBox ( const CComboBox & x ) { *this = x; }
    CComboBox & operator = ( const CComboBox & x )
    {
      copyPaste( *this, x );
      idx = x.getSelected();
      offer = x.get();
      return *this;
    }
    CComboBox & add ( const string & x )
    {
      offer.push_back ( x );
      return *this;
    }
    void printer ( ostream & os,  vector<bool> ender, bool end, bool solo ) const override
    {
      for ( auto y : ender ) if ( !y ) os << "|  "; else os << "   ";
      if ( !solo ) os << "+- "; 
      smallPrinter ( os, *this, ""s, "ComboBox"s, false );
      if ( !solo ) { ender.push_back(false); }
      for ( unsigned int i = 0; i < offer.size(); ++i )
      {
        for ( unsigned int j = 0; j < ender.size(); ++j ) if ( ender[j] || ( end && j + 1 == ender.size() ) ) os << "   "; else os << "|  ";
        if ( i == idx )
          os <<"+->" << offer[i] << "<" << endl;
        else
          os <<"+- " << offer[i] << endl;
      }
      if ( ender.size() ) { ender.pop_back(); }
    }                                                        
    void setSelected ( unsigned int x ) { idx = x; }
    unsigned int getSelected () const { return idx; }
    vector<string> get() const { return offer; }
  private:
    vector<string> offer;
    unsigned int idx;
};

class CInput : public CParent
{
  public:
    CInput ( int id, const CRect & relPos, const string & value ) : CParent ( id, 2, relPos, relPos ) { setValue( value ); }
    CInput ( int id,  const CRect & parPos, const CRect & relPos, const string & value ) : CParent ( id, 2, parPos, relPos ) { setValue( value ); }
    CInput ( const CInput & x ) { *this = x; }
    CInput & operator = ( const CInput & x )
    {
      copyPaste( *this, x );
      value = x.getValue();
      return *this;
    }
    void setValue ( const string & x ) { value = x; } 
    string getValue ( void ) const { return value; }
    void printer ( ostream & os, vector<bool> ender, bool end, bool solo  ) const override 
    { 
      for ( auto y : ender ) if ( !y ) os << "|  "; else os << "   ";
      if ( !solo ) os << "+- ";
      smallPrinter ( os, *this, value, "Input"s, true ); 
    }
  private:
    string value;

};

class CButton : public CParent
{
  public:
    CButton ( const CButton & x ) { *this = x; }
    CButton ( int id, const CRect & relPos, const string & name ) : CParent ( id, 1, relPos, relPos ) { this->name = name; }
    CButton ( int id, const CRect & parPos, const CRect & relPos, const string & name ) : CParent ( id, 1, parPos, relPos ) { this->name = name; }
    string getName () const { return name; }
    CButton & operator = ( const CButton & x )
    {
      copyPaste( *this, x );
      name = x.getName();
      return *this;
    }
    void printer ( ostream & os,vector<bool> ender, bool end, bool solo  ) const override 
    { 
      for ( auto y : ender ) if ( !y ) os << "|  "; else os << "   ";
      if ( !solo ) os << "+- ";
      smallPrinter ( os, *this, name, "Button"s, true ); 
    }
  private:
    string name;
};

class CLabel : public CParent
{
  public:
    CLabel ( int id, const CRect & relPos, const string & label ) : CParent ( id, 3, relPos, relPos ) { this->label = label; }
    CLabel ( int id, const CRect & parPos, const CRect & relPos, const string & label ) : CParent ( id, 3, parPos, relPos ) { this->label = label; }
    CLabel ( const CLabel & x ) { *this = x; }
    string getLabel() const { return label; }
    CLabel & operator = ( const CLabel & x )
    {
      copyPaste( *this, x );
      label = x.getLabel();
      return *this;
    }
    void printer ( ostream & os, vector<bool> ender, bool end, bool solo  ) const override 
    { 
      for ( auto y : ender ) if ( !y ) os << "|  "; else os << "   ";
      if ( !solo ) os << "+- ";
      smallPrinter ( os, *this, label, "Label"s, true ); 
    }
  private:
    string label;
};

void castNAdd ( vector<CParent*> & v, const CParent & x, const CRect & f, const CRect & s );

class CPanel : public CParent
{
  public:
    CPanel ( int id, const CRect & relPos ) : CParent ( id, 5, relPos, relPos ) { }
    CPanel ( int id, const CRect & parPos, const CRect & relPos ) : CParent ( id, 5, parPos, relPos ) { }
    CPanel & operator = ( const CPanel & x )
    {
      destroy();
      copyPaste( *this, x );
      for ( const auto & z : x.get() ) add ( *z );
      return *this;
    }
    CPanel & add ( const CParent & x ) 
    {
      CRect a ( pos.first.m_W * pos.second.m_X + pos.first.m_X, pos.first.m_H * pos.second.m_Y 
      + pos.first.m_Y,pos.first.m_W * pos.second.m_W, pos.first.m_H * pos.second.m_H);
      castNAdd( storage, x, a, x.getPos().second );
      return *this;
    }
    void printer ( ostream & os, vector<bool> ender, bool end, bool solo ) const override 
    {
      for ( auto y : ender ) if ( !y ) os << "|  "; else os << "   ";
      if ( !solo ) os << "+- ";
      smallPrinter ( os, *this, ""s, "Panel"s, false );
      if ( end && !solo ) ender.push_back( true );
      else if ( !solo ) ender.push_back(false);
      for ( unsigned int i = 0; i < storage.size(); ++i )
      {
        if ( i + 1 == storage.size() ) storage[i]->printer( os, ender, true, false );
        else storage[i]->printer( os, ender, false, false );
      }
    }    
    vector<CParent*> get() const { return storage; }
    CParent * search ( int f )
    {
      for ( auto & x : storage )
      {
        if ( x->getType() == 5 && f != x->getId() )
        {
          auto a = dynamic_cast<CPanel *>(x);
          CParent * z = a->search( f );
          if ( z != nullptr ) return z;
        }
        else if ( x->getId() == f ) return x; 
      }
      return nullptr;
    }
    void setPosition ( const CRect & np )
    {
      for ( auto x : storage ) x->setPos( make_pair( np, x->getPos().second ) );
      
    }
    void destroy () override 
    {
      for ( auto x : storage )
      {
        x->destroy();
        delete x;
      }
      storage.clear();
    }
    //~CPanel () { this->destroy(); }
  private:
    vector<CParent*> storage;
};

void castNAdd ( vector<CParent*> & v, const CParent & x, const CRect & f, const CRect & s )
{
  if ( x.getType() == 5 )
  {
    auto b = dynamic_cast<const CPanel &> ( x );
    CPanel * a = new CPanel ( b.getId(), f );
    for ( auto u : b.get() ) a->add(*u);
    v.push_back( a );
  }
  else if ( x.getType() == 4 ) 
  {
    auto b = dynamic_cast<const CComboBox &> ( x );
    CComboBox * a = new CComboBox ( b.getId(), f );
    for ( auto u : b.get() ) a->add(u);
    v.push_back( a );
  }
  else if ( x.getType() == 3 ) 
  { 
    auto z = dynamic_cast<const CLabel &> (x);
    v.push_back( new CLabel ( z.getId(), f, s, z.getLabel() ) );
  }
  else if ( x.getType() == 2 )
  {
    auto z = dynamic_cast<const CInput &> ( x );
    v.push_back( new CInput ( z.getId(), f, s, z.getValue() ) );
  }
  else if ( x.getType() == 1 ) 
  {
    auto z = dynamic_cast<const CButton &> ( x );
    v.push_back( new CButton ( z.getId(), f, s, z.getName() ) );
  }
}

class CWindow : public CParent
{
  public:
    CWindow ( int id, const string & title, const CRect & absPos ) : CParent ( id, 0, absPos, CRect(1,1,1,1) ) { this->title = title; }
    CWindow ( const CWindow & x ){ *this = x; }
    CParent & operator * ( ) { return *this; }
    CWindow & operator = ( const CWindow & x )
    {
      destroy();
      storage.clear();
      title = x.getTitle();
      copyPaste( *this, x );
      for ( auto & ptr : x.get() ) this->add( *ptr );
      return *this;
    }
    CWindow & add ( const CParent & x )
    {
      castNAdd( storage, x, pos.first, x.getPos().second );
      return *this;
    }
    void printer( ostream & os, vector<bool> ender, bool end, bool solo ) const override
    {
      end = false;
      os << '[' << getId() << "] Window \"" << title << "\" " << pos.first << "\n";
      for ( unsigned int i = 0; i < storage.size(); ++i )
      {
        if ( i + 1 == storage.size() ) end = true;
        if ( storage[i]->getType() == 5 ) 
        {
          auto z = dynamic_cast< CPanel & >(*(storage[i]));
          z.printer( os,ender, end, false );
        }
        else if ( storage[i]->getType() == 4 ) 
        {
          auto z = dynamic_cast< CComboBox & >(*(storage[i]));
          z.printer( os, ender, end, false );
        }
        else if ( storage[i]->getType() == 3 ) 
        {
          auto z = dynamic_cast< CLabel & >(*(storage[i]));
          z.printer( os,ender, end, false );
        }
        else if ( storage[i]->getType() == 2 ) 
        {
          auto z = dynamic_cast< CInput & >(*(storage[i]));
          z.printer( os, ender, end, false );
        }
        else if ( storage[i]->getType() == 1 ) 
        {
          auto z = dynamic_cast< CButton & >(*(storage[i]));
          z.printer( os, ender, end, false );
        }
      }
        
    }  
    CParent * search ( int f )
    {
      for ( auto & x : storage )
      {
        if ( x->getType() == 5 && f != x->getId() )
        {
          auto a = dynamic_cast<CPanel *>(x);
          CParent * z = a->search( f );
          if ( z != nullptr ) return z;
        }
        else if ( x->getId() == f ) return x; 
      }
      return nullptr;
    }
    void setPosition ( const CRect & np )
    {
      pos.first = CRect ( np.m_X, np.m_Y, np.m_W, np.m_H );
      for ( auto & x : storage )
      {
        if ( x->getType() == 5 )
        {
          CRect oldP ( x->getPos().second.m_X, x->getPos().second.m_Y, x->getPos().second.m_W, x->getPos().second.m_H );
          x->setPos( make_pair( np, oldP ) );
          CRect a ( x->getPos().first.m_W * x->getPos().second.m_X + x->getPos().first.m_X, x->getPos().first.m_H * x->getPos().second.m_Y 
          + x->getPos().first.m_Y,x->getPos().first.m_W * x->getPos().second.m_W, x->getPos().first.m_H * x->getPos().second.m_H);
          auto z = dynamic_cast<CPanel *>( x );
          z->setPosition ( a );
        }
        else
        {
          CRect oldP ( x->getPos().second.m_X, x->getPos().second.m_Y, x->getPos().second.m_W, x->getPos().second.m_H );
          x->setPos( make_pair( np, oldP ) );
        }
      }
    }
    vector<CParent*> get() const { return storage; }
    void destroy () override 
    {
      for ( auto x : storage )
      {
        x->destroy();
        delete x;
      }
      storage.clear();
    }
    string getTitle() const { return title; }
    ~CWindow () { this->destroy(); }
  private:
    vector <CParent*> storage;
    string title;
};

#ifndef __PROGTEST__
template <typename T_>
string toString ( const T_ & x )
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
  assert ( sizeof ( CButton ) - sizeof ( string ) <= sizeof ( CPanel ) - sizeof ( vector<void*> ) );
  assert ( sizeof ( CInput ) - sizeof ( string ) <= sizeof ( CPanel ) - sizeof ( vector<void*> ) );
  assert ( sizeof ( CLabel ) - sizeof ( string ) <= sizeof ( CPanel ) - sizeof ( vector<void*> ) );
  CWindow a ( 0, "Sample window", CRect ( 10, 10, 600, 480 ) );
  a . add ( CButton ( 1, CRect ( 0.1, 0.8, 0.3, 0.1 ), "Ok" ) ) . add ( CButton ( 2, CRect ( 0.6, 0.8, 0.3, 0.1 ), "Cancel" ) );
  a . add ( CLabel ( 10, CRect ( 0.1, 0.1, 0.2, 0.1 ), "Username:" ) );
  a . add ( CInput ( 11, CRect ( 0.4, 0.1, 0.5, 0.1 ), "chucknorris" ) );
  a . add ( CPanel ( 12, CRect ( 0.1, 0.3, 0.8, 0.7 ) ) 
    . add ( CComboBox ( 20, CRect ( 0.1, 0.3, 0.8, 0.1 ) ) 
      . add ( "Karate" ) . add ( "Judo" ) . add ( "Box" ) . add ( "Progtest" ) ) );
  cout << toString( a );
  assert ( toString ( a ) ==
    "[0] Window \"Sample window\" (10,10,600,480)\n"
    "+- [1] Button \"Ok\" (70,394,180,48)\n"
    "+- [2] Button \"Cancel\" (370,394,180,48)\n"
    "+- [10] Label \"Username:\" (70,58,120,48)\n"
    "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
    "+- [12] Panel (70,154,480,336)\n"
    "   +- [20] ComboBox (118,254.8,384,33.6)\n"
    "      +->Karate<\n"
    "      +- Judo\n"
    "      +- Box\n"
    "      +- Progtest\n" );
  CWindow b = a;
  assert ( toString ( *b . search ( 20 ) ) ==
    "[20] ComboBox (118,254.8,384,33.6)\n"
    "+->Karate<\n"
    "+- Judo\n"
    "+- Box\n"
    "+- Progtest\n" );
  assert ( dynamic_cast<CComboBox &> ( *b . search ( 20 ) ) . getSelected () == 0 );
  dynamic_cast<CComboBox &> ( *b . search ( 20 ) ) . setSelected ( 3 );
  assert ( dynamic_cast<CInput &> ( *b . search ( 11 ) ) . getValue () == "chucknorris" );
  dynamic_cast<CInput &> ( *b . search ( 11 ) ) . setValue ( "chucknorris@fit.cvut.cz" );
  CPanel & p = dynamic_cast<CPanel &> ( *b . search ( 12 ) );
  p . add ( CComboBox ( 21, CRect ( 0.1, 0.5, 0.8, 0.1 ) ) . add ( "PA2" ) . add ( "OSY" ) . add ( "Both" ) );
  //cout << toString ( b );
  assert ( toString ( b ) ==
    "[0] Window \"Sample window\" (10,10,600,480)\n"
    "+- [1] Button \"Ok\" (70,394,180,48)\n"
    "+- [2] Button \"Cancel\" (370,394,180,48)\n"
    "+- [10] Label \"Username:\" (70,58,120,48)\n"
    "+- [11] Input \"chucknorris@fit.cvut.cz\" (250,58,300,48)\n"
    "+- [12] Panel (70,154,480,336)\n"
    "   +- [20] ComboBox (118,254.8,384,33.6)\n"
    "   |  +- Karate\n"
    "   |  +- Judo\n"
    "   |  +- Box\n"
    "   |  +->Progtest<\n"
    "   +- [21] ComboBox (118,322,384,33.6)\n"
    "      +->PA2<\n"
    "      +- OSY\n"
    "      +- Both\n" );
  assert ( toString ( a ) ==
    "[0] Window \"Sample window\" (10,10,600,480)\n"
    "+- [1] Button \"Ok\" (70,394,180,48)\n"
    "+- [2] Button \"Cancel\" (370,394,180,48)\n"
    "+- [10] Label \"Username:\" (70,58,120,48)\n"
    "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
    "+- [12] Panel (70,154,480,336)\n"
    "   +- [20] ComboBox (118,254.8,384,33.6)\n"
    "      +->Karate<\n"
    "      +- Judo\n"
    "      +- Box\n"
    "      +- Progtest\n" );
  //cout << toString ( p );
  assert ( toString ( p ) ==
    "[12] Panel (70,154,480,336)\n"
    "+- [20] ComboBox (118,254.8,384,33.6)\n"
    "|  +- Karate\n"
    "|  +- Judo\n"
    "|  +- Box\n"
    "|  +->Progtest<\n"
    "+- [21] ComboBox (118,322,384,33.6)\n"
    "   +->PA2<\n"
    "   +- OSY\n"
    "   +- Both\n" );
  b . setPosition ( CRect ( 20, 30, 640, 520 ) );
  assert ( toString ( b ) ==
    "[0] Window \"Sample window\" (20,30,640,520)\n"
    "+- [1] Button \"Ok\" (84,446,192,52)\n"
    "+- [2] Button \"Cancel\" (404,446,192,52)\n"
    "+- [10] Label \"Username:\" (84,82,128,52)\n"
    "+- [11] Input \"chucknorris@fit.cvut.cz\" (276,82,320,52)\n"
    "+- [12] Panel (84,186,512,364)\n"
    "   +- [20] ComboBox (135.2,295.2,409.6,36.4)\n"
    "   |  +- Karate\n"
    "   |  +- Judo\n"
    "   |  +- Box\n"
    "   |  +->Progtest<\n"
    "   +- [21] ComboBox (135.2,368,409.6,36.4)\n"
    "      +->PA2<\n"
    "      +- OSY\n"
    "      +- Both\n" );
  p . add ( p );
  assert ( toString ( p ) ==
    "[12] Panel (84,186,512,364)\n"
    "+- [20] ComboBox (135.2,295.2,409.6,36.4)\n"
    "|  +- Karate\n"
    "|  +- Judo\n"
    "|  +- Box\n"
    "|  +->Progtest<\n"
    "+- [21] ComboBox (135.2,368,409.6,36.4)\n"
    "|  +->PA2<\n"
    "|  +- OSY\n"
    "|  +- Both\n"
    "+- [12] Panel (135.2,295.2,409.6,254.8)\n"
    "   +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
    "   |  +- Karate\n"
    "   |  +- Judo\n"
    "   |  +- Box\n"
    "   |  +->Progtest<\n"
    "   +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
    "      +->PA2<\n"
    "      +- OSY\n"
    "      +- Both\n" );
  p . add ( p );
  assert ( toString ( p ) ==
    "[12] Panel (84,186,512,364)\n"
    "+- [20] ComboBox (135.2,295.2,409.6,36.4)\n"
    "|  +- Karate\n"
    "|  +- Judo\n"
    "|  +- Box\n"
    "|  +->Progtest<\n"
    "+- [21] ComboBox (135.2,368,409.6,36.4)\n"
    "|  +->PA2<\n"
    "|  +- OSY\n"
    "|  +- Both\n"
    "+- [12] Panel (135.2,295.2,409.6,254.8)\n"
    "|  +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
    "|  |  +- Karate\n"
    "|  |  +- Judo\n"
    "|  |  +- Box\n"
    "|  |  +->Progtest<\n"
    "|  +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
    "|     +->PA2<\n"
    "|     +- OSY\n"
    "|     +- Both\n"
    "+- [12] Panel (135.2,295.2,409.6,254.8)\n"
    "   +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
    "   |  +- Karate\n"
    "   |  +- Judo\n"
    "   |  +- Box\n"
    "   |  +->Progtest<\n"
    "   +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
    "   |  +->PA2<\n"
    "   |  +- OSY\n"
    "   |  +- Both\n"
    "   +- [12] Panel (176.16,371.64,327.68,178.36)\n"
    "      +- [20] ComboBox (208.928,425.148,262.144,17.836)\n"
    "      |  +- Karate\n"
    "      |  +- Judo\n"
    "      |  +- Box\n"
    "      |  +->Progtest<\n"
    "      +- [21] ComboBox (208.928,460.82,262.144,17.836)\n"
    "         +->PA2<\n"
    "         +- OSY\n"
    "         +- Both\n" );
  p . add ( p );
  assert ( toString ( p ) ==
    "[12] Panel (84,186,512,364)\n"
    "+- [20] ComboBox (135.2,295.2,409.6,36.4)\n"
    "|  +- Karate\n"
    "|  +- Judo\n"
    "|  +- Box\n"
    "|  +->Progtest<\n"
    "+- [21] ComboBox (135.2,368,409.6,36.4)\n"
    "|  +->PA2<\n"
    "|  +- OSY\n"
    "|  +- Both\n"
    "+- [12] Panel (135.2,295.2,409.6,254.8)\n"
    "|  +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
    "|  |  +- Karate\n"
    "|  |  +- Judo\n"
    "|  |  +- Box\n"
    "|  |  +->Progtest<\n"
    "|  +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
    "|     +->PA2<\n"
    "|     +- OSY\n"
    "|     +- Both\n"
    "+- [12] Panel (135.2,295.2,409.6,254.8)\n"
    "|  +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
    "|  |  +- Karate\n"
    "|  |  +- Judo\n"
    "|  |  +- Box\n"
    "|  |  +->Progtest<\n"
    "|  +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
    "|  |  +->PA2<\n"
    "|  |  +- OSY\n"
    "|  |  +- Both\n"
    "|  +- [12] Panel (176.16,371.64,327.68,178.36)\n"
    "|     +- [20] ComboBox (208.928,425.148,262.144,17.836)\n"
    "|     |  +- Karate\n"
    "|     |  +- Judo\n"
    "|     |  +- Box\n"
    "|     |  +->Progtest<\n"
    "|     +- [21] ComboBox (208.928,460.82,262.144,17.836)\n"
    "|        +->PA2<\n"
    "|        +- OSY\n"
    "|        +- Both\n"
    "+- [12] Panel (135.2,295.2,409.6,254.8)\n"
    "   +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
    "   |  +- Karate\n"
    "   |  +- Judo\n"
    "   |  +- Box\n"
    "   |  +->Progtest<\n"
    "   +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
    "   |  +->PA2<\n"
    "   |  +- OSY\n"
    "   |  +- Both\n"
    "   +- [12] Panel (176.16,371.64,327.68,178.36)\n"
    "   |  +- [20] ComboBox (208.928,425.148,262.144,17.836)\n"
    "   |  |  +- Karate\n"
    "   |  |  +- Judo\n"
    "   |  |  +- Box\n"
    "   |  |  +->Progtest<\n"
    "   |  +- [21] ComboBox (208.928,460.82,262.144,17.836)\n"
    "   |     +->PA2<\n"
    "   |     +- OSY\n"
    "   |     +- Both\n"
    "   +- [12] Panel (176.16,371.64,327.68,178.36)\n"
    "      +- [20] ComboBox (208.928,425.148,262.144,17.836)\n"
    "      |  +- Karate\n"
    "      |  +- Judo\n"
    "      |  +- Box\n"
    "      |  +->Progtest<\n"
    "      +- [21] ComboBox (208.928,460.82,262.144,17.836)\n"
    "      |  +->PA2<\n"
    "      |  +- OSY\n"
    "      |  +- Both\n"
    "      +- [12] Panel (208.928,425.148,262.144,124.852)\n"
    "         +- [20] ComboBox (235.142,462.604,209.715,12.4852)\n"
    "         |  +- Karate\n"
    "         |  +- Judo\n"
    "         |  +- Box\n"
    "         |  +->Progtest<\n"
    "         +- [21] ComboBox (235.142,487.574,209.715,12.4852)\n"
    "            +->PA2<\n"
    "            +- OSY\n"
    "            +- Both\n" );
  assert ( toString ( b ) ==
    "[0] Window \"Sample window\" (20,30,640,520)\n"
    "+- [1] Button \"Ok\" (84,446,192,52)\n"
    "+- [2] Button \"Cancel\" (404,446,192,52)\n"
    "+- [10] Label \"Username:\" (84,82,128,52)\n"
    "+- [11] Input \"chucknorris@fit.cvut.cz\" (276,82,320,52)\n"
    "+- [12] Panel (84,186,512,364)\n"
    "   +- [20] ComboBox (135.2,295.2,409.6,36.4)\n"
    "   |  +- Karate\n"
    "   |  +- Judo\n"
    "   |  +- Box\n"
    "   |  +->Progtest<\n"
    "   +- [21] ComboBox (135.2,368,409.6,36.4)\n"
    "   |  +->PA2<\n"
    "   |  +- OSY\n"
    "   |  +- Both\n"
    "   +- [12] Panel (135.2,295.2,409.6,254.8)\n"
    "   |  +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
    "   |  |  +- Karate\n"
    "   |  |  +- Judo\n"
    "   |  |  +- Box\n"
    "   |  |  +->Progtest<\n"
    "   |  +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
    "   |     +->PA2<\n"
    "   |     +- OSY\n"
    "   |     +- Both\n"
    "   +- [12] Panel (135.2,295.2,409.6,254.8)\n"
    "   |  +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
    "   |  |  +- Karate\n"
    "   |  |  +- Judo\n"
    "   |  |  +- Box\n"
    "   |  |  +->Progtest<\n"
    "   |  +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
    "   |  |  +->PA2<\n"
    "   |  |  +- OSY\n"
    "   |  |  +- Both\n"
    "   |  +- [12] Panel (176.16,371.64,327.68,178.36)\n"
    "   |     +- [20] ComboBox (208.928,425.148,262.144,17.836)\n"
    "   |     |  +- Karate\n"
    "   |     |  +- Judo\n"
    "   |     |  +- Box\n"
    "   |     |  +->Progtest<\n"
    "   |     +- [21] ComboBox (208.928,460.82,262.144,17.836)\n"
    "   |        +->PA2<\n"
    "   |        +- OSY\n"
    "   |        +- Both\n"
    "   +- [12] Panel (135.2,295.2,409.6,254.8)\n"
    "      +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
    "      |  +- Karate\n"
    "      |  +- Judo\n"
    "      |  +- Box\n"
    "      |  +->Progtest<\n"
    "      +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
    "      |  +->PA2<\n"
    "      |  +- OSY\n"
    "      |  +- Both\n"
    "      +- [12] Panel (176.16,371.64,327.68,178.36)\n"
    "      |  +- [20] ComboBox (208.928,425.148,262.144,17.836)\n"
    "      |  |  +- Karate\n"
    "      |  |  +- Judo\n"
    "      |  |  +- Box\n"
    "      |  |  +->Progtest<\n"
    "      |  +- [21] ComboBox (208.928,460.82,262.144,17.836)\n"
    "      |     +->PA2<\n"
    "      |     +- OSY\n"
    "      |     +- Both\n"
    "      +- [12] Panel (176.16,371.64,327.68,178.36)\n"
    "         +- [20] ComboBox (208.928,425.148,262.144,17.836)\n"
    "         |  +- Karate\n"
    "         |  +- Judo\n"
    "         |  +- Box\n"
    "         |  +->Progtest<\n"
    "         +- [21] ComboBox (208.928,460.82,262.144,17.836)\n"
    "         |  +->PA2<\n"
    "         |  +- OSY\n"
    "         |  +- Both\n"
    "         +- [12] Panel (208.928,425.148,262.144,124.852)\n"
    "            +- [20] ComboBox (235.142,462.604,209.715,12.4852)\n"
    "            |  +- Karate\n"
    "            |  +- Judo\n"
    "            |  +- Box\n"
    "            |  +->Progtest<\n"
    "            +- [21] ComboBox (235.142,487.574,209.715,12.4852)\n"
    "               +->PA2<\n"
    "               +- OSY\n"
    "               +- Both\n" );
  assert ( toString ( a ) ==
    "[0] Window \"Sample window\" (10,10,600,480)\n"
    "+- [1] Button \"Ok\" (70,394,180,48)\n"
    "+- [2] Button \"Cancel\" (370,394,180,48)\n"
    "+- [10] Label \"Username:\" (70,58,120,48)\n"
    "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
    "+- [12] Panel (70,154,480,336)\n"
    "   +- [20] ComboBox (118,254.8,384,33.6)\n"
    "      +->Karate<\n"
    "      +- Judo\n"
    "      +- Box\n"
    "      +- Progtest\n" );
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
