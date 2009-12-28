#include "gramatica.hpp"

Termino* construir_variable(const std::string& v)
{
     return new Variable(v);
}

Termino* construir_funcion(const gramatica::funcion& f, gramatica::t_attrs& atributos)
{
     using namespace gramatica;
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

Literal* gramatica::construir_literal(const literal& lit, t_attrs& atributos)
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

Clausula gramatica::construir_clausula(const std::vector<literal>& lits, t_attrs& atributos)
{
     Clausula claus;
     BOOST_FOREACH(literal const& lit, lits)
     {
	  Literal* l = construir_literal(lit, atributos);
	  claus.agregarLiteral(*l);
	  delete l;
     }
     return claus;
}
