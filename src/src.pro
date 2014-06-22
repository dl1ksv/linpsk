CONFIG(debug, debug|release) {
    QMAKE_CXXFLAGS_DEBUG += -g3 \
    -fpermissive \
    -O0
DEFINES +=SOUND_DEBUG
CONFIG +=  debug
} else {
    DEFINES += QT_NO_DEBUG
    DEFINES += QT_NO_DEBUG_OUTPUT
    QMAKE_CXXFLAGS_DEBUG +=-fpermissive \
    -O2
}

SOURCES += main.cpp \
 bpskdemodulator.cpp \
 bpskmodulator.cpp \
 cdemodulator.cpp \
 cledbutton.cpp \
 cmodulator.cpp \
 cpskdemodulator.cpp \
 crxchannel.cpp \
 crxwindow.cpp \
 csound.cpp \
 csquelch.cpp \
 ctxbuffer.cpp \
 deinterleaver.cpp \
 feccoder.cpp \
 firfilter.cpp \
 frequencyselect.cpp \
 fskmodulator.cpp \
 input.cpp \
 interleaver.cpp \
 mfskdemodulator.cpp \
 mfskmodulator.cpp \
 mfskvaricode.cpp \
 parameter.cpp \
 pskmodulator.cpp \
 qpskdemodulator.cpp \
 qpskmodulator.cpp \
 rttydemodulator.cpp \
 rttymodulator.cpp \
 textinput.cpp \
 viterbi.cpp \
 waveinput.cpp \
 readonlystringlistmodel.cpp \
 processlogdata.cpp \
    tabwidget.cpp \
    spectrumwindow.cpp \
    waterfallwindow.cpp
HEADERS += bpskdemodulator.h \
 bpskmodulator.h \
 cdemodulator.h \
 cledbutton.h \
 cmodulator.h \
 color.h \
 constants.h \
 cpskdemodulator.h \
 crxchannel.h \
 crxwindow.h \
 csound.h \
 csquelch.h \
 ctxbuffer.h \
 deinterleaver.h \
 feccoder.h \
 fircoeffs.h \
 firfilter.h \
 frequencyselect.h \
 input.h \
 interleaver.h \
 mfskdemodulator.h \
 mfskmodulator.h \
 mfskvaricode.h \
 parameter.h \
 pskmodulator.h \
 psktable.h \
 qpskdemodulator.h \
 qpskmodulator.h \
 rttydemodulator.h \
 rttymodulator.h \
 textinput.h \
 viterbi.h \
 waveinput.h \
 readonlystringlistmodel.h \
 fskmodulator.h \
 processlogdata.h \
    tabwidget.h \
    spectrumwindow.h \
    waterfallwindow.h
TEMPLATE = app
CONFIG += warn_on \
	  thread \
          qt \
 debug
TARGET = ../bin/linpsk
RESOURCES = application.qrc
DESTDIR = .

QT += network

INCLUDEPATH += ../gui

LIBS += ../gui/libgui.a \
-L/usr/lib64 \
-L/usr/lib \
-lasound \
-lfftw3
POST_TARGETDEPS += ../gui/libgui.a

DISTFILES += ../README ../COPYING \
 ../ChangeLog \
 ../asoundrc \
 ../images/linpsk.png



INSTALLS += target

target.path = /usr/local/bin

FORMS +=

