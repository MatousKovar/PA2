/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 6/5/23
 */

#pragma once

#include "CView.h"
#include "CViewDay.h"
#include "CViewMonth.h"
#include "CInterface.h"
#include "CCalendar.h"
#include "CViewWeek.h"
#include <string>
#include <map>
#include <memory>

/**
 * Class that reads additional information for Viewing events, executes correct CView version
 */
class CViewManager {
public:
    CViewManager() {
        m_Views.emplace("day", CViewDay().Clone());
        m_Views.emplace("month", CViewMonth().Clone());
        m_Views.emplace("week", CViewWeek().Clone());
    }

    /**
     * Reads type of view and date of view.
     * @param interface
     * @param calendar
     */
    void GetView(const CInterface & interface, const CCalendar & calendar);

    /**
     * Cecks if date is in format [dd.mm.yyyy]
     * @param itnterface
     * @return
     */
    static CDate GetDate(const CInterface & itnterface);

private:
    /**
     * For reading one attribute of Date from stringstream
     * @param stream
     * @param delimiter
     * @return attribute value
     */
    static int GetAttribute(std::stringstream & stream, const char delimiter);

    static bool IsDigits(const std::string & str);

    std::map<std::string, std::shared_ptr<CView> > m_Views;

};


