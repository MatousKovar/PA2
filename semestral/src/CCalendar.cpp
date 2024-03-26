/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 6.5.2023
 */

#include "CCalendar.h"

bool CCalendar::Remove(const CInterface & interface) {
    try {
        std::string buffer = interface.ReadCommand("Starting date of event[dd.mm.yyyy hh:mm]:\n");
        CDate date = CEvent::GetDate(buffer);
        if (Remove(date, interface)) {
            interface.PrintPrompt("Event removed succesfully!\n");
            return true;
        } else {
            interface.PrintPrompt("Could not remove, event not found.\n");;
            return false;
        }
    }
    catch (std::invalid_argument & err) {
        interface.PrintPrompt(err.what());
        return false;
    }
    catch (std::logic_error & err) {
        interface.PrintPrompt(err.what());
        return false;
    }
}

bool CCalendar::Remove(const CDate & date, const CInterface & interface) {
    CEvent tmp(date, date.NextDay(), "", "", "", "", "", "");
    auto iter = std::lower_bound(m_Events.begin(), m_Events.end(), tmp);
    if (iter == m_Events.end())
        return false;
    if (!(iter->GetFrom() == date)) {
        return false;
    }
    m_Events.erase(iter);
    return true;

}

bool CCalendar::CheckForIntersection(const CEvent & event, std::vector<CEvent>::iterator iter) const {
    if ((m_Events.size() == 1) && (iter == m_Events.begin()))
        return event.Intersecting(*iter);
    if (iter == m_Events.begin())
        return event.Intersecting(*iter) || event.Intersecting(*(iter + 1));
    if (iter == m_Events.end())
        return event.Intersecting(*(iter - 1));
    else {
        return event.Intersecting(*iter) || event.Intersecting(*(iter + 1)) || event.Intersecting(*(iter - 1));
    }
}

bool CCalendar::Add(const CInterface & interface) {
    try {
        CEvent event = ReadEvent(interface);
        if (Add(event)) {
            interface.PrintPrompt("Event Added Succesfully!\n");
            RepeatEvent(interface, event);
            return true;
        } else {
            auto collision = GetEventsIn(event.GetFrom(), event.GetTo());
            interface.PrintPrompt("Cannot add event, collision with: " + collision[0].Print());
            return false;
        }
    }
    catch (std::invalid_argument & err) {
        interface.PrintPrompt(err.what());
        return false;
    }
    catch (std::logic_error & err) {
        interface.PrintPrompt(err.what());
        return false;
    }
}

bool CCalendar::Add(const CEvent & event) {
    if (m_Events.size() == 0) {
        m_Events.push_back(event);
        return true;
    }
    auto iter = std::lower_bound(m_Events.begin(), m_Events.end(), event);
    if (CheckForIntersection(event, iter)) {
        return false;
    }
    m_Events.insert(iter, event);
    return true;

}

CEvent CCalendar::ReadEvent(const CInterface & interface) const {
    CEvent event = CEvent::ReadEvent(interface);
    return event;
}

std::unique_ptr<CCalendar> CCalendar::Clone() const {
    return std::make_unique<CCalendar>(*this);
}

std::vector<CEvent> CCalendar::GetEventsIn(const CDate & from, const CDate & to) const {
    CEvent tmp(from, to, "", "", "", "", "", "");
    std::vector<CEvent> res;

    auto iter = std::lower_bound(m_Events.begin(), m_Events.end(),
                                 CEvent(from, from.NextDay(), "", "", "", "", "", ""));
    if (m_Events.size() == 0)
        return res;
    if (iter != m_Events.begin() && (iter - 1)->Intersecting(tmp))
        res.push_back(*(iter - 1));
    while (iter != m_Events.end() && iter->Intersecting(tmp)) {
        res.push_back(*iter);
        iter++;
    }
    return res;
}

std::vector<CEvent>::const_iterator CCalendar::Find(const CDate & date) const {
    auto iter = std::lower_bound(m_Events.begin(), m_Events.end(),
                                 CEvent(date, date.NextDay(), "", "", "", "", "", ""));
    if (m_Events.size() == 0) {
        return m_Events.end();
    }
    if (iter == m_Events.end() && date <= (iter - 1)->GetTo())
        return (iter - 1);
    if (iter != m_Events.end() && iter->GetFrom() == date) {
        return iter;
    } else if (iter != m_Events.begin() && date.IsInRange((iter - 1)->GetFrom(), (iter - 1)->GetTo())) {
        return iter;
    } else {
        return m_Events.end();
    }
}

bool CCalendar::Find(const CInterface & interface) const {
    try {
        std::string dateString = interface.ReadCommand("Input date when event starts[mm.dd.yyyy hh:mm]:\n");
        CDate date = CEvent::GetDate(dateString);
        auto iter = Find(date);
        if (iter == m_Events.end()) {
            interface.PrintPrompt("Could not find event on date: " + date.PrintDate() + "\n");
            return false;
        } else {
            interface.PrintPrompt("Found event " + (iter)->Print());
            return true;
        }
    }
    catch (std::invalid_argument & err) {
        interface.PrintPrompt(err.what());
        return false;
    }
    catch (std::logic_error & err) {
        interface.PrintPrompt(err.what());
        return false;
    }

}

bool CCalendar::SaveEvents(const CInterface & interface) const {
    return CFileManager::SaveEvents(interface, m_Events);
}

bool CCalendar::LoadEvents(const CInterface & interface) {
    try {
        std::vector<CEvent> events;
        CFileManager::LoadEvents(interface, events);
        for (size_t i = 0; i < events.size(); i++)
            this->Add(events[i]);
        return true;
    }
    catch (std::invalid_argument & err) {
        return false;
    }
    catch (std::runtime_error & err) {
        return false;
    }
}

bool CCalendar::LoadEvents(const std::string & filename, const CInterface & interface) {
    try {
        std::vector<CEvent> events;
        CFileManager::LoadEvents(interface, events, filename);
        for (size_t i = 0; i < events.size(); i++)
            this->Add(events[i]);
        return true;
    }
    catch (std::invalid_argument & err) {
        return false;
    }
    catch (std::runtime_error & err) {
        return false;
    }
}

std::vector<CEvent> CCalendar::GetByDate(const CDate & date) const {
    std::vector<CEvent> res;
    for (size_t i = 0; i < m_Events.size(); i++) {
        if (m_Events[i].GetFrom() == date) {
            res.push_back(m_Events[i]);
            return res;
        }
    }
    return res;
}

std::vector<CEvent> CCalendar::GetByName(const std::string & name) const {
    std::vector<CEvent> res;
    for (size_t i = 0; i < m_Events.size(); i++) {
        if (m_Events[i].GetName() == name) {
            res.push_back(m_Events[i]);
        }
    }
    return res;
}

std::vector<CEvent> CCalendar::GetByNote(const std::string & note) const {
    std::vector<CEvent> res;
    for (size_t i = 0; i < m_Events.size(); i++) {
        if (m_Events[i].GetNote() == note) {
            res.push_back(m_Events[i]);
        }
    }
    return res;
}

std::vector<CEvent> CCalendar::GetByPlace(const std::string & place) const {
    std::vector<CEvent> res;
    for (size_t i = 0; i < m_Events.size(); i++) {
        if (m_Events[i].GetPlace() == place) {
            res.push_back(m_Events[i]);
        }
    }
    return res;
}

std::vector<CEvent> CCalendar::GetByPeople(const std::string & people) const {
    std::vector<CEvent> res;
    for (size_t i = 0; i < m_Events.size(); i++) {
        if (m_Events[i].GetPeople() == people) {
            res.push_back(m_Events[i]);
        }
    }
    return res;
}

std::vector<CEvent> CCalendar::GetByType(const std::string & type) const {
    std::vector<CEvent> res;
    for (size_t i = 0; i < m_Events.size(); i++) {
        if (m_Events[i].GetType() == type) {
            res.push_back(m_Events[i]);
        }
    }
    return res;
}

std::vector<CEvent> CCalendar::GetByTag(const std::string & tag) const {
    std::vector<CEvent> res;
    for (size_t i = 0; i < m_Events.size(); i++) {
        if (m_Events[i].GetTag() == tag) {
            res.push_back(m_Events[i]);
        }
    }
    return res;
}

void CCalendar::RepeatEvent(const CInterface & interface, const CEvent & event) {
    std::string type = interface.ReadCommand("Do you want to repeat this event[y/n]?\n");
    if (type != "y")
        return;
    std::string frequency = interface.ReadCommand("How often do you want to repeat the event[day/week/month]?\n");
    std::string repeats = interface.ReadCommand("Number of repetitions?\n");
    try {
        int repeatsInt = std::stoi(repeats);
        if (frequency == "day")
            RepeatEventDay(event, repeatsInt);
        else if (frequency == "month")
            RepeatEventMonth(event, repeatsInt);
        else if (frequency == "week")
            RepeatEventWeek(event, repeatsInt);
    }
    catch (std::invalid_argument & err) {
        interface.PrintPrompt("Invalid number: \"" + repeats + "\"\n");
    }
}

void CCalendar::RepeatEventDay(const CEvent & event, int repeats) {
    CEvent tmp = event;
    int i = 0;
    while (i < repeats) {
        tmp = tmp.ShiftEventDay();
        Add(tmp);
        i++;
    }
}

void CCalendar::RepeatEventMonth(const CEvent & event, int repeats) {
    CEvent tmp = event;
    int i = 0;
    while (i < repeats) {
        tmp = tmp.ShiftEventMonth();
        Add(tmp);
        i++;
    }
}

void CCalendar::RepeatEventWeek(const CEvent & event, int repeats) {
    CEvent tmp = event;
    int i = 0;
    while (i < repeats) {
        tmp = tmp.ShiftEventWeek();
        Add(tmp);
        i++;
    }
}



