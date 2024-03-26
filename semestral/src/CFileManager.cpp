/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 6/6/23
 */

#include "CFileManager.h"


bool CFileManager::SaveEvents(const CInterface & interface, const std::vector<CEvent> & events) {
    std::string filename = interface.ReadCommand("Enter filename, to save events to: \n");
    std::ofstream out;
    out.open(filename);
    if (!out.is_open()) {
        interface.PrintPrompt("Unable to open file: " + filename);
        return false;
    }
    for (size_t i = 0; i < events.size(); i++) {
        out << events[i].SaveEvent();
        if (out.fail())
            return false;
    }
    out.close();
    return true;
}

std::string CFileManager::GetAttribute(std::stringstream & stream, const char delimiter) {
    std::string res;
    std::getline(stream, res, delimiter);
    if (res.front() != '"' || res.back() != '"')
        throw std::logic_error("Wrong file format\n");
    res = res.substr(1, res.size() - 2);
    return res;
}

CEvent CFileManager::ReadEvent(std::string line) {
    try {
        std::stringstream stream(line);
        CDate from = CEvent::GetDate(CFileManager::GetAttribute(stream, ';'));
        CDate to = CEvent::GetDate(CFileManager::GetAttribute(stream, ';'));
        std::string note = CFileManager::GetAttribute(stream, ';');
        std::string type = CFileManager::GetAttribute(stream, ';');
        std::string place = CFileManager::GetAttribute(stream, ';');
        std::string tag = CFileManager::GetAttribute(stream, ';');
        std::string name = CFileManager::GetAttribute(stream, ';');
        std::string people = CFileManager::GetAttribute(stream, '\n');
        return CEvent(from, to, note, type, place, tag, name, people);
    }
    catch (std::logic_error & err) {
        throw std::logic_error("Invalid input: " + line + "\n");
    }
}

bool CFileManager::LoadEvents(const CInterface & interface, std::vector<CEvent> & events) {
    std::string filename = interface.ReadCommand("Enter filename, to load events from:\n");
    try {
        LoadEvents(interface, events, filename);
        return true;
    }
    catch (std::runtime_error & err) {
        interface.PrintPrompt(err.what());
        return false;
    }
    catch (std::logic_error & err) {
        interface.PrintPrompt(err.what());
        return false;
    }
}

bool CFileManager::LoadEvents(const CInterface & interface, std::vector<CEvent> & events, std::string filename) {
    std::ifstream in;
    in.open(filename);
    if (!in.is_open()) {
        interface.PrintPrompt("Unable to open file: " + filename + "\n");
        return false;
    }

    std::string line;
    while (std::getline(in, line)) {
        if (in.fail()) {
            in.close();
            throw (std::runtime_error("Unexpectedly broken stream\n"));
        }
        CEvent event = CFileManager::ReadEvent(line);
        events.push_back(event);
    }
    in.close();
    interface.PrintPrompt("Events loaded succesfully!\n");
    return true;
}


