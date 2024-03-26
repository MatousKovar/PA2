#ifndef __PROGTEST__

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <deque>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <iterator>
#include <functional>

using namespace std;

class CDate {
public:
    //---------------------------------------------------------------------------------------------
    CDate(int y,
          int m,
          int d)
            : m_Year(y),
              m_Month(m),
              m_Day(d) {
    }

    //---------------------------------------------------------------------------------------------
    int compare(const CDate &x) const {
        if (m_Year != x.m_Year)
            return m_Year - x.m_Year;
        if (m_Month != x.m_Month)
            return m_Month - x.m_Month;
        return m_Day - x.m_Day;
    }

    //---------------------------------------------------------------------------------------------
    int year(void) const {
        return m_Year;
    }

    //---------------------------------------------------------------------------------------------
    int month(void) const {
        return m_Month;
    }

    //---------------------------------------------------------------------------------------------
    int day(void) const {
        return m_Day;
    }

    //---------------------------------------------------------------------------------------------
    friend ostream &operator<<(ostream &os,
                               const CDate &x) {
        char oldFill = os.fill();
        return os << setfill('0') << setw(4) << x.m_Year << "-"
                  << setw(2) << static_cast<int> ( x.m_Month ) << "-"
                  << setw(2) << static_cast<int> ( x.m_Day )
                  << setfill(oldFill);
    }
    //---------------------------------------------------------------------------------------------
private:
    int16_t m_Year;
    int8_t m_Month;
    int8_t m_Day;
};

#endif /* __PROGTEST__ */

class CInvoice {
public:
    CInvoice(const CDate &date,
             const string & seller,
             const string & buyer,
             unsigned int amount,
             double vat)
            : m_buyer(buyer),
              m_seller(seller),
              m_date(date),
              m_amount(amount),
              m_vat(vat) {}

    CDate date(void) const {return m_date;}

    string buyer(void) const {return m_buyer;}

    string seller(void) const {return m_seller;}

    unsigned int amount(void) const {return m_amount;}

    double vat(void) const {return m_vat;}

    bool operator<(const CInvoice &other) const {
        if (m_buyer != other.m_buyer)
            return m_buyer < other.m_buyer;
        if (m_seller != other.m_seller)
            return m_seller < other.m_seller;
        if (m_date.compare(other.m_date) != 0)
            return m_date.compare(other.m_date);
        if (m_vat != other.m_vat)
            return m_vat < other.m_vat;
        return m_amount < other.m_amount;
    }

    bool operator==(const CInvoice &other) const {
        return m_buyer == other.m_buyer && m_seller == other.m_seller && m_vat == other.m_vat &&
               m_date.compare(other.m_date) == 0 && m_amount == other.m_amount;
    }

    friend ostream &operator<<(ostream &a, const CInvoice &invoice);

private:
    string m_buyer;
    string m_seller;
    CDate m_date;
    unsigned int m_amount;
    double m_vat;
};

ostream &operator<<(ostream &out, const CInvoice &invoice) {
    out << "Date: " << invoice.date() << " Seller: " << invoice.seller() << " Buyer: " << invoice.buyer() << " Amount: "
        << invoice.amount() << " Vat: " << invoice.vat();
    return out;
}

struct compareStrings {
    //need to compare strings case insensitive, ignore any spaces on the beginning and ending of the string
    //any consecutive spaces should be trimmed into just one, and then we can compare the strings
    string removeSpaces(const string &b) const {
        string a = b;
        //remove spaces from beginning
        size_t indexFirst = a.find_first_not_of(" ", 0);
        a = a.substr(indexFirst);
        //remove spaces from end
        size_t indexLast = a.find_last_not_of(" ");
        a = a.substr(0, indexLast + 1);
        //remove more than 1 consecutive space
        for (auto iter = a.begin(); iter != a.end(); ++iter) {
            if (*iter == ' ' && *(iter + 1) == ' ') {
                a.erase(iter + 1);
                iter--;
            }
        }
        return a;
    }

    bool operator()(const string &lhs, const string &rhs) const {
        //making copies not to change the originals
        string leftCopy = lhs;
        string rightCopy = rhs;
        //making everything uppercase
        transform(leftCopy.begin(), leftCopy.end(), leftCopy.begin(), ::toupper);
        transform(rightCopy.begin(), rightCopy.end(), rightCopy.begin(), ::toupper);
        leftCopy = removeSpaces(leftCopy);
        rightCopy = removeSpaces(rightCopy);

        return leftCopy < rightCopy;
    }
};
//comparing Object, which stores keys to sort with and comparing method
class CSortOpt {
public:
    static const int BY_DATE = 0;
    static const int BY_BUYER = 1;
    static const int BY_SELLER = 2;
    static const int BY_AMOUNT = 3;
    static const int BY_VAT = 4;

    CSortOpt(void) {}

    CSortOpt &addKey(int key, bool ascending = true) {
        m_keys.push_back({key, ascending});
        return *this;
    }

    bool compare(const CInvoice &lhs, const CInvoice &rhs) const {
        if (m_keyIndex == m_keys.size())
            return true;
        if (m_keys[m_keyIndex].m_key == BY_DATE) {
            if (lhs.date().compare(rhs.date()) != 0)
                return m_keys[m_keyIndex].m_ascending ? lhs.date().compare(rhs.date()) : rhs.date().compare(lhs.date());
        } else if (m_keys[m_keyIndex].m_key == BY_BUYER) {
            if (lhs.buyer() != rhs.buyer())
                return m_keys[m_keyIndex].m_ascending ? compareStrings()(lhs.buyer(), rhs.buyer()) : compareStrings()(
                        rhs.buyer(), lhs.buyer()   );
            //lhs.buyer() < rhs.buyer() : rhs.buyer() < lhs.buyer();
        } else if (m_keys[m_keyIndex].m_key == BY_SELLER) {
            if (lhs.seller() != rhs.seller())
                return m_keys[m_keyIndex].m_ascending ? compareStrings()(lhs.seller(), rhs.seller()) : compareStrings()(
                        rhs.seller(), lhs.seller());
            //lhs.seller() < rhs.seller() : rhs.seller() < lhs.seller();
        } else if (m_keys[m_keyIndex].m_key == BY_AMOUNT) {
            if (lhs.amount() != rhs.amount())
                return m_keys[m_keyIndex].m_ascending ? lhs.amount() < rhs.amount() : rhs.amount() < lhs.amount();
        } else if (m_keys[m_keyIndex].m_key == BY_VAT) {
            if (lhs.vat() != rhs.vat())
                return m_keys[m_keyIndex].m_ascending ? lhs.vat() < rhs.vat() : rhs.vat() < lhs.vat();
        }
        m_keyIndex++;
        if (m_keyIndex == m_keys.size())
            return true;
        else
            return compare(lhs, rhs);
    }

private:
    struct Ckey {
        int m_key;
        bool m_ascending;
    };
    mutable int m_keyIndex = 0;
    vector<Ckey> m_keys;
};

bool InvoiceComparator (const CInvoice & lhs , const CInvoice & rhs , const CSortOpt & sortOpt)
{
    return sortOpt.compare( lhs, rhs);
}

class CCompany {
public:
    CCompany() {}

    CCompany(const string &name)
            : m_name(name) {}

    bool addToIssued(const CInvoice &a) {
        if (m_issued.find(a) != m_issued.end())
            return false;
        m_issued.insert(a);
        return true;
    }

    bool addToAccepted(const CInvoice &a) {
        //If invoice already exists
        if (m_accepted.find(a) != m_accepted.end())
            return false;
        m_accepted.insert(a);
        return true;
    }

    bool delIssued(const CInvoice &a) {
        //if invoice does not exist, returning false
        if (m_issued.find(a) == m_issued.end())
            return false;
        m_issued.erase(a);
        return true;
    }

    bool delAccepted(const CInvoice &a) {
        if (m_accepted.find(a) == m_accepted.end())
            return false;
        m_accepted.erase(a);
        return true;
    }

    string name() {
        return m_name;
    }

private:
    string m_name;
    set<CInvoice> m_issued;
    set<CInvoice> m_accepted;
};
class CVATRegister {
public:
    CVATRegister(void) {}

    bool registerCompany(const string &name) {
        try {
            m_data.at(name);
        }
        catch (std::out_of_range) {
            m_data[name] = CCompany(name);
            return true;
        }
        return false;
    }

    bool addIssued(const CInvoice &x) {
        if (!correctInvoice(x))
            return false;
        CInvoice canonical = CInvoice(x.date(), m_data[x.seller()].name(), m_data[x.buyer()].name(), x.amount(),x.vat());
        //addToIssued returns false it there already is same CInvoice
        if ( m_data[x.seller()].addToIssued(canonical) ) {
            m_Issued.insert(canonical);
            return true;
        }
        return false;
    }

    bool addAccepted(const CInvoice &x) {
        if (!correctInvoice(x))
            return false;
        CInvoice canonical = CInvoice(x.date(), m_data[x.seller()].name(), m_data[x.buyer()].name(), x.amount(),x.vat());
        if( m_data[x.buyer()].addToAccepted(canonical) ){
            m_Accepted.insert(canonical);
            return true;
        }
        return false;
    }

    bool delIssued(const CInvoice &x) {
        if (!correctInvoice(x))
            return false;
        if(m_data[x.seller()].delIssued(x)){
            m_Issued.erase(x);
            return true;
        }
        return false;
    }

    bool delAccepted(const CInvoice &x) {
        if (!correctInvoice(x))
            return false;
        if(m_data[x.buyer()].delAccepted(x))
        {
            m_Accepted.erase(x);
            return true;
        }
        return false;
    }

    list<CInvoice> unmatched(const string &company, const CSortOpt &sortBy) const {
        auto a = m_data.find(company);
        string canonicalName = a->first;
        list<CInvoice> res;
        //for loop iterates through all unpairet invoices and adds those, which have buyer or seller name same as company
        for (auto &a: m_Issued) {
            if (a.seller() == canonicalName || a.buyer() == canonicalName)
                if (m_Accepted.find(a) == m_Accepted.end())
                    res.push_back(a);
        }
        auto compare = bind(InvoiceComparator, placeholders::_1, placeholders::_2, sortBy);
//        res.sort(InvoiceComparator{sortBy});
        res.sort(compare);
        return res;
    }

    void printSet() {
        for (auto i = m_Issued.begin(); i != m_Issued.end(); i++) {
            cout << *i << endl;
        }
        for (auto i = m_Accepted.begin(); i != m_Accepted.end(); i++) {
            cout << *i << endl;
        }
        cout << endl;
    }

    void printRegistered() {
        for (auto a: m_data) {
            cout << a.first << " ";
        }
        cout << endl;
    }


private:
    bool correctInvoice(const CInvoice &invoice) const {
        // if company does not yet exist in the map of companies
        if (m_data.find(invoice.buyer()) == m_data.end() || m_data.find(invoice.seller()) == m_data.end())
            return false;
        // if buyer and seller are the same
        if (!compareStrings()(invoice.buyer(), invoice.seller()) &&
            !compareStrings()(invoice.seller(), invoice.buyer())) {
            return false;
        }
        return true;
    }

    map<string, CCompany, compareStrings> m_data;
    //if set contains invoice, then it means that it is unpaired
    set<CInvoice> m_Issued;
    set<CInvoice> m_Accepted;
};

void printList(list<CInvoice> &a) {
    for (auto i = a.begin(); i != a.end(); i++) {
        cout << *i << endl;
    }
    cout << endl;
}


#ifndef __PROGTEST__

bool equalLists(const list<CInvoice> &a, const list<CInvoice> &b) {
    if (a.size() != b.size())
        return false;
    auto iterA = a.begin();
    auto iterB = b.begin();
    while (iterA != a.end()) {
        if (!(iterA == iterB))
            return false;
        iterA++;
        iterB++;
    }
    return true;
}

int main(void) {

    CVATRegister a;

    CVATRegister r;
    assert (r.registerCompany("first Company"));
    assert (r.registerCompany("Second     Company"));
    assert (r.registerCompany("ThirdCompany, Ltd."));
    assert (r.registerCompany("Third Company, Ltd."));
    assert (!r.registerCompany("Third Company, Ltd."));
    assert (!r.registerCompany(" Third  Company,  Ltd.  "));
    r.printRegistered();
    assert (r.addIssued(CInvoice(CDate(2000, 1, 1), "First Company", "Second Company ", 100, 20)));
    assert (r.addIssued(CInvoice(CDate(2000, 1, 2), "FirSt Company", "Second Company ", 200, 30)));
    assert (r.addIssued(CInvoice(CDate(2000, 1, 1), "First Company", "Second Company ", 100, 30)));
    assert (r.addIssued(CInvoice(CDate(2000, 1, 1), "First Company", "Second Company ", 300, 30)));
    assert (r.addIssued(CInvoice(CDate(2000, 1, 1), "First Company", " Third  Company,  Ltd.   ", 200, 30)));
    assert (r.addIssued(CInvoice(CDate(2000, 1, 1), "Second Company ", "First Company", 300, 30)));
    assert (r.addIssued(CInvoice(CDate(2000, 1, 1), "Third  Company,  Ltd.", "  Second    COMPANY ", 400, 34)));
    assert (!r.addIssued(CInvoice(CDate(2000, 1, 1), "First Company", "Second Company ", 300, 30)));
    assert (!r.addIssued(CInvoice(CDate(2000, 1, 4), "First Company", "First   Company", 200, 30)));
    assert (!r.addIssued(CInvoice(CDate(2000, 1, 4), "Another Company", "First   Company", 200, 30)));
    r.printSet();
    cout << endl;
    list<CInvoice> invList = r.unmatched("First Company",CSortOpt().addKey(CSortOpt::BY_SELLER, true)
                                                                                      .addKey(CSortOpt::BY_BUYER, false)
                                                                                      .addKey(CSortOpt::BY_DATE, false));
    printList(invList);

//  assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, false ) . addKey ( CSortOpt::BY_DATE, false ) ),
//           list<CInvoice>
//           {
//             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
//             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
//             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
//             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
//             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
//             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 )
//           } ) );
    /*
    assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_DATE, true ) . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) ),
             list<CInvoice>
             {
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 )
             } ) );
    assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_VAT, true ) . addKey ( CSortOpt::BY_AMOUNT, true ) . addKey ( CSortOpt::BY_DATE, true ) . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) ),
             list<CInvoice>
             {
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 )
             } ) );
    assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () ),
             list<CInvoice>
             {
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
               CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 )
             } ) );
    assert ( equalLists ( r . unmatched ( "second company", CSortOpt () . addKey ( CSortOpt::BY_DATE, false ) ),
             list<CInvoice>
             {
               CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "Third Company, Ltd.", "Second     Company", 400, 34.000000 )
             } ) );
    assert ( equalLists ( r . unmatched ( "last company", CSortOpt () . addKey ( CSortOpt::BY_VAT, true ) ),
             list<CInvoice>
             {
             } ) );
    assert ( r . addAccepted ( CInvoice ( CDate ( 2000, 1, 2 ), "First Company", "Second Company ", 200, 30 ) ) );
    assert ( r . addAccepted ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", " Third  Company,  Ltd.   ", 200, 30 ) ) );
    assert ( r . addAccepted ( CInvoice ( CDate ( 2000, 1, 1 ), "Second company ", "First Company", 300, 32 ) ) );
    assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) . addKey ( CSortOpt::BY_DATE, true ) ),
             list<CInvoice>
             {
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 32.000000 )
             } ) );
    assert ( !r . delIssued ( CInvoice ( CDate ( 2001, 1, 1 ), "First Company", "Second Company ", 200, 30 ) ) );
    assert ( !r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "A First Company", "Second Company ", 200, 30 ) ) );
    assert ( !r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Hand", 200, 30 ) ) );
    assert ( !r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company", 1200, 30 ) ) );
    assert ( !r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company", 200, 130 ) ) );
    assert ( r . delIssued ( CInvoice ( CDate ( 2000, 1, 2 ), "First Company", "Second Company", 200, 30 ) ) );
    assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) . addKey ( CSortOpt::BY_DATE, true ) ),
             list<CInvoice>
             {
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 32.000000 )
             } ) );
    assert ( r . delAccepted ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", " Third  Company,  Ltd.   ", 200, 30 ) ) );
    assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) . addKey ( CSortOpt::BY_DATE, true ) ),
             list<CInvoice>
             {
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 32.000000 )
             } ) );
    assert ( r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", " Third  Company,  Ltd.   ", 200, 30 ) ) );
    assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) . addKey ( CSortOpt::BY_DATE, true ) ),
             list<CInvoice>
             {
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 32.000000 )
             } ) );
             */
    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */
