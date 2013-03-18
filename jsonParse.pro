#-------------------------------------------------
#
# Project created by QtCreator 2012-12-09T18:39:52
#
#-------------------------------------------------

QT       += core gui network sql

TARGET = jsonParse
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        title.cpp \
        qxtjson.cpp \
        filedownloader.cpp \
        md5.cpp \
        serie.cpp \
        secondwindow.cpp

HEADERS  += mainwindow.h \
            title.h \
            qxtjson.h \
            filedownloader.h \
            md5.h \
            serie.h \
            secondwindow.h

FORMS    += mainwindow.ui \
            secondwindow.ui

REVISION = $$system(git rev-parse HEAD)

DEFINES += APP_REVISION=\\\"$$REVISION\\\"
