/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 5/10/23
 */

#include "CEvent.h"

bool CEvent::IsDigits(const std::string & str) {
    return str.find_first_not_of("0123456789 \n.:") == std::string::npos;
}


CEvent::CEvent(const CDate & mFrom, const CDate & mTo, const std::string & mNote, const std::string & mType,
               const std::string & mPlace, const std::string & mTag, const std::string & mName,
               const std::string & mPeople) {
    if (!(mFrom < mTo))
        throw (std::logic_error("From date is not less than To date.\n"));
    m_From = std::move(mFrom);
    m_To = std::move(mTo);
    m_Note = std::move(mNote);
    m_Type = std::move(mType);
    m_Place = std::move(mPlace);
    m_Tag = std::move(mTag);
    m_Name = std::move(mName);
    m_People = std::move(mPeople);
}

std::string CEvent::Print() const {
    std::stringstream stream;
    stream << "From: \"" << m_From << "\" To: \"" << m_To << "\" Name: \"" << m_Name << "\" Type: \"" << m_Type
           << "\" Place: \"" << m_Place <<
           "\" Note: \"" << m_Note << "\" People: \""<< m_People << "\" TAG: \"" << m_Tag << "\"" << std::endl;
    return stream.str();
}

bool CEvent::operator<(const CEvent & other) const {
    return this->m_From < other.m_From;
}

bool CEvent::Intersecting(const CEvent & other) const {
    if (m_From == other.m_From || m_To == other.m_To || m_From == other.m_To || m_To == other.m_From)
        return true;
    if ((other.m_From <= m_From && m_From <= other.m_To) || (m_From <= other.m_From && other.m_From <= m_To))
        return true;
    if ((other.m_From <= m_To && m_To <= other.m_To) || (m_From <= other.m_To && other.m_To <= m_To))
        return true;
    if ((other.m_From <= m_From && m_To <= other.m_To) || (m_From <= other.m_From && other.m_To <= m_To))
        return true;
    return false;
}

const std::string & CEvent::GetNote() const {
    return m_Note;
}

const std::string & CEvent::GetType() const {
    return m_Type;
}

const std::string & CEvent::GetPlace() const {
    return m_Place;
}

const std::string & CEvent::GetTag() const {
    return m_Tag;
}

const std::string & CEvent::GetName() const {
    return m_Name;
}

const std::string & CEvent::GetPeople() const {
    return m_People;
}


int CEvent::GetAttribute(std::stringstream & stream, const char delimiter) {
    std::string attrString;
    try {
        std::getline(stream, attrString, delimiter);
        int attrInt = std::stoi(attrString);
        return attrInt;
    }
    catch (std::invalid_argument &) {
        throw std::invalid_argument("Invalid Date attribute: \"" + attrString + "\"\n");
    }
}

CDate CEvent::GetDate(const std::string & date) {
    std::stringstream stream(date);
    if (!IsDigits(date))
        throw (std::invalid_argument("Invalid Date: " + date + "\n"));
    int day = GetAttribute(stream, '.');
    int month = GetAttribute(stream, '.');
    int year = GetAttribute(stream, ' ');
    int hour = GetAttribute(stream, ':');
    int minute = GetAttribute(stream, '\n');
    return CDate(year, month, day, hour, minute);

}

CEvent CEvent::ReadEvent(const CInterface & interface) {
    std::string buffer;
    CDate from = GetDate(interface.ReadCommand("Input date, when event starts: [dd.mm.yyyy hh:mm]\n"));
    CDate to = GetDate(interface.ReadCommand("Input date, when event ends: [dd.mm.yyyy hh:mm]\n"));

    std::string name = interface.ReadCommand("Input name of event:\n");
    std::string type = interface.ReadCommand("Input type of event:\n");
    std::string place = interface.ReadCommand("Input place of event:\n");
    std::string note = interface.ReadCommand("Input note for event:\n");
    std::string tag = interface.ReadCommand("Input TAG of event:\n");
    std::string people = interface.ReadCommand("Input people participating in event:\n");

    CEvent event(from, to, note, type, place, tag, name, people);
    return event;
}

const CDate & CEvent::GetFrom() const {
    return m_From;
}

const CDate & CEvent::GetTo() const {
    return m_To;
}

void CEvent::PrintDayEvent(const CInterface & interface) const {

    interface.PrintPrompt(" Name: \"" + m_Name + "\" Type: \"" + m_Type + "\" Place: \"" + m_Place +
                          "\" Note: \"" + m_Note + "\" TAG: \"" + m_Tag + "\"\n");
}

void CEvent::PrintWeekEvent(const CInterface & interface) const {
    //if event happens in just one day
    if (m_From.GetYear() == m_To.GetYear() && m_From.GetMonth() == m_To.GetMonth() && m_From.GetDay() == m_To.GetDay())
        interface.PrintPrompt(
                m_From.DayOfWeek() + ": " + "Name: \"" + m_Name + "\" From: " + m_From.PrintJustTime() + " To: " +
                m_To.PrintJustTime() + " Type: \"" + m_Type + "\"" + " Place: \"" + m_Place + "\"\n");
    else {
        interface.PrintPrompt(
                m_From.DayOfWeek() + ": " + "Name: \"" + m_Name + "\" From: " + m_From.PrintDate() + " Type: \"" +
                m_Type + "\"" + " Place: \"" + m_Place + "\"\n");
        interface.PrintPrompt(".\n.\nTo: " + m_To.DayOfWeek() + " " + m_To.PrintDate() + "\n");
    }
}

void CEvent::PrintMonthEvent(const CInterface & interface) const {
    if (m_From.GetYear() == m_To.GetYear() && m_From.GetMonth() == m_To.GetMonth() &&
        m_From.GetDay() == m_To.GetDay()) {
        interface.PrintPrompt(
                m_From.PrintJustDateNoYear() + ": Name: \"" + m_Name + " From: " + m_From.PrintJustTime()
                + " To: " + m_To.PrintJustTime() + "\n");
    } else {
        interface.PrintPrompt(
                m_From.PrintJustDateNoYear() + ": " + "Name: \"" + m_Name + " From: " + m_From.PrintJustTime() +
                " Type: \"" + m_Type + "\"" + " Place: \"" + m_Place + "\"\n");
        interface.PrintPrompt(".\n.\nTo: " + m_To.PrintDate() + "\n");
    }
}


std::string CEvent::SaveEvent() const {
    std::string res =
            "\"" + m_From.PrintDate() + "\";\"" + m_To.PrintDate() + "\";\"" + m_Note + "\";\"" + m_Type + "\";\"" +
            m_Place + "\";\"" + m_Tag + "\";\""
            + m_Name + "\";\"" + m_People + "\"\n";
    return res;
}

CEvent CEvent::ShiftEventDay() const {
    return CEvent(m_From.NextDayKeepTime(), m_To.NextDayKeepTime(), m_Note, m_Type, m_Place, m_Tag, m_Name, m_People);
}

CEvent CEvent::ShiftEventWeek() const {
    return CEvent(m_From.DateWeekLaterKeepTime(), m_To.DateWeekLaterKeepTime(), m_Note, m_Type, m_Place, m_Tag, m_Name,
                  m_People);
}

CEvent CEvent::ShiftEventMonth() const {
    return CEvent(m_From.DateMonthLaterKeepTime(), m_To.DateMonthLaterKeepTime(), m_Note, m_Type, m_Place, m_Tag,
                  m_Name, m_People);
}

