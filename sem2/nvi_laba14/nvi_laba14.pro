QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    arc.cpp \
    brush.cpp \
    canvas.cpp \
    color.cpp \
    curve.cpp \
    curvedialog.cpp \
    ellipse.cpp \
    elliptic.cpp \
    line.cpp \
    main.cpp \
    mainwindow.cpp \
    pen.cpp \
    pixel.cpp \
    point.cpp \
    rectangle.cpp \
    tooldialog.cpp \
    paintpath.cpp \
    polygon.cpp

HEADERS += \
    arc.h \
    brush.h \
    canvas.h \
    color.h \
    consts.h \
    curve.h \
    curvedialog.h \
    ellipse.h \
    elliptic.h \
    line.h \
    mainwindow.h \
    pen.h \
    pixel.h \
    point.h \
    rectangle.h \
    tooldialog.h \
    paintpath.h \
    polygon.h

FORMS += \
    curvedialog.ui \
    mainwindow.ui \
    tooldialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
