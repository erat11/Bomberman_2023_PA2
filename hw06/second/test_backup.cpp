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
    CParent ( const CParent & x ) : id ( x.getId() ) ,type ( x.getType() ), 
    pos ( x.getPos().first, x.getPos().second ) {}
    virtual CParent & operator = ( const CParent & x )
    {
      return *this;
    }
    friend ostream & operator << ( ostream & os, const CParent & x )
    {
      x.print( os );
      return os;
    }
    virtual ~CParent () {}
    virtual void print( ostream & os ) const { }
    int getId () const { return id; }
    int getType ( ) const { return type; }
    void setId ( int i ) { id = i; }
    void setType ( int i ) { type = i; }
    pair<CRect, CRect> getPos () const { return pos; }
    void setPos ( const pair<CRect, CRect> & x ) { pos = make_pair ( CRect ( x.first.m_X, x.first.m_Y, x.first.m_W, x.first.m_H ), 
                                                                     CRect ( x.second.m_X, x.second.m_Y, x.second.m_W, x.second.m_H ) ); }
  protected:
    int id, type; // 0 window, 1 button, 2 input, 3 label, 4 combobox
    pair<CRect, CRect> pos; //parent, relativePos
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
    CComboBox ( int id, const CRect & relPos ) : CParent ( id, 4, relPos, CRect(1,1,1,1) ) { idx = 0; }
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
    void print ( ostream & os ) const override
    {
      smallPrinter ( os, *this, ""s, "ComboBox"s, false );
      for ( unsigned int i = 0; i < offer.size(); ++i )
        if ( i == idx )
          os <<"+->" << offer[i] << "<" << endl;
        else
          os <<"+- " << offer[i] << endl;
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
    CInput ( int id, const CRect & relPos, const string & value ) : CParent ( id, 2, relPos, CRect(1,1,1,1) ) { setValue( value ); }
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
    void print( ostream & os ) const  override { smallPrinter ( os, *this, value, "Input"s, true ); }
  private:
    string value;

};

class CButton : public CParent
{
  public:
    CButton ( const CButton & x ) { *this = x; }
    CButton ( int id, const CRect & relPos, const string & name ) : CParent ( id, 1, relPos, CRect(1,1,1,1) ) { this->name = name; }
    CButton ( int id, const CRect & parPos, const CRect & relPos, const string & name ) : CParent ( id, 1, parPos, relPos ) { this->name = name; }
    string getName () const { return name; }
    CButton & operator = ( const CButton & x )
    {
      copyPaste( *this, x );
      name = x.getName();
      return *this;
    }
    void print( ostream & os ) const override { smallPrinter ( os, *this, name, "Button"s, true ); }
  private:
    string name;
};

class CLabel : public CParent
{
  public:
    CLabel ( int id, const CRect & relPos, const string & label ) : CParent ( id, 3, relPos, CRect(1,1,1,1) ) { this->label = label; }
    CLabel ( int id, const CRect & parPos, const CRect & relPos, const string & label ) : CParent ( id, 3, parPos, relPos ) { this->label = label; }
    CLabel ( const CLabel & x ) { *this = x; }
    string getLabel() const { return label; }
    CLabel & operator = ( const CLabel & x )
    {
      copyPaste( *this, x );
      label = x.getLabel();
      return *this;
    }
    void print( ostream & os ) const override { smallPrinter ( os, *this, label, "Label"s, true ); }
  private:
    string label;
};

class CPanel : public CParent
{
  public:
    CPanel ( int id, const CRect & relPos ) : CParent ( id, 5, relPos, CRect(1,1,1,1) ) { }
    CPanel ( int id, const CRect & parPos, const CRect & relPos ) : CParent ( id, 5, parPos, relPos ) { }
    CPanel & operator = ( const CPanel & x )
    {
      copyPaste( *this, x );
      for ( const auto & z : x.get() ) add ( z );
      return *this;
    }
    CPanel & add ( const CParent & x )
    {
      if ( x.getType() == 4 ) 
      {
        CComboBox a = CComboBox ( x.getId(), this->getPos().first, x.getPos().second );
        const CComboBox & b = dynamic_cast<const CComboBox &> ( x );
        for ( const auto & c : b.get() ) a.add( c );
        storage.push_back( a );
      }
      else if ( x.getType() == 3 ) 
      { 
        auto z = dynamic_cast<const CLabel &> (x);
        storage.push_back( CLabel ( z.getId(), this->getPos().first, x.getPos().second, z.getLabel() ) );
      }
      else if ( x.getType() == 2 )
      {
        auto z = dynamic_cast<const CInput &> ( x );
        storage.push_back( CInput ( z.getId(), this->getPos().first, x.getPos().second, z.getValue() ) );
      }
      else if ( x.getType() == 1 ) 
      {
        auto z = dynamic_cast<const CButton &> ( x );
        storage.push_back( CButton ( z.getId(), this->getPos().first, x.getPos().second, z.getName() ) );
      }
      else
      { 
        CPanel a ( x.getId(), this->getPos().first, x.getPos().second );
        const CPanel & b = dynamic_cast<const CPanel &> ( x );
        for ( const auto & c : b.get() ) a.add( c );
        storage.push_back( a );
      }
      return *this;
    } 
    void print ( ostream & os ) const override
    {
      smallPrinter ( os, *this, ""s, "Panel"s, false );
      for ( unsigned int i = 0; i < storage.size(); ++i ) os << storage[i];
    }  
    vector<CParent> get() const { return storage; }
  private:
    vector<CParent> storage;
};

class CWindow : public CParent
{
  public:
    CWindow ( int id, const string & title, const CRect & absPos ) : CParent ( id, 0, absPos, CRect(1,1,1,1) ) { this->title = title; }
    CWindow ( const CWindow & x ){ *this = x; }
    CParent & operator * ( ) { return *this; }
    CWindow & operator = ( const CWindow & x )
    {
      for ( auto & x : storage ) delete x;
      storage.clear();
      title = x.getTitle();
      copyPaste( *this, x );
      for ( auto & ptr : x.get() ) this->add( *ptr );
      return *this;
    }
    CWindow & add ( const CParent & x )
    {
        if ( x.getType() == 4 ) 
        {
          CComboBox * a = new CComboBox ( x.getId(), this->getPos().first, x.getPos().second );
          auto b = dynamic_cast<const CComboBox &> ( x );
          for ( auto & c : b.get() ) a->add( c );
          storage.push_back( a );
        }
        else if ( x.getType() == 3 ) 
        { 
          auto z = dynamic_cast<const CLabel &> (x);
          storage.push_back( new CLabel ( z.getId(), this->getPos().first, x.getPos().second, z.getLabel() ) );
        }
        else if ( x.getType() == 2 )
        {
          auto z = dynamic_cast<const CInput &> ( x );
          storage.push_back( new CInput ( z.getId(), this->getPos().first, x.getPos().second, z.getValue() ) );
        }
        else if ( x.getType() == 1 ) 
        {
          auto z = dynamic_cast<const CButton &> ( x );
          storage.push_back( new CButton ( z.getId(), this->getPos().first, x.getPos().second, z.getName() ) );
        }
        else
        {
          CPanel  * a = new CPanel ( x.getId(), this->getPos().first, x.getPos().second );
          auto b = dynamic_cast< const CPanel & > ( x );
          for ( const auto & c : b.get() ) a->add( c );
          storage.push_back( a );
        }
        return *this;
    }
    void print( ostream & os ) const override
    {
      char c = '|';
      os << '[' << getId() << "] Window \"" << title << "\" " << pos.first << "\n";
      string s = "   ";
      for ( unsigned int i = 0; i < storage.size(); ++i )
        if ( storage[i]->getType() == 4 ) 
        {
          os << "+- ";
          auto z = dynamic_cast< CComboBox & >(*(storage[i]));
          smallPrinter ( os, z, ""s, "ComboBox"s, false );
          if ( i + 1 == storage.size() ) c = ' ';
          for ( unsigned int i = 0; i < z.get().size(); ++i )
          {
              if ( i == z.getSelected() )
                os << c << "  +->" << z.get()[i] << "<" << endl;
              else if ( i != z.getSelected()  )
                os << c << "  +- " << z.get()[i] << endl;
          }
        }
        else if ( storage[i]->getType() == 5 )
        {
          os << "+- ";
          auto z = dynamic_cast< CPanel & >(*(storage[i]));
          smallPrinter ( os, z, ""s, "Panel"s, false );
          for ( auto & x : z.get() )
                os << c << "  +- " << x << endl;
        }
        else os << "+- " << *(storage[i]);
        
    }  
    CParent * search ( int f )
    {
      for ( auto & x : storage ) if ( x->getId() == f ) return x; 
      return nullptr;
    }
    void setPosition ( const CRect & np )
    {
      pos.first = CRect ( np.m_X, np.m_Y, np.m_W, np.m_H );
      for ( auto & x : storage )
      {
        CRect oldP ( x->getPos().second.m_X, x->getPos().second.m_Y, x->getPos().second.m_W, x->getPos().second.m_H );
        x->setPos( make_pair( np, oldP ) );
      }
    }
    vector<CParent*> get() const { return storage; }
    ~CWindow (  ) override { for ( auto y : storage ) delete y; }
    string getTitle() const { return title; }
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
  CPanel a( 12, CRect ( 0.1, 0.3, 0.8, 0.7 ) );
  a 
    . add ( CComboBox ( 20, CRect ( 0.1, 0.3, 0.8, 0.1 ) ) 
      . add ( "Karate" ) . add ( "Judo" ) . add ( "Box" ) . add ( "Progtest" ) );
  /*CWindow a ( 0, "Sample window", CRect ( 10, 10, 600, 480 ) );
  a . add ( CButton ( 1, CRect ( 0.1, 0.8, 0.3, 0.1 ), "Ok" ) ) . add ( CButton ( 2, CRect ( 0.6, 0.8, 0.3, 0.1 ), "Cancel" ) );
  a . add ( CLabel ( 10, CRect ( 0.1, 0.1, 0.2, 0.1 ), "Username:" ) );
  a . add ( CInput ( 11, CRect ( 0.4, 0.1, 0.5, 0.1 ), "chucknorris" ) );
  a . add ( CPanel ( 12, CRect ( 0.1, 0.3, 0.8, 0.7 ) ) 
    . add ( CComboBox ( 20, CRect ( 0.1, 0.3, 0.8, 0.1 ) ) 
      . add ( "Karate" ) . add ( "Judo" ) . add ( "Box" ) . add ( "Progtest" ) ) );*/
  cout << a;
  /*assert ( toString ( a ) ==
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
    "      +- Progtest\n" );*/
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
