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

#include "parser.hpp"
#include <cassert>
#include <algorithm>

void Parser::verificarAtributos(bool& error, std::pair<t_error, std::string>& E,
				const gramatica::t_attrs& atr_clausula)
{
     error = false;
     gramatica::t_attrs::const_iterator it = atr_clausula.begin();
     while (!error && it != atr_clausula.end()) {
	  //errores internos de la clausula
	  if (it->second.size() > 1) {
	       error = true;
	       gramatica::t_attr::const_iterator it_atr = it->second.begin();
	       gramatica::t_attr::const_iterator sig = it_atr;
	       ++sig;
	       if (it_atr->first != sig->first)
		    E = std::pair<t_error, std::string>(Aridad, it->first);
	       else
		    E = std::pair<t_error, std::string>(TipoId, it->first);
	  } else {
	       //errores con las otras clausulas
	       std::pair<int, gramatica::t_id> at = *(it->second.begin());
	       t_attr::const_iterator it_glob = atributosId.find(it->first);
	       if (it_glob != atributosId.end()) {
		    if (at != it_glob->second) {
			 error = true;
			 if (at.first != it_glob->second.first)
			      E = std::pair<t_error, std::string>(Aridad, it->first);
			 else
			      E = std::pair<t_error, std::string>(TipoId, it->first);
		    }
	       }
	  }
	  ++it;
     }
}

void Parser::agregarAtributos(int id, const gramatica::t_attrs& atr_clausula)
{
     gramatica::t_attrs::const_iterator it;
     for (it = atr_clausula.begin(); it != atr_clausula.end(); ++it) {
	  atributosId[it->first] = *(it->second.begin());
	  t_apar::iterator it_a = aparicionesId.find(it->first);
	  if (it_a != aparicionesId.end())
	       it_a->second.insert(id);
	  else {
	       std::set<int> s;
	       s.insert(id);
	       aparicionesId[it->first] = s;
	  }
     }
}

void Parser::eliminarClausula(int id)
{
     std::map<int, Clausula>::iterator claus = clausulas.find(id);
     assert(claus != clausulas.end());
     clausulas.erase(claus++);
     t_apar::iterator it_a;
     for (it_a = aparicionesId.begin(); it_a != aparicionesId.end(); ++it_a) {
	  std::set<int> s = it_a->second;
	  if (s.find(id) != s.end()) {
	       s.erase(id);
	       if (s.empty())
		    atributosId.erase(it_a->first);
	       it_a->second = s;
	  }
     }
}

void Parser::parseClausula(int id, const std::string& clausula,
			   bool& error, std::pair<t_error, std::string>& E)
{
     assert(clausulas.find(id) == clausulas.end());
     std::vector<gramatica::literal> C;
     gramatica_clausula g;
     using boost::spirit::ascii::space;
     std::string::const_iterator iter = clausula.begin();
     std::string::const_iterator end = clausula.end();
     bool r = phrase_parse(iter, end, g, space, C);
     if (r && iter == end) {
	  Clausula cl;
	  gramatica::t_attrs atr_claus;
	  cl = gramatica::construir_clausula(C, atr_claus);
	  verificarAtributos(error, E, atr_claus);
	  if (!error) {
	       agregarAtributos(id, atr_claus);
	       clausulas[id] = cl;
	  }
     } else {
	  error = true;
	  E.first = Sintactico;
     }
}

Clausula Parser::getClausula(int id) const
{
     std::map<int, Clausula>::const_iterator i = clausulas.find(id);
     assert(i != clausulas.end());
     return i->second;
}

void Parser::getClausulas(std::list<Clausula>& l) const
{
     std::map<int, Clausula>::const_iterator it;
     for (it = clausulas.begin(); it != clausulas.end(); ++it)
	  l.push_back(it->second);
}

void Parser::getClausulas(ConjuntoClausulas<>& claus) const
{
     std::map<int, Clausula>::const_iterator it;
     for (it = clausulas.begin(); it != clausulas.end(); ++it)
	  claus.agregarClausula(it->second);
}

Literal* Parser::parseLiteral(const std::string& lit, bool& error,
			      std::pair<t_error, std::string>& E)
{
     error = false;
     gramatica::literal c_l;
     gramatica_literal g_lit;
     using boost::spirit::ascii::space;
     std::string::const_iterator iter = lit.begin();
     std::string::const_iterator end = lit.end();
     bool r = phrase_parse(iter, end, g_lit, space, c_l);
     Literal* res = NULL;
     if (r && iter == end) {
	  gramatica::t_attrs atr_lit;
	  res = gramatica::construir_literal(c_l, atr_lit);
	  
	  gramatica::t_attrs::const_iterator it = atr_lit.begin();
	  while (!error && it != atr_lit.end()) {
	       if (it->second.size() > 1) {
		    error = true;
		    gramatica::t_attr::const_iterator it_atr = it->second.begin();
		    gramatica::t_attr::const_iterator sig = it_atr;
		    ++sig;
		    if (it_atr->first != sig->first)
			 E = std::pair<t_error, std::string>(Aridad, it->first);
		    else
			 E = std::pair<t_error, std::string>(TipoId, it->first);
	       }
	       ++it;
	  }
	  if (error) {
	       delete res;
	       res = NULL;
	  }
     } else {
	  error = true;
	  E.first = Sintactico;
     }
     return res;
}
