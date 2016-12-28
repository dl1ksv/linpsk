CONFIG += warn_on \
          qt \
         thread
CONFIG += link_pkgconfig

!packagesExist(fftw3) {
error("LinPSK requires fftw3")
}

CONFIG(debug, debug|release) {
message(Building Qt$$QT_VERSION debug version)
QMAKE_CFLAGS_DEBUG += '-g3 -O0'
QMAKE_CXXFLAGS_DEBUG += '-g3 -O0'
} else {
message(Building Qt$$QT_VERSION release)
DEFINES += QT_NO_DEBUG
}
QT += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TEMPLATE = app
TARGET = linpsk
target.path=/usr/local/bin
desktop.path=/usr/share/applications
desktop.files=data/linpsk.desktop
images.path=/usr/share/pixmaps
images.files=images/linpsk.png

INSTALLS +=desktop
INSTALLS +=images
INSTALLS +=target
INCLUDEPATH += . src gui

LIBS += -lasound -lfftw3


packagesExist( hamlib ) {
      message(LinPSK builds with hamlib)
      LIBS +=$$system("pkg-config --libs hamlib")
      DEFINES += WITH_HAMLIB
      HEADERS +=src/rigcontrol.h
      SOURCES +=src/rigcontrol.cpp
}
else {
    message(LinPSK builds without hamlib)
}
# Input

HEADERS += gui/activatemacros.h \
           gui/addmacro.h \
           gui/addrxwindow.h \
           gui/controlpanel.h \
           gui/crxdisplay.h \
           gui/ctxdisplay.h \
           gui/definebandlist.h \
           gui/deletemacro.h \
           gui/editmacro.h \
           gui/generalsettings.h \
           gui/linpsk.h \
           gui/macrocontrol.h \
           gui/modemenu.h \
           gui/qsodata.h \
           gui/renamemacro.h \
           gui/spectrumdisplay.h \
           gui/txwindow.h \
           src/bpskdemodulator.h \
           src/bpskmodulator.h \
           src/cdemodulator.h \
           src/cledbutton.h \
           src/cmodulator.h \
           src/color.h \
           src/constants.h \
           src/cpskdemodulator.h \
           src/crxchannel.h \
           src/crxwindow.h \
           src/csound.h \
           src/csquelch.h \
           src/ctxbuffer.h \
           src/deinterleaver.h \
           src/feccoder.h \
           src/fircoeffs.h \
           src/firfilter.h \
           src/frequencyselect.h \
           src/fskmodulator.h \
           src/input.h \
           src/interleaver.h \
           src/mfskdemodulator.h \
           src/mfskmodulator.h \
           src/mfskvaricode.h \
           src/parameter.h \
           src/processlogdata.h \
           src/pskmodulator.h \
           src/psktable.h \
           src/qpskdemodulator.h \
           src/qpskmodulator.h \
           src/readonlystringlistmodel.h \
           src/rttydemodulator.h \
           src/rttymodulator.h \
           src/spectrumwindow.h \
           src/tabwidget.h \
           src/viterbi.h \
           src/waterfallwindow.h \
           src/waveinput.h
FORMS += gui/activatemacros.ui \
         gui/addmacro.ui \
         gui/addrxwindow.ui \
         gui/controlpanel.ui \
         gui/crxdisplay.ui \
         gui/ctxdisplay.ui \
         gui/definebandlist.ui \
         gui/deletemacro.ui \
         gui/editmacro.ui \
         gui/generalsettings.ui \
         gui/linpsk.ui \
         gui/macrocontrol.ui \
         gui/modemenu.ui \
         gui/qsodata.ui \
         gui/renamemacro.ui \
         gui/spectrumdisplay.ui \
         gui/txwindow.ui
SOURCES += gui/activatemacros.cpp \
           gui/addmacro.cpp \
           gui/addrxwindow.cpp \
           gui/controlpanel.cpp \
           gui/crxdisplay.cpp \
           gui/ctxdisplay.cpp \
           gui/definebandlist.cpp \
           gui/deletemacro.cpp \
           gui/editmacro.cpp \
           gui/generalsettings.cpp \
           gui/linpsk.cpp \
           gui/macrocontrol.cpp \
           gui/modemenu.cpp \
           gui/qsodata.cpp \
           gui/renamemacro.cpp \
           gui/spectrumdisplay.cpp \
           gui/txwindow.cpp \
           src/bpskdemodulator.cpp \
           src/bpskmodulator.cpp \
           src/cdemodulator.cpp \
           src/cledbutton.cpp \
           src/cmodulator.cpp \
           src/cpskdemodulator.cpp \
           src/crxchannel.cpp \
           src/crxwindow.cpp \
           src/csound.cpp \
           src/csquelch.cpp \
           src/ctxbuffer.cpp \
           src/deinterleaver.cpp \
           src/feccoder.cpp \
           src/firfilter.cpp \
           src/frequencyselect.cpp \
           src/fskmodulator.cpp \
           src/input.cpp \
           src/interleaver.cpp \
           src/main.cpp \
           src/mfskdemodulator.cpp \
           src/mfskmodulator.cpp \
           src/mfskvaricode.cpp \
           src/parameter.cpp \
           src/processlogdata.cpp \
           src/pskmodulator.cpp \
           src/qpskdemodulator.cpp \
           src/qpskmodulator.cpp \
           src/readonlystringlistmodel.cpp \
           src/rttydemodulator.cpp \
           src/rttymodulator.cpp \
           src/spectrumwindow.cpp \
           src/tabwidget.cpp \
           src/viterbi.cpp \
           src/waterfallwindow.cpp \
           src/waveinput.cpp
RESOURCES += src/application.qrc

DISTFILES += \
    README \
    ChangeLog \
    COPYING
