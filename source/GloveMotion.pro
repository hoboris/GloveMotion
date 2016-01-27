#-------------------------------------------------
#
# Project created by QtCreator 2015-03-22T17:06:56
#
#-------------------------------------------------

QT       += core gui
QT       += serialport
QT       += multimedia multimediawidgets

# QMAKE_CXXFLAGS += -std=c++0x

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GloveMotion
TEMPLATE = app


SOURCES += main.cpp\
    View/Widgets/videoplayerwidget.cpp \
    View/Widgets/homewidget.cpp \
    View/Widgets/gallerywidget.cpp \
    View/Widgets/loadingwidget.cpp \
    View/Widgets/settingswidget.cpp \
    Model/ldrvalue.cpp \
    View/mainwindow.cpp \
    View/modulewidget.cpp \
    Model/mpuvalues.cpp \
    Model/sensordata.cpp \
    Controller/sensorthread.cpp \
    View/Widgets/datawidget.cpp \
    View/Widgets/menuwidget.cpp \
    View/Widgets/desktopwidget.cpp

HEADERS  += \
    View/Widgets/homewidget.h \
    View/Widgets/gallerywidget.h \
    View/Widgets/loadingwidget.h \
    View/Widgets/settingswidget.h \
    View/Widgets/videoplayerwidget.h \
    View/mainwindow.h \
    View/modulewidget.h \
    Model/ldrvalue.h \
    Model/mpuvalues.h \
    Model/sensordata.h \
    Controller/sensorthread.h \
    View/Widgets/datawidget.h \
    View/Widgets/menuwidget.h \
    View/Widgets/desktopwidget.h

FORMS    += \
    View/Widgets/datawidget.ui

RESOURCES += \
    resources.qrc

DISTFILES += \
    icon.rc

RC_FILE += \
    icon.rc
