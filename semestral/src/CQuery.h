/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 5/9/23
 */

#pragma once

#include "CCalendar.h"
#include "CSubQuery.h"
#include "CSubQueryName.h"
#include "CSubQueryNote.h"
#include "CSubQueryTag.h"
#include "CSubQueryPlace.h"
#include "CSubQueryPeople.h"
#include "CSubQueryType.h"
#include "CSubQuery.h"
#include <vector>
#include <iostream>
#include <memory>
#include <string>
#include <map>

/**
 * Class to create queries on Calendar, can also save result
 */
class CQuery {
public:
    CQuery();

    /**
     * Main method of CQuery
     * @param calendar
     * @param interface
     */
    void Run(const CCalendar & calendar, const CInterface & interface);


private:
    /**
     * Prints out events in long format
     * @param interface
     * @param res
     */
    void Print(const CInterface & interface, const std::vector<CEvent> & res) const;

    /**
     * Gets date from and to and gets events from vector, that begin in this timespan
     * @param events
     * @param interface
     * @return f
     */
    std::vector<CEvent> GetInterval (std::vector<CEvent> & events, const CInterface & interface);

    /**
     * Implemented by std::set_intersection
     * @param lhs
     * @param rhs
     * @return
     */
    std::vector<CEvent> Intersection(std::vector<CEvent> & lhs, std::vector<CEvent> & rhs);

    /**
     * Implemented by std::set_union
     * @param lhs
     * @param rhs
     * @return
     */
    std::vector<CEvent> Union(std::vector<CEvent> & lhs, std::vector<CEvent> & rhs);

    std::map<std::string, std::unique_ptr<CSubQuery> > m_Subqueries;
};