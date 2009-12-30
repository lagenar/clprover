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

/**
 * Clase que define el orden parcial para un conjunto de literales.
 * Utilizada en la clase Clausula para parametrizar el tipo set de la stl.
 */
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

/**
 * Clase que representa una cláusula de lógica de primer orden.
 * Una cláusula es una disyunción de literales.
 * Aquí se representa con un contenedor iterable de literales(set).
 */
class Clausula {
public:
     typedef std::set<Literal, LitComp>::const_iterator const_iterator;
     typedef std::set<Literal, LitComp>::iterator iterator;
     /**
      * tipos de clausulas de Horn
      */
     typedef enum {REGLA, HECHO, OBJ} t_horn;

     Clausula() : tautologica(false) { }
     
     int cantLiterales() const;
     
     /**
      * @returns true si contiene un sólo literal,
      * false en caso contrario.
      */
     bool esUnitaria() const;

     /**
      * @returns true si es una cláusula de Horn, false en caso contrario.
      * Una cláusula es de Horn si tiene como máximo un literal positivo.
      * @param t Si la cláusula es de Horn aquí se guarda el tipo
      * @see t_horn
      */     
     bool esDeHorn(t_horn& t) const;

     /**
      * @returns true si es una cláusula de Horn, false en caso contrario.
      * @see esDeHorn(t_horn&)
      */
     bool esDeHorn() const;

     /**
      * @returns true si no contiene literales, false en caso contrario.
      */
     bool esVacia() const;

     /**
      * @returns true si es tautológica, false en caso contrario.
      * Una cláusula es tautológica si contiene dos literales con
      * igual identificador y argumentos pero de signo opuesto.
      */
     bool esTautologica() const;
     
     const std::string getString() const;
     
     const_iterator begin() const;

     const_iterator end() const;

     /**
      * @returns true si la cláusula contiene un literal que pueda
      * resolverse con <EM>lit</EM>, false en caso contrario.
      */
     bool contieneComplementario(const Literal& lit) const;
     
     /**
      * @returns true si la cláusula contiene un literal
      * con identificador <EM>id_pred</EM>, false en caso contrario.
      */
     bool contienePredicado(const std::string& id_pred) const;
     
     /**
      * Inserta todas las resolventes entre la cláusula y <EM>claus</EM> a la
      * lista de cláusulas <EM>res</EM>.
      * @param claus Claúsula con la que se resuelve.
      * @param res Lista en donde se insertan las resolventes.
      */
     void resolventes(const Clausula& claus, std::list<Clausula>& res) const;
     
     /**
      * Calcula una resolvente entre la cláusula y <EM>claus</EM> usando literales
      * con identificador <EM>id_pred</EM>. La resolvente(si existe) se asigna a <EM>res</EM>.
      * Si una cláusula contiene más de un literal con identificador <EM>id_pred</EM>, no
      * está definido cual se usa para la resolución.
      * @returns true si fue posible calcular la resolvente, false en caso contrario.
      * @param claus Cláusula con la que se resuelve.
      * @param id_pred Identificador del predicado a utilizar.
      * @param res Cláusula contenedora del resultado.
      */
     bool resolventeUsandoPred(const Clausula& claus,
			       const std::string& id_pred, Clausula& res) const;
     
     /**
      * Inserta los factores de la cláusula a la lista <EM>fact</EM>.
      * Un factor de una cláusula se genera unificando dos literales en ésta.
      * Por ejemplo:
      * a(X) es un factor de la cláusula a(X) | a(Y).
      * @param fact Contenedor en donde se insertan los factores.
      */
     void factores(std::list<Clausula>& fact) const;

     /**
      * @returns true si la cláusula es equivalente a C,
      * false en caso contrario.
      * La equivalencia en este método no se considera al igual que
      * en lógica de primer orden. Aquí, dos cláusulas son equivalentes
      * si luego de renombrar las variables de las cláusulas con el mismo <EM>comienzo</EM>,
      * las cláusulas contienen los mismos literales. Esto es más restrictivo que
      * la relación de equivalencia de la lógica de primer orden. Por lo que este método,
      * si se lo compara con la relación de equivalencia más general, es correcto pero incompleto,
      * es decir, si las cláusulas no son equivalentes retorna false, pero si son equivalentes puede
      * que retorne false también, dependiendo de si puede o no detectar la equivalencia.
      * @param C Cláusula a comparar por equivalencia.
      * @see renombrarVariables
      */
     bool equivalente(const Clausula& C) const;
     
     /**
      * Renombra las variables de la forma Xcomienzo, Xcomienzo+1,..., Xcomienzo+n.
      * El valor final de <EM>comienzo</EM> es <EM>comienzo</EM> + n + 1.
      * @param comienzo Comienzo de la secuencia de renombre.
      */
     Clausula renombrarVariables(int& comienzo) const;
     
     /**
      * @returns Un mapa con la cantidad de apariciones de cada identificador de predicado.
      */
     std::map<std::string, int> aparicionesPredicados() const;

     /**
      * Agrega un literal a la cláusula, si la cláusula ya contiene
      * un literal igual, por propiedad de idempotencia no se agrega.
      * @param lit Literal a agregar.
      */
     void agregarLiteral(const Literal& lit);
private:
     std::set<Literal, LitComp> literales;
     bool tautologica;
     
     void agregarLitsResolvente(const Clausula& c, const const_iterator& lit,
				const Sustitucion& s);
};

#endif
