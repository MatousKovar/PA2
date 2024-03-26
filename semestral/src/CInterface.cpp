/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 5/8/23
 */

#include "CInterface.h"

std::string CInterface::ReadCommand() const {
    std::string buffer;
    std::getline(m_In, buffer);
    if (m_In.eof() || m_In.fail()) {
        m_In.clear();
        return "";
    }
    return buffer;
}

std::string CInterface::ReadCommand(const std::string & message) const {
    m_Out << message;
    return ReadCommand();
}

std::ostream & CInterface::PrintPrompt(const std::string & a) const {
    return m_Out << a;
}


std::ostream & CInterface::PrintTime(const int timeHour, const int timeMinute) const {
    return m_Out << std::setfill('0') << std::setw(2) << timeHour << ":" << std::setw(2) << std::setfill('0')
                 << timeMinute;
}

CInterface::CInterface(std::ostream & out, std::istream & in)
        : m_Out(out),
          m_In(in) {
    m_In.exceptions(std::ios::eofbit);
}
