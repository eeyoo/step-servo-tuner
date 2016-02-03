QT += widgets serialport

TARGET = terminal
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    settingsdialog.cpp \
    console.cpp \
    form.cpp

HEADERS += \
    mainwindow.h \
    settingsdialog.h \
    console.h \
    form.h

FORMS += \
    mainwindow.ui \
    settingsdialog.ui \
    form.ui

RESOURCES += \
    terminal.qrc
