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

#ifndef SUSTITUCION_HPP
#define SUSTITUCION_HPP

#include <map>
#include <iostream>
#include "termino.hpp"

class Termino;

/**
 * Esta clase representa un sustitución de variables a términos.
 */
class Sustitucion {
public:     
     const std::string getString() const;
     
     /**
      * @param id Identificador de la variable.
      * @returns Un puntero al término que sustituye a la variable
      * con identificador id, si la variable no tiene sustitución
      * retorna NULL
      */
     const Termino* getSustitucion(const std::string& id) const;

     /**
      * Agrega una sustitución, si la variable ya tenía una sustitución asignada
      * la sobreescribe.
      * @param id Identificador de la variable.
      * @param t Término sustituyente.
      */
     void agregarSustitucion(const std::string& id, const Termino& t);
     
     /**
      * Composición con otra sustitución.
      * @param s Sustitución con la cual componer.
      */
     void componer(const Sustitucion& s);

     ~Sustitucion();
private:
     typedef std::map<std::string, Termino*> t_sust;
     t_sust susts;
};

#endif
