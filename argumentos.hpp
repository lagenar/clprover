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

#ifndef ARGUMENTOS_HPP
#define ARGUMENTOS_HPP

#include <list>
#include <map>
#include <iostream>

#include "termino.hpp"
#include "sustitucion.hpp"

class Termino;
class Sustitucion;

/**
 * Esta clase representa los argumentos(lista de términos) de
 * una función o un literal.
 */
class Argumentos {

public:
     typedef std::list<Termino*>::iterator iterator;
     typedef std::list<Termino*>::const_iterator const_iterator;

     Argumentos() { }

     Argumentos(const Argumentos& Args);

     /**
      * @returns Cantidad de argumentos.
      */
     int aridad() const;

     /**
      * @returns Representación en forma de cadena de los argumentos.
      * i.e. (a1, a2, ..., an)
      */
     const std::string getString() const;

     const_iterator begin() const;

     const_iterator end() const;

     /**
      * Sobrecarga del operador ==. Dos objetos de la clase argumentos son
      * iguales si sus argumentos son iguales uno por uno.
      * @param otro Argumentos a comparar.
      * @returns true si son iguales, false en caso contrario.
      */
     bool operator==(const Argumentos& otro) const;

     /**
      * @returns true si alguno de los argumentos contiene una variable con
      * identificador id, false en caso contrario.
      */
     bool contieneVariable(const std::string& id) const;

     /**
      * Calcula el unificador más general con otro objeto de tipo Argumentos.
      * La sustitución s es el contexto de unificación. Las sustituciones del unificador
      * más general se agregan a s(si es que existe el umg).
      * @param s Contexto de unificación.
      * @param otro Argumentos con los que se unifica.
      * @returns true si fue posible unificar, false en caso contrario.
      * @see Sustitucion
      */
     bool unificar(Sustitucion& s, const Argumentos& otro) const;

     /**
      * Agrega un término como argumento.
      * @param t término a agregar.
      */
     void agregarArgumento(const Termino& t);

     /**
      * Aplica una sustitución de variables a terminos.
      * @param s Sustitución a aplicar.
      */
     void aplicarSustitucion(const Sustitucion& s);

     /**
      * Renombra las variables para que queden de la forma Xcomienzo, Xcomienzo+1, ..., xcomienzo+n.
      * El valor de comienzo termina siendo el último valor utilizado para el renombre.
      * Se actualiza el mapa de renombres con los renombres realizados.
      * @param comienzo Número de comienzo del renombre.
      * @param renombre mapa con los renombres que se hicieron hasta el momento.
      */
     void renombrarVariables(std::map<std::string,std::string>& renombre, int& comienzo);

     ~Argumentos();

private:
     std::list<Termino*> args;
};

#endif     
