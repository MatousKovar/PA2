/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 5/8/23
 */

#pragma once

#include "CCalendar.h"
#include "CCommand.h"
#include "CInterface.h"
#include "CAddCommand.h"
#include "CRemoveCommand.h"
#include "CFindCommand.h"
#include "CQueryCommand.h"
#include "CSaveCommand.h"
#include "CViewCommand.h"
#include "CLoadCommand.h"
#include "CHelpCommand.h"
#include <string>
#include <map>
#include <memory>
#include <iostream>

class CApplication {
public:

    CApplication(std::string filename = "");

    ~CApplication() = default;

    CApplication(const CApplication & a) = delete;

    CApplication & operator=(const CApplication & other) = delete;

    void Run();

private:
    CCalendar m_Calendar;
    std::map<std::string, std::unique_ptr<CCommand> > m_Commands;
    std::unique_ptr<CInterface> m_Interface;
    std::string m_Filename;
};


