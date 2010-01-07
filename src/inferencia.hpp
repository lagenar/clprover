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

#ifndef INFERENCIA_HPP
#define INFERENCIA_HPP
#include "clausula.hpp"
#include <list>
#include <cassert>

/**
 * Clase abstracta que representa una regla de inferencia
 * en la búsqueda por resolución.
 * Es utilizada exclusivamente para proveer información al usuario
 * sobre el procedimiento de resolución y su traza.
 */
class Inferencia {
public:
     typedef enum {Hipot, Factor, Res} t_inf;

     Inferencia(const Clausula& cl) :
	  claus(cl) { }
     
     /**
      * @returns El tipo de inferencia
      */
     virtual t_inf getTipo() const = 0;
     
     /**
      * @returns La clausula inferida
      */
     const Clausula& getClausula() const {
	  return claus;
     }
     
     /**
      * Inserta en la lista pasada los padres de la inferencia(premisas)
      */
     virtual void getPadres(std::list<int>&) const = 0;

     /**
      * Modifica los padres de la inferencia
      */
     virtual void setPadres(const std::list<int>&) = 0;
protected:
     Clausula claus;
};

class InferenciaHipotesis : public Inferencia {
public:
     InferenciaHipotesis(const Clausula& cl) :
	  Inferencia(cl) { }

     t_inf getTipo() const {
	  return Hipot;
     }
     
     void getPadres(std::list<int>& /*p*/) const { }

     void setPadres(const std::list<int>&) { }
};

class InferenciaResolucion : public Inferencia {
public:
     InferenciaResolucion(int res1, int res2, const Clausula& cl) :
	  Inferencia(cl), res1(res1), res2(res2) { }
     
     t_inf getTipo() const {
	  return Res;
     }

     void getPadres(std::list<int>& p) const {
	  p.push_back(res1);
	  p.push_back(res2);
     }

     void setPadres(const std::list<int>& p) {
	  assert(p.size() == 2);
	  std::list<int>::const_iterator it = p.begin();
	  res1 = *it;
	  ++it;
	  res2 = *it;
     }
private:
     int res1, res2;
};

class InferenciaFactorizacion : public Inferencia {
public:
     InferenciaFactorizacion(int fact, const Clausula& cl) :
	  Inferencia(cl), fact(fact){ }

     t_inf getTipo() const {
	  return Factor;
     }

     void getPadres(std::list<int>& p) const {
	  p.push_back(fact);
     }
     
     void setPadres(const std::list<int>& p) {
	  assert(p.size() == 1);
	  std::list<int>::const_iterator it = p.begin();
	  fact = *it;
     }
private:
     int fact;
};

#endif
