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
    hexstring.cpp \
    configdialog.cpp \
    movedialog.cpp \
    filedialog.cpp

HEADERS += \
    mainwindow.h \
    settingsdialog.h \
    console.h \
    form.h \
    command.h \
    hexstring.h \
    configdialog.h \
    movedialog.h \
    filedialog.h

FORMS += \
    mainwindow.ui \
    settingsdialog.ui \
    form.ui \
    configdialog.ui \
    movedialog.ui \
    filedialog.ui

RESOURCES += \
    tuner.qrc

DISTFILES += \
    ../doc/todo.txt
