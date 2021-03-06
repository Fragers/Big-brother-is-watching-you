QT       += core gui
QT += network charts


HEADERS += \
    createnewfile.h \
    donutbreakdownchart.h \
    gethttpfile.h \
    httpserver.h \
    initstart.h \
    mainslice.h \
    parseitems.h \
    postrequest.h \
    savedia.h

SOURCES += \
    createnewfile.cpp \
    donutbreakdownchart.cpp \
    gethttpfile.cpp \
    httpserver.cpp \
    initstart.cpp \
    mainslice.cpp \
    parseitems.cpp \
    postrequest.cpp \
    savedia.cpp

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS \
           LOCALDIR=\\\"$$PWD/\\\" \
           WEBDIR=\\\"$$PWD/webFiles/\\\"
# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    chart.cpp \
    main.cpp \
    mainwindow.cpp \
    #myserver.cpp \
    treeitem.cpp \
    treemodel.cpp
    #widget.cpp

HEADERS += \
    chart.h \
    mainwindow.h \
    #myserver.h \
    treeitem.h \
    treemodel.h
  #  widget.h

FORMS += \
    chart.ui \
    createnewfile.ui \
    initstart.ui \
    mainwindow.ui \
    parseitems.ui \
    savedia.ui
    #widget.ui

# Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target

#INSTALLS += target
RESOURCES += \
    simpletreemodel.qrc

