QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    exportfile.cpp \
    image_scaled_widget.cpp \
    main.cpp \
    mainwindow.cpp \
    showres.cpp

HEADERS += \
    exportfile.h \
    image_scaled_widget.h \
    mainwindow.h \
    showres.h

FORMS += \
    exportfile.ui \
    mainwindow.ui \
    showres.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    res/onstart.jpg \
    res/onstart2h.jpg \
    res/onstart3.jpg \
    res/onstart4.jpg

RESOURCES += \
    res.qrc
