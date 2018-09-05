QT       += core

TARGET = GoldbachTester
TEMPLATE = app
CONFIG += console c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    goldbachworker.cpp \
    goldbachcalculator.cpp \
    test.cpp \
    goldbachtester.cpp

HEADERS += \
    goldbachworker.h \
    goldbachcalculator.h \
    goldbachtester.h
