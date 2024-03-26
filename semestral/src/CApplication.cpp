/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 5/8/23
 */

#include "CApplication.h"

CApplication::CApplication(std::string filename) {
    m_Commands.emplace("add", CAddCommand().Clone());
    m_Commands.emplace("remove", CRemoveCommand().Clone());
    m_Commands.emplace("find", CFindCommand().Clone());
    m_Commands.emplace("view", CViewCommand().Clone());
    m_Commands.emplace("save", CSaveCommand().Clone());
    m_Commands.emplace("load", CLoadCommand().Clone());
    m_Commands.emplace("query", CQueryCommand().Clone());
    m_Commands.emplace("help", CHelpCommand().Clone());
    m_Filename = filename;
}

void CApplication::Run() {
    //m_Calendar = CCalendar();
    CInterface m_Interface(std::cout, std::cin);
    m_Interface.PrintPrompt("Welcome to your calendar, type \"help\" to see available commands.\n");
    if (m_Filename != "")
        m_Calendar.LoadEvents(m_Filename, m_Interface);
    try {
        while (true) {
            std::string command = m_Interface.ReadCommand("Enter command:\n");
            if (command == "exit")
                break;
            if (m_Commands.find(command) == m_Commands.end()) {
                m_Interface.PrintPrompt("Invalid command: \"" + command + "\"\n");
                continue;
            }
            m_Commands[command]->Execute(m_Calendar, m_Interface);
        }
    }
    catch (std::exception & err) {
        m_Interface.PrintPrompt("Encountered EOF - Exiting.\n");
    }
}


