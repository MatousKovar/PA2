/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 6/8/23
 */

#pragma once

#include "CSubQuery.h"
#include <vector>


class CSubQueryPeople : public CSubQuery {
public:
    ~CSubQueryPeople() override = default;

    std::vector<CEvent> Execute(std::string value, const CCalendar & calendar) override;

    std::unique_ptr<CSubQuery> Clone() override;
};

