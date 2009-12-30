/*
* clprover
* Copyright (C) 2009 Lucas Moauro
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef THREADRESOLUCION_H
#define THREADRESOLUCION_H

#include <QThread>
#include "resolucion.hpp"

class ThreadResolucion : public QThread
{
public:
    typedef enum {General, Unitaria} t_resolucion;

    ThreadResolucion(const ConjuntoClausulas<>& claus, QObject* parent = 0);

    void run();

    Resolucion::t_prueba getPrueba() const;

    t_resolucion getTipoResolucion() const;

    bool esSatisfacible() const;

    void terminar();

private:
    bool satisfacible;
    ConjuntoClausulas<> claus;
    Resolucion::t_prueba prueba;
    bool seguir_busqueda;
    t_resolucion tipo_res;
};

#endif // THREADRESOLUCION_H
