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
    void print( ostream & os ) const  override { smallPrinter ( os, *this, value, "Input"s, true ); }
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
    void print( ostream & os ) const override { smallPrinter ( os, *this, name, "Button"s, true ); }
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
    void print( ostream & os ) const override { smallPrinter ( os, *this, label, "Label"s, true ); }
  private:
    string label;
};

class CWindow : public CParent
{
  public:
    CWindow ( int id, const string & title, const CRect & absPos ) : CParent ( id, 0, absPos, absPos ) { this->title = title; }
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
          const CComboBox & b = dynamic_cast<const CComboBox &> ( x );
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
        else storage.push_back( new CWindow ( dynamic_cast<const CWindow &> ( x ) ) );
        return *this;
    }
    void print( ostream & os ) const override
    {
      char c = '|';
      os << '[' << getId() << "] Window \"" << title << "\" " << pos.first << "\n";
      string s = "   ";
      for ( unsigned int i = 0; i < storage.size(); ++i )
        if ( storage[i]->getType() != 4 ) os << "+- " << *(storage[i]);
        else
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