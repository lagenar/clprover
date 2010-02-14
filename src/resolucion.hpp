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
#include <vector>
#include <set>
#include <boost/shared_ptr.hpp>

/**
 * Clase abstracta que representa una estrategia de
 * resolución en lógica de primer orden.
 */
class Resolucion {
public:
     typedef ConjuntoClausulas<> ConjClaus;
     typedef std::vector<boost::shared_ptr<Inferencia> > t_prueba;

     /**
      * Constructor de la clase Resolucion.
      * @param claus Conjunto inicial de clausulas.
      */
     Resolucion(const ConjClaus& claus) :
	  claus(claus), id_resolucion(0) { }
     
     /**
      * @returns false si el conjunto de cláusulas pasado en el constructor es
      * insatisfacible. Si el conjunto es satisfacible, puede que el método retorne true
      * o que no retorne. La ejecución de este método puede detenerse desde otro hilo
      * de ejecución utilizando el parámetro por referencia <emph>seguir_busqueda</emph>.
      * Si <emph>seguir_busqueda</emph> es inicialmente false, o se lo setea a false desde
      * otro hilo a través de una referencia externa mientras esSatisfacible se ejecuta,
      * se detiene la ejecución y retorna true.
      * @param Prueba Aquí se insertan las reglas de inferencia usadas.
      * @param seguir_busqueda Usado para la sincronización con otro hilo de ejecución.
      */
     virtual bool esSatisfacible(t_prueba& Prueba, const bool& seguir_busqueda) = 0;

     /**
      * @returns false si el conjunto de cláusulas pasado en el constructor es
      * insatisfacible. Si el conjunto es satisfacible, puede que el método retorne true
      * o que no retorne.
      * @param Prueba Aquí se insertan las reglas de inferencia usadas.
      */
     virtual bool esSatisfacible(t_prueba& Prueba) = 0;
protected:
     ConjClaus claus;
     ConjClaus combinables;
     ConjClaus procesadas;
     int id_resolucion;

     void resolverPredicadosEliminables(ConjClaus& claus, t_prueba& prueba, bool& resolvio_vacia);

     void eliminarPredicado(ConjClaus& claus, t_prueba& prueba, const std::string& p, bool& resolvio_vacia);

     void simplificarPrueba(t_prueba& prueba) const;

     void agregarInferenciasUsadas(const t_prueba& prueba, int id, std::set<int>& inferencias_usadas) const;

     void agregarFactores(t_prueba& prueba, const Clausula& comb, std::list<Clausula>& factores);

     bool clausulaSimplificable(const Clausula& cl) const;
};

class ResolucionGeneral : public Resolucion {
public:
     ResolucionGeneral(const ConjClaus& claus) :
	  Resolucion(claus) { }

     bool esSatisfacible(t_prueba& prueba, const bool& seguir_busqueda);

     bool esSatisfacible(t_prueba& prueba);
};

class ResolucionUnitaria : public Resolucion {
public:
     ResolucionUnitaria(const ConjClaus& claus) :
	  Resolucion(claus) { }

     bool esSatisfacible(t_prueba& prueba, const bool& seguir_busqueda);

     bool esSatisfacible(t_prueba& prueba);
};

#endif

