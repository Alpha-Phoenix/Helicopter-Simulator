TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += $$PWD/include\

LIBS += -lGL -lglfw -lX11 -lpthread -lXrandr -lXi -ldl

SOURCES += main.cpp \
    src/glad.c \
    Mesh.cpp \
    utils.cpp

HEADERS += \
    learnopengl/camera.h \
    learnopengl/shader_m.h \
    Mesh.hpp \
    Utils.hpp \
    include/glad/glad.h \
    include/KHR/khrplatform.h \
    learnopengl/mesh.h \
    learnopengl/model.h

DISTFILES += \
    shaders/default.frag \
    shaders/default.vert \
    models/sphere.obj \
    models/container.obj \
    shaders/sample.vert \
    shaders/sample.frag \
    shaders/notex.frag \
    shaders/notex.vert \
    models/monkey.obj \
    models/helicopter/helice_sup.obj \
    models/helicopter/helice_back.obj \
    models/helicopter/cabine.obj \
    models/helicopter/base.obj
