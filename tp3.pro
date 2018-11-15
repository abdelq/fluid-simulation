QT += core gui opengl

TARGET = tp3
TEMPLATE = app
CONFIG += silent

DEFINES += _USE_MATH_DEFINES

CONFIG(debug,debug|release) {
} else {
    QMAKE_CXXFLAGS -= -O2
    QMAKE_CXXFLAGS += -O3 -fopenmp
    QMAKE_LFLAGS -= -O1
    QMAKE_LFLAGS += -O3 -fopenmp
}

CONFIG += c++11
QMAKE_CFLAGS += -std=c99

contains(QT_VERSION, ^4.*) {
QMAKE_CXXFLAGS += -std=gnu++0x
}

SOURCES += \
    src/Geometry/*.cpp \
    src/Scenes/*.cpp \
    src/SPH/*.cpp \
    src/*.cpp

HEADERS  += \
    src/Geometry/*.h \
    src/Scenes/*.h \
    src/SPH/*.h \
    src/*.h

FORMS    += \
    gui/MainWindow.ui

INCLUDEPATH += \
    src/

OTHER_FILES += \
    shaders/*.vs \
    shaders/*.fs

DEPENDPATH += \
    shaders

#LIBS += -lopengl32

CONFIG(debug, debug|release) {
    DESTDIR = ./
    OBJECTS_DIR = build
    MOC_DIR = build
    UI_DIR = src
}
else
{
    DESTDIR = ./
    OBJECTS_DIR = build
    MOC_DIR = build
    UI_DIR = src
}

























