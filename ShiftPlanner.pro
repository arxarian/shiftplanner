QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

INCLUDEPATH += 3rdparty/or-tools/include

unix {
    LIBS += -L"$$PWD/3rdparty/or-tools/lib" -lortools
}

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    sources/main.cpp \
    sources/mainwindow.cpp \
    sources/planner/planner.cpp \
    sources/workers/workeritem.cpp \
    sources/workers/workermodel.cpp \
    sources/skills/skillitem.cpp \
    sources/skills/skillmodel.cpp \
    sources/skills/skilltablemodel.cpp \
    sources/availability/availabilityitem.cpp \
    sources/availability/availabilitymodel.cpp \
    sources/availability/availabilitytablemodel.cpp \
    sources/schedule/scheduletablemodel.cpp \
    sources/shifts/shiftstablemodel.cpp \
    sources/shifts/shiftitem.cpp \
    sources/shifts/shiftmodel.cpp

HEADERS += \
    sources/global.h \
    sources/mainwindow.h \
    sources/planner/planner.h \
    sources/workers/workeritem.h \
    sources/workers/workermodel.h \
    sources/skills/skillitem.h \
    sources/skills/skillmodel.h \
    sources/skills/skilltablemodel.h \
    sources/availability/availabilityitem.h \
    sources/availability/availabilitymodel.h \
    sources/availability/availabilitytablemodel.h \
    sources/schedule/scheduletablemodel.h \
    sources/shifts/shiftstablemodel.h \
    sources/shifts/shiftitem.h \
    sources/shifts/shiftmodel.h

FORMS += \
    sources/mainwindow.ui

TRANSLATIONS += \
    ShiftPlanner_cs_CZ.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
