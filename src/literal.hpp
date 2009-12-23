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
 * Basicamente es un predicado con signo. Caracterizado por tener
 * identificador, signo y argumentos(éstos son términos).
 */
class Literal {
public:
     typedef Argumentos::const_iterator const_iterator;

     Literal(const std::string& id, bool signo);

     Literal(const std::string& id, const Argumentos& args, bool signo);

     bool getSigno() const;

     const std::string& getId() const;

     const std::string getString() const;
     
     int aridad() const;
     
     bool operator==(const Literal& otro) const;
     
     bool operator!=(const Literal& otro) const;

     const_iterator begin() const;

     const_iterator end() const;
     
     /**
      * Unifica con otro literal. Las sustituciones del unificador más general(umg),
      * si existe, se insertan a s.      
      * @param otro Literal con el que se unifica.
      * @param s Sustitución en la que se inserta el resultado de la unificación.
      * @returns true si existe el umg, false en caso contrario.
      */
     bool unificar(const Literal& otro, Sustitucion& s) const;
     
     bool unificable(const Literal& otro) const;

     bool unificarComplementario(const Literal& otro, Sustitucion& s) const;
     
     void aplicarSustitucion(const Sustitucion& s);

     void agregarArgumento(const Termino& t);

     void setSigno(bool s);
     
     void renombrarVariables(std::map<std::string,std::string>& renombre, int& comienzo);

     void renombrarVariables(int& comienzo);
private:
     Argumentos args;
     bool signo;
     std::string id;
};

#endif
