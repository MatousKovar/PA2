cmake_minimum_required(VERSION 3.24)
project(kovarm46)

set(CMAKE_CXX_STANDARD 17)

add_executable(kovarm46 src/main.cpp src/CCalendar.h src/CCalendar.cpp src/CEvent.h src/CDate.cpp src/CDate.h src/CEvent.h src/CApplication.cpp src/CApplication.h src/CInterface.cpp src/CInterface.h src/CView.h src/CViewDay.cpp src/CViewDay.h src/CViewWeek.cpp src/CViewWeek.h src/CViewMonth.cpp src/CViewMonth.h src/CCommand.h src/CQuery.cpp src/CQuery.h src/CEvent.cpp src/CAddCommand.h
        src/CSaveCommand.h src/CSaveCommand.cpp src/CRemoveCommand.h src/CRemoveCommand.cpp src/CLoadCommand.h src/CLoadCommand.cpp src/CFindCommand.h src/CFindCommand.cpp src/CAddCommand.cpp src/CAddCommand.h src/CCommand.cpp src/CViewCommand.cpp src/CViewCommand.h src/CViewManager.cpp src/CViewManager.h src/CFileManager.cpp src/CFileManager.h src/CQueryCommand.cpp src/CQueryCommand.h src/CSubQuery.cpp src/CSubQuery.h src/CSubQueryNote.cpp src/CSubQueryNote.h src/CSubQueryName.cpp src/CSubQueryName.h src/CSubQueryPlace.cpp src/CSubQueryPlace.h src/CSubQueryTag.cpp src/CSubQueryTag.h src/CSubQueryPeople.cpp src/CSubQueryPeople.h src/CSubQueryType.cpp src/CSubQueryType.h src/CHelpCommand.cpp src/CHelpCommand.h)
