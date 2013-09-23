TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    src/main.cpp \
    src/server.cpp

LIBS += -lpthread

# Boost system
unix:!macx: LIBS += -L$$PWD/../../../../../../../../../usr/local/lib/ -lboost_system

INCLUDEPATH += $$PWD/../../../../../../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../../../../../../usr/local/include

unix:!macx: PRE_TARGETDEPS += $$PWD/../../../../../../../../../usr/local/lib/libboost_system.a

# Boost thread
unix:!macx: LIBS += -L$$PWD/../../../../../../../../../usr/local/lib/ -lboost_thread

INCLUDEPATH += $$PWD/../../../../../../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../../../../../../usr/local/include

unix:!macx: PRE_TARGETDEPS += $$PWD/../../../../../../../../../usr/local/lib/libboost_thread.a

HEADERS += \
    src/server.hpp
