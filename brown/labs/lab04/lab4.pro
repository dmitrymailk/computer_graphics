# -------------------------------------------------
# Project created by QtCreator 2010-08-22T14:12:19
# -------------------------------------------------
QT += opengl gui
TARGET = lab4
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++14
QMAKE_LFLAGS += -no-pie
CONFIG += c++14

unix:!macx {
    LIBS += -lGLU
}
macx {
    QMAKE_CFLAGS_X86_64 += -mmacosx-version-min=10.7
    QMAKE_CXXFLAGS_X86_64 = $$QMAKE_CFLAGS_X86_64
    QMAKE_LFLAGS += -stdlib=libc++
    QMAKE_CXXFLAGS += -stdlib=libc++
    CONFIG += c++11
}
win32 {
    DEFINES += GLEW_STATIC
    LIBS += -lopengl32 -lglu32
}

SOURCES += \
    ui/Canvas2D.cpp \
    ui/SupportCanvas2D.cpp \
    ui/Settings.cpp \
    ui/mainwindow.cpp \
    ui/Databinding.cpp \
    main.cpp \
    filter/Filter.cpp \
    filter/FilterGray.cpp \
    filter/FilterInvert.cpp \
    filter/FilterIdentity.cpp \
    filter/FilterShift.cpp \
    filter/FilterUtils.cpp \
    lib/RGBA.cpp

HEADERS += \
    ui/Canvas2D.h \
    ui/SupportCanvas2D.h \
    ui/Settings.h \
    ui/mainwindow.h \
    ui/Databinding.h \
    ui_mainwindow.h \
    filter/Filter.h \
    filter/FilterGray.h \
    filter/FilterInvert.h \
    filter/FilterIdentity.h \
    filter/FilterShift.h \
    filter/FilterUtils.h \
    lib/RGBA.h

FORMS += ui/mainwindow.ui

INCLUDEPATH += lib ui filter
DEPENDPATH += lib ui filter

RESOURCES += \
    resources.qrc
