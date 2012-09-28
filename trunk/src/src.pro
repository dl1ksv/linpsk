SOURCES += linpsk.cpp \
           main.cpp \
 macros.cpp \
 macrowindow.cpp \
 bpskdemodulator.cpp \
 bpskmodulator.cpp \
 cdemodulator.cpp \
 cledbutton.cpp \
 cmodulator.cpp \
 controlpanel.cpp \
 cpskdemodulator.cpp \
 crecording.cpp \
 crxchannel.cpp \
 crxdisplay.cpp \
 crxwindow.cpp \
 csound.cpp \
 csquelch.cpp \
 ctrigger.cpp \
 ctxbuffer.cpp \
 ctxdisplay.cpp \
 ctxwindow.cpp \
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
 spectrumdisplay.cpp \
 readonlystringlistmodel.cpp \
 processlogdata.cpp
HEADERS += linpsk.h \
 bpskdemodulator.h \
 bpskmodulator.h \
 cdemodulator.h \
 cledbutton.h \
 cmodulator.h \
 color.h \
 constants.h \
 controlpanel.h \
 cpskdemodulator.h \
 crecording.h \
 crxchannel.h \
 crxdisplay.h \
 crxwindow.h \
 csound.h \
 csquelch.h \
 ctrigger.h \
 ctxbuffer.h \
 ctxdisplay.h \
 ctxwindow.h \
 deinterleaver.h \
 feccoder.h \
 fircoeffs.h \
 firfilter.h \
 frequencyselect.h \
 input.h \
 interleaver.h \
 macros.h \
 macrowindow.h \
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
 spectrumdisplay.h \
 readonlystringlistmodel.h \
 fskmodulator.h \
 processlogdata.h
TEMPLATE = app
CONFIG += warn_on \
	  thread \
          qt \
 debug
TARGET = ../bin/linpsk
RESOURCES = application.qrc
DESTDIR = .



QMAKE_CXXFLAGS_DEBUG += -g3 \
-fpermissive \
-O0
QMAKE_CXXFLAGS_RELEASE += -f permissive \ 
-O2



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

CONFIG -= release

