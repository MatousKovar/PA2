/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 6/8/23
 */

#pragma once

#include "CSubQuery.h"
#include <vector>
#include <string>

class CSubQueryPlace : public CSubQuery {
public:
    ~CSubQueryPlace() override = default;

    std::vector<CEvent> Execute(std::string value, const CCalendar & calendar) override;

    std::unique_ptr<CSubQuery> Clone() override;
};


