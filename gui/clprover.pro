# -------------------------------------------------
# Project created by QtCreator 2009-12-18T14:37:45
# -------------------------------------------------
TARGET = clprover
TEMPLATE = app
SOURCES += main.cpp \
    qclprover.cpp \
    ../src/termino.cpp \
    ../src/sustitucion.cpp \
    ../src/resolucion.cpp \
    ../src/parser.cpp \
    ../src/literal.cpp \
    ../src/clausula.cpp \
    ../src/argumentos.cpp \
    threadresolucion.cpp \
    mensajeserror.cpp \
    dialogumg.cpp \
    dialogsimplificacion.cpp
HEADERS += qclprover.h \
    ../src/termino.hpp \
    ../src/sustitucion.hpp \
    ../src/resolucion.hpp \
    ../src/parser.hpp \
    ../src/literal.hpp \
    ../src/inferencia.hpp \
    ../src/gramatica.hpp \
    ../src/clausulas.hpp \
    ../src/clausula.hpp \
    ../src/argumentos.hpp \
    threadresolucion.h \
    mensajeserror.h \
    dialogumg.h \
    dialogsimplificacion.h
FORMS += qclprover.ui \
    dialogumg.ui \
    dialogsimplificacion.ui
INCLUDEPATH += ../src
