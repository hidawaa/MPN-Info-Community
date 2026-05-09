QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
TARGET  = MPN-Info
VERSION = 0.20.0

DESTDIR = $$OUT_PWD/../build

# Windows executable metadata (Properties > Details)
win32 {
    QMAKE_TARGET_COMPANY     = "Direktorat Jenderal Pajak"
    QMAKE_TARGET_DESCRIPTION = "MPN Info Community"
    QMAKE_TARGET_COPYRIGHT   = "Copyright 2024"
    QMAKE_TARGET_PRODUCT     = "MPN-Info"
    RC_LANG                  = 0x0421  # Indonesian
}

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# Fail to compile if deprecated Qt APIs (before Qt 6.0) are used.
# This enforces full Qt6 compatibility.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

INCLUDEPATH +=../pdk/include

SOURCES += \
    src/aboutdialog.cpp \
    src/ccommon.cpp \
    src/cdata.cpp \
    src/cdatabase.cpp \
    src/cdatabasecompatibility.cpp \
    src/changelogdialog.cpp \
    src/checkedheaderview.cpp \
    src/databaseconnectiondialog.cpp \
    src/databaseinfo.cpp \
    src/databasesettings.cpp \
    src/engine.cpp \
    src/klusearchdialog.cpp \
    src/kluselectdialog.cpp \
    src/logindialog.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/mapsearchdialog.cpp \
    src/passwordchangedialog.cpp \
    src/pegawaimanagedialog.cpp \
    src/pegawaimultipleselectdialog.cpp \
    src/pegawaiselectdialog.cpp \
    src/pegawaiupdatedialog.cpp \
    src/seksimanagedialog.cpp \
    src/seksiupdatedialog.cpp \
    src/settingsdialog.cpp \
    src/simplecrypt.cpp \
    src/usermanagedialog.cpp \
    src/userupdatedialog.cpp

HEADERS += \
    src/aboutdialog.h \
    src/ccommon.h \
    src/cdata.h \
    src/cdatabase.h \
    src/cdatabasecompatibility.h \
    src/changelogdialog.h \
    src/checkedheaderview.h \
    src/databaseconnectiondialog.h \
    src/databaseinfo.h \
    src/databasesettings.h \
    src/engine.h \
    src/klusearchdialog.h \
    src/kluselectdialog.h \
    src/logindialog.h \
    src/mainwindow.h \
    src/mapsearchdialog.h \
    src/passwordchangedialog.h \
    src/pegawaimanagedialog.h \
    src/pegawaimultipleselectdialog.h \
    src/pegawaiselectdialog.h \
    src/pegawaiupdatedialog.h \
    src/seksimanagedialog.h \
    src/seksiupdatedialog.h \
    src/settingsdialog.h \
    src/simplecrypt.h \
    src/usermanagedialog.h \
    src/userupdatedialog.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
