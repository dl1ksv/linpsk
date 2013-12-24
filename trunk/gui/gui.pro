CONFIG(debug, debug|release) {
    QMAKE_CXXFLAGS_DEBUG += -g3 \
    -fpermissive \
    -O0
CONFIG +=  debug
} else {
    DEFINES += QT_NO_DEBUG
    DEFINES += QT_NO_DEBUG_OUTPUT
    QMAKE_CXXFLAGS_DEBUG +=-fpermissive \
    -O2
}

FORMS += addmacro.ui \
addrxwindow.ui \
deletemacro.ui \
editmacro.ui \
generalsettings.ui \
modemenu.ui \
qsodata.ui \
renamemacro.ui \
    controlpanel.ui \
    linpsk.ui \
    crxdisplay.ui \
    spectrumdisplay.ui \
    ctxdisplay.ui \
    macrocontrol.ui \
    txwindow.ui

CONFIG += build_all \
 staticlib \
 debug

DESTDIR = .

SOURCES += generalsettings.cpp \
 addmacro.cpp \
 addrxwindow.cpp \
 deletemacro.cpp \
 modemenu.cpp \
 qsodata.cpp \
 renamemacro.cpp \
 editmacro.cpp \
    controlpanel.cpp \
    linpsk.cpp \
    crxdisplay.cpp \
    spectrumdisplay.cpp \
    ctxdisplay.cpp \
    macrocontrol.cpp \
    txwindow.cpp

HEADERS += generalsettings.h \
 addmacro.h \
 addrxwindow.h \
 deletemacro.h \
 modemenu.h \
 qsodata.h \
 renamemacro.h \
 editmacro.h \
    controlpanel.h \
    linpsk.h \
    crxdisplay.h \
    spectrumdisplay.h \
    ctxdisplay.h \
    macrocontrol.h \
    txwindow.h


TEMPLATE = lib

QT += network

INCLUDEPATH += ../src

RESOURCES += \
    ../src/application.qrc

