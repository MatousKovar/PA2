/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 5/9/23
 */

#pragma once

#include <iostream>
#include "CView.h"
#include "CDate.h"
#include <memory>

class CViewDay : public CView {
public:
    CViewDay() = default;

    void Print(const CCalendar & calendar, const CDate & date, const CInterface & interface) const override;

    int PrintEvent(const CInterface & interface, const CEvent & event) const;

    std::shared_ptr<CView> Clone() override;

private:
    void IterateThroughEvents(int i, const CInterface & interface, size_t idx, const std::vector<CEvent> events) const;
};

