# -------------------------------------------------
# Project created by QtCreator 2009-12-18T14:37:45
# -------------------------------------------------
TARGET = qtclausula
TEMPLATE = app
SOURCES += main.cpp \
    qtclausula.cpp \
    ../termino.cpp \
    ../sustitucion.cpp \
    ../resolucion.cpp \
    ../parser.cpp \
    ../literal.cpp \
    ../clausula.cpp \
    ../argumentos.cpp \
    threadresolucion.cpp \
    mensajeserror.cpp \
    dialogumg.cpp
HEADERS += qtclausula.h \
    ../termino.hpp \
    ../sustitucion.hpp \
    ../resolucion.hpp \
    ../parser.hpp \
    ../literal.hpp \
    ../inferencia.hpp \
    ../gramatica.hpp \
    ../clausulas.hpp \
    ../clausula.hpp \
    ../argumentos.hpp \
    threadresolucion.h \
    mensajeserror.h \
    dialogumg.h
FORMS += qtclausula.ui \
    dialogumg.ui
INCLUDEPATH += ../
