#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;
#endif /* __PROGTEST__ */

/**
 *
 * @tparam T
 * @param lhs
 * @param rhs
 * basic function for swapping, using template for reusability
 */
template<typename T>
void Swap ( T & lhs, T & rhs) {
    T tmp = lhs;
    lhs = rhs;
    rhs = tmp;
}

/**
 * Basic vector class used to store data
 * Using template for reusability
 */
template<typename T>
class CVector
{

public:
    CVector<T>()
    {
        m_Capacity = 50;
        m_Size = 0;
        m_Data = new T [m_Capacity];
    }
    /**
     * @param a - vector to be copied
     * copy constructor, copies all elements of a into m_Data
     */
    CVector<T>(const CVector & a)
              :m_Size(a.m_Size),
               m_Capacity(a.m_Capacity)
    {
        m_Data = new T [m_Capacity];
        for ( size_t i = 0 ; i < a.m_Size ; i++)
        {
            m_Data[i] = a.m_Data[i];
        }
    }
    ~CVector()
    {
        delete [] m_Data;
    }

    T & operator [] (size_t index) const   {
        return m_Data[index];
    }
    /**
     *
     * @param a - CVector to be assigned to *this
     * @return reference to vector that has been assigned different CVector
     * swaps all elements of two CVectors
     */
    CVector & operator= (CVector a)
    {
        swapItems( a);
        return *this;
    }
    /**
     * @param a CVector to swap elements with
     * swaps all elements of two objects using Swap function
     */
    void swapItems(CVector & a)
    {
        Swap(m_Data,a.m_Data);
        Swap(m_Size,a.m_Size);
        Swap(m_Capacity,a.m_Capacity);
    }
    /**
     *
     * @param value
     * method used to add values to end of CVector
     */
    void pushBack(const T & value)
    {
        if ( m_Size == m_Capacity)
            realloc();
        m_Data[m_Size] = value;
        m_Size ++;
    }
    size_t size () const
    {
        return m_Size;
    }
    T * begin()
    {
        return m_Data;
    }
    T * end()
    {
        return (m_Data + m_Size);
    }
    /**
     * basic method to print elements of vector into cout
     */
    void printVector()
    {
        for ( size_t i = 0; i < m_Size ; i++)
        {
            std::cout << m_Data[i] << " ";
        }
        std::cout << std::endl;
    }
private:
    /**@var m_Size - number of elements in CVector*/
    size_t m_Size;
    /**@var m_Capacity - maximal number of elements in vector*/
    size_t m_Capacity;
    /**@var m_Data - array of elements*/
    T * m_Data;
    /**
     * method for resizing arrat to twice its size
     */
    void realloc()
    {
        T * tmp = m_Data;

        m_Capacity *= 2;
        m_Data = new T [m_Capacity];

        for( size_t i = 0 ; i < m_Size ; i++)
        {
            m_Data[i] = tmp[i];
        }
        delete [] tmp;
    }
};
/**
 * class that represents string
 */
class CString
{
public:
    CString()
    :m_Len(0)
    {
        m_Data = new char[m_Len + 1];
        m_Data[0] = '\0';
    }
    /**
     * constructor
     * @param a - array of chars to turn into string
     */
    CString(const char * a)
    {
        m_Len = strlen(a);
        m_Data = new char[m_Len + 1];
        memcpy(m_Data,a,m_Len + 1);
    }
    ~CString()
    {
        delete [] m_Data;
    }
    /**
     * copy constructor
     * @param a - string to copy
     */
    CString (const CString & a)
    {
        m_Len = a.m_Len;
        m_Data = new char [m_Len + 1];
        memcpy(m_Data, a.m_Data, a.m_Len + 1);
    }
    /**
     * assignment operator with use of swap method and copy constructor
     * @param a - CString which *this will be assigned
     * @return reference to *this - string that was assigned a new value
     */
    CString & operator = (CString a)
    {
        swapItems( a );
        return *this;
    }
    /**
     * swaps parameter attributes with *this attributes
     * @param a - string which will have attributes swaped
     */
    void swapItems(CString & a) {
        Swap(m_Data,a.m_Data);
        Swap(m_Len,a.m_Len);
    }
    void printString() const
    {
        cout << m_Data << endl;
    }
    friend ostream & operator << (ostream & os, const CString & a);
    size_t getLength() {return m_Len;}
    bool operator== (const CString & a ) const
    {
        return strcmp(m_Data, a.m_Data) == 0;
    }
private:
    /** @var m_Len - length of string without '\0'*/
    size_t m_Len;
    /**@var m_Data - array of chars to store length of string*/
    char * m_Data;
};
ostream & operator<< (ostream & os, const CString & a)
{
    os << a.m_Data;
    return os;
}

/** Class that represents mail*/
class CMail
{
  public:
    CMail()
    {}
    CMail ( const char * from,const char  * to, const char * body )
        : m_From(from),
          m_To(to),
          m_Body(body)
    {}
    /**
     * copy constructor, uses copy constructor of CString
     * @param a
     */
    CMail ( const CMail & a)
        :m_From(CString(a.m_From)),
         m_To(CString(a.m_To)),
         m_Body(CString(a.m_Body))
    {}
    bool operator == ( const CMail     & x ) const
    {
        return (m_From == x.m_From) && (m_To == x.m_To) && (m_Body == x.m_Body);
    }

    /**
     * assignment operator with use of swap method and copy constructor
     * @param a - CMail which *this will be assigned
     * @return reference to *this - CMail that was assigned a new value
     */
    CMail & operator= (CMail a )
    {
        swapItems(a);
        return *this;
    }
    /**
     * method for swapping all attributes of CMail objects
     * @param a - CMail to swap attributes with
     */
    void swapItems(CMail & a) {
        Swap(a.m_From, m_From);
        Swap(a.m_To, m_To);
        Swap(a.m_Body, m_Body);

    }
    friend ostream  & operator << ( ostream         & os,const CMail     & m );
  private:
    friend class CMailServer;
    friend class CEmailAddress;
    /**@var m_From - sender of email*/
    CString m_From;
    /**@var m_To - receiver of email*/
    CString m_To;
    /**@var m_Body - content of email*/
    CString m_Body;
};
ostream & operator << (ostream & os, const CMail & m)
{
    os << "From: " << m.m_From << ", To: "<< m.m_To << ", Body: "<< m.m_Body;
    return os;
}

/**
 * Class to store information about email address
 */
class CEmailAddress
{
public:
    CEmailAddress()
    {}
    CEmailAddress(const CString & address)
    : m_Address(address)
    {}
    /**
     * Copy constructor, which uses copy constructor of CString and CVector
     * @param a - CEmailAddress to copy
     */
    CEmailAddress(const CEmailAddress & a)
        :m_Address(CString(a.m_Address)),
         m_Inbox(CVector<CMail>(a.m_Inbox)),
         m_Outbox(CVector<CMail>(a.m_Outbox))
    {}
    /**
     * assignment operator with use of swap method and copy constructor
     * @param a - CEmailAddress which *this will be assigned
     * @return reference to *this - CEmailAddress that was assigned a new values
     */
    CEmailAddress & operator= (CEmailAddress a)
    {
        swapItems(a);
        return *this;
    }
    /**
     * method for swapping attributes between two objects of CEmailAddress
     * @param a - object to swap attributes with
     */
    void swapItems(CEmailAddress & a)
    {
        Swap(m_Inbox,a.m_Inbox);
        Swap(m_Outbox,a.m_Outbox);
        Swap(m_Address,a.m_Address);
    }
    void addToInbox( const CMail & a)
    {
        m_Inbox.pushBack(a);
    }
    void addToOutbox(const CMail & a)
    {
        m_Outbox.pushBack(a);
    }
    /**
     * printing method for debugging
     */
    void printMessageInbox () const
    {
        for (size_t i = 0 ; i< m_Inbox.size() ; i++)
        {
            cout << m_Inbox[i] << " || " ;
        }
        cout << endl;
    }
private:
    friend class CMailServer;
    /**@var m_Address - name of email address */
    CString m_Address;
    /**@var m_Inbox - vector of mails delivered to address*/
    CVector<CMail> m_Inbox;
    /**@var m_Outbox - vector of emails sent from address*/
    CVector<CMail> m_Outbox;
};

class CMailIterator
{
  public:
    CMailIterator()
    :m_Data(nullptr),
     m_Begin(nullptr),
     m_End(nullptr)
    {}
    CMailIterator(CMail * a, CMail * begin , CMail * end)
    :m_Data(a),
     m_Begin(begin),
     m_End(end)
    {}
    ~CMailIterator()
    {}
    CMailIterator(const CMailIterator & a)
        :m_Data(a.m_Data),
         m_Begin(a.m_Begin),
         m_End(a.m_End)
    {}
    /**
     * assignment operator with use of swap method and copy constructor
     * @param a - iterator which *this will be assigned
     * @return reference to *this - Iterator that was set to different value
     */
    CMailIterator & operator= (CMailIterator a)
    {
        swapItems(a);
        return *this;
    }
    void swapItems(CMailIterator & a)
    {
        Swap(m_Data,a.m_Data);
        Swap(m_End, a.m_End);
        Swap(m_Begin, a.m_Begin);
    }
    /**
     * @return true if iterator is pointing to valid element
     */
    explicit operator bool ( void ) const
    {
        return !(operator !());
    }

    /**
     * @return true if iterator is pointing to invalid element
     */
    bool operator! ( void ) const
    {
        return m_Data == nullptr || m_Data >= m_End;
    }
    /**
     * Operator used for dereferencing CMailIterator
     * @return CMail that iterator was pointing to
     */
    const CMail & operator * ( void ) const
    {
        return *m_Data;
    }
    /**
     * moves iterator using pointer arithmetics
     * @return iterator that is pointing to the old iterators next element
     */
    CMailIterator & operator++ ( void )
    {
        m_Data++;
        return *this;
    }
  private:
    /**@var m_Data - element CMailIterator is pointing to */
    CMail * m_Data;
    /**@var m_Begin - pointer to the beginning of inbox or outbox iterator is pointing to*/
    CMail * m_Begin;
    /**@var m_End - pointer to the end of inbox or outbox iterator is pointing to*/
    CMail * m_End;
};


class CMailServer
{
    //struktura:
    //  mail server ma vektor emailovych adres
    //      jednotlive emailove adresy maji vektor mailu pro inbox a pro outbox
    //          mail ma klasicky informace odesilatel, adresat, a data
  public:
    CMailServer ( void )
    {}
    CMailServer ( const CMailServer & src )
    :m_Address(CVector<CEmailAddress>(src.m_Address))
    {}

    ~CMailServer ( void )
    {}
    CMailServer & operator= (CMailServer a)
    {
        swapItems(a);
        return *this;
    }
    void swapItems(CMailServer & a)
    {
        Swap(m_Address,a.m_Address);
    }

    /**
     * method adds @param m to outbox of email address that is sender of @param m
     * if email is not existing, it is created and added to vector of email addresses
     * @param m - email to add
     */
    void addToOutbox(const CMail & m)
    {
        for ( size_t i = 0 ; i < m_Address.size() ; i++)
        {
            if (m.m_From == m_Address[i].m_Address)
            {
                m_Address[i].addToOutbox(m);
                return;
            }
        }
        CEmailAddress a(m.m_From);
        m_Address.pushBack(a);
        m_Address[m_Address.size() - 1].addToOutbox(m);
    }
    /**
     * method that adds @param m to inbox of email address that is receiver of @param m
     * if receiver does not exist, it is created and added to vector of email addresses
     * @param m
     */
    void addToInbox ( const CMail & m)
    {
        for ( size_t i = 0 ; i < m_Address.size() ; i++)
        {
            if (m.m_To == m_Address[i].m_Address)
            {
                m_Address[i].addToInbox(m);
                return;
            }
        }
        //kdyz jeste email neni zaznamenany
        CEmailAddress a(m.m_To);
        m_Address.pushBack(a);
        m_Address[m_Address.size() - 1 ].addToInbox(m);
    }

    /**
     * method uses above methods addToInbox and addToOutbox, to add email to corresponding boxes
     * @param m - mail that will be sent
     */
    void                     sendMail                      ( const CMail     & m )
    {
        addToInbox(m);
        addToOutbox(m);
    }
    /**
     * Method searches vector of all addresses for email address with name @param email and returns iterator if found
     * @param email - name of email address
     * @return CMailIterator that points to the first element in outbox of @param email and empty iterator if @param email does not exist
     */
    CMailIterator outbox ( const char      * email ) const
    {
        //nalezneme emailovou adresu a vratime iterator na prvni prvek jejiho outboxu
        for(size_t i = 0 ; i < m_Address.size() ; i++)
        {
            if (m_Address[i].m_Address == CString(email))
                return CMailIterator(&m_Address[i].m_Outbox[0],m_Address[i].m_Outbox.begin(),m_Address[i].m_Outbox.end());
        }
        return CMailIterator();
    }
    /**
     * Method searches vector of all addresses for email address with name @param email and returns iterator if found
     * @param email - name of email address
     * @return CMailIterator that points to the first element in inbox of @param email and empty iterator if @param email does not exist
     */
    CMailIterator inbox ( const char      * email ) const
    {
        //vraci iterator na prvni prvek inboxu urciteho emailu
        for(size_t i = 0 ; i < m_Address.size() ; i++)
        {
            if (m_Address[i].m_Address == CString(email))
                return CMailIterator(&m_Address[i].m_Inbox[0], m_Address[i].m_Inbox.begin(),m_Address[i].m_Inbox.end());
        }
        return CMailIterator();
    }
  private:
    /**@var m_Address vector of all email addresses*/
    CVector<CEmailAddress> m_Address;
};


#ifndef __PROGTEST__
bool                         matchOutput                   ( const CMail     & m,
                                                             const char      * str )
{
  ostringstream oss;
  oss << m;
  return oss . str () == str;
}

int main ( void )
{
    char from[100], to[100], body[1024];

    assert ( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "john", "peter", "progtest deadline" ) );
    assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "john", "progtest deadline", "peter" ) ) );
    assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "peter", "john", "progtest deadline" ) ) );
    assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "peter", "progtest deadline", "john" ) ) );
    assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "progtest deadline", "john", "peter" ) ) );
    assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "progtest deadline", "peter", "john" ) ) );
    CMailServer s0;
    s0 . sendMail ( CMail ( "john", "peter", "some important mail" ) );
    strncpy ( from, "john", sizeof ( from ) );
    strncpy ( to, "thomas", sizeof ( to ) );
    strncpy ( body, "another important mail", sizeof ( body ) );
    s0 . sendMail ( CMail ( from, to, body ) );
    strncpy ( from, "john", sizeof ( from ) );
    strncpy ( to, "alice", sizeof ( to ) );
    strncpy ( body, "deadline notice", sizeof ( body ) );
    s0 . sendMail ( CMail ( from, to, body ) );
    s0 . sendMail ( CMail ( "alice", "john", "deadline confirmation" ) );
    s0 . sendMail ( CMail ( "peter", "alice", "PR bullshit" ) );
    CMailIterator i0 = s0 . inbox ( "alice" );
    assert ( i0 && *i0 == CMail ( "john", "alice", "deadline notice" ) );
    assert ( matchOutput ( *i0,  "From: john, To: alice, Body: deadline notice" ) );
    assert ( ++i0 && *i0 == CMail ( "peter", "alice", "PR bullshit" ) );
    assert ( matchOutput ( *i0,  "From: peter, To: alice, Body: PR bullshit" ) );
    assert ( ! ++i0 );

    CMailIterator i1 = s0 . inbox ( "john" );
    assert ( i1 && *i1 == CMail ( "alice", "john", "deadline confirmation" ) );
    assert ( matchOutput ( *i1,  "From: alice, To: john, Body: deadline confirmation" ) );
    assert ( ! ++i1 );

    CMailIterator i2 = s0 . outbox ( "john" );
    assert ( i2 && *i2 == CMail ( "john", "peter", "some important mail" ) );
    assert ( matchOutput ( *i2,  "From: john, To: peter, Body: some important mail" ) );
    assert ( ++i2 && *i2 == CMail ( "john", "thomas", "another important mail" ) );
    assert ( matchOutput ( *i2,  "From: john, To: thomas, Body: another important mail" ) );
    assert ( ++i2 && *i2 == CMail ( "john", "alice", "deadline notice" ) );
    assert ( matchOutput ( *i2,  "From: john, To: alice, Body: deadline notice" ) );
    assert ( ! ++i2 );

    CMailIterator i3 = s0 . outbox ( "thomas" );
    assert ( ! i3 );

    CMailIterator i4 = s0 . outbox ( "steve" );
    assert ( ! i4 );

    CMailIterator i5 = s0 . outbox ( "thomas" );
    s0 . sendMail ( CMail ( "thomas", "boss", "daily report" ) );
    CMailIterator i99 = s0 . outbox ( "thomas" );
    assert(i99);
    assert ( ! i5 );

    CMailIterator i6 = s0 . outbox ( "thomas" );
    assert ( i6 && *i6 == CMail ( "thomas", "boss", "daily report" ) );
    assert ( matchOutput ( *i6,  "From: thomas, To: boss, Body: daily report" ) );
    assert ( ! ++i6 );

    CMailIterator i7 = s0 . inbox ( "alice" );
    s0 . sendMail ( CMail ( "thomas", "alice", "meeting details" ) );
    assert ( i7 && *i7 == CMail ( "john", "alice", "deadline notice" ) );
    assert ( matchOutput ( *i7,  "From: john, To: alice, Body: deadline notice" ) );
    assert ( ++i7 && *i7 == CMail ( "peter", "alice", "PR bullshit" ) );
    assert ( matchOutput ( *i7,  "From: peter, To: alice, Body: PR bullshit" ) );
    assert ( ! ++i7 );

    CMailIterator i8 = s0 . inbox ( "alice" );
    assert ( i8 && *i8 == CMail ( "john", "alice", "deadline notice" ) );
    assert ( matchOutput ( *i8,  "From: john, To: alice, Body: deadline notice" ) );
    assert ( ++i8 && *i8 == CMail ( "peter", "alice", "PR bullshit" ) );
    assert ( matchOutput ( *i8,  "From: peter, To: alice, Body: PR bullshit" ) );
    assert ( ++i8 && *i8 == CMail ( "thomas", "alice", "meeting details" ) );
    assert ( matchOutput ( *i8,  "From: thomas, To: alice, Body: meeting details" ) );
    assert ( ! ++i8 );

    CMailServer s1 ( s0 );
    s0 . sendMail ( CMail ( "joe", "alice", "delivery details" ) );
    s1 . sendMail ( CMail ( "sam", "alice", "order confirmation" ) );
    CMailIterator i9 = s0 . inbox ( "alice" );
    assert ( i9 && *i9 == CMail ( "john", "alice", "deadline notice" ) );
    assert ( matchOutput ( *i9,  "From: john, To: alice, Body: deadline notice" ) );
    assert ( ++i9 && *i9 == CMail ( "peter", "alice", "PR bullshit" ) );
    assert ( matchOutput ( *i9,  "From: peter, To: alice, Body: PR bullshit" ) );
    assert ( ++i9 && *i9 == CMail ( "thomas", "alice", "meeting details" ) );
    assert ( matchOutput ( *i9,  "From: thomas, To: alice, Body: meeting details" ) );
    assert ( ++i9 && *i9 == CMail ( "joe", "alice", "delivery details" ) );
    assert ( matchOutput ( *i9,  "From: joe, To: alice, Body: delivery details" ) );
    assert ( ! ++i9 );

    CMailIterator i10 = s1 . inbox ( "alice" );
    assert ( i10 && *i10 == CMail ( "john", "alice", "deadline notice" ) );
    assert ( matchOutput ( *i10,  "From: john, To: alice, Body: deadline notice" ) );
    assert ( ++i10 && *i10 == CMail ( "peter", "alice", "PR bullshit" ) );
    assert ( matchOutput ( *i10,  "From: peter, To: alice, Body: PR bullshit" ) );
    assert ( ++i10 && *i10 == CMail ( "thomas", "alice", "meeting details" ) );
    assert ( matchOutput ( *i10,  "From: thomas, To: alice, Body: meeting details" ) );
    assert ( ++i10 && *i10 == CMail ( "sam", "alice", "order confirmation" ) );
    assert ( matchOutput ( *i10,  "From: sam, To: alice, Body: order confirmation" ) );
    assert ( ! ++i10 );

    CMailServer s2;
    s2 . sendMail ( CMail ( "alice", "alice", "mailbox test" ) );
    CMailIterator i11 = s2 . inbox ( "alice" );
    assert ( i11 && *i11 == CMail ( "alice", "alice", "mailbox test" ) );
    assert ( matchOutput ( *i11,  "From: alice, To: alice, Body: mailbox test" ) );
    assert ( ! ++i11 );

    s2 = s0;
    s0 . sendMail ( CMail ( "steve", "alice", "newsletter" ) );
    s2 . sendMail ( CMail ( "paul", "alice", "invalid invoice" ) );
    CMailIterator i12 = s0 . inbox ( "alice" );
    assert ( i12 && *i12 == CMail ( "john", "alice", "deadline notice" ) );
    assert ( matchOutput ( *i12,  "From: john, To: alice, Body: deadline notice" ) );
    assert ( ++i12 && *i12 == CMail ( "peter", "alice", "PR bullshit" ) );
    assert ( matchOutput ( *i12,  "From: peter, To: alice, Body: PR bullshit" ) );
    assert ( ++i12 && *i12 == CMail ( "thomas", "alice", "meeting details" ) );
    assert ( matchOutput ( *i12,  "From: thomas, To: alice, Body: meeting details" ) );
    assert ( ++i12 && *i12 == CMail ( "joe", "alice", "delivery details" ) );
    assert ( matchOutput ( *i12,  "From: joe, To: alice, Body: delivery details" ) );
    assert ( ++i12 && *i12 == CMail ( "steve", "alice", "newsletter" ) );
    assert ( matchOutput ( *i12,  "From: steve, To: alice, Body: newsletter" ) );
    assert ( ! ++i12 );

    CMailIterator i13 = s2 . inbox ( "alice" );
    assert ( i13 && *i13 == CMail ( "john", "alice", "deadline notice" ) );
    assert ( matchOutput ( *i13,  "From: john, To: alice, Body: deadline notice" ) );
    assert ( ++i13 && *i13 == CMail ( "peter", "alice", "PR bullshit" ) );
    assert ( matchOutput ( *i13,  "From: peter, To: alice, Body: PR bullshit" ) );
    assert ( ++i13 && *i13 == CMail ( "thomas", "alice", "meeting details" ) );
    assert ( matchOutput ( *i13,  "From: thomas, To: alice, Body: meeting details" ) );
    assert ( ++i13 && *i13 == CMail ( "joe", "alice", "delivery details" ) );
    assert ( matchOutput ( *i13,  "From: joe, To: alice, Body: delivery details" ) );
    assert ( ++i13 && *i13 == CMail ( "paul", "alice", "invalid invoice" ) );
    assert ( matchOutput ( *i13,  "From: paul, To: alice, Body: invalid invoice" ) );
    assert ( ! ++i13 );

    CMailIterator i14 = s2.inbox("alice");
  CMailIterator i15 = i14;
  cout << *i15 << endl;
  cout << *i14 << endl;
  ++i14;
  cout << *i15 << endl;
  cout << *i14 << endl;
  ++i14;
  cout << *i14 << endl;
  ++i14;
  cout << *i14 << endl;
  ++i14;
  cout << *i14 << endl;

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
