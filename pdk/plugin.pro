#! [0]
TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets sql
INCLUDEPATH    += $$PWD/include
HEADERS         = plugin.h
SOURCES         = plugin.cpp
DESTDIR         = $$OUT_PWD/../../build/addons
#! [0]

EXAMPLE_FILES = plugin.json
