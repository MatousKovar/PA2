/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 6.5.2023
 */

#include "CDate.h"

bool CDate::operator<(const CDate & other) const {
    if (m_Year != other.m_Year)
        return m_Year < other.m_Year;
    if (m_Month != other.m_Month)
        return m_Month < other.m_Month;
    if (m_Day != other.m_Day)
        return m_Day < other.m_Day;
    if (m_Hour != other.m_Hour)
        return m_Hour < other.m_Hour;
    return m_Minute < other.m_Minute;
}

/**
 * Function goes from general incorrect dates, to more specific
 * first layer
 * second layer - all months that always have 31 days and pass first layer are valid
 * third layer - all 30 day months
 * fourth layer - leap years source: https://learn.microsoft.com/en-us/office/troubleshoot/excel/determine-a-leap-year
 * @param year
 * @param month
 * @param day
 * @param hour
 * @param minute
 * @return
 */
bool CDate::ValidateDate(unsigned int year, unsigned int month, unsigned int day, unsigned int hour,
                         unsigned int minute) const {
    if (day == 0 || day > 31 || hour >= 24 || minute >= 60 || month > 12 || month == 0)
        return false;
    if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
        return true;
    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30)
        return false;
    if (((month == 2) && IsLeap(year) && day > 29) || ((month == 2) && !IsLeap(year) && day > 28))
        return false;
    return true;
}

/**
 * Helper method for ValidateDate method
 * @param year
 * @return
 */
bool CDate::IsLeap(unsigned int year) {
    if (year % 4 == 0) {
        if (year % 100 == 0) {
            if (year % 400 == 0)
                return true;
            return false;
        } else
            return true;
    } else
        return false;
}


CDate CDate::NextDay() const {
    if (ValidateDate(m_Year, m_Month, m_Day + 1, m_Hour, m_Minute))
        return CDate(m_Year, m_Month, m_Day + 1, 0, 0);
    if (ValidateDate(m_Year, m_Month + 1, 1, 0, 0))
        return CDate(m_Year, m_Month + 1, 1, 0, 0);
    return CDate(m_Year + 1, 1, 1, 0, 0);
}

std::ostream & operator<<(std::ostream & out, const CDate & date) {
    return out << date.m_Day << "." << date.m_Month << "." << date.m_Year << " " << std::setw(2) << std::setfill('0')
               << date.m_Hour << ":" << std::setw(2) << std::setfill('0') << date.m_Minute;
}

bool CDate::operator==(const CDate & other) const {
    return !(*this < other) && !(other < *this);
}

bool CDate::operator<=(const CDate & other) const {
    return (*this < other) || (*this == other);
}

std::string CDate::PrintJustDate() const {
    std::stringstream stream;
    stream << m_Day << "." << m_Month << "." << m_Year;
    return stream.str();
}

std::string CDate::PrintDate() const {
    std::stringstream stream;
    stream << m_Day << "." << m_Month << "." << m_Year << " " << std::setw(2) << std::setfill('0') << m_Hour << ":"
           << std::setw(2) << std::setfill('0') << m_Minute;
    return stream.str();
}

std::string CDate::PrintJustTime() const {
    std::stringstream stream;
    stream << std::setw(2) << std::setfill('0') << m_Hour << ":" << std::setw(2) << std::setfill('0') << m_Minute;
    return stream.str();
}

std::string CDate::PrintJustDateNoYear() const {
    return std::string(std::to_string(m_Day) + "." + std::to_string(m_Month));
}

/*
 * Source: https://www.geeksforgeeks.org/find-day-of-the-week-for-a-given-date/
 */
std::string CDate::DayOfWeek() const {
    int year = m_Year;
    std::string names[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday",
                           "Saturday"};
    int t[] = {0, 3, 2, 5, 0, 3,
               5, 1, 4, 6, 2, 4};
    year -= m_Month < 3;
    return names[(year + year / 4 - year / 100 +
                  year / 400 + t[m_Month - 1] + m_Day) % 7];
}


bool CDate::IsInRange(const CDate & from, const CDate & to) const {
    return *this <= to && from <= *this;
}

CDate CDate::DateWeekLater() const {
    CDate res = *this;
    for (int i = 0; i < 7; i++)
        res = res.NextDay();
    return res;
}

CDate CDate::DateMonthLater() const {
    if (IsLeap(m_Year) && m_Month == 2 && m_Day == 29)
        return CDate(m_Year, 3, 28, 0, 0);
    if (!ValidateDate(m_Year, m_Month + 1, m_Day, 0, 0))
        return CDate(m_Year + 1, 1, m_Day, 0, 0);
    return CDate(m_Year, m_Month + 1, m_Day, 0, 0);
}

CDate & CDate::operator++() {
    if (ValidateDate(m_Year, m_Month, m_Day, m_Hour, m_Minute + 1)) {
        m_Minute++;
        return *this;
    }
    if (ValidateDate(m_Year, m_Month, m_Day, m_Hour + 1, 0)) {
        m_Minute = 0;
        m_Hour++;
        return *this;
    }
    if (ValidateDate(m_Year, m_Month, m_Day + 1, 0, 0)) {
        m_Day++;
        m_Minute = 0;
        m_Hour = 0;
        return *this;
    }
    if (ValidateDate(m_Year, m_Month + 1, 1, 0, 0)) {
        m_Month++;
        m_Day = 1;
        m_Minute = 0;
        m_Hour = 0;
        return *this;
    } else {
        m_Year++;
        m_Month = 1;
        m_Day = 1;
        m_Minute = 0;
        m_Hour = 0;
        return *this;
    }
}

CDate CDate::NextDayKeepTime() const {
    if (ValidateDate(m_Year, m_Month, m_Day + 1, m_Hour, m_Minute))
        return CDate(m_Year, m_Month, m_Day + 1, m_Hour, m_Minute);
    if (ValidateDate(m_Year, m_Month + 1, 1, m_Hour, m_Minute))
        return CDate(m_Year, m_Month + 1, 1, m_Hour, m_Minute);
    return CDate(m_Year + 1, 1, 1, m_Hour, m_Minute);
}

CDate CDate::DateWeekLaterKeepTime() const {
    CDate res = *this;
    for (int i = 0; i < 7; i++)
        res = res.NextDayKeepTime();
    return res;
}

CDate CDate::DateMonthLaterKeepTime() const {
    if (IsLeap(m_Year) && m_Month == 2 && m_Day == 29)
        return CDate(m_Year, 3, 28, m_Hour, m_Minute);
    if (!ValidateDate(m_Year, m_Month + 1, m_Day, m_Hour, m_Minute))
        return CDate(m_Year + 1, 1, m_Day, m_Hour, m_Minute);
    return CDate(m_Year, m_Month + 1, m_Day, m_Hour, m_Minute
    );
}

CDate::CDate(unsigned int year, unsigned int month, unsigned int day, unsigned int hour, unsigned int minute) {
    std::string date = std::to_string(day) + "." + std::to_string(month) + "." + std::to_string(year)
                       + " " + std::to_string(hour) + ":" + std::to_string(minute);
    if (!ValidateDate(year, month, day, hour, minute))
        throw std::logic_error("Invalid date: " + date + "\n");
    if (year > 9999 || year < 1000)
        throw std::out_of_range("Year out of range " + std::to_string(year));
    m_Year = year;
    m_Month = month;
    m_Day = day;
    m_Hour = hour;
    m_Minute = minute;
}





