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

     struct constructor_termino : boost::static_visitor<Termino*>
     {					       
	  Termino* operator()(const std::string& var) const
	  {
	       return new Variable(var);
	  }
	  
	  Termino* operator()(const funcion& f) const
	  {    
	       Funcion* func = new Funcion(f.nombre);

	       BOOST_FOREACH(termino const& t, f.args)
	       {
		    Termino* arg;
		    arg = boost::apply_visitor(constructor_termino(), t);
		    func->agregarArgumento(*arg);
		    delete arg;
	       }

	       return func;
	  }
     };


     Literal* construir_literal(const literal& lit)
     {
	  Literal* res = new Literal(lit.nombre, lit.signo);
	  
	  BOOST_FOREACH(termino const& t, lit.args)
	  {
	       Termino* arg = boost::apply_visitor(constructor_termino(), t);
	       res->agregarArgumento(*arg);
	       delete arg;
	  }
	  return res;
     }

     void construir_clausula(const std::vector<literal>& lits, Clausula& claus)
     {
	  BOOST_FOREACH(literal const& lit, lits)
	  {
	       Literal* l = construir_literal(lit);
	       claus.agregarLiteral(*l);
	       delete l;
	  }
     }
	  
     template <typename Iterator>
     struct gramatica_termino : qi::grammar<Iterator, std::vector<literal>(), ascii::space_type>
     {
	  gramatica_termino() : gramatica_termino::base_type(claus)
	  {
	       using ascii::char_;
	       using namespace qi::labels;
	       using qi::eps;
	       using phoenix::at_c;
	       using phoenix::push_back;

	       var %= char_("A-Z") >> *char_("a-z")
		    ;

	       funId %= char_("a-z") >> *char_("a-z")
		    ;

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
	       
int main(int argc, char **argv)
{
     std::string s(argv[1]);
     std::vector<client::literal> t;
     typedef client::gramatica_termino<std::string::const_iterator> gramatica_termino;
     gramatica_termino g;
     using boost::spirit::ascii::space;
     std::string::const_iterator iter = s.begin();
     std::string::const_iterator end = s.end();
     bool r = phrase_parse(iter, end, g, space, t);
     if (r && iter == end) {
	  Clausula C;
	  construir_clausula(t, C);
	  std::cout << C.getString() << std::endl;
     }
     else
     	  std::cout << "Fallo de parseo" << std::endl;
     
     return 0;
}
