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
    CInvoice(const CDate &date,const string & seller,const string & buyer,unsigned int amount,double vat)
            : m_buyer(buyer),
              m_seller(seller),
              m_date(date),
              m_amount(amount),
              m_vat(vat){}


    CInvoice(const CDate &date,const string & seller,const string & buyer,unsigned int amount,double vat,int index)
            : m_buyer(buyer),
              m_seller(seller),
              m_date(date),
              m_amount(amount),
              m_vat(vat),
              m_index(index){}

    CDate date(void) const {return m_date;}

    const string & buyer(void) const {return m_buyer;}

    const string & seller(void) const {return m_seller;}

    unsigned int amount(void) const {return m_amount;}

    double vat(void) const {return m_vat;}

    int index(void) const {return m_index;}

    bool operator<(const CInvoice &other) const {
        if (m_date.compare(other.m_date) != 0)
            return m_date.compare(other.m_date) < 0;
        if (m_buyer != other.m_buyer)
            return m_buyer < other.m_buyer;
        if (m_seller != other.m_seller)
            return m_seller < other.m_seller;
        if (m_amount != other.m_amount)
            return m_amount < other.m_amount;
        return m_vat < other.m_vat;
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
    int m_index;
};

ostream &operator<<(ostream &out, const CInvoice &invoice) {
    out << "Date: " << invoice.date() << " Seller: " << invoice.seller() << " Buyer: " << invoice.buyer() << " Amount: "
        << invoice.amount() << " Vat: " << invoice.vat();
    return out;
}

struct compareStrings {
    //need to compare strings case insensitive, ignore any spaces on the beginning and ending of the string
    //any consecutive spaces should be trimmed into just one, and then we can compare the strings
    void removeSpaces(string &b) const {
        //remove spaces from beginning
        size_t indexFirst = b.find_first_not_of(" ", 0);
        b = b.substr(indexFirst);
        //remove spaces from end
        size_t indexLast = b.find_last_not_of(" ");
        b = b.substr(0, indexLast + 1);
        //remove more than 1 consecutive space
        for (auto iter = b.begin(); iter != b.end(); ++iter) {
            if (*iter == ' ' && *(iter + 1) == ' ') {
                b.erase(iter + 1);
                iter--;
            }
        }
    }

    bool operator()(const string &lhs, const string &rhs) const {
        //making copies not to change the originals
        string leftCopy = lhs;
        string rightCopy = rhs;
        //making everything uppercase
        transform(leftCopy.begin(), leftCopy.end(), leftCopy.begin(), ::toupper);
        transform(rightCopy.begin(), rightCopy.end(), rightCopy.begin(), ::toupper);
        removeSpaces(leftCopy);
        removeSpaces(rightCopy);

        return leftCopy < rightCopy;
    }
    bool compare ( const string & lhs, const string & rhs)const
    {
        string left = lhs;
        string right = rhs;
        transform(left.begin(),left.end(),left.begin(), ::tolower);
        transform(right.begin(),right.end(),right.begin(),::tolower);
        return left < right;
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
        size_t m_keyIndex = 0;

        while (m_keyIndex != m_keys.size()) {
            if (m_keys[m_keyIndex].m_key == BY_DATE) {
                if (lhs.date().compare(rhs.date()) != 0)
                    return m_keys[m_keyIndex].m_ascending ? lhs.date().compare(rhs.date()) < 0 :
                           rhs.date().compare(lhs.date()) < 0;
            } else if (m_keys[m_keyIndex].m_key == BY_BUYER) {
                if (strcasecmp(lhs.buyer().c_str(), rhs.buyer().c_str()) != 0)
//                return m_keys[m_keyIndex].m_ascending ? compareStrings().compare(lhs.buyer(), rhs.buyer()) : compareStrings().compare(rhs.buyer(), lhs.buyer());
                    return m_keys[m_keyIndex].m_ascending ? strcasecmp(lhs.buyer().c_str(), rhs.buyer().c_str()) < 0 :
                           strcasecmp(rhs.buyer().c_str(), lhs.buyer().c_str()) < 0;
                //lhs.buyer() < rhs.buyer() : rhs.buyer() < lhs.buyer();
            } else if (m_keys[m_keyIndex].m_key == BY_SELLER) {
                if (strcasecmp(lhs.seller().c_str(), rhs.seller().c_str()) != 0)
//                return m_keys[m_keyIndex].m_ascending ? compareStrings().compare(lhs.seller(),rhs.seller()): compareStrings().compare(rhs.seller(), lhs.seller());
                    return m_keys[m_keyIndex].m_ascending ? strcasecmp(lhs.seller().c_str(), rhs.seller().c_str()) < 0 :
                           strcasecmp(rhs.seller().c_str(), lhs.seller().c_str()) < 0;
                //lhs.seller() < rhs.seller() : rhs.seller() < lhs.seller();
            } else if (m_keys[m_keyIndex].m_key == BY_AMOUNT) {
                if (lhs.amount() != rhs.amount())
                    return m_keys[m_keyIndex].m_ascending ? lhs.amount() < rhs.amount() : rhs.amount() < lhs.amount();
            } else if (m_keys[m_keyIndex].m_key == BY_VAT) {
                if (lhs.vat() != rhs.vat())
                    return m_keys[m_keyIndex].m_ascending ? lhs.vat() < rhs.vat() : rhs.vat() < lhs.vat();
            }
            m_keyIndex++;
        }
        return lhs.index() < rhs.index ();
    }

private:
    struct Ckey {
        int m_key;
        bool m_ascending;
    };
    vector<Ckey> m_keys;
};

bool InvoiceComparator (const CInvoice & lhs , const CInvoice & rhs , const CSortOpt & sortOpt)
{
    return sortOpt.compare( lhs, rhs);
}

class CCompany {
public:
    //TODO pujde nahradit vector za set a hleani bude logaritmicke
    CCompany() {}

    CCompany(const string &name)
            : m_name(name) {}

    bool addToIssued(const shared_ptr<CInvoice> & a) {
        //If invoice already exists
        auto iter = m_issued.find(a);
        //if there already exists same element
        if (iter != m_issued.end())
            return false;
        m_issued.insert(a);
        cout <<  "Inserting to Issued: " <<a.use_count() << endl;
        return true;
    }

    bool addToAccepted(const shared_ptr<CInvoice> & a) {
        //If invoice already exists
        auto iter = m_accepted.find(a);
        //if there already exists same element
        if (iter != m_accepted.end())
            return false;
        m_accepted.insert(a);
        cout << "Inserting to accepted: " << a.use_count() << endl;
        return true;
    }
    bool addToPending(const shared_ptr<CInvoice> & a) {
        //If invoice already exists
        auto iter = m_pending.find(a);
        //if there already exists same element
        if (iter != m_pending.end())
            return false;
        m_pending.insert(a);
        return true;
    }
    bool delPending( const shared_ptr <CInvoice> & a)
    {
        auto iter = m_pending.find(a);
        m_pending.erase(iter);
        return true;
    }
    bool delIssued(const shared_ptr<CInvoice> &a) {
        //if invoice does not exist, returning false
        auto iter = m_issued.find(a);
        if (iter == m_issued.end())
            return false;
        m_issued.erase(iter);
        return true;
    }

    bool delAccepted(const shared_ptr<CInvoice> & a) {
        //if CInvoice pointer does not exist
        auto iter = m_accepted.find(a);
        if (iter == m_accepted.end())
            return false;
        m_accepted.erase(iter);
        return true;
    }

    string name() {
        return m_name;
    }
    list<CInvoice> unpaired () const
    {
        set<CInvoice> res;
        for(const auto & a : m_issued)
        {
            if (a.use_count() < 4)
                res.insert(*a);
         }
        for(const auto & a : m_accepted)
        {
            if (a.use_count() < 4)
                res.insert(*a);
        }
        for(const auto & a : m_pending)
        {
            if (a.use_count() < 4)
                res.insert(*a);
        }
        list<CInvoice> resList;
        for(const auto & elem : res )
            resList.push_back(elem);
        return resList;
    }
    void printIssued(){
        for ( const auto & a : m_issued)
        {
            cout << *a << " " <<  a.use_count() <<endl;
        }
    }
    void printAccepted()
    {
        for ( const auto & a : m_accepted)
        {
            cout << *a << " " <<  a.use_count() <<endl;
        }
    }
private:
    string m_name;
    set < shared_ptr<CInvoice> > m_issued;
    set < shared_ptr<CInvoice> > m_accepted;
    set < shared_ptr<CInvoice> > m_pending;
};


class CVATRegister {
public:
    CVATRegister(void) :m_index(0) {}

    bool registerCompany(const string & name) {
        auto iter = m_data.find(name);
        if (iter == m_data.end()) {
            m_data[name] = CCompany(name);
            return true;
        }
        return false;
    }

    bool addIssued(const CInvoice &x) {
        if (!correctInvoice(x))
            return false;
        CInvoice canonical = CInvoice(x.date(), m_data[x.seller()].name(), m_data[x.buyer()].name(), x.amount(),x.vat(),m_index);
        auto iter = m_Invoices.find(canonical);
        //Invoice does not yet exist
        if ( iter == m_Invoices.end())
        {
            shared_ptr<CInvoice> ptr = make_shared<CInvoice> (canonical);
            m_Invoices[canonical] = ptr;
            m_data[canonical.seller()].addToIssued(ptr);
            m_data[canonical.buyer()].addToPending(ptr);
            m_index++;
            return true;
        }
        //addToIssued returns false it there already is same CInvoice
        if (! m_data[canonical.seller()].addToIssued(iter->second))
            return false;
        m_data[canonical.buyer()].addToPending(iter->second);
        m_index++;
        return true;
    }


    bool addAccepted(const CInvoice &x) {
        if (!correctInvoice(x))
            return false;
        //invoices in set are always in canonical form.
        CInvoice canonical = CInvoice(x.date(), m_data[x.seller()].name(), m_data[x.buyer()].name(), x.amount(),x.vat(),m_index);
        auto iter = m_Invoices.find(canonical);
        //Invoice does not yet exist
        if ( iter == m_Invoices.end())
        {
            shared_ptr<CInvoice> ptr = make_shared<CInvoice> (canonical);
            m_Invoices[canonical] = ptr;
            m_data[canonical.buyer()].addToAccepted(ptr);
            m_data[canonical.seller()].addToPending(ptr);
            m_index++;
            return true;
        }
        //addToIssued returns false it there already is same CInvoice
        cout << "Already there: " << iter->first << endl;
        if (! m_data[canonical.buyer()].addToAccepted(iter->second))
            return false;
        m_data[canonical.seller()].addToPending(iter->second);
        m_index++;
        return true;
    }

    bool delIssued(const CInvoice &x) {
        if (!correctInvoice(x))
            return false;
        CInvoice canonical = CInvoice(x.date(), m_data[x.seller()].name(), m_data[x.buyer()].name(), x.amount(),x.vat());
        auto iter = m_Invoices.find(canonical);
        //invoice does not exist
        if ( iter == m_Invoices.end())
            return false;
        if (! m_data[canonical.seller()].delIssued(iter->second))
            return false;
        m_data[canonical.buyer()].delPending(iter->second);
        //if erasing the last encounter of CInvoice
        if (iter->second.use_count() == 1)
            m_Invoices.erase(iter);
        return true;
    }

    bool delAccepted(const CInvoice &x) {
        if (!correctInvoice(x))
            return false;
        CInvoice canonical = CInvoice(x.date(), m_data[x.seller()].name(), m_data[x.buyer()].name(), x.amount(),x.vat());
        auto iter = m_Invoices.find(canonical);
//        auto iter = m_Invoices()
        //invoice does not exist
        if ( iter == m_Invoices.end())
            return false;
        if (! m_data[canonical.buyer()].delAccepted(iter->second))
            return false;
        m_data[canonical.seller()].delPending(iter->second);
        if (iter->second.use_count() == 1)
            m_Invoices.erase(iter);
        return true;
    }
    bool invoiceExists( const CInvoice & a )
    {
        return m_Invoices.find(CInvoice(a.date(),m_data[a.seller()].name(),m_data[a.buyer()].name(),a.amount(),a.vat())) != m_Invoices.end();
    }

    list<CInvoice> unmatched(const string &company, const CSortOpt &sortBy) const {
        list <CInvoice> res;
        auto iter = m_data.find(company);
//        if company does not exist
        if(iter== m_data.end())
            return res;
        string canonicalName = iter->first;
        res = iter->second.unpaired();
//        for (auto const & elem : m_Invoices)
//        {
//            if ((canonicalName == elem.first.seller() || canonicalName == elem.first.buyer()) && (elem.second.use_count() < 3)) {
//                res.push_back(elem.first);
//            }
//        }
        res.sort(bind(InvoiceComparator,placeholders::_1,placeholders::_2,sortBy));

        return res;
    }

    void printSet();
    void printRegistered();
    void printCompany ( const string & name);

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

    //mapa nazev spolecnosti a objekt spolecnosti
    map<string, CCompany, compareStrings> m_data;
    //mapa faktury a pointeru na fakturu
    map <CInvoice, shared_ptr<CInvoice>> m_Invoices;
    int m_index;
};

void printList(list<CInvoice> &a) {
    for (auto i = a.begin(); i != a.end(); i++) {
        cout << *i << endl;
    }
    cout << endl;
}
void CVATRegister::printCompany(const string & name)
{
    cout << "Issued:" << endl;
    m_data[name].printIssued();
    cout << endl << "Accepted: " << endl;
    m_data[name].printAccepted();
}
void CVATRegister::printRegistered()
{   for (auto a: m_data) {
        cout << a.first << " ";
    }
    cout << endl;
}
void CVATRegister::printSet(){
    for (auto i = m_Invoices.begin(); i != m_Invoices.end(); i++) {
        cout << i->first << endl;
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
        if (!(*iterA == *iterB))
            return false;
        iterA++;
        iterB++;
    }
    return true;
}

int main(void) {

    //hodit invoicy do setu normalne - vyhledavani logaritmicke. Musi se pretizit operator porovnani, unmatched bude asi trosku delsi, ale to nevadi
    //proste invoice vzdycky do m_Invoices. Kontroluju vzdycky jestli existuje m_Invoice - log, pridavani - log, odebirani - log, unmatched linearni

    CVATRegister a;
    a.registerCompany("Spolecnost jedna");
    a.registerCompany("Druha spolecnost");

    a.addIssued(CInvoice(CDate(2000,1,1),"spolecnost jedna" , "druha    spolecnost",300, 30));
    cout << a.invoiceExists(CInvoice(CDate(2000,1,1),"spolecnost jedna" , "druha    spolecnost",300, 30)) << endl;
    a.addAccepted(CInvoice(CDate(2000,1,1),"spolecnost jedna" , "druha    spolecnost",300, 30));
    a.printSet();

    CVATRegister r;
    assert (r.registerCompany("first Company"));
    assert (r.registerCompany("Second     Company"));
    assert (r.registerCompany("ThirdCompany, Ltd."));
    assert (r.registerCompany("Third Company, Ltd."));
    assert (!r.registerCompany("Third Company, Ltd."));
    assert (!r.registerCompany(" Third  Company,  Ltd.  "));
//    r.printRegistered();
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
//    r.printSet();
//
//    cout << endl;
//    list<CInvoice> invList = r.unmatched("First Company",CSortOpt().addKey(CSortOpt::BY_SELLER, true)
//                                                                                      .addKey(CSortOpt::BY_BUYER, false)
//                                                                                      .addKey(CSortOpt::BY_DATE, false));
//    printList(invList);

  assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, false ) . addKey ( CSortOpt::BY_DATE, false ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 )
           } ) );

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


//    list<CInvoice> invList2 = r.unmatched("First Company",CSortOpt());
//    printList(invList2);
 //todo
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



    //TODO
    assert ( r . addAccepted ( CInvoice ( CDate ( 2000, 1, 2 ), "First Company", "Second Company ", 200, 30 ) ) );
    assert ( r . addAccepted ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", " Third  Company,  Ltd.   ", 200, 30 ) ) );
    assert ( r . addAccepted ( CInvoice ( CDate ( 2000, 1, 1 ), "Second company ", "First Company", 300, 32 ) ) );
    r.printSet();
//    r.printCompany("Second Company");
    list<CInvoice> a1 = r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) . addKey ( CSortOpt::BY_DATE, true ));
    printList(a1);
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

    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */
