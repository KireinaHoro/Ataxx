#-------------------------------------------------
#
# Project created by QtCreator 2017-11-25T11:48:24
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Ataxx
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


SOURCES += \
        main.cpp \
    gameai.cpp \
    gameprocess.cpp \
    botzonerunner.cpp \
    mainwindow.cpp \
    clickablelabel.cpp \
    fixedratiowidget.cpp \
    highscoredialog.cpp \
    ataxxplayerthread.cpp \
    aiataxxplayer.cpp

HEADERS += \
    gameai.h \
    gameprocess.h \
    botzonerunner.h \
    mainwindow.h \
    clickablelabel.h \
    fixedratiowidget.h \
    highscore_columns.h \
    highscoredialog.h \
    utils.h \
    iataxxplayer.h \
    ataxxplayerthread.h \
    aiataxxplayer.h \
    connect.h

FORMS +=

CONFIG += c++14

RESOURCES += \
    images.qrc

TRANSLATIONS += \
    languages/Translation_en_US.ts \
    languages/Translation_ja_JP.ts \
    languages/Translation_zh_TW.ts
