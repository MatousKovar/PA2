/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 6/10/23
 */

#pragma once

#include <string>
#include "CCommand.h"

class CHelpCommand : public CCommand {
public:
    ~CHelpCommand() override = default;

    void Execute(CCalendar & calendar, const CInterface & interface) override;

    std::unique_ptr<CCommand> Clone() const override;
};


