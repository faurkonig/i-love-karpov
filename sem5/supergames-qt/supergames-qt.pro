QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    sgdeveloper.cpp \
    sguser.cpp \
    utils/databasecontainer.cpp \
    utils/dialoghelper.cpp \
    utils/hash.cpp \
    view/addfrienddialog.cpp \
    view/components/devgameitem.cpp \
    view/components/frienditem.cpp \
    view/components/gameitem.cpp \
    view/connectiondialog.cpp \
    view/developerdashboardwindow.cpp \
    view/gamedialog.cpp \
    view/gameeditdialog.cpp \
    view/mainwindow.cpp \
    view/registerdialog.cpp \
    view/reviewdialog.cpp \
    view/storewindow.cpp \
    view/usercartdialog.cpp \
    view/usercollectiondialog.cpp \
    view/userprofiledialog.cpp

HEADERS += \
    sgdeveloper.h \
    sguser.h \
    utils/commonpatterns.h \
    utils/databasecontainer.h \
    utils/dialoghelper.h \
    utils/hash.h \
    view/addfrienddialog.h \
    view/components/devgameitem.h \
    view/components/frienditem.h \
    view/components/gameitem.h \
    view/connectiondialog.h \
    view/developerdashboardwindow.h \
    view/gamedialog.h \
    view/gameeditdialog.h \
    view/mainwindow.h \
    view/registerdialog.h \
    view/reviewdialog.h \
    view/storewindow.h \
    view/usercartdialog.h \
    view/usercollectiondialog.h \
    view/userprofiledialog.h

FORMS += \
    view/addfrienddialog.ui \
    view/components/devgameitem.ui \
    view/components/frienditem.ui \
    view/components/gameitem.ui \
    view/connectiondialog.ui \
    view/developerdashboardwindow.ui \
    view/gamedialog.ui \
    view/gameeditdialog.ui \
    view/mainwindow.ui \
    view/registerdialog.ui \
    view/reviewdialog.ui \
    view/storewindow.ui \
    view/usercartdialog.ui \
    view/usercollectiondialog.ui \
    view/userprofiledialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
