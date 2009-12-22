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

#ifndef RESOLUCION_HPP
#define RESOLUCION_HPP

#include "clausulas.hpp"
#include "inferencia.hpp"
#include <list>
#include <boost/shared_ptr.hpp>

class Resolucion {
public:
     typedef ConjuntoClausulas<> ConjClaus;
     typedef std::list<boost::shared_ptr<Inferencia> > t_prueba;

     Resolucion(const ConjClaus& claus) :
	  claus(claus) { }
 
     virtual bool esSatisfacible(t_prueba& Prueba, const bool& seguir_busqueda) = 0;

     virtual bool esSatisfacible(t_prueba& Prueba);
protected:
     ConjuntoClausulas<> claus;

     void resolverPredicadosEliminables(t_prueba& prueba, bool& resolvio_vacia);

     void eliminarPredicado(t_prueba& prueba, const std::string& p, bool& resolvio_vacia);
};

class ResolucionGeneral : public Resolucion {
public:
     ResolucionGeneral(const ConjClaus& claus) :
	  Resolucion(claus) { }

     bool esSatisfacible(t_prueba& Prueba, const bool& seguir_busqueda);

     bool esSatisfacible(t_prueba& Prueba);
};

#endif
