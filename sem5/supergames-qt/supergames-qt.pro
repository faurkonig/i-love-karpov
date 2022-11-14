QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addfrienddialog.cpp \
    connectiondialog.cpp \
    databasecontainer.cpp \
    dialoghelper.cpp \
    frienditem.cpp \
    gamedialog.cpp \
    gameitem.cpp \
    hash.cpp \
    main.cpp \
    mainwindow.cpp \
    registerdialog.cpp \
    sgdeveloper.cpp \
    sguser.cpp \
    storewindow.cpp \
    usercartdialog.cpp \
    usercollectiondialog.cpp \
    userprofiledialog.cpp

HEADERS += \
    addfrienddialog.h \
    commonpatterns.h \
    connectiondialog.h \
    databasecontainer.h \
    dialoghelper.h \
    frienditem.h \
    gamedialog.h \
    gameitem.h \
    hash.h \
    mainwindow.h \
    registerdialog.h \
    sgdeveloper.h \
    sguser.h \
    storewindow.h \
    usercartdialog.h \
    usercollectiondialog.h \
    userprofiledialog.h

FORMS += \
    addfrienddialog.ui \
    connectiondialog.ui \
    frienditem.ui \
    gamedialog.ui \
    gameitem.ui \
    mainwindow.ui \
    registerdialog.ui \
    storewindow.ui \
    usercartdialog.ui \
    usercollectiondialog.ui \
    userprofiledialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
