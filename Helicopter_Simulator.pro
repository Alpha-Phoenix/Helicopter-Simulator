TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += $$PWD/include\

LIBS += -lGL -lglfw -lX11 -lpthread -lXrandr -lXi -ldl

SOURCES += main.cpp \
    src/glad.c \
    Mesh.cpp \
    Utils.cpp

HEADERS += \
    learnopengl/camera.h \
    learnopengl/shader_m.h \
    Mesh.hpp \
    Utils.hpp \
    include/glad/glad.h \
    include/KHR/khrplatform.h \
    learnopengl/stb_image.h

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
    models/helicopter/base.obj \
    models/smooth_sphere.obj \
    shaders/lighting.vert \
    shaders/lighting.frag \
    textures/container.jpg \
    textures/cabine_texture.png \
    textures/helice_tras_texture.jpg \
    textures/helice_sup_texture.jpg \
    textures/cabine_texture.jpg
