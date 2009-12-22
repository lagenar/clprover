#ifndef THREADRESOLUCION_H
#define THREADRESOLUCION_H

#include <QThread>
#include "resolucion.hpp"

class ThreadResolucion : public QThread
{
public:
    ThreadResolucion(const ConjuntoClausulas<>& claus, QObject* parent = 0);

    void run();

    Resolucion::t_prueba getPrueba() const;

    bool esSatisfacible() const;

    void terminar();

private:
    bool satisfacible;
    ConjuntoClausulas<> claus;
    Resolucion::t_prueba prueba;
    bool seguir_busqueda;
};

#endif // THREADRESOLUCION_H
