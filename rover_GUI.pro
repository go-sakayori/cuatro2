#-------------------------------------------------
#
# Project created by QtCreator 2016-09-21T17:33:56
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = rover_GUI
TEMPLATE = app

INCLUDEPATH += /usr/local/include/opencv
LIBS += -L/usr/local/lib -lopencv_core -lopencv_imgcodecs -lopencv_highgui
LIBS += -L/usr/local/lib -lGLU

#This is for qwt plugin
include(/home/go/Qt5.7.0/qwt-6.1.3/qwt.prf)
CONFIG += qwt

SOURCES += main.cpp\
        mainwindow.cpp \
    pointcloud.cpp \
    righteye.cpp \
    lefteye.cpp \
    roll.cpp \
    pitch.cpp \
    yaw.cpp \
    rover_3d.cpp

HEADERS  += mainwindow.h \
    pointcloud.h \
    righteye.h \
    lefteye.h \
    roll.h \
    pitch.h \
    yaw.h \
    rover_3d.h

FORMS    += mainwindow.ui
