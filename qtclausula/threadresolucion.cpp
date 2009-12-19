#include "threadresolucion.h"

ThreadResolucion::ThreadResolucion(const ConjuntoClausulas<ClausComp>& claus, QObject* parent)
        : QThread(parent), satisfacible(false), claus(claus), seguir_busqueda(true)
{
}

Resolucion::t_prueba ThreadResolucion::getPrueba() const
{
    return prueba;
}

bool ThreadResolucion::esSatisfacible() const
{
    return satisfacible;
}

void ThreadResolucion::run()
{
    ResolucionGeneral res(claus);
    satisfacible = res.esSatisfacible(prueba, seguir_busqueda);
}

void ThreadResolucion::terminar()
{
    seguir_busqueda = false;
}
