#ifndef __PROGTEST__
#include <cstring>
#include <__nullptr>
#include <iostream>

using namespace std;

class CLinkedSetTester;
#endif /* __PROGTEST__ */


class CLinkedSet
{
private:
    struct CNode
    {
        CNode * m_Next;
        CNode * m_Prev;
        char * m_Data;
        const char * Value () const
        {
            return m_Data;
        }
    };
    CNode * m_Begin;
    CNode * m_End;


public:
    // default constructor
    CLinkedSet() {
        m_Begin = nullptr;
        m_End = nullptr;
    }
    // copy constructor
    CLinkedSet( const CLinkedSet & a)
    {
        m_Begin = nullptr;
        m_End = nullptr;
        //potreba zkopirovat begin, value, m_Next
        CNode * ptr = a.m_Begin;
        while (ptr)
        {
            this->Insert(ptr->Value());
            ptr = ptr->m_Next;
        }
    }

    ~CLinkedSet()
    {
       CNode * tmp = m_Begin;
       while ( tmp )
       {
           CNode * del = tmp;
           tmp = tmp->m_Next;
           delete [] del->m_Data;
           delete(del);
       }
    }

    void push_front(CNode *& insert)
    {
        CNode * tmp = m_Begin;
        m_Begin = insert;
        m_Begin->m_Next = tmp;
        if ( tmp == nullptr)
        {
            m_End = m_Begin;
            return;
        }
        tmp->m_Prev = m_Begin;
    }


    void pop_front ()
    {
        if (m_Begin == nullptr )
            return;
        if (m_Begin == m_End)
        {
            delete [] m_Begin->m_Data;
            delete m_Begin;
            m_Begin = nullptr;
            m_End = nullptr;
            return;
        }
        CNode * tmp = m_Begin;
        m_Begin = m_Begin -> m_Next;
        m_Begin -> m_Prev = nullptr;
        delete [] tmp->m_Data;
        delete tmp;
    }
    void push_back(CNode * insert)
    {
        CNode * tmp = m_End;
        m_End = insert;
        insert -> m_Prev = tmp;
        tmp->m_Next = insert;
        m_End->m_Next = nullptr;
    }
    void pop_back ()
    {
        if (m_Begin == m_End) {
            pop_front();
            return;
        }
        CNode * tmp = m_End;
        m_End = m_End -> m_Prev;
        m_End -> m_Next = nullptr;
        delete [] tmp->m_Data;
        delete tmp;
    }
    void push (CNode *& insert)
    {
        if ( this -> Empty())
        {
            push_front(insert);
            return;
        }
        if (strcmp(insert->Value(), m_Begin->Value()) <= 0)
        {
            push_front(insert);
            return;
        }
        if (strcmp(insert->Value(),m_End->Value()) >= 0)
        {
            push_back(insert);
            return;
        }
        CNode * tmp = m_Begin;
        for (;tmp; tmp = tmp -> m_Next )
        {
            if ( strcmp(insert->Value(), tmp->Value()) <= 0 )
            {
                insert -> m_Next = tmp;
                insert -> m_Prev = tmp -> m_Prev;
                tmp -> m_Prev -> m_Next = insert;
                tmp ->  m_Prev = insert;
                return;
            }
        }
    }
    void swap (CLinkedSet * other)
    {
        CNode * thisBegin = m_Begin;
        CNode * thisEnd = m_End;
        m_Begin = other->m_Begin;
        m_End = other -> m_End;
        other ->m_Begin = thisBegin;
        other -> m_End = thisEnd;
    }

    CLinkedSet & operator= ( CLinkedSet other)
    {
        if (this != & other)
            swap(&other);
        return *this;
    }
    bool Insert ( const char * value )
    {
        if (Contains(value))
            return false;
        CNode * tmp = new CNode();
        tmp -> m_Data = new char [strlen(value) + 1];
        memcpy(tmp->m_Data, value, strlen(value) + 1 );
        push(tmp);
        return true;
    }
    bool Remove ( const char * value )
    {
        if (this->Empty())
            return false;
        if (! strcmp(value, m_Begin->Value()))
        {
            pop_front();
            return true;
        }
        if (!strcmp(value, m_End->Value()))
        {
            pop_back();
            return true;
        }
        CNode * tmp = m_Begin;
        for (; tmp ; tmp = tmp -> m_Next )
        {
            if ( ! strcmp( value, tmp->Value()))
            {
                tmp -> m_Prev -> m_Next = tmp ->m_Next;
                tmp -> m_Next -> m_Prev = tmp -> m_Prev;
                delete [] tmp -> m_Data;
                delete tmp;
                return true;
            }
        }
        return false;
    }
    bool Empty () const
    {
        return m_Begin == nullptr;
    }
    size_t Size () const
    {
        size_t count = 0;
        CNode * tmp = m_Begin;
        while ( tmp )
        {
            count++;
            tmp = tmp->m_Next;
        }
        return count;
    }
    bool Contains ( const char * value ) const
    {
        for ( CNode * tmp = m_Begin ; tmp ; tmp = tmp->m_Next)
        {
            if (!strcmp(value, tmp->Value()))
                return true;
        }
        return false;
    }
    void printList()
    {
        CNode * tmp = m_Begin;
        cout << "NULL";
        while ( tmp )
        {
            cout << " <- "<< tmp->Value();
            tmp = tmp -> m_Next;
        }
        cout << " <- NULL" << endl;
    }
    friend class ::CLinkedSetTester;
};

#ifndef __PROGTEST__
#include <cassert>

struct CLinkedSetTester
{
    static void test0()
    {
        CLinkedSet x0;
        assert( x0.Empty());
        assert ( ! x0.Contains("Hd"));
        assert( x0 . Insert( "Jerabek Michal" ) );
        assert( x0 . Insert( "Pavlik Ales" ) );
        assert( x0 . Insert( "Dusek Zikmund" ) );
        assert( x0 . Size() == 3 );
        assert( x0 . Contains( "Dusek Zikmund" ) );
        assert( !x0 . Contains( "Pavlik Jan" ) );
        assert( x0 . Remove( "Jerabek Michal" ) );
        assert( !x0 . Remove( "Pavlik Jan" ) );
        assert( !x0 . Empty() );
    }

    static void test1()
    {
        CLinkedSet x0;
        assert( x0 . Insert( "Jerabek Michal" ) );
        assert( x0 . Insert( "Pavlik Ales" ) );
        assert( x0 . Insert( "Dusek Zikmund" ) );
        assert( x0 . Size() == 3 );
        assert( x0 . Contains( "Dusek Zikmund" ) );
        assert( !x0 . Contains( "Pavlik Jan" ) );
        assert( x0 . Remove( "Jerabek Michal" ) );
        assert( !x0 . Remove( "Pavlik Jan" ) );

        CLinkedSet x1 ( x0 );
        //x1.printList();
        //cout << endl;
        //x0.printList();
        assert( x0 . Insert( "Vodickova Saskie" ) );
        assert( x1 . Insert( "Kadlecova Kvetslava" ) );
        assert( x0 . Size() == 3 );
        assert( x1 . Size() == 3 );
        assert( x0 . Contains( "Vodickova Saskie" ) );
        assert( !x1 . Contains( "Vodickova Saskie" ) );
        assert( !x0 . Contains( "Kadlecova Kvetslava" ) );
        assert( x1 . Contains( "Kadlecova Kvetslava" ) );
    }

    static void test2()
    {
        CLinkedSet x0;
        CLinkedSet x1;
         assert( x0 . Insert( "Jerabek Michal" ) );
        assert( x0 . Insert( "Pavlik Ales" ) );
        assert( x0 . Insert( "Dusek Zikmund" ) );
        assert( x0 . Size() == 3 );
        assert( x0 . Contains( "Dusek Zikmund" ) );
        assert( !x0 . Contains( "Pavlik Jan" ) );
        assert( x0 . Remove( "Jerabek Michal" ) );
        assert( !x0 . Remove( "Pavlik Jan" ) );
        x1 = x0;
        assert( x0 . Insert( "Vodickova Saskie" ) );
        assert( x1 . Insert( "Kadlecova Kvetslava" ) );
        assert( x0 . Size() == 3 );
        assert( x1 . Size() == 3 );
        assert( x0 . Contains( "Vodickova Saskie" ) );
        assert( !x1 . Contains( "Vodickova Saskie" ) );
        assert( !x0 . Contains( "Kadlecova Kvetslava" ) );
        assert( x1 . Contains( "Kadlecova Kvetslava" ) );
        assert( x1 . Remove( "Kadlecova Kvetslava" ) );
        assert( x1 . Remove( "Pavlik Ales" ) );
        assert( ! x1 . Contains( "Pavlik Ales" ) );
    }
    static void test3()
    {
        CLinkedSet x0;
        CLinkedSet x1;
        CLinkedSet x3;
        assert (x0.Empty());
        assert (x1.Empty());
        assert (x0.Insert("Honza"));
        assert (x0.Insert("Kuba"));
        assert (x0.Insert("Jarda"));
        assert (x0.Insert("Zuzka"));
        assert (x0.Contains("Zuzka"));
        assert (x0.Contains("Honza"));
        assert (x0.Contains("Kuba"));
        assert (x0.Contains("Jarda"));
        assert ( ! x0.Contains("Jrda"));
        assert (x0.Remove("Zuzka"));
        assert (! x0.Contains("Zuzka"));
        assert ( !x0.Insert("Honza"));
        x1 = x0;
        assert (x1.Contains("Jarda"));
        assert (x1.Contains("Honza"));
        assert (x1.Contains("Kuba"));
        assert (x1.Remove("Kuba"));
        assert (x1.Remove("Honza"));
        assert (x1.Remove("Jarda"));
        assert ( x1.Empty());
        assert (! x1.Remove("sdf"));
    }
    static void test4()
    {
        CLinkedSet x3;
        assert (x3.Insert("ahoj"));
        assert (x3.Remove("ahoj"));
        assert (x3.Empty());
        assert (x3.Insert("ahoj"));
        assert (x3.Insert("domino"));
        assert (x3.Insert("zakaz"));
        assert (x3.Insert("automat"));
        assert (x3.Remove("domino"));
        x3.printList();

    }
    static void test5()
    {
        CLinkedSet x1;
        assert (x1.Insert("ahoj"));
        assert (x1.Insert("ju"));
        assert (x1.Insert("hele"));
        assert (x1.Insert("a"));
        assert (x1.Insert("z"));
        assert (x1.Insert("david"));
        assert (x1.Insert("aa"));
        assert (x1.Remove("a"));
        assert (x1.Remove("z"));
        x1.printList();
        assert (x1.Remove("david"));
        assert (x1.Remove("ahoj"));
        assert (x1.Remove("aa"));
        assert (x1.Remove("hele"));
        assert (x1.Remove("ju"));
        assert (x1.Empty());

        x1.printList();
    }

};

int main ()
{
    CLinkedSetTester::test0();
    CLinkedSetTester::test1();
    CLinkedSetTester::test2();
    CLinkedSetTester::test3();
    CLinkedSetTester::test4();
    CLinkedSetTester::test5();
    return 0;
}
#endif /* __PROGTEST__ */
