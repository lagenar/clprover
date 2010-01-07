TARGET = clprover-cli
SOURCES += main.cpp \
    ../src/termino.cpp \
    ../src/sustitucion.cpp \
    ../src/resolucion.cpp \
    ../src/parser.cpp \
    ../src/literal.cpp \
    ../src/clausula.cpp \
    ../src/argumentos.cpp \
    ../src/gramatica.cpp
HEADERS += ../src/termino.hpp \
    ../src/sustitucion.hpp \
    ../src/resolucion.hpp \
    ../src/parser.hpp \
    ../src/literal.hpp \
    ../src/inferencia.hpp \
    ../src/gramatica.hpp \
    ../src/clausulas.hpp \
    ../src/clausula.hpp \
    ../src/argumentos.hpp
INCLUDEPATH += ../src
