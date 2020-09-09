QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
TARGET = MPN-Info
DESTDIR = $$OUT_PWD/../build

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

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
