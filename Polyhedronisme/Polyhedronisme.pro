CONFIG+=sdk_no_version_check

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

#QMAKE_CFLAGS += -Wshorten-64-to-32
QMAKE_CXXFLAGS += -Wshorten-64-to-32

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    gl01_widget.cpp \
    gl_widget.cpp \
    glpnt_widget.cpp \
    main.cpp \
    mainwindow.cpp \
    mesh.cpp \
    poly/johnson.cpp \
    poly/parser.cpp \
    renderer.cpp

HEADERS += \
    gl01_widget.h \
    gl_widget.h \
    glpnt_widget.h \
    mainwindow.h \
    mesh.h \
    poly/color.hpp \
    poly/common.hpp \
    poly/fastflags.h \
    poly/johnson.hpp \
    poly/parser.hpp \
    poly/poly_operations.hpp \
    poly/polyhedron.hpp \
    poly/seeds.hpp \
    renderer.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
