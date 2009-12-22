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

#ifndef CLAUSULA_HPP
#define CLAUSULA_HPP

#include <list>
#include <set>
#include "literal.hpp"

class LitComp 
{
public:
     bool operator()(const Literal& l1, const Literal& l2) 
     {
	  int r = l1.getId().compare(l2.getId());
	  if (r == 0) {
	       if (l1.getSigno() == l2.getSigno())
		    return l1.getString() < l2.getString();
	       else
		    return (!l1.getSigno() && l2.getSigno())
			 || l1.getString() < l2.getString();
	  } else
	       return r < 0;
     }
};

class Clausula {
public:
     typedef std::set<Literal, LitComp>::const_iterator const_iterator;
     typedef std::set<Literal, LitComp>::iterator iterator;
     typedef enum { REGLA, HECHO, OBJ } t_horn;

     Clausula() : tautologica(false) { }
     
     int cantLiterales() const;

     bool esUnitaria() const;

     bool esDeHorn(t_horn& t) const;

     bool esDeHorn() const;

     bool esVacia() const;

     bool esTautologica() const;
     
     const std::string getString() const;
     
     const_iterator begin() const;

     const_iterator end() const;

     bool contieneComplementario(const Literal& lit) const;
     
     bool contienePredicado(const std::string& id_pred) const;

     void resolventes(const Clausula& claus, std::list<Clausula>& res) const;
     
     bool resolventeUsandoPred(const Clausula& claus,
			       const std::string& id_pred, Clausula& res) const;

     void factores(std::list<Clausula>& fact) const;

     bool equivalente(const Clausula& C) const;

     Clausula renombrarVariables(int& comienzo) const;
     
     std::map<std::string, int> aparicionesPredicados() const;

     void agregarLiteral(const Literal& lit);
private:
     std::set<Literal, LitComp> literales;
     bool tautologica;
     
     void agregarLitsResolvente(const Clausula& c, const const_iterator& lit,
				const Sustitucion& s);
};

#endif
