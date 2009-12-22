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

class Parser {
public:
     typedef enum {Sintactico, Aridad, TipoId} t_error;
     
     void parseClausula(int id, const std::string& clausula,
			bool& error, std::pair<t_error, std::string>& E);

     void eliminarClausula(int id);

     Clausula getClausula(int id) const;

     void getClausulas(std::list<Clausula>& l) const;

     void getClausulas(ConjuntoClausulas<>& claus) const;

     static Literal* parseLiteral(const std::string& lit, bool& error,
				  std::pair<t_error, std::string>& E);
private:   
     std::map<int, Clausula> clausulas;
     
     //para manejo de errores semánticos
     typedef std::map<std::string, std::set<int> > t_apar;
     typedef std::map<std::string, std::pair<int, client::t_id> > t_attr;
     
     t_apar aparicionesId;
     t_attr atributosId;

     typedef client::gramatica_clausula<std::string::const_iterator> gramatica_clausula;
     typedef client::gramatica_literal<std::string::const_iterator> gramatica_literal;

     void verificarAtributos(bool& error,
			     std::pair<t_error, std::string>& E, const client::t_attrs& at);
     
     void agregarAtributos(int id, const client::t_attrs& atr_clausula);
};

#endif
