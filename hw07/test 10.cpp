#ifndef __PROGTEST__

#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <set>
#include <list>
#include <map>
#include <vector>
#include <queue>
#include <string>
#include <stack>
#include <queue>
#include <deque>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <functional>
#include <iterator>
#include <stdexcept>

using namespace std;
#endif /* __PROGTEST__ */


template <typename M_>
int comparator (const M_ & obj)
{
    if (obj.first > obj.second)
        return 1;
    if ( obj.first < obj.second)
        return -1;
    else return 0;
}

template<typename M_>
struct CDuel
{
  string m_Contestant1;
  string m_Contestant2;
  M_ m_Result;
  bool operator == (const CDuel & a)
  {
      return m_Contestant1 == a.m_Contestant1 && m_Contestant2 == a.m_Contestant2 && m_Result == a.m_Result;
  }
  bool compare(const M_ & lhs, const M_ & rhs)
  {
      if (lhs.first != rhs.first)
          return lhs.first < rhs.first;
      return lhs.second < rhs.second;
  }
  bool operator < (const CDuel & a)
  {
      if (m_Contestant1 != m_Contestant1)
          return m_Contestant1 < a.m_Contestant1;
      if (m_Contestant2 != a.m_Contestant2)
          return m_Contestant2 < a.m_Contestant2;
      return compare(m_Result,a.m_Result);
  }
};

template <typename M_>
struct CContestant {
public:
    bool add ( const CDuel <M_> & a)
    {
        if (m_Matches.find(a) != m_Matches.end())
            return false;
        m_Matches.insert(a);
        return true;
    }
    int m_Wins = 0;
    set< CDuel <M_> > m_Matches;
};

template<typename M_>
class CContest {
public:
    CContest() = default;
    ~CContest () = default;
    //TODO vymyslet jak bude vypadat comparator...
    CContest & addMatch (const string & contestant1,const string & contestant2, const M_ & result)
    {
        CDuel<M_> a {contestant1,contestant2,result};
        m_Contestants[contestant1].add(a);
        m_Contestants[contestant2].add(a);
    }
    // isOrdered ( comparator )
    bool isOrdered(int compare (const M_ & obj))
    {
        return true;
    }
    // results ( comparator )
    list<string> results ( int compare (const M_ & obj))
    {
        list<string> res;

        return res;
    }
private:
    //Udelat shared ptr na CContestant? a mit to v setu serazeno podle vyher a v mape podle jmena??? To nezni uplne spatne
    map <string, CContestant <M_> > m_Contestants;
    vector<CDuel<M_> > m_Matches;
};

#ifndef __PROGTEST__

struct CMatch {
public:
    CMatch(int a,
           int b)
            : m_A(a),
              m_B(b) {
    }

    int m_A;
    int m_B;
};



class HigherScoreThreshold {
public:
    HigherScoreThreshold(int diffAtLeast)
            : m_DiffAtLeast(diffAtLeast) {
    }

    int operator()(const CMatch &x) const {
        return (x.m_A > x.m_B + m_DiffAtLeast) - (x.m_B > x.m_A + m_DiffAtLeast);
    }

private:
    int m_DiffAtLeast;
};

int HigherScore(const CMatch &x) {
    return (x.m_A > x.m_B) - (x.m_B > x.m_A);
}

int main(void) {
    CContest<CMatch> x;

    x.addMatch("C++", "Pascal", CMatch(10, 3))
            .addMatch("C++", "Java", CMatch(8, 1))
            .addMatch("Pascal", "Basic", CMatch(40, 0))
            .addMatch("Java", "PHP", CMatch(6, 2))
            .addMatch("Java", "Pascal", CMatch(7, 3))
            .addMatch("PHP", "Basic", CMatch(10, 0));


    assert (!x.isOrdered(HigherScore));
    try {
        list<string> res = x.results(HigherScore);
        assert ("Exception missing!" == nullptr);
    }
    catch (const logic_error &e) {
    }
    catch (...) {
        assert ("Invalid exception thrown!" == nullptr);
    }

    x.addMatch("PHP", "Pascal", CMatch(3, 6));

    assert (x.isOrdered(HigherScore));
    try {
        list<string> res = x.results(HigherScore);
        assert ((res == list<string>{"C++", "Java", "Pascal", "PHP", "Basic"}));
    }
    catch (...) {
        assert ("Unexpected exception!" == nullptr);
    }


    assert (!x.isOrdered(HigherScoreThreshold(3)));
    try {
        list<string> res = x.results(HigherScoreThreshold(3));
        assert ("Exception missing!" == nullptr);
    }
    catch (const logic_error &e) {
    }
    catch (...) {
        assert ("Invalid exception thrown!" == nullptr);
    }

    assert (x.isOrdered([](const CMatch &x) {
        return (x.m_A < x.m_B) - (x.m_B < x.m_A);
    }));
    try {
        list<string> res = x.results([](const CMatch &x) {
            return (x.m_A < x.m_B) - (x.m_B < x.m_A);
        });
        assert ((res == list<string>{"Basic", "PHP", "Pascal", "Java", "C++"}));
    }
    catch (...) {
        assert ("Unexpected exception!" == nullptr);
    }

    CContest<bool> y;

    y.addMatch("Python", "PHP", true)
            .addMatch("PHP", "Perl", true)
            .addMatch("Perl", "Bash", true)
            .addMatch("Bash", "JavaScript", true)
            .addMatch("JavaScript", "VBScript", true);

    assert (y.isOrdered([](bool v) {
        return v ? 10 : -10;
    }));
    try {
        list<string> res = y.results([](bool v) {
            return v ? 10 : -10;
        });
        assert ((res == list<string>{"Python", "PHP", "Perl", "Bash", "JavaScript", "VBScript"}));
    }
    catch (...) {
        assert ("Unexpected exception!" == nullptr);
    }

    y.addMatch("PHP", "JavaScript", false);
    assert (!y.isOrdered([](bool v) {
        return v ? 10 : -10;
    }));
    try {
        list<string> res = y.results([](bool v) {
            return v ? 10 : -10;
        });
        assert ("Exception missing!" == nullptr);
    }
    catch (const logic_error &e) {
    }
    catch (...) {
        assert ("Invalid exception thrown!" == nullptr);
    }

    try {
        y.addMatch("PHP", "JavaScript", false);
        assert ("Exception missing!" == nullptr);
    }
    catch (const logic_error &e) {
    }
    catch (...) {
        assert ("Invalid exception thrown!" == nullptr);
    }

    try {
        y.addMatch("JavaScript", "PHP", true);
        assert ("Exception missing!" == nullptr);
    }
    catch (const logic_error &e) {
    }
    catch (...) {
        assert ("Invalid exception thrown!" == nullptr);
    }
    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */
