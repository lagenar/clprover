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

#ifndef LITERAL_HPP
#define LITERAL_HPP

#include <list>
#include <iostream>
#include <map>

#include "termino.hpp"
#include "argumentos.hpp"
#include "sustitucion.hpp"

/**
 * Esta clase representa un literal de lógica de primer orden,
 * Básicamente es un predicado con signo. Caracterizado por tener
 * identificador, signo y argumentos(éstos son términos).
 */
class Literal {
public:
     typedef Argumentos::const_iterator const_iterator;
     
     Literal(const std::string& id, bool signo);
     
     Literal(const std::string& id, const Argumentos& args, bool signo);

     /**
      * @returns true si el signo del literal es positivo,
      * false en caso contrario.
      */
     bool getSigno() const;
     
     const std::string& getId() const;

     const std::string getString() const;
     
     int aridad() const;
     
     /**
      * Sobrecarga del operador de igualdad, dos literales son
      * iguales si tienen el mismo signo, mismo identificador y sus argumentos son
      * iguales.
      * @param otro Literal a comparar.
      */
     bool operator==(const Literal& otro) const;
     
     bool operator!=(const Literal& otro) const;

     const_iterator begin() const;

     const_iterator end() const;
     
     /**
      * Unifica con otro literal. Las sustituciones del unificador más general(umg)
      * (si existe) se insertan a s. 
      * @param otro Literal con el que se unifica.
      * @param s Sustitución en la que se inserta el resultado de la unificación.
      * @returns true si existe el umg, false en caso contrario.
      */
     bool unificar(const Literal& otro, Sustitucion& s) const;
     
     /**
      * @returns true si el literal es unificable con <EM>otro</EM>,
      * false si no son unificables.
      */
     bool unificable(const Literal& otro) const;
     
     /**
      * Unifica con otro literal, pero a diferencia de <EM>unificar</EM>,
      * unifica literales complementarios(mismo identificador y signo opuesto).
      * El umg(si existe) se inserta a <EM>s</EM>.
      * @param otro literal con el que se unifica.
      * @param s Sustitución en la cual se inserta el resultado de unificación.
      * @returns true si existe el umg, false en caso contrario.
      */
     bool unificarComplementario(const Literal& otro, Sustitucion& s) const;
     
     bool unificarSubsuncion(const Literal& otro, Sustitucion& s) const;

     /**
      * Aplica una sustitucion de variables a términos a los argumentos
      * del literal.
      * @param s sustitución a aplicar.
      */
     void aplicarSustitucion(const Sustitucion& s);

     void agregarArgumento(const Termino& t);
     
     void setSigno(bool s);

     /**
      * Renombra las variables del literal.
      * Para las variables que tienen un renombre definido en el mapa <EM>renombre</EM>,
      * se usa ese valor. A las variables que no tienen un renombre definido se les
      * asigna identificadores de la forma Xcomienzo, Xcomienzo+1, ..., Xcomienzo+n y
      * se insertan los renombres en el mapa.
      * El valor final de <EM>comienzo</EM> es <EM>comienzo</EM> + n + 1.
      * @param renombre Mapa de renombres de variables.
      * @param comienzo Número de comienzo del renombre.
      */
     void renombrarVariables(std::map<std::string,std::string>& renombre, int& comienzo);

     /**
      * Sobrecarga del método renombrarVariables que no aplica renombres externos,
      * las variables quedan identificadas de la forma Xcomienzo, ..., Xcomienzo+n.
      * El valor final de <EM>comienzo</EM> es <EM>comienzo</EM> + n + 1.
      * @param comienzo Número de comienzo del renombre.
      * @see renombrarVariables(std::map<std::string,std::string>&, int&)
      */
     void renombrarVariables(int& comienzo);     
private:
     Argumentos args;
     bool signo;
     std::string id;
};

#endif
