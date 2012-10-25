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
    ctxdisplay.ui

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
    ctxdisplay.cpp

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
    ctxdisplay.h


TEMPLATE = lib

QMAKE_CXXFLAGS_DEBUG += -g3 \
-O0
QMAKE_CXXFLAGS_RELEASE += -O2


CONFIG -= release


QT += network

INCLUDEPATH += ../src

RESOURCES += \
    ../src/application.qrc

