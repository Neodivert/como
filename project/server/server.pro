TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    src/main.cpp \
    src/server.cpp \
    src/session.cpp

LIBS += -lpthread

HEADERS += \
    src/server.hpp \
    src/session.hpp

# Link boost libraries
unix|win32: LIBS += -lboost_system
unix|win32: LIBS += -lboost_thread

# http://qt-project.org/forums/viewthread/19989
CONFIG += c++11
