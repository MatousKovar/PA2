/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 5/9/23
 */

#pragma once

#include "CCalendar.h"
#include <string>
#include <functional>
#include <memory>

/**
 * Abstract class for all commands
 * Methods explained below
 */
class CCommand {
public:
    CCommand() = default;

    virtual ~CCommand() = default;

    /**
     * Method for executing command
     * @param interface - interface which is used to print out result
     * @param calendar - calendar upon which is the command executed
     * @param Args - arguments of command (specification of event that should be added etc...)
     */
    virtual void Execute(CCalendar & calendar, const CInterface & interface) = 0;

    virtual std::unique_ptr<CCommand> Clone() const = 0;


};


