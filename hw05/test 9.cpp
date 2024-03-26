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


// element bude nejaka trida z ktere budou vsechny prvky okna dedit
// vsechny prvky musi obsahovat id, velikost a obsah, tudiz bude stacit je vytvorit v AElemetn
// pravdepodobne nebude vhodne pouzit abstraktni tridu, protoze v ni chceme ukladat data
class CElement
{
public:
    //TODO je tohle oka? Kdyz mam CCOmboBox, tak ten nema primo prvek content - lepsi pretizit u CComboBoxu, nebo odebrat content element?
     CElement (int id,  const CRect &  a)
        :m_Id(id),
         m_Abs(a)
         {}
     virtual ~CElement() = default;
     virtual string Name() const = 0;
     virtual shared_ptr<CElement> Clone () const = 0;
     virtual ostream & Print (ostream & oss, int level = 0) const = 0;
     virtual void setAbsolute(const CRect & window ) = 0;
     int id()
     {
         return m_Id;
     }

protected:
    int m_Id;
    CRect m_Abs;
};
//tirida vsech prvku, ktere se budou pridavat do okna
class CSubwindow : public CElement
{
public:
    CSubwindow(int id, const CRect & a)
    :CElement(id,a),
     m_Relative(a)
    {}

    void setAbsolute (const CRect & window) override
    {
        m_Abs.m_X = window.m_X + m_Relative.m_X * window.m_W;
        m_Abs.m_Y = window.m_Y + m_Relative.m_Y * window.m_H;
        m_Abs.m_W = window.m_W * m_Relative.m_W;
        m_Abs.m_H = window.m_H * m_Relative.m_H;
    }
    ~CSubwindow() override = default;
protected:
    CRect m_Relative;
};

class CWindow : public CElement
{
  public:
    CWindow ( int id,const string & title,const CRect & absPos )
        :CElement(id,absPos),
         m_Name(title)
         {}
     CWindow (const CWindow & a )
        :CElement(a.m_Id,a.m_Abs),
         m_Name(a.m_Name)
     {
        for (size_t i = 0 ; i < a.m_Elements.size() ; i++)
        {
            m_Elements.push_back(a.m_Elements[i]->Clone());
        }
     }
     CWindow & operator= (CWindow a)
     {
        swap(m_Name, a.m_Name);
        swap(m_Id, a.m_Id);
        swap(m_Elements,a.m_Elements);
        swap(m_Abs,a.m_Abs);
        return *this;
     }
    ~CWindow() override = default;
    shared_ptr<CElement> Clone () const override
    {
        return make_shared<CWindow>(*this);
    }
    string Name () const override
    {
        return "Window";
    }
    void setAbsolute (const CRect & a) override
    {
      return;
    }
    CWindow & add ( const CSubwindow & a )
    {
        shared_ptr<CElement> ptr = a.Clone();
        m_Elements.push_back(ptr);
        m_Elements[m_Elements.size() - 1]->setAbsolute(m_Abs);
        return *this;
    }
    ostream & Print (ostream & oss, int level = 0) const override
    {
        oss << "[" << m_Id << "] " << Name() << " \"" << m_Name << "\" "<< m_Abs << endl;
        for (size_t i = 0 ; i < m_Elements.size(); i++) {
            if (i != m_Elements.size() - 1) {
                m_Elements[i]->Print(oss, 2);
                continue;
            }
            m_Elements[i]->Print(oss,1);

        }
        return oss;
    }

    shared_ptr<CElement> search (int id) const
    {
        for(size_t i = 0 ; i < m_Elements.size() ; i++)
        {
            if(id == m_Elements[i]->id())
            {
                return m_Elements[i];
            }
        }
        return nullptr;
    }
    void setPosition (const CRect & a)
    {
        m_Abs.m_X = a.m_X;
        m_Abs.m_Y = a.m_Y;
        m_Abs.m_W = a.m_W;
        m_Abs.m_H = a.m_H;
        for (size_t i = 0 ; i < m_Elements.size() ; i++)
        {
            m_Elements[i]->setAbsolute(a);
        }
    }
protected:
    vector<shared_ptr<CElement> > m_Elements;
    string m_Name;
};
class CPrimitive : public CSubwindow
{
public:
    CPrimitive(int id, const CRect & a , string content)
    :CSubwindow(id,a),
     m_Content(content)
     {}
     ~CPrimitive () override = default;
     ostream & Print(ostream & oss, int level = 0) const override
     {
         string prefix;
         if ( level == 1 || level == 2)
             prefix = "+- ";
         else
             prefix = "";
        oss << prefix << "[" << m_Id << "] " << Name() << " \"" << m_Content << "\" " << m_Abs << endl;
        return oss;
     }

protected:
    string m_Content;
};
class CButton : public CPrimitive
{
  public:
    CButton ( int id,const CRect & relPos,const string & name )
        :CPrimitive(id,relPos, name)
        {}
    ~CButton () override = default;
    string Name () const override
    {
        return "Button";
    }
    shared_ptr<CElement> Clone() const override
    {
        return make_shared<CButton> (*this);
    }
};

class CInput : public CPrimitive
{
  public:
    CInput ( int id,const CRect & relPos,const string & value )
        :CPrimitive(id,relPos,value)
         {}
     ~CInput() override = default;
    string Name () const override
    {
        return "Input";
    }
    void setValue(const string & a)
    {
        m_Content = a;
    }
    string getValue()
    {
        return m_Content;
    }
    shared_ptr<CElement> Clone() const override
    {
        return make_shared<CInput>(*this);
    }
};
class CLabel : public CPrimitive
{
  public:
    CLabel ( int id,const CRect & relPos,const string & label )
        :CPrimitive(id,relPos,label)
        {}
    ~CLabel() override = default;
    shared_ptr<CElement> Clone() const override
    {
        return make_shared<CLabel>(*this);
    }
    string Name () const override
    {
        return "Label";
    }
};

//TODO u comboboxy je potreba zmenit print metodu - musi printovat i elementy v seznamu
class CComboBox : public CSubwindow
{
  public:
    CComboBox ( int id, const CRect & relPos )
        :CSubwindow(id,relPos),
         m_Selected(0)
         {}
     ~CComboBox() override = default;
    shared_ptr<CElement> Clone() const override
    {
        return make_shared<CComboBox>(*this);
    }
     string Name () const override
    {
        return "ComboBox";
    }
    ostream & Print (ostream & oss, int level = 0) const override
    {
        string prefix1, prefix2;
        if (level == 1)
        {
            prefix1 = "+- ";
            prefix2 = "   +-";
        }
        else if (level == 2)
        {
            prefix1 = "+- ";
            prefix2 = "|  +-";
        }
        else {
            prefix1 = "";
            prefix2 = "+-";
        }
        oss << prefix1 << "[" << m_Id << "] " << Name() << " " << m_Abs << endl;
        for (size_t i = 0 ; i < m_Data.size(); i++)
        {
            if ( i == m_Selected) {
                oss << prefix2 << ">" << m_Data[i] << "<" << endl;
                continue;
            }
            oss << prefix2 << " "<< m_Data[i] << endl;
        }
        return oss;
    }
    CComboBox & add (const string & item )
    {
        m_Data.push_back(item);
        return (*this);
    }
    void setSelected(int x)
    {
        m_Selected = x;
    }
    int getSelected()
    {
        return m_Selected;
    }
protected:
    vector<string> m_Data;
    size_t m_Selected;
};
ostream & operator << ( ostream & oss ,  const CElement & elem)
{
    elem.Print(oss);
    return oss;
}



#ifndef __PROGTEST__
template <typename _T>
string toString ( const _T & x )
{
  ostringstream oss;
  oss << x;
  return oss . str ();
}
/*
 * Tak prvni odhad bude si vytvorit nejakou tridu CElement
 * - ta bude obsahovat virtualni metodu add, urcite prvek id, metoda print??
 *
 */
int main ( void )
{
  assert ( sizeof ( CInput ) - sizeof ( string ) < sizeof ( CComboBox ) - sizeof ( vector<string> ) );
  assert ( sizeof ( CButton ) - sizeof ( string ) < sizeof ( CComboBox ) - sizeof ( vector<string> ) );
  assert ( sizeof ( CLabel ) - sizeof ( string ) < sizeof ( CComboBox ) - sizeof ( vector<string> ) );

  CWindow a ( 0, "Sample window", CRect ( 10, 10, 600, 480 ) );
  a . add ( CButton ( 1, CRect ( 0.1, 0.8, 0.3, 0.1 ), "Ok" ) ) . add ( CButton ( 2, CRect ( 0.6, 0.8, 0.3, 0.1 ), "Cancel" ) );
  a . add ( CLabel ( 10, CRect ( 0.1, 0.1, 0.2, 0.1 ), "Username:" ) );
  a . add ( CInput ( 11, CRect ( 0.4, 0.1, 0.5, 0.1 ), "chucknorris" ) );
  a . add ( CComboBox ( 20, CRect ( 0.1, 0.3, 0.8, 0.1 ) ) . add ( "Karate" ) . add ( "Judo" ) . add ( "Box" ) . add ( "Progtest" ) );
  cout << toString(a);
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
  cout << *b.search (20) ;
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
 cout << a;
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
