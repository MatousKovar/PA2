/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 5/9/23
 */

#pragma once

#include "CView.h"
#include "CDate.h"
#include <iostream>
#include <memory>

class CViewWeek : public CView {
public:
    CViewWeek() = default;

    void Print(const CCalendar & calendar, const CDate & date, const CInterface & interface) const override;

    std::shared_ptr<CView> Clone() override;

    /**
     * Prints out event
     * @param interface
     * @param event
     * @return date when event ends
     */
    CDate PrintEvent(const CInterface & interface, const CEvent & event, const CDate & from, const CDate & to) const;

private:
};
