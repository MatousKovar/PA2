/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 5/9/23
 */

#include "CQuery.h"

CQuery::CQuery() {
    m_Subqueries.emplace("name", CSubQueryName().Clone());
    m_Subqueries.emplace("place", CSubQueryPlace().Clone());
    m_Subqueries.emplace("note", CSubQueryNote().Clone());
    m_Subqueries.emplace("people", CSubQueryPeople().Clone());
    m_Subqueries.emplace("tag", CSubQueryTag().Clone());
    m_Subqueries.emplace("type", CSubQueryType().Clone());
}

bool CheckConjunction(const std::string & conjunction) {
    return !(conjunction != "and" && conjunction != "or" && conjunction != "end");
}

void CQuery::Run(const CCalendar & calendar, const CInterface & interface) {
    std::vector<CEvent> res;
    std::string conjunction;
    //Read first events
    std::string attribute = interface.ReadCommand("Input name of attribute:\n");
    if (m_Subqueries.find(attribute) == m_Subqueries.end()) {
        throw (std::runtime_error("Invalid attribute: " + attribute + "\n"));
    }
    std::string value = interface.ReadCommand("Input value:\n");
    res = m_Subqueries[attribute]->Execute(value, calendar);

    //reads events until reaching conjunction == "end"
    while (true) {
        std::string conjunction = interface.ReadCommand("Input conjunction:\n");
        if (conjunction == "end")
            break;
        if (!CheckConjunction(conjunction))
            throw (std::runtime_error("Invalid conjunction: \"" + conjunction + "\"\n"));
        std::vector<CEvent> tmp;
        attribute = interface.ReadCommand("Input name of attribute:\n");
        if (m_Subqueries.find(attribute) == m_Subqueries.end()) {
            throw (std::runtime_error("Invalid attribute: " + attribute + "\n"));
        }
        value = interface.ReadCommand("Input value:\n");
        tmp = m_Subqueries[attribute]->Execute(value, calendar);
        if (conjunction == "and")
            res = Intersection(res, tmp);
        else if (conjunction == "or")
            res = Union(res, tmp);
    }
    GetInterval(res,interface);
    Print(interface, res);
    std::string save = interface.ReadCommand("Save search[type yes]?\n");
    if (save == "yes")
        CFileManager::SaveEvents(interface, res);
}

std::vector<CEvent> CQuery::Intersection(std::vector<CEvent> & lhs, std::vector<CEvent> & rhs) {
    std::vector<CEvent> result;
    std::set_intersection(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), std::back_inserter(result));
    return result;
}

std::vector<CEvent> CQuery::Union(std::vector<CEvent> & lhs, std::vector<CEvent> & rhs) {
    std::vector<CEvent> result;
    std::set_union(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), std::back_inserter(result));
    return result;
}

void CQuery::Print(const CInterface & interface, const std::vector<CEvent> & res) const {
    for (size_t i = 0; i < res.size(); i++)
        interface.PrintPrompt(res[i].Print());
}

std::vector<CEvent> CQuery::GetInterval(std::vector<CEvent> & events, const CInterface & interface) {
    CDate from = CEvent::GetDate(interface.ReadCommand("Enter date from[dd.mm.yyyy mm:hh]:\n"));
    CDate to = CEvent::GetDate(interface.ReadCommand("Enter date to[dd.mm.yyyy mm:hh]:\n"));
    if (! (from <= to))
        throw (std::invalid_argument("From date is not less than to date.\n"));
    for(size_t i = 0; i < events.size(); i++ ){
        if(events[i].GetFrom() < from || to < events[i].GetFrom() ) {
            events.erase(events.begin() + i);
            i--;
        }

    }
    return events;
}

