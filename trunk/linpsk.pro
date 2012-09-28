CONFIG += warn_on \
          qt \
          thread  \
 ordered \
 debug
DESTDIR = .

SUBDIRS += gui \
  src



CONFIG -= release

QT += network


TEMPLATE = subdirs

LIBS += gui/libgui.a

POST_TARGETDEPS += gui/libgui.a \
src/../bin/linpsk
