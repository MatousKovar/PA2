#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <iostream> 
#include <iomanip> 
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */



class CPerson
{
private:
    string m_firstName;
    string m_surname;
    string m_email;
    unsigned int m_salary;
public:
    CPerson(const string & name,const string & surname,const string & email, unsigned int salary)
    :m_firstName(name),
    m_surname(surname),
    m_email(email),
    m_salary(salary)
    {}
    ~CPerson (void)
    {}
//    bool isSame (const string & name, const string & surname) const
//    {
//        return (m_firstName == name) && (m_surname == surname);
//    }
    int isSmaller (const string & name, const string & surname)
    {
        //if ((m_firstName == name && m_surname == surname) || (email == m_email))
        //    return -1;
        if(m_surname == surname)
            return m_firstName > name;
        return m_surname > surname;
    }
    int isSmaller(const string & mail)
    {
        return m_email > mail;
    }
    void printPerson() const
    {
        cout << m_firstName << " " << m_surname << " " << m_email << " " << m_salary << endl;
    }
    string getName() const { return m_firstName;}
    void setName(const string & name, const string & surname) {m_firstName = name;m_surname = surname;}
    string getSurname() const  { return m_surname;}
    void setSalary(unsigned int sal) {m_salary = sal;}
    unsigned int getSalary() const {return m_salary;}
    void setEmail(const string & email) {m_email = email;}
    string getEmail() const {return m_email;}
    bool hasEmail(const string & email) const {return m_email == email;}
    bool operator== (const CPerson & person) const // operator na porovnavani dvou osob
    {
        return (person.getName() == m_firstName &&
                person.getSurname() == m_surname);
    }

    friend bool compare(const CPerson & person1 ,const CPerson & person2) ;
};

bool compareEmails(const CPerson & person1 , const CPerson & person2)
{
    return (person1.getEmail() < person2.getEmail());
}

bool  compare (const CPerson & person1 ,const CPerson & person2)
{
    if (person1.m_surname < person2.m_surname)
        return true;
    else if (person1.m_surname == person2.m_surname)
        return person1.m_firstName < person2.m_firstName;
    else
        return false;
}

class CPersonalAgenda {
public:
    CPersonalAgenda(void)
    {}

    ~CPersonalAgenda(void)
    {}
    // melo by byt funkcni, porovnavani resime v CPerson class, a vracime -1 kdyz ma nekdo stejny jmeno a prijmeni

    bool          add(const string &name,const string &surname,const string &email, unsigned int salary)
    {
        if (m_people.empty()) {
            m_people.push_back(CPerson(name,surname,email,salary));
            m_emails.push_back(CPerson(name,surname,email,salary));
            return true;
        }

        if (!(isUnique(name,surname)) || !(isUnique(email)))
            return false;

        CPerson person(name,surname,email,salary);
        size_t i = 0;
        for (; i < m_people.size(); i++)
        {
            if (m_people[i].isSmaller(name,surname))
                break;
        }
        m_people.insert(m_people.begin() + i, person);

        size_t j = 0;
        for ( ; j < m_emails.size() ; j++)
        {
            if (m_emails[j].isSmaller(email))
                break;
        }
        m_emails.insert(m_emails.begin() + j , person);
        //addSalary(salary);
        return true;
    }
    bool          del              ( const string & name, const string & surname )
    {
        int index = getIndex(name,surname);
        cout << index << endl;
        if (index == -1)
            return false;
        int indexEmail = getIndex(m_people[index].getEmail());
        m_emails.erase(m_emails.begin() + indexEmail);
        m_people.erase(m_people.begin() + index);
        return true;
    }
    bool          del              ( const string    & email )
    {
        int indexEmail = getIndex(email);
        if (indexEmail == - 1)
            return false;
        int index = getIndex(m_emails[indexEmail].getName(), m_emails[indexEmail].getSurname());
        m_emails.erase(m_emails.begin() + indexEmail );
        m_people.erase(m_people.begin() + index);

        return true;
    }
    bool          changeName       ( const string    & email,const string    & newName,const string    & newSurname )
    {
        int indexEmail = getIndex(email);
        if (indexEmail == -1 )
            return false;

        if(!(isUnique(newName,newSurname)))
            return false;

        int index = getIndex(m_emails[indexEmail].getName(),m_emails[indexEmail].getSurname()); // nastaveni v druhym poli, poradi se nezmeni
        m_emails.erase(m_emails.begin()+ indexEmail);

        //m_people[index].setName(newName,newSurname);
        //cout << m_people[index].getEmail()<< " " << m_people[index].getName() <<  " "<<m_people[index].getSurname() << " " << m_people[index].getSalary();
        unsigned int salary = m_people[index].getSalary();
        m_people.erase(m_people.begin() + index);

        add(newName,newSurname,email,salary);
        return true;
    }
    bool          changeEmail      ( const string    & name, const string    & surname, const string    & newEmail )
    {
        int index = getIndex(name,surname);
        if (index == -1 )
            return false;
        if (!(isUnique(newEmail)))
            return false;
        int indexEmail = getIndex(m_people[index].getEmail());
        unsigned int salary = m_people[index].getSalary();
        m_people.erase(m_people.begin()+ index);
        m_emails.erase(m_emails.begin()+ indexEmail);

        add(name,surname,newEmail,salary);

        return true;
    }
    bool          setSalary        ( const string    & name,const string    & surname,unsigned int      salary )
    {
        int index = getIndex(name,surname);
        if (index == -1)
            return false;
        m_people[index].setSalary(salary);
        int indexEmail = getIndex(m_people[index].getEmail());
        m_emails[indexEmail].setSalary(salary);
        return true;
    }
    bool          setSalary        ( const string    & email,unsigned int      salary )
    {
        int indexEmail = getIndex(email);
        if (indexEmail == -1 )
            return false;
        m_emails[indexEmail].setSalary(salary);
        int index = getIndex(m_emails[indexEmail].getName(), m_emails[indexEmail].getSurname());
        m_people[index].setSalary(salary);
        return true;
    }
    unsigned int  getSalary        ( const string    & name,const string    & surname ) const
    {
        int index = getIndex(name,surname);
        if (index == -1 )
            return 0;
        return m_people[index].getSalary();
    }
    unsigned int  getSalary        ( const string    & email ) const
    {
        int index = getIndex(email);
        if (index == -1)
            return 0;
        return m_emails[index].getSalary();
    }
    bool          getRank          ( const string    & name,const string & surname,int & rankMin,int & rankMax ) const
    {
        int index = getIndex(name,surname);
        if (index == -1)
            return false;
        int better = 0;
        int worse = 0;
        int same = 0;
        unsigned int salary = m_people[index].getSalary();
        for (size_t i = 0; i < m_people.size() ; i++)
        {
            if (salary < m_people[i].getSalary()) {
                better++;
                continue;
            }
            if (salary > m_people[i].getSalary()) {
                worse++;
                continue;
            }
            else
                same ++;
        }
        rankMin = worse;
        rankMax = worse + same - 1;
        return true;
    }
    bool          getRank          ( const string    & email,int & rankMin,int & rankMax ) const
    {
        int index = getIndex(email);
        if (index == -1 )
            return false;
        int better = 0;
        int worse = 0;
        int same = 0;
        unsigned int salary = m_emails[index].getSalary();
        for (size_t i = 0; i < m_emails.size() ; i++)
        {
            if (salary < m_emails[i].getSalary()) {
                better++;
                continue;
            }
            if (salary > m_emails[i].getSalary()) {
                worse++;
                continue;
            }
            else
                same ++;
        }
        rankMin = worse;
        rankMax = worse + same - 1;
        return true;
    }
    bool          getFirst         ( string          & outName,string          & outSurname ) const
    {
        if (m_people.empty())
            return false;
        outName = m_people[0].getName();
        outSurname = m_people[0].getSurname();
        return true;
    }
    bool          getNext          ( const string & name,const string & surname,string & outName,string & outSurname ) const
    {
        int index = getIndex(name,surname); // index toho jehoz parametry posilame do funkce
        if (index == -1 || index == (int)m_people.size() -1 )//kdyz je posledni
            return false;
        index ++;

        outName = m_people[index].getName();
        outSurname = m_people[index].getSurname();

        return true;
    }
    void printAgenda() const
    {
        for (size_t i = 0; i < m_people.size() ; i++)
            m_people[i].printPerson();
        for (size_t i = 0 ; i < m_emails.size()  ; i++)
            m_emails[i].printPerson();
    }
    int getIndex(const string & firstName, const string & surname) const
    {
        // lower bound pracuje na bazi binary search
        //dam mu prvni, posledni prvek, prvek ktery chci najit, a porovnavac - hadam ze by mohl byt udelan urcite lepe(bez get,set), zatim nevim jak
        auto iter = lower_bound(m_people.begin(),m_people.end(),CPerson(firstName,surname,"",0), compare);
        if ((iter == m_people.end()) || !(*iter ==  CPerson(firstName, surname, "", 0))) // kdyz neexistuje
            return -1;
        return iter - m_people.begin() ;
    }
private:
    vector<CPerson> m_people;
    vector <CPerson> m_emails;
    int getIndex (const string & email ) const
    {
        auto iter = lower_bound(m_emails.begin(), m_emails.end(), CPerson("","",email,0),compareEmails);
        if ((iter == m_emails.end()) || (iter->getEmail()!=email))
            return -1;
        return iter - m_emails.begin();

    }

    bool isUnique(const string & name, const string & surname) const
    {
        if (getIndex(name,surname) == -1)
            return true;
        return false;

    }
    bool isUnique(const string & email) const
    {
        if ( getIndex(email) == -1)
            return true;
        return false;

    }

};



#ifndef __PROGTEST__
int main ( void )
{
  string outName, outSurname;
  int lo, hi;
/*
 * OTESTOVANO
 *  - delete prazdneho seznamu pomoci mailu i jmena
 *  - get first na prazdnej
 *  - get next na posledni
 */
  CPersonalAgenda b1;
    //b1 . add ( "John", "Smith", "john", 30000 );
    //b1 . add ( "John", "Miller", "johnm", 35000 );
    //b1 . add ( "Peter", "Smith", "peter", 23000 );
    //b1.printAgenda();
    //b1.getNext("Peter","Smith",outName, outSurname);
    //cout << outName << " " << outSurname << endl;
    CPersonalAgenda b0;
//    assert (!b0.del(  "sdf"));
//    assert (!b0.del("sdf", "sdf"));
//    assert (!b0.getFirst(outName,outSurname));
    assert (b0.add("Adam", "Korbel", "korbi", 9990));
    assert (b0.add("Alca", "Pavelkova", "Korbi", 9990));
    assert (b0.add("Mira", "Bartik", "mira123", 99));
    assert (b0.getSalary("Mira", "Bartik") == 99);
//    b0.printAgenda();
//
//    cout << b0.getIndex("Alca","Pavelkova") << endl;
//
//
    assert (b0.del("mira123"));
    assert (b0.add("Mira", "Bartik", "mira12", 99));
//    assert (!b0.add("Mira", "Brantik", "mira123", 99));
    assert (!b0.del("Mira", "Brantik"));
    assert (!b0.del("mira123"));
//
//    assert (b0.del("Mira","Bartik"));
//    assert (!b0.add("Mira", "Bartik", "m123", 99));
//
//    assert (!b0.getRank("Mira", "Bartik",lo,hi));
//    assert (!b0.getRank( "Bartik",lo,hi));
//
//    assert (b0.add("Adam", "Korbel", "korbi", 9990));
//    assert (b0.add("Mira", "Bartik", "mira123", 99));
//    assert (b0.add("Jiri", "Brantik", "jirkasirka", 99));
//    assert (!b0.setSalary("jan123",4000));
//    assert (!b0.setSalary("jan", "jan",4000));
//
//
//
//    assert (!b0.add("Mira", "Brantik", "mira123", 99));





    assert ( b1 . add ( "John", "Smith", "john", 30000 ) );
    assert ( b1 . add ( "John", "Miller", "johnm", 35000 ) );
  assert ( b1 . add ( "Peter", "Smith", "peter", 23000 ) );
  assert ( b1 . getFirst ( outName, outSurname )
           && outName == "John"
           && outSurname == "Miller" );
  assert ( b1 . getNext ( "John", "Miller", outName, outSurname )
           && outName == "John"
           && outSurname == "Smith" );
  assert ( b1 . getNext ( "John", "Smith", outName, outSurname )
           && outName == "Peter"
           && outSurname == "Smith" );
  assert ( ! b1 . getNext ( "Peter", "Smith", outName, outSurname ) );
  assert ( b1 . setSalary ( "john", 32000 ) );
  assert ( b1 . getSalary ( "john" ) ==  32000 );
  assert ( b1 . getSalary ( "John", "Smith" ) ==  32000 );
  //b1.pritRanks();
    b1 . getRank ( "John", "Smith", lo, hi );
    cout << lo << " " << hi << endl;
  assert ( b1 . getRank ( "John", "Smith", lo, hi )
           && lo == 1
           && hi == 1 );
  assert ( b1 . getRank ( "john", lo, hi )
           && lo == 1
           && hi == 1 );
  assert ( b1 . getRank ( "peter", lo, hi )
           && lo == 0
           && hi == 0 );
  assert ( b1 . getRank ( "johnm", lo, hi )
           && lo == 2
           && hi == 2 );
  assert ( b1 . setSalary ( "John", "Smith", 35000 ) );
  assert ( b1 . getSalary ( "John", "Smith" ) ==  35000 );
  assert ( b1 . getSalary ( "john" ) ==  35000 );
  assert ( b1 . getRank ( "John", "Smith", lo, hi )
           && lo == 1
           && hi == 2 );
  assert ( b1 . getRank ( "john", lo, hi )
           && lo == 1
           && hi == 2 );
  assert ( b1 . getRank ( "peter", lo, hi )
           && lo == 0
           && hi == 0 );
  assert ( b1 . getRank ( "johnm", lo, hi )
           && lo == 1
           && hi == 2 );
  assert ( b1 . changeName ( "peter", "James", "Bond" ) );
  assert ( b1 . getSalary ( "peter" ) ==  23000 );
  assert ( b1 . getSalary ( "James", "Bond" ) ==  23000 );
  assert ( b1 . getSalary ( "Peter", "Smith" ) ==  0 );
  assert ( b1 . getFirst ( outName, outSurname )
           && outName == "James"
           && outSurname == "Bond" );
  assert ( b1 . getNext ( "James", "Bond", outName, outSurname )
           && outName == "John"
           && outSurname == "Miller" );
  assert ( b1 . getNext ( "John", "Miller", outName, outSurname )
           && outName == "John"
           && outSurname == "Smith" );
  assert ( ! b1 . getNext ( "John", "Smith", outName, outSurname ) );
  assert ( b1 . changeEmail ( "James", "Bond", "james" ) );
  assert ( b1 . getSalary ( "James", "Bond" ) ==  23000 );
  cout << endl;
  b1.printAgenda();
  cout << b1.getIndex("James","Bond") << endl;
    cout << b1 . getSalary ( "james" ) << endl;
    assert ( b1 . getSalary ( "james" ) ==  23000 );
  assert ( b1 . getSalary ( "peter" ) ==  0 );
  assert ( b1 . del ( "james" ) );
  assert ( b1 . getRank ( "john", lo, hi )
           && lo == 0
           && hi == 1 );
  assert ( b1 . del ( "John", "Miller" ) );
  assert ( b1 . getRank ( "john", lo, hi )
           && lo == 0
           && hi == 0 );
  assert ( b1 . getFirst ( outName, outSurname )
           && outName == "John"
           && outSurname == "Smith" );
  assert ( ! b1 . getNext ( "John", "Smith", outName, outSurname ) );
  assert ( b1 . del ( "john" ) );
  assert ( ! b1 . getFirst ( outName, outSurname ) );
  assert ( b1 . add ( "John", "Smith", "john", 31000 ) );
  assert ( b1 . add ( "john", "Smith", "joHn", 31000 ) );
  assert ( b1 . add ( "John", "smith", "jOhn", 31000 ) );

  CPersonalAgenda b2;
  assert ( ! b2 . getFirst ( outName, outSurname ) );
  assert ( b2 . add ( "James", "Bond", "james", 70000 ) );
  assert ( b2 . add ( "James", "Smith", "james2", 30000 ) );
  assert ( b2 . add ( "Peter", "Smith", "peter", 40000 ) );
  assert ( ! b2 . add ( "James", "Bond", "james3", 60000 ) );
  assert ( ! b2 . add ( "Peter", "Bond", "peter", 50000 ) );
  assert ( ! b2 . changeName ( "joe", "Joe", "Black" ) );
  assert ( ! b2 . changeEmail ( "Joe", "Black", "joe" ) );
  assert ( ! b2 . setSalary ( "Joe", "Black", 90000 ) );
  assert ( ! b2 . setSalary ( "joe", 90000 ) );
  assert ( b2 . getSalary ( "Joe", "Black" ) ==  0 );
  assert ( b2 . getSalary ( "joe" ) ==  0 );
  assert ( ! b2 . getRank ( "Joe", "Black", lo, hi ) );
  assert ( ! b2 . getRank ( "joe", lo, hi ) );
  assert ( ! b2 . changeName ( "joe", "Joe", "Black" ) );
  assert ( ! b2 . changeEmail ( "Joe", "Black", "joe" ) );
  assert ( ! b2 . del ( "Joe", "Black" ) );
  assert ( ! b2 . del ( "joe" ) );
  assert ( ! b2 . changeName ( "james2", "James", "Bond" ) );
  assert ( ! b2 . changeEmail ( "Peter", "Smith", "james" ) );
  assert ( ! b2 . changeName ( "peter", "Peter", "Smith" ) );
  assert ( ! b2 . changeEmail ( "Peter", "Smith", "peter" ) );
  assert ( b2 . del ( "Peter", "Smith" ) );
  assert ( ! b2 . changeEmail ( "Peter", "Smith", "peter2" ) );
  assert ( ! b2 . setSalary ( "Peter", "Smith", 35000 ) );
  assert ( b2 . getSalary ( "Peter", "Smith" ) ==  0 );
  assert ( ! b2 . getRank ( "Peter", "Smith", lo, hi ) );
  assert ( ! b2 . changeName ( "peter", "Peter", "Falcon" ) );
  assert ( ! b2 . setSalary ( "peter", 37000 ) );
  assert ( b2 . getSalary ( "peter" ) ==  0 );
  assert ( ! b2 . getRank ( "peter", lo, hi ) );
  assert ( ! b2 . del ( "Peter", "Smith" ) );
  assert ( ! b2 . del ( "peter" ) );
  assert ( b2 . add ( "Peter", "Smith", "peter", 40000 ) );
  assert ( b2 . getSalary ( "peter" ) ==  40000 );

  assert (  b2. changeName("james", "a", "b"));
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */




