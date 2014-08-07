TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

# Include files and parameters that are common to both client and server.
include( ../common/common.pri )

INCLUDEPATH += ../../src/

LIBS += -lboost_unit_test_framework

SOURCES += \
    ../../src/tests/common/ids/test_resource_id.cpp \
    ../../src/tests/common/packables/packable_integers.cpp \
    ../../src/tests/common/tests_main.cpp

HEADERS +=

