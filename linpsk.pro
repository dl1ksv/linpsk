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

CONFIG += warn_on \
          qt \
          thread  \
 ordered
DESTDIR = .

SUBDIRS += gui \
  src

QT += network


TEMPLATE = subdirs

LIBS += build/gui/libgui.a

POST_TARGETDEPS += build/gui/libgui.a \
src/../bin/linpsk

desktop.path=/usr/share/applications
desktop.files=data/linpsk.desktop
images.path=/usr/share/pixmaps
images.files=images/linpsk.png

INSTALLS +=desktop
INSTALLS +=images
