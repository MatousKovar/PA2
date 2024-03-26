/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 5/8/23
 */

#pragma once

#include <iostream>
#include <memory>
#include "CDate.h"
#include "CCalendar.h"
#include "CInterface.h"

/**
 * Abstract class for the dirrerent Views of calendar
 */
class CView {
public:
    CView() = default;

    virtual ~CView() = default;

    virtual void Print(const CCalendar & calendar, const CDate & date, const CInterface & interface) const = 0;

    virtual std::shared_ptr<CView> Clone() = 0;

protected:
    CDate m_Date;
};


