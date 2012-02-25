#-------------------------------------------------
#
# Project created by QtCreator 2012-02-18T18:32:28
#
#-------------------------------------------------

QT += core gui

TEMPLATE = app

win32 {
	TARGET = bin/color-rect
	CONFIG += release
}
unix {
	TARGET = bin/color-rect
}

# Anti-trash
OBJECTS_DIR = tmp
MOC_DIR = tmp
UI_DIR = tmp
RCC_DIR = tmp

SOURCES += src/main.cpp\
    src/mainwindow.cpp \
    src/scene.cpp \
    src/rect.cpp \
    src/regeneratedialog.cpp \
    src/quadratictree.cpp

HEADERS  += src/mainwindow.h \
    src/scene.h \
    src/rect.h \
    src/regeneratedialog.h \
    src/quadratictree.h

FORMS += \
    ui/regeneratedialog.ui
