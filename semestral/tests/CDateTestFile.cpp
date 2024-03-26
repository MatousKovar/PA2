/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 6/1/23
 */
#include <cassert>
#include <iostream>
#include "../src/CDate.h"
#include "../src/CInterface.h"

using namespace std;

template <typename T>
string toString ( const T & x )
{
    ostringstream oss;
    oss << x;
    return oss . str ();
}

int main ()
{
    CDate date1;
    //Leap years
    assert (date1.ValidateDate(2000,2,2,0,0));
    assert ( ! date1.ValidateDate(2024,2,30,1,1));
    assert (  date1.ValidateDate(2024,2,29,1,1));
    assert ( ! date1.ValidateDate(2023,2,29,1,1));
    assert ( date1.ValidateDate(2023,2,28,1,1));
    assert ( ! date1.ValidateDate(2023,2,29,1,1));
    assert ( ! date1.ValidateDate(1900,2,29,1,1));
    assert ( ! date1.ValidateDate(2200,2,29,1,1));
    assert ( date1.ValidateDate(2400,2,29,1,1));

    //Some edge cases
    assert ( ! date1.ValidateDate(2000,1,32,10,10));
    assert ( ! date1.ValidateDate(2000,1,0,10,10));
    assert ( ! date1.ValidateDate(2000,4,31,10,10));
    assert ( date1.ValidateDate(2000,1,30,0,0));
    assert ( ! date1.ValidateDate(2000,13,30,0,0));
    assert ( ! date1.ValidateDate(2000,0,30,0,0));

    //Parsing date from string

    //NextDay
    CDate date2(2002,1,8,11,40);
    CDate date3 = date2.NextDay();
    assert (toString(date3) == "9.1.2002 0:0");

    CDate date4 (2002,8,31,11,11);
    date4 = date4.NextDay();
    assert (toString(date4) == "1.9.2002 0:0");

    CDate date5 (2024,2,29,11,11);
    date4 = date5.NextDay();
    assert (toString(date4) == "1.3.2024 0:0");

    CDate date6 (2023,2,28,0,0);
    date6 = date6.NextDay();
    assert(toString(date6) == "1.3.2023 0:0");

    try
    {
        CDate date7(2023,1,1,11,11);
    }
    catch (...)
    {
        assert("Exception thrown" == nullptr);
    }
    CDate date7 (4000,6,6,0,0);
    cout << date7.DayOfWeek() << endl;

    return 0;
}