QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG(debug, debug|release) {
  message("Debug")
  DESTDIR = debug
  CONFIG += debug
  DEFINES += DEBUG
  TARGET = $$TARGET-debug
}else{
  message("Release")
  //DEFINES += QT_NO_DEBUG_OUTPUT
  DESTDIR = release
  TARGET = $$TARGET-release
}

MOC_DIR = build/$${DESTDIR}/moc
OBJECTS_DIR = build/$${DESTDIR}/obj
UI_DIR = build/$${DESTDIR}/ui

SOURCES += \
    ChartsDock.cpp \
    FileParser.cpp \
    SkyChart_base.cpp \
    TabularChoice.cpp \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    ChartsDock.h \
    FileParser.h \
    MainWindow.h \
    SkyChart_base.h \
    TabularChoice.h

FORMS += \
    MainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
