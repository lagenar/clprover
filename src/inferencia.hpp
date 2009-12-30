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

/**
 * Clase abstracta que representa una regla de inferencia
 * en la búsqueda por resolución.
 * Es utilizada exclusivamente para proveer información al usuario
 * sobre el procedimiento de resolución y su traza.
 */
class Inferencia {
public:
     Inferencia(const Clausula& cl) :
	  claus(cl) { }
     
     /**
      * @returns Un identificador del tipo de inferencia.
      */
     virtual std::string getId() const = 0;
     
     /**
      * @returns La clausula inferida
      */
     const Clausula& getClausula() const {
	  return claus;
     }
     
     /**
      * Inserta en la lista pasada los padres de la inferencia(premisas)
      */
     virtual void getPadres(std::list<Clausula>&) const = 0;
protected:
     Clausula claus;
};

class InferenciaHipotesis : public Inferencia {
public:
     InferenciaHipotesis(const Clausula& cl) :
	  Inferencia(cl) { }
     
     std::string getId() const {
	  return "Hipot";
     }    
     
     void getPadres(std::list<Clausula>& /*p*/) const { }
};

class InferenciaResolucion : public Inferencia {
public:
     InferenciaResolucion(Clausula res1, Clausula res2, const Clausula& cl) :
	  Inferencia(cl), res1(res1), res2(res2) { }
     
     std::string getId() const {
	  return "Res";
     }

     void getPadres(std::list<Clausula>& p) const {
	  p.push_back(res1);
	  p.push_back(res2);
     }
private:
     Clausula res1, res2;
};

class InferenciaFactorizacion : public Inferencia {
public:
     InferenciaFactorizacion(Clausula fact, const Clausula& cl) :
	  Inferencia(cl), fact(fact){ }

     std::string getId() const {
	  return "Factor";
     }

     void getPadres(std::list<Clausula>& p) const {
	  p.push_back(fact);
     }
private:
     Clausula fact;
};

#endif
