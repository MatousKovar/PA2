/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 5/10/23
 */

#pragma once

#include "CCommand.h"
#include <string>

/**
 * Class representing command used for adding events to calendar
 */
class CAddCommand : public CCommand {
public:
    CAddCommand() {}

    void Execute(CCalendar & calendar, const CInterface & interface) override;

    std::unique_ptr<CCommand> Clone() const override;
};


