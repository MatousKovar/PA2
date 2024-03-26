/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 5/9/23
 */

#pragma once

#include <iostream>
#include <memory>
#include "CView.h"

class CViewMonth : public CView {
public:
    CViewMonth() = default;

    void Print(const CCalendar & calendar, const CDate & date, const CInterface & interface) const override;

    std::shared_ptr<CView> Clone() override;

    CDate PrintEvent(const CInterface & interface, const CEvent & event) const;
};
