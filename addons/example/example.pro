#! [0]
TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets
INCLUDEPATH    += $$PWD/../../pdk/include
HEADERS         = plugin.h \
    exampleaddon.h
SOURCES         =
DESTDIR         = $$OUT_PWD/../../build/addons
#! [0]

EXAMPLE_FILES = plugin.json

