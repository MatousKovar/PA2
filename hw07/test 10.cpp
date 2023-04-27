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
    CContestant(const string & a) : m_Name(a), m_In(0) {}
    CContestant()
        :m_In(0)
    {}
    ~CContestant() = default;
    void add ( const string & a)
    {
        m_Neighbours.insert(a);
    }
    void addIn ()
    {
        m_In ++;
    }
    int inEdges () const
    {return m_In;}
    void  print ( ) const
    {
        for (const auto & a : m_Neighbours)
            cout << a << ", ";
        cout << "Pocet vstupu: " << m_In << endl;
    }
private:
    friend class CGraph;
    set<string> m_Neighbours;
    int m_In;
    string m_Name;
};

class CGraph
{
public:
    CGraph () = default;
    void graphAdd (const string & winner , const string & loser)
    {
        //todo tady se asi neinicializuje jmeno souteziciho
        m_Contestants[winner].add(loser);
        m_Contestants[loser].addIn();
    }
    string lowestIncomingEdges ()
    {
        int check = 0;
        string res;
        for (const auto & a : m_Contestants)
            if ( a.second.m_In == 0) {
                res = a.first;
                check ++;
            }
        if (check > 1)
            return "";
        return res;
    }
    void lowerInEdges (const string & name)
    {
        m_Contestants[name].m_In--;
    }
    //returns all neighbouring nodes and lowers their in count
    vector<string> neighbourNodes (const string & name)
    {
        vector <string> res;
        for (const string & a : m_Contestants[name].m_Neighbours) {
            res.push_back(a);
            m_Contestants[a].m_In --;
        }
        return res;
    }
    void deleteNode (const string & a)
    {
        m_Contestants.erase(a);
    }
    void printGraph () const
    {
        for (const auto & a : m_Contestants) {
            cout <<"Soutezici: " << a.first << " := ";
            a.second.print();
            cout << endl;
        }
    }

private:
    map <string, CContestant> m_Contestants;
};



template<typename M_>
class CContest {
public:
    CContest() = default;
    ~CContest () = default;
    CContest & addMatch (const string & contestant1,const string & contestant2, const M_ & result)
    {
        //ujistit se ze existuji oba soutezici
        //zjistit jestli se mezi nimi uz neudal zapas
        for (size_t i = 0 ; i < m_Matches.size() ; i++)
            if (m_Matches[i].contestant1 == contestant1
                && m_Matches[i].contestant2 == contestant2)
            throw std::logic_error("Existing match");
        //ulozit zaznam
        m_Matches.push_back({contestant1, contestant2, result});
        return *this;
    }
    bool isOrdered(function<int(M_)> funct) const
    {
//        TODO tady je potreba rozhodnout ktery node ma 0 vstupujicich hran
//        tento node pridame do vysledku
//        dale pridame do queue vsechny vrcholy na ktere ukazuje a snizime jim m_In o jedna
//        zkontrolujeme jestli ma pouze jeden m_In == 0; pokud ne vracime false;
//        opakujeme -> pop z vectoru ten, ktery ma m_In = 0; pridani jeho potomku .... az dojdeme do konce
        CGraph graph;
        //fill graph
        set<string> buffer;
        for (const auto & a : m_Matches)
        {
            if ( funct(a.result) > 1 )
                graph.graphAdd(a.contestant1,a.contestant2);
            else if (funct(a.result) == 0)
                return false;
            else
                graph.graphAdd(a.contestant2,a.contestant1);
        }
        //ted chci pouze prochazet graf a kontrolovat jestli v kazdem kroku neexistuje vice nodu s zadnymi vstupy
        while (true) {
            string lowest = graph.lowestIncomingEdges();
            if (lowest == "")
                return false;
            vector<string> tmp = graph.neighbourNodes(lowest);
            if (tmp.size() == 0)
                return true;
            for (size_t i = 0; i < tmp.size(); i++)
                buffer.insert(tmp[i]);
            graph.deleteNode(lowest);
        }
        return true;
    }
    list<string> results (function<int(M_)> funct) const
    {
        list<string> res;
        CGraph graph;
        //fill graph
        set<string> buffer;
        for (const auto & a : m_Matches)
        {
            if ( funct(a.result) > 1 )
                graph.graphAdd(a.contestant1,a.contestant2);
            else if (funct(a.result) == 0)
                throw logic_error("Remiza");
            else
                graph.graphAdd(a.contestant2,a.contestant1);
        }
        graph.printGraph();
        //ted chci pouze prochazet graf a kontrolovat jestli v kazdem kroku neexistuje vice nodu s zadnymi vstupy
        while (true) {
            string lowest = graph.lowestIncomingEdges();
            if (lowest == "")
                throw logic_error("Razeni neni jednoznacne");
            res.push_front(lowest);
            vector<string> tmp = graph.neighbourNodes(lowest);
            if (tmp.size() == 0)
                return res;
            for (size_t i = 0; i < tmp.size(); i++)
                buffer.insert(tmp[i]);
            graph.deleteNode(lowest);
        }
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
};

void printList ( const list<string> & a)
{
    for (const auto & x : a)
        cout << x << ", ";
    cout << endl;
}
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
//            .addMatch("PHP", "Pascal", CMatch(3, 6))
            .addMatch("PHP", "Basic", CMatch(10, 0));

//    CContest<CMatch> y;
//
//    y.addMatch("A","B", CMatch(10,3)).addMatch("B","C", CMatch(10,3))
//     .addMatch("A","C", CMatch(10,3)).addMatch("E","A", CMatch(10,3));

//    list<string> a = y.results(HigherScore);
//    printList(a);

//    assert(y.isOrdered(HigherScore));

    assert (! x.isOrdered(HigherScore));
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
    list<string> b = x.results(HigherScore);
    printList(b);

//    try {
//        list<string> res = x.results(HigherScore);
//        assert ((res == list<string>{"C++", "Java", "Pascal", "PHP", "Basic"}));
//    }
//    catch (...) {
//        assert ("Unexpected exception!" == nullptr);
//    }
//
//
//    assert (!x.isOrdered(HigherScoreThreshold(3)));
//    try {
//        list<string> res = x.results(HigherScoreThreshold(3));
//        assert ("Exception missing!" == nullptr);
//    }
//    catch (const logic_error &e) {
//    }
//    catch (...) {
//        assert ("Invalid exception thrown!" == nullptr);
//    }
//
//    assert (x.isOrdered([](const CMatch &x) {
//        return (x.m_A < x.m_B) - (x.m_B < x.m_A);
//    }));
//    try {
//        list<string> res = x.results([](const CMatch &x) {
//            return (x.m_A < x.m_B) - (x.m_B < x.m_A);
//        });
//        assert ((res == list<string>{"Basic", "PHP", "Pascal", "Java", "C++"}));
//    }
//    catch (...) {
//        assert ("Unexpected exception!" == nullptr);
//    }
//
//    CContest<bool> y;
//
//    y.addMatch("Python", "PHP", true)
//            .addMatch("PHP", "Perl", true)
//            .addMatch("Perl", "Bash", true)
//            .addMatch("Bash", "JavaScript", true)
//            .addMatch("JavaScript", "VBScript", true);
//
//    assert (y.isOrdered([](bool v) {
//        return v ? 10 : -10;
//    }));
//    try {
//        list<string> res = y.results([](bool v) {
//            return v ? 10 : -10;
//        });
//        assert ((res == list<string>{"Python", "PHP", "Perl", "Bash", "JavaScript", "VBScript"}));
//    }
//    catch (...) {
//        assert ("Unexpected exception!" == nullptr);
//    }
//
//    y.addMatch("PHP", "JavaScript", false);
//    assert (!y.isOrdered([](bool v) {
//        return v ? 10 : -10;
//    }));
//    try {
//        list<string> res = y.results([](bool v) {
//            return v ? 10 : -10;
//        });
//        assert ("Exception missing!" == nullptr);
//    }
//    catch (const logic_error &e) {
//    }
//    catch (...) {
//        assert ("Invalid exception thrown!" == nullptr);
//    }
//
//    try {
//        y.addMatch("PHP", "JavaScript", false);
//        assert ("Exception missing!" == nullptr);
//    }
//    catch (const logic_error &e) {
//    }
//    catch (...) {
//        assert ("Invalid exception thrown!" == nullptr);
//    }
//
//    try {
//        y.addMatch("JavaScript", "PHP", true);
//        assert ("Exception missing!" == nullptr);
//    }
//    catch (const logic_error &e) {
//    }
//    catch (...) {
//        assert ("Invalid exception thrown!" == nullptr);
//    }
    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */
