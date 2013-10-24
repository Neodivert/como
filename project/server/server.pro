TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    ../../src/server/main.cpp \
    ../../src/server/server.cpp \
    ../../src/server/session.cpp \
    ../../src/common/utilities/endianness.cpp \
    ../../src/common/packets/user_accepted.cpp \
    ../../src/common/packets/packet.cpp \
    ../../src/common/packets/packable.cpp \
    ../../src/common/packets/new_user.cpp

LIBS += -lpthread

HEADERS += \
    ../../src/server/server.hpp \
    ../../src/server/session.hpp \
    ../../src/common/utilities/endianness.hpp \
    ../../src/common/packets/user_accepted.hpp \
    ../../src/common/packets/packet.hpp \
    ../../src/common/packets/packable.hpp \
    ../../src/common/packets/new_user.hpp \
    ../../src/common/packets/packets.hpp

# Link boost libraries
unix|win32: LIBS += -lboost_system
unix|win32: LIBS += -lboost_thread

# http://qt-project.org/forums/viewthread/19989
CONFIG += c++11
