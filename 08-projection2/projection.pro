#-------------------------------------------------
#
# Project created by QtCreator 2012-02-18T18:32:28
#
#-------------------------------------------------

QT += core gui

TEMPLATE = app

TARGET = bin/projection

win32 {
	CONFIG += release
}

# Anti-trash
OBJECTS_DIR = tmp
MOC_DIR = tmp
UI_DIR = tmp
RCC_DIR = tmp

SOURCES += src/main.cpp\
    src/mainwindow.cpp \
    src/scene.cpp \
    src/projection.cpp

HEADERS  += src/mainwindow.h \
    src/scene.h \
    src/projection.h
