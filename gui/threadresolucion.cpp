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

#include "threadresolucion.h"

ThreadResolucion::ThreadResolucion(const ConjuntoClausulas<ClausComp>& claus, QObject* parent)
        : QThread(parent), satisfacible(false), claus(claus), seguir_busqueda(true), tipo_res(General)
{
}

Resolucion::t_prueba ThreadResolucion::getPrueba() const
{
    return prueba;
}

ThreadResolucion::t_resolucion ThreadResolucion::getTipoResolucion() const
{
    return tipo_res;
}

bool ThreadResolucion::esSatisfacible() const
{
    return satisfacible;
}

void ThreadResolucion::run()
{
    if (claus.esDeHorn()) {
        tipo_res = Unitaria;
        ResolucionUnitaria res(claus);
        satisfacible = res.esSatisfacible(prueba, seguir_busqueda);
    } else {
        tipo_res = General;
        ResolucionGeneral res(claus);
        satisfacible = res.esSatisfacible(prueba, seguir_busqueda);
    }
}

void ThreadResolucion::terminar()
{
    seguir_busqueda = false;
}
