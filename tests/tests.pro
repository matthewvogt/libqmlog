TEMPLATE = app
QT -= Gui
TARGET = logtest
INSTALLS = target tests

LIBS += -lqmlog -L../src

INCLUDEPATH += ../H

SOURCES = logtest.cpp

tests.path = /usr/share/qmlog-tests
tests.files = tests.xml

target.path = /usr/bin
