#-------------------------------------------------
#
# Project created by QtCreator 2016-09-27T13:19:07
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = HMI_IEC_104
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    drawing.cpp \
    qcustomplot.cpp \
    date.cpp

HEADERS  += mainwindow.h \
    drawing.h \
    qcustomplot.h \
    date.h

FORMS    += mainwindow.ui \
    date.ui

DISTFILES += \
    img/bar-chart (2).png \
    img/brake-system-warning (1).png \
    img/car-battery (4).png \
    img/download (3).png \
    img/oie_PlRerqHzvvK4 (1).png \
    img/repairing-service (2).png \
    img/rounded-plug (2).png \
    img/signal (2).png \
    img/solar-battery (5).png \
    img/water-drop (3).png \
    img/weighing-scale (2).png \
    img/event-date-and-time-symbol (1).png \
    dbconfig.ini

RESOURCES += \
    image.qrc
