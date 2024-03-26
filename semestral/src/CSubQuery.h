/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 6/8/23
 */

#pragma once

#include "CCalendar.h"
#include <string>
#include <algorithm>
#include <vector>
#include <memory>


class CSubQuery {
public:
    virtual ~CSubQuery() = default;

    virtual std::vector<CEvent> Execute(std::string value, const CCalendar & calendar) = 0;

    virtual std::unique_ptr<CSubQuery> Clone() = 0;
};


