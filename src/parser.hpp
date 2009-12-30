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

#ifndef PARSER_HPP
#define PARSER_HPP

#include <map>
#include "gramatica.hpp"
#include "clausulas.hpp"

/**
 * La clase parser es utilizada para analizar la sintaxis y semántica
 * de un conjunto de cláusulas en forma incremental. Se encarga de verificar
 * que las cláusulas estén bien definidas en términos de si mismas
 * (que esté sintácticamente bien definida, y posibles errores semánticos
 * en la misma cláusula) y en relación a las otras cláusulas anteriormente
 * ingresadas(por ejemplo, que las aridades de los identificadores sean las
 * mismas que en las otras cláusulas).
 * También provee soporte para eliminar cláusulas del conjunto analizado.
 * Los clientes de esta clase deben proveer identificadores(enteros) para cada cláusula
 * para luego poder eliminarla o pedirla.
 */
class Parser {
public:
     /**
      * Tipos de errores.
      */
     typedef enum {
	  Sintactico, /**< La clausula no está bien definida sintácticamente*/
	  Aridad, /**< Un identificador es utilizado con diferentes aridades*/
	  TipoId /**< Un identificador es utilizado para más de un tipo
		    de estructura lógica(predicado y función)
		    por ejemplo el literal p(p(X)) no es válido*/
     } t_error;
     
     /**
      * Agrega una cláusula(si está bien formada) al conjunto analizado.
      * Si hay un error, <EM>error</EM> se vuelve true, se asigna el tipo
      * de error al primer elemento del par <EM>E</EM> y si es un error
      * semántico el segundo elemento del par contiene el identificador
      * con error.
      * @pre No existe ninguna clausula con identificador <EM>id</EM>.
      * @param id Identificador asignado a la cláusula.
      * @param clausula Representación en forma de cadena de la cláusula.
      * @see t_error
      */
     void parseClausula(int id, const std::string& clausula,
			bool& error, std::pair<t_error, std::string>& E);

     /**
      * Elimina una cláusula del conjunto analizado.
      * @pre Existe una clausula con identificador <EM>id</EM>.
      * @param id Identificador de la cláusula.
      */
     void eliminarClausula(int id);
     
     /**
      * @pre Existe una cláusula con identificador <EM>id</EM>.
      * @returns La claúsula con identificador <EM>id</EM>.
      */
     Clausula getClausula(int id) const;

     /**
      * Inserta las cláusulas analizadas a <EM>l</EM>.
      */
     void getClausulas(std::list<Clausula>& l) const;
     
     /**
      * Inserta las cláusulas analizadas a <EM>claus</EM>.
      */
     void getClausulas(ConjuntoClausulas<>& claus) const;

     /**
      * Método de clase para obtener un literal a partir de su
      * representación en la sintáxis concreta.
      * El parámetro <EM>error</EM> se vuelve true si se detectó
      * un error sintáctico o semántico en el análisis. En este caso a
      * <EM>E</EM> se le asigna la información del error.
      * @returns Un puntero al literal creado si no hubo errores, NULL
      * en caso contrario. El cliente debe liberar el puntero.
      */
     static Literal* parseLiteral(const std::string& lit, bool& error,
				  std::pair<t_error, std::string>& E);
private:   
     std::map<int, Clausula> clausulas;
     
     //para manejo de errores semánticos
     typedef std::map<std::string, std::set<int> > t_apar;
     typedef std::map<std::string, std::pair<int, gramatica::t_id> > t_attr;
     
     t_apar aparicionesId;
     t_attr atributosId;

     typedef gramatica::gramatica_clausula<std::string::const_iterator> gramatica_clausula;
     typedef gramatica::gramatica_literal<std::string::const_iterator> gramatica_literal;

     void verificarAtributos(bool& error,
			     std::pair<t_error, std::string>& E, const gramatica::t_attrs& at);
     
     void agregarAtributos(int id, const gramatica::t_attrs& atr_clausula);
};

#endif
