/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 5/10/23
 */

#pragma once

#include "CCommand.h"

/**
 * Command used for finding events in given date
 */
class CFindCommand : public CCommand {
public:
    CFindCommand() {}

    void Execute(CCalendar & calendar, const CInterface & interface) override;

    std::unique_ptr<CCommand> Clone() const override;

};


