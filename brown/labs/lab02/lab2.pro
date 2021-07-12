QT += core gui opengl

TARGET = lab2
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
    CONFIG += c++11
}
win32 {
    DEFINES += GLEW_STATIC
    LIBS += -lopengl32 -lglu32
}

INCLUDEPATH += src glm cs123_lib ../glew-1.10.0/include
DEPENDPATH += src glm cs123_lib ../glew-1.10.0/include

SOURCES += src/settings.cpp \
    src/mainwindow.cpp \
    src/main.cpp \
    src/glwidget.cpp \
    src/databinding.cpp \
    cs123_lib/resourceloader.cpp \
    src/openglshape.cpp \
    src/gl/datatype/VBO.cpp \
    src/gl/datatype/VBOAttribMarker.cpp \
    src/gl/datatype/VAO.cpp \
    src/gl/datatype/IBO.cpp \
    src/gl/GLDebug.cpp \
    ../glew-1.10.0/src/glew.c


HEADERS += \
    src/settings.h \
    src/mainwindow.h \
    src/glwidget.h \
    src/databinding.h \
    cs123_lib/resourceloader.h \
    src/openglshape.h \
    src/gl/datatype/VBO.h \
    src/gl/datatype/VBOAttribMarker.h \
    src/gl/datatype/VAO.h \
    src/gl/datatype/IBO.h \
    src/gl/shaders/ShaderAttribLocations.h \
    src/gl/GLDebug.h \
    ../glew-1.10.0/include/GL/glew.h \
    src/sphere.h

FORMS += src/mainwindow.ui

OTHER_FILES += \
    shaders/solid.frag \
    shaders/solid.vert \
    shaders/gradient.frag \
    shaders/gradient.vert \
    shaders/texture.frag \
    shaders/texture.vert \
    shaders/shader.frag \
    shaders/shader.vert

RESOURCES += \
    shaders/shaders.qrc \
    images/images.qrc
