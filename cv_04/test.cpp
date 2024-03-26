#ifndef __PROGTEST__
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <array>
#include <cassert>
using namespace std;
#endif /* __PROGTEST__ */

class CPerson
{
private:
    string m_FirstName;
    string m_LastName;
    string m_PhoneNumber;

public:
    bool PersonContains ( const string query) const
    {
        if ((m_FirstName == query) || (m_LastName == query))
            return true;
        return false;
    }
    void PrintPerson( ostream & out = cout) const
    {
        out << m_FirstName << " " << m_LastName << " " << m_PhoneNumber << endl;
    }

    CPerson(const string & firstName, const string & lastName, const string & phoneNumber)
    {
        m_FirstName = firstName;
        m_LastName = lastName;
        m_PhoneNumber = phoneNumber;
    }

};
//---------------------------------------------------------------

bool isValidNumber (const string number)
{
    for (char ch : number) {
        if (!((int)ch >= 48 && ch <= 57)) {
            return false;
        }
    }
    if ( number[0] == '0')
        return false;
    if (number.length() != 9)
        return false;
    return true;

}
//---------------------------------------------------------------
bool getPerson(ifstream & stream, vector<CPerson> & TVector)
{
    string buffer;
    std::getline(stream, buffer);

    //nandame slova do promennych
    stringstream words(buffer);
    string firstName, lastName, phoneNumber, check;
    words >> firstName >> lastName >> phoneNumber >> check;

    //prazdne stringy, prebyvajici string
    if ((firstName.empty()) || (lastName.empty()) || (phoneNumber.empty()) || (!(check.empty())))
        return false;
    if (!(isValidNumber(phoneNumber)))
        return false;

    TVector.push_back(CPerson(firstName, lastName, phoneNumber));
    return true;
}
//---------------------------------------------------------------
//tahle funkce je teda celkem hnusna, idealne bych ji mel rozdelit do vic funkci
bool createVector(const string & fileName, vector<CPerson> & TVector, vector<string> & queries)
{
    //vytvorime stream z ktereho nacitame
    ifstream stream;
    stream.open(fileName);

    if (stream.fail())
        return false;

    //nacitame lidi
    while (true) {
        string buffer;
        std::getline(stream, buffer);

        if (stream.eof())
            break;
        if (buffer.length() == 0) {
            //nacitame dotazy
            while (true)
            {
                if (stream.eof()) {
                    stream.close();
                    return true;
                }
                string name;
                std::getline(stream, name);
                queries.push_back(name);
            }

        }
         // rozdelime radek a vytvorime objekt
        stringstream words(buffer);
        string firstName, lastName, phoneNumber, check;
        words >> firstName >> lastName >> phoneNumber >> check;
        if ((firstName.empty()) || (lastName.empty()) || (phoneNumber.empty()) || (!(check.empty())))
            return false;
        if (!(isValidNumber(phoneNumber)))
            return false;
        TVector.push_back(CPerson(firstName, lastName, phoneNumber));
        //cout << firstName << lastName << phoneNumber << endl;
    }
    stream.close();
    return false;
}
//--------------------------------------------------------------------
//mam dotaz a vyzkousim osoby pro shodu, vypisi pokud nastane
int isInVector(const string name, const vector<CPerson> & person, ostream & out)
{
    int count = 0;
    for (size_t i = 0 ; i< person.size() ; i++) {
        if (person[i].PersonContains(name)) {
            count++;
            person[i].PrintPerson(out);
        }
    }
    return count;
}
//--------------------------------------------------------------------
void printResult(const vector<CPerson> & TPerson, const vector<string> & queries, ostream & out)
{
    //bereme vzdy jeden dotaz a postupne prochazime pole lidi pro schodu
    //u kazdeho dotazu vypisujeme pocet shod
    int counter = 0;
    for (size_t i = 0 ; i < queries.size()-1; i++)
    {
        counter = isInVector(queries[i],TPerson, out);
        out <<  "-> "<< counter << endl;
    }


}
//--------------------------------------------------------------------
bool report ( const string & fileName, ostream & out )
{
    //vector s lidmi
    vector<CPerson> TVector;
    //vector s dotazy
    vector<string> queries;

    if (!(createVector(fileName,TVector,queries)))
        return false;

    printResult(TVector,queries,out);
    return true;
}
//----------------------------------------------------------------------

#ifndef __PROGTEST__
int main ()
{

    ostringstream oss;
    oss . str ( "" );
    //report("tests/test1_in.txt",oss);
    //report("tests/test0_in.txt", oss);


    assert ( report( "tests/test0_in.txt", oss ) == true );
    assert ( oss . str () ==
             "John Christescu 258452362\n"
             "John Harmson 861647702\n"
             "-> 2\n"
             "-> 0\n"
             "Josh Dakhov 264112084\n"
             "Dakhov Speechley 865216101\n"
             "-> 2\n"
             "John Harmson 861647702\n"
             "-> 1\n" );

    oss . str ( "" );
    assert ( report( "tests/test1_in.txt", oss ) == false );

    return 0;
}
#endif /* __PROGTEST__ */
