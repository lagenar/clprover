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

#ifndef CLAUSULAS_HPP
#define CLAUSULAS_HPP

#include <set>
#include "clausula.hpp"

/**
 * Clase que provee un orden parcial para el ordenamiento
 * de cláusulas en la clase ConjuntoClausulas.
 * Este orden parcial ordena las cláusulas por cantidad de literales.
 * Dos cláusulas se consideran iguales sólo si las representaciones
 * en forma de cadena son iguales.
 */
class ClausComp {
public:
     bool operator()(const Clausula& c1, const Clausula& c2) const
     {
	  if (c1.cantLiterales() < c2.cantLiterales())
	       return true;
	  else if (c1.cantLiterales() > c2.cantLiterales())
	       return false;
	  else
	       return c1.getString() < c2.getString();
     }
};

/**
 * Clase que representa un conjunto de clausulas. Provee métodos
 * para agregar y eliminar cláusulas y para indagar sobre
 * propiedades del conjunto.
 * Esta clase puede parametrizarse con una clase que defina el orden
 * parcial para la comparación de las cláusulas.
 * La clase utilizada por defecto es ClausComp.
 */
template <typename Compare = ClausComp>
class ConjuntoClausulas {
public:
     typedef typename std::set<Clausula, Compare>::const_iterator const_iterator;
     typedef typename std::set<Clausula, Compare>::iterator iterator;
     typedef typename std::set<Clausula, Compare>::size_type size_type;

     ConjuntoClausulas() { }
     
     template<typename Iterator>
     ConjuntoClausulas(Iterator begin, Iterator end, bool numerar=false)
     {
	  int id = 0;
	  for (Iterator it = begin; it != end; ++it) {
	       Clausula cl = *it;
	       if (numerar) {
		    cl.setIdResolucion(id);
		    id++;
	       }
	       agregarClausula(cl);
	  }
     }
     
     size_type cantidadClausulas() const { return clausulas.size(); }
     
     /**
      * @returns true si no contiene cláusulas,
      * false en caso contrario.
      */
     bool esVacio() const { return clausulas.size() == 0; }

     /**
      * @returns true si cada una de las cláusulas del conjunto
      * es de Horn, false en caso contrario.
      * @see Clausula::esDeHorn
      */
     bool esDeHorn() const 
     {
	  for (const_iterator it = clausulas.begin(); it != clausulas.end(); ++it)
	       if (!it->esDeHorn())
		    return false;
	  return true;
     }
     
     /**
      * @returns true si el conjunto es una lógica de
      * programa. Un conjunto es una lógica de programa
      * cuando todas sus cláusulas son de Horn, pero ninguna
      * de ellas es una cláusula objetivo.
      * @see Clausula::t_horn
      */
     bool esLogicaPrograma() const
     {
	  for (const_iterator it = clausulas.begin(); it != clausulas.end(); ++it) {
	       Clausula::t_horn t;
	       if (!it->esDeHorn(t) || t == Clausula::OBJ)
		    return false;
	  }
	  return true;
     }

     void agregarClausula(const Clausula& C)
     {
	  clausulas.insert(C);
     }
     
     /**
      * Elimina las cláusulas tautológicas del conjunto.
      * @returns true si alguna cláusula fue eliminada,
      * false en caso contrario.
      * @see Clausula::esTautologica
      */
     bool simplificarPorTautologicas()
     {
	  bool simp = false;
	  iterator claus = clausulas.begin();
	  while (claus != clausulas.end()) {
	       if (claus->esTautologica()) {
		    simp = true;
		    clausulas.erase(claus++);		   
	       } else
		    ++claus;
	  }
	  return simp;
     }

     /**
      * Simplica el conjunto por cláusulas equivalentes.
      * Si <EM>n</EM> > 1 cláusulas son equivalentes entre sí,
      * se eliminan n-1 de éstas cláusulas.
      * @returns true si se realizó alguna simplificación,
      * false en caso contrario.
      * @see Clausula::equivalente
      */
     bool simplificarPorEquivalentes()
     {
	  bool simp = false;
	  for (iterator it1 = clausulas.begin(); it1 != clausulas.end(); ++it1) {
	       iterator it2 = it1;
	       ++it2;
	       while (it2 != clausulas.end()) {
		    if (it1->equivalente(*it2)) {
			 simp = true;
			 clausulas.erase(it2++);
		    } else
			 ++it2;
	       }
	  }
	  return simp;
     }
     
     /**
      * Simplifica el conjunto por literales
      * puros. Un literal es puro en un conjunto,
      * si no existe un literal complementario en
      * el conjunto. En este caso, la cláusula que contiene
      * al literal se elimina.
      * @returns true si se eliminó alguna clausula, false
      * en caso contrario.
      */
     bool simplificarLiteralesPuros()
     {
	  bool simp = false;
	  iterator claus = clausulas.begin();	  
	  while (claus != clausulas.end()) {
	       bool claus_eliminada = false;
	       Clausula::const_iterator lit = claus->begin();	       
	       while (!claus_eliminada && lit != claus->end()) {
		    bool contieneComp = false;
		    const_iterator claus2 = clausulas.begin();
		    while (!contieneComp && claus2 != clausulas.end()) {		      
			 contieneComp = claus2->contieneComplementario(*lit);
			 ++claus2;
		    }		    
		    if (!contieneComp) {
			 simp = true;
			 clausulas.erase(claus++);
			 claus_eliminada = true;
		    } else
			 ++lit;
	       }
	       if (!claus_eliminada)
		    ++claus;
	  }
	  return simp;
     }
     
     /**
      * @returns true si la cláusula <EM>c</EM> está
      * en el conjunto. La comparación por igualdad de las clausulas
      * está determinada por la parametrización de la clase. 
      */
     bool contieneClausula(const Clausula& c) const
     {
	  const_iterator it = clausulas.find(c);
	  return it != clausulas.end();
     }

     void getClausulas(std::list<Clausula>& l) const
     {
	  for (const_iterator it = clausulas.begin(); it != clausulas.end(); ++it)
	       l.push_back(*it);
     }
     
     /**
      * @returns Un conjunto de identificadores de predicados eliminables
      * del conjunto. Un predicado(literal) es eliminable en un conjunto
      * si no aparece en más de una ocasión en cada una de las cláusulas.
      */
     std::set<std::string> predicadosEliminables() const
     {
	  std::map<std::string, bool> eliminable;
	  for (const_iterator it = clausulas.begin(); it != clausulas.end(); ++it) {
	       std::map<std::string, int> apar = it->aparicionesPredicados();
	       std::map<std::string, int>::const_iterator it_apar;
	       for (it_apar = apar.begin(); it_apar != apar.end(); ++it_apar)
		    if (it_apar->second > 1)
			 eliminable[it_apar->first] = false;
		    else {
			 std::map<std::string, bool>::const_iterator it_elim = eliminable.find(it_apar->first);
			 if (it_elim == eliminable.end())
			      eliminable[it_apar->first] = true;
		    }
	  }
	  std::set<std::string> eliminables;
	  for (std::map<std::string, bool>::const_iterator it_elim = eliminable.begin();
	       it_elim != eliminable.end(); ++it_elim)
	       if (it_elim->second)
		    eliminables.insert(it_elim->first);

	  return eliminables;
     }

     const_iterator begin() const
     {
	  return clausulas.begin();
     }

     const_iterator end() const
     {
	  return clausulas.end();
     }
     
     iterator begin()
     {
	  return clausulas.begin();
     }

     iterator end()
     {
	  return clausulas.end();
     }
     
     void eliminar(iterator c)
     {
	  clausulas.erase(c);
     }
private:
     std::set<Clausula, Compare> clausulas;
};

#endif
