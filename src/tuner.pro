QT += widgets serialport

TARGET = tuner
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    settingsdialog.cpp \
    console.cpp \
    form.cpp \
    command.cpp \
    configdialog.cpp \
    commandline.cpp

HEADERS += \
    mainwindow.h \
    settingsdialog.h \
    console.h \
    form.h \
    command.h \
    configdialog.h \
    commandline.h

FORMS += \
    mainwindow.ui \
    settingsdialog.ui \
    form.ui \
    configdialog.ui

RESOURCES += \
    tuner.qrc

DISTFILES += \
    ../doc/todo.txt
