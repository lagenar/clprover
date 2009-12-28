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

#ifndef GRAMATICA_HPP
#define GRAMATICA_HPP

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <boost/foreach.hpp>

#include <iostream>
#include <string>
#include <vector>

#include "termino.hpp"
#include "literal.hpp"
#include "clausula.hpp"

namespace gramatica
{
     namespace fusion = boost::fusion;
     namespace phoenix = boost::phoenix;
     namespace qi = boost::spirit::qi;
     namespace ascii = boost::spirit::ascii;

     struct funcion;

     typedef boost::variant<
	  std::string,
	  boost::recursive_wrapper<funcion> > termino;

     struct funcion {
	  std::string nombre;
	  std::vector<termino> args;
     };

     struct literal {
	  std::string nombre;
	  std::vector<termino> args;
	  bool signo;
     };
}

BOOST_FUSION_ADAPT_STRUCT(
     gramatica::funcion,
     (std::string, nombre)
     (std::vector<gramatica::termino>, args)
)

BOOST_FUSION_ADAPT_STRUCT(
     gramatica::literal,
     (std::string, nombre)
     (std::vector<gramatica::termino>, args)
     (bool, signo)
)

namespace gramatica
{
     typedef enum { Func, Pred } t_id;
     typedef std::set<std::pair<int, t_id> > t_attr;
     typedef std::map<std::string, t_attr> t_attrs;
     
     Literal* construir_literal(const literal& lit, t_attrs& atributos);

     Clausula construir_clausula(const std::vector<literal>& lits, t_attrs& atributos);
     
     template <typename Iterator>
     struct gramatica_literal : qi::grammar<Iterator, literal(), ascii::space_type>
     {
	  gramatica_literal() : gramatica_literal::base_type(lit)
	  {
	       using ascii::char_;
	       using namespace qi::labels;
	       using qi::eps;
	       using phoenix::at_c;
	       using phoenix::push_back;
	       
	       //Identificador de variables
	       varId %= char_("A-Z") >> *char_("a-zA-Z0-9");
	       //Identificador de funciones y predicados
	       funPredId %= char_("a-z") >> *char_("a-zA-Z0-9");
	       
	       fun  = funPredId           [at_c<0>(_val) = _1]
		    >> -('('
			 >> term[push_back(at_c<1>(_val), _1)] % ',' //argumentos
			 >> ')');
	       
	       term %= (varId | fun);

	       lit = eps              [at_c<2>(_val) = true] //signo por defecto positivo
		    >> -char_('~')    [at_c<2>(_val) = false]
		    >> funPredId      [at_c<0>(_val) = _1]
		    >> '('
		    >> term[push_back(at_c<1>(_val), _1)] % ',' //argumentos
		    >> ')';
	
	  }
	  qi::rule<Iterator, literal(), ascii::space_type> lit;
	  qi::rule<Iterator, termino(), ascii::space_type> term;
	  qi::rule<Iterator, funcion(), ascii::space_type> fun;
	  qi::rule<Iterator, std::string(), ascii::space_type> funPredId;
	  qi::rule<Iterator, std::string(), ascii::space_type> varId;
     };

     template <typename Iterator>
     struct gramatica_clausula : qi::grammar<Iterator, std::vector<literal>(), ascii::space_type>
     {
	  gramatica_clausula() : gramatica_clausula::base_type(claus)
	  {
	       using ascii::char_;
	       using namespace qi::labels;
	       using qi::eps;
	       using phoenix::at_c;
	       using phoenix::push_back;
	       
	       // una clausula es uno o más literales separados por '|'
	       claus %= lit % '|';
	       
	  }
	  qi::rule<Iterator, std::vector<literal>(), ascii::space_type> claus;
	  gramatica_literal<Iterator> lit;
     };
}	       
#endif
