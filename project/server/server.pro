TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    ../../src/server/main.cpp \
    ../../src/server/server.cpp \
    ../../src/server/session.cpp \
    ../../src/common/package_fragments/package_fragment.cpp \
    ../../src/common/package_fragments/user_accepted.cpp \
    ../../src/common/utilities/endianness.cpp \
    ../../src/common/package_fragments/new_user.cpp

LIBS += -lpthread

HEADERS += \
    ../../src/server/server.hpp \
    ../../src/server/session.hpp \
    ../../src/common/package_fragments/package_fragment.hpp \
    ../../src/common/package_fragments/user_accepted.hpp \
    ../../src/common/utilities/endianness.hpp \
    ../../src/common/package_fragments/new_user.hpp

# Link boost libraries
unix|win32: LIBS += -lboost_system
unix|win32: LIBS += -lboost_thread

# http://qt-project.org/forums/viewthread/19989
CONFIG += c++11
