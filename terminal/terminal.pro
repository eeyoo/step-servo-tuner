QT += widgets serialport

TARGET = terminal
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    settingsdialog.cpp \
    console.cpp \
    form.cpp \
    command.cpp

HEADERS += \
    mainwindow.h \
    settingsdialog.h \
    console.h \
    form.h \
    command.h

FORMS += \
    mainwindow.ui \
    settingsdialog.ui \
    form.ui

RESOURCES += \
    terminal.qrc

DISTFILES += \
    readme.txt
