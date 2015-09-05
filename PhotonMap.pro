TEMPLATE = app
CONFIG -= console
CONFIG -= qt

INCLUDEPATH += include

QMAKE_CXXFLAGS += -std=c++14
QMAKE_CXXFLAGS_RELEASE += -O3 -Ofast

LIBS += -lSDL -pthread

SOURCES += src/main.cpp \
    src/system/device.cpp \
    src/shape.cpp \
    src/material.cpp \
    src/ray.cpp \
    src/camera.cpp \
    src/raytracer.cpp \
    src/shapestorage.cpp \
    src/light.cpp \
    src/world.cpp

HEADERS += \
    include/system/device.hpp \
    include/shape.hpp \
    include/ray.hpp \
    include/material.hpp \
    include/camera.hpp \
    include/raytracer.hpp \
    include/shapestorage.hpp \
    include/world.hpp \
    include/light.hpp \
    include/system/threadpool.hpp
