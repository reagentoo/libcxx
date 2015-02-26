TARGET = libcxx
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

#QMAKE_CC = clang
#QMAKE_CXX = clang++
#QMAKE_LINK = clang++
QMAKE_CC = gcc-4.9.2
QMAKE_CXX = g++-4.9.2
QMAKE_LINK = g++-4.9.2

#QMAKE_CXXFLAGS += -std=c++0x
QMAKE_CXXFLAGS += -std=c++1y

INCLUDEPATH += include

HEADERS += \
    include/backend.h \
    include/global.h \
    include/mux.h \
    include/same.h \
    include/tuple.h \
    include/type_traits.h \
    include/value_traits.h \
    include/variadic_traits.h \
    include/range.h

SOURCES += tests/tuple.cpp
