/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 6/8/23
 */

#pragma once

#include "CQuery.h"
#include "CCommand.h"
#include <string>
#include <memory>

class CQueryCommand : public CCommand {

public:
    CQueryCommand() { query = std::make_unique<CQuery>(); }

    ~CQueryCommand() override = default;

    void Execute(CCalendar & calendar, const CInterface & interface) override;

    std::unique_ptr<CCommand> Clone() const override;

private:
    std::unique_ptr<CQuery> query;
};


