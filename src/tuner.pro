QT += widgets serialport

TARGET = tuner
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    settingsdialog.cpp \
    form.cpp \
    command.cpp \
    configdialog.cpp \
    commandline.cpp \
    commanditemmodel.cpp

HEADERS += \
    mainwindow.h \
    settingsdialog.h \
    form.h \
    command.h \
    configdialog.h \
    commandline.h \
    commanditemmodel.h

FORMS += \
    mainwindow.ui \
    settingsdialog.ui \
    form.ui \
    configdialog.ui

RESOURCES += \
    tuner.qrc

DISTFILES += \
    ../doc/todo.txt

RC_ICONS = images/app.ico
