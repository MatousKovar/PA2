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

//vrchol bude obsahovat vsechny soupere, ktere porazil
class CContestant
{
public:
    CContestant(const string & a) : m_Name(a) {}
    CContestant() = default;
    ~CContestant() = default;
    void add ( const string & a)
    {
        m_Edges.insert(a);
    }
    void addIn ()
    {
        m_In ++;
    }
    int inEdges ()
    {return m_In;}

    //returns true if two competitors are in relation
    bool isInRelation(const string & a)
    {
        return m_Edges.find(a) != m_Edges.end();
    }
    bool operator < (const CContestant & other) const
    {
        return m_Name < other.m_Name;
    }
private:
    set<string> m_Edges;
    int m_In;
    string m_Name;
};



template<typename M_>
class CContest {
public:
    CContest() = default;
    ~CContest () = default;
    CContest & addMatch (const string & contestant1,const string & contestant2, const M_ & result)
    {
        //ujistit se ze existuji oba soutezici
        m_Contestants[contestant1] = CContestant(contestant1);
        m_Contestants[contestant2] = CContestant(contestant2);
        //zjistit jestli se mezi nimi uz neudal zapas
        for (size_t i = 0 ; i < m_Matches.size() ; i++)
            if (m_Matches[i].contestant1 == contestant1
                && m_Matches[i].contestant2 == contestant2)
            throw std::logic_error("Existing match");
        //ulozit zaznam
        m_Matches.push_back({contestant1, contestant2, result});
        return *this;
    }
    bool isOrdered(function<int(M_)> funct)
    {
//        TODO tady je potreba rozhodnout ktery node ma 0 vstupujicich hran
//        tento node pridame do vysledku
//        dale pridame do queue vsechny vrcholy na ktere ukazuje a snizime jim m_In o jedna
//        zkontrolujeme jestli ma pouze jeden m_In == 0; pokud ne vracime false;
//        opakujeme -> pop z vectoru ten, ktery ma m_In = 0; pridani jeho potomku .... az dojdeme do konce
        if (!createGraph(funct))
            return false;

        return true;
    }
    list<string> results (function<int(M_)> funct)
    {
        list<string> res;
        queue<CContestant> tmp_queue;

        for (size_t i = 0 ; i < m_Contestants.size() ; i ++)

        return res;
    }
private:
    struct Contest {
        string contestant1;
        string contestant2;
        M_ result;
    };
    map<string,CContestant> m_Contestants;
    vector<Contest> m_Matches;
    queue<CContestant> m_Queue;
    set<string> m_Visited;
    //takes vector of matches and adds results to correct Contestants
    bool createGraph (function<int (M_) > funct)
    {
        for (size_t i = 0 ; i < m_Matches.size() ; i++)
        {
            if (funct > 1 ) {
                m_Contestants[m_Matches[i].contestant1].add(m_Matches[i].contestant2);
                m_Contestants[m_Matches[i].contestant2].addIn();
            }
            else if (funct == 0)
                return false;
            else
                m_Contestants[m_Matches[i].contestant2].add(m_Matches[i].contestant1);
                m_Contestants[m_Matches[i].contestant1].addIn();
        }
    }
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
