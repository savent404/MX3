#-------------------------------------------------
#
# Project created by QtCreator 2018-07-27T17:00:20
#
#-------------------------------------------------

QT       += core gui widgets multimedia

TARGET = UI
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

CONFIG += c++11 debug
QMAKE_CXXFLAGS_DEBUG += -pg
QMAKE_LFLAGS_DEBUG += -pg

DEFINES += USE_QT=1
INCLUDEPATH += \
        ../MXSchedule/Inc \
        ../MXSchedule/Interface
SOURCES += \
        main.cpp \
        mainwindow.cpp \
        ../MXSchedule/Src/common-const.cpp \
        ../MXSchedule/Src/iParam.cpp \
        ../MXSchedule/Src/iSchedule.cpp \
        ../MXSchedule/Src/re.c \
        ../MXSchedule/Src/textHelper.cpp \
        ../MXSchedule/Src/iPower.cpp \
        ../MXSchedule/Src/iAudio.cpp \
        ../MXSchedule/Src/AudioFormat.cpp \
        qtparameter.cpp \
    qtaudio.cpp \
    powerlevel.cpp \
    qtkey.cpp \
    qtschedule.cpp \
    qtblade.cpp \
    ../MXSchedule/Src/iBlade.cpp \
    qthand.cpp

HEADERS += \
        mainwindow.h \
        ../MXSchedule/Inc/common.h \
        ../MXSchedule/Inc/iEvent.hpp \
        ../MXSchedule/Inc/textHelper.h \
        ../MXSchedule/Inc/iSchedule.h \
        ../MXSchedule/Interface/iAudio.h \
        ../MXSchedule/Interface/iBlade.h \
        ../MXSchedule/Interface/iKey.h \
        ../MXSchedule/Interface/iParam.h \
        ../MXSchedule/Interface/iPower.h \
        ../MXSchedule/Interface/iHand.h \
        ../MXSchedule/Inc/re.h \
        ../MXSchedule/Inc/color.hpp \
        ../MXSchedule/Inc/AudioFormat.h \
        qtparameter.h \
    qtaudio.h \
    powerlevel.h \
    qtkey.h \
    qtschedule.h \
    qtblade.h \
    qthand.h

DEFINES += USE_QT=1

FORMS += \
        mainwindow.ui \
    powerlevel.ui \
    qtkey.ui \
    qthand.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
