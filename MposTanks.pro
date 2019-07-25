#-------------------------------------------------
#
# Project created by QtCreator 2019-07-24T16:45:23
#
#-------------------------------------------------

QT       += core gui sql printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MposTanks
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
# QXlsx code for Application Qt project
QXLSX_PARENTPATH=./         # current QXlsx path is . (. means curret directory)
QXLSX_HEADERPATH=./header/  # current QXlsx header path is ./header/
QXLSX_SOURCEPATH=./source/  # current QXlsx source path is ./source/
include(./QXlsx.pri)

CONFIG += c++11

SOURCES += \
        ConnectionSettingDialog/connectionsettingdialog.cpp \
        DataBases/databases.cpp \
        DataBases/databasesettings.cpp \
        DataBases/options.cpp \
        LoggingCategories/loggingcategories.cpp \
        main.cpp \
        mainwindow.cpp \
        mymodel.cpp

HEADERS += \
        ConnectionSettingDialog/connectionsettingdialog.h \
        DataBases/databases.h \
        DataBases/databasesettings.h \
        DataBases/options.h \
        LoggingCategories/loggingcategories.h \
        mainwindow.h \
        mymodel.h

FORMS += \
        ConnectionSettingDialog/connectionsettingdialog.ui \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
