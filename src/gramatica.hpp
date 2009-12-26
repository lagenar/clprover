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

namespace client
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
     client::funcion,
     (std::string, nombre)
     (std::vector<client::termino>, args)
)

BOOST_FUSION_ADAPT_STRUCT(
     client::literal,
     (std::string, nombre)
     (std::vector<client::termino>, args)
     (bool, signo)
)

namespace client
{
     typedef enum { Func, Pred } t_id;
     class Cmp {
     public:
	  bool operator()(const std::pair<int, t_id>& p1, const std::pair<int, t_id>& p2)
	  {
		if (p1.first < p2.first)
		     return true;
		else if (p1.first == p2.first)
		     return p1.second < p2.second;
		else 
		     return false;
	  }
     };

     typedef std::set<std::pair<int, t_id>, Cmp> t_attr;
     typedef std::map<std::string, t_attr> t_attrs;
     
     inline Termino* construir_variable(const std::string& v)
     {
	  return new Variable(v);
     }
     
     inline Termino* construir_funcion(const funcion& f, t_attrs& atributos)
     {
	  Funcion* res = new Funcion(f.nombre);
	  t_attrs::iterator it = atributos.find(f.nombre);
	  if (it != atributos.end())
	       it->second.insert(std::pair<int, t_id>(f.args.size(), Func));
	  else {
	       t_attr at;
	       at.insert(std::pair<int, t_id>(f.args.size(), Func));
	       atributos[f.nombre] = at;
	  }

	  BOOST_FOREACH(termino const& t, f.args)
	  {
	       Termino* arg;
	       if (const std::string* s = boost::get<std::string>(&t))
		    arg = construir_variable(*s);
	       else {
		    const funcion* f = boost::get<funcion>(&t);
		    arg = construir_funcion(*f, atributos);
	       }
	       res->agregarArgumento(*arg);
	       delete arg;
	  }
	  return res;
     }

     inline Literal* construir_literal(const literal& lit, t_attrs& atributos)
     {
	  Literal* res = new Literal(lit.nombre, lit.signo);
	  t_attrs::iterator it = atributos.find(lit.nombre);
	  if (it != atributos.end())
	       it->second.insert(std::pair<int, t_id>(lit.args.size(), Pred));
	  else {
	       t_attr at;
	       at.insert(std::pair<int, t_id>(lit.args.size(), Pred));
	       atributos[lit.nombre] = at;
	  }
	  	  
	  BOOST_FOREACH(termino const& t, lit.args)
	  {
	       Termino* arg;
	       if (const std::string* s = boost::get<std::string>(&t))
		    arg = construir_variable(*s);
	       else {
		    const funcion* f = boost::get<funcion>(&t);
		    arg = construir_funcion(*f, atributos);
	       }
	       res->agregarArgumento(*arg);
	       delete arg;
	  }
	  return res;
     }

     inline void construir_clausula(const std::vector<literal>& lits, Clausula& claus, t_attrs& atributos)
     {
	  BOOST_FOREACH(literal const& lit, lits)
	  {
	       Literal* l = construir_literal(lit, atributos);
	       claus.agregarLiteral(*l);
	       delete l;
	  }
     }
     
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

	       var %= char_("A-Z") >> *char_("a-zA-Z0-9");
	       
	       funId %= char_("a-z") >> *char_("a-zA-Z0-9");
	
	       fun  = funId [at_c<0>(_val) = _1]
		    >> -('(' >> term[push_back(at_c<1>(_val), _1)] % ',' >> ')');
	       
	       term %= (var | fun);

	       lit = eps [at_c<2>(_val) = true]
		    >> -char_('~') [at_c<2>(_val) = false]
		    >> funId [at_c<0>(_val) = _1]
		    >> '(' >> term[push_back(at_c<1>(_val), _1)] % ',' >> ')';
	
	  }
	  qi::rule<Iterator, literal(), ascii::space_type> lit;
	  qi::rule<Iterator, termino(), ascii::space_type> term;
	  qi::rule<Iterator, funcion(), ascii::space_type> fun;
	  qi::rule<Iterator, std::string(), ascii::space_type> funId;
	  qi::rule<Iterator, std::string(), ascii::space_type> var;
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

	       var %= char_("A-Z") >> *char_("a-zA-Z0-9");

	       funId %= char_("a-z") >> *char_("a-zA-Z0-9");
	
	       fun  = funId [at_c<0>(_val) = _1]
		    >> -('(' >> term[push_back(at_c<1>(_val), _1)] % ',' >> ')');
	       
	       term %= (var | fun);

	       lit = eps [at_c<2>(_val) = true]
		    >> -char_('~') [at_c<2>(_val) = false]
		    >> funId [at_c<0>(_val) = _1]
		    >> '(' >> term[push_back(at_c<1>(_val), _1)] % ',' >> ')';
		    

	       claus %= lit % '|';
	       
	  }
	  qi::rule<Iterator, std::vector<literal>(), ascii::space_type> claus;
	  qi::rule<Iterator, literal(), ascii::space_type> lit;
	  qi::rule<Iterator, termino(), ascii::space_type> term;
	  qi::rule<Iterator, funcion(), ascii::space_type> fun;
	  qi::rule<Iterator, std::string(), ascii::space_type> funId;
	  qi::rule<Iterator, std::string(), ascii::space_type> var;
     };
}	       
#endif
