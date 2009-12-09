#include "termino.hpp"
#include <sstream>

/* Termino */
Termino::Termino(const std::string& id, Termino::t tipo)
     : id(id), tipo(tipo)
{
} 

const std::string& Termino::getId() const
{
     return id;
}

Termino::t Termino::getTipo() const
{
     return tipo;
}

/* Variable */
Variable::Variable(const std::string& id)
  : Termino(id, Termino::Var)
{

}

const std::string Variable::getString() const
{
     return id;
}

bool Variable::operator==(const Termino& otro) const
{
     return otro.getTipo() == Termino::Var && otro.getId() == id;
}

bool Variable::contieneVariable(const std::string& id) const
{
     return this->id == id;
}

Termino* Variable::aplicarSustitucion(const Sustitucion& s) const
{
     const Termino* t;
     if ((t = s.getSustitucion(id)) != NULL)
	  return t->clonar();
     return clonar();
}

bool Variable::unificar(Sustitucion& s, const Termino& otro) const
{
     const Termino* t = s.getSustitucion(id);
     if (t == NULL)
	  t = this;

     if (t->getTipo() == Func)
	  return t->unificar(s, otro);

     const Termino* otro_sust = otro.aplicarSustitucion(s);
     bool res = false;
     if (otro_sust->contieneVariable(id))
	  res = otro_sust->getTipo() == Var;
     else {
	  Sustitucion s1;
	  s1.agregarSustitucion(t->getId(), *otro_sust);
	  s.componer(s1);
	  res = true;
     }
     delete otro_sust;
     return res;
}

Termino* Variable::clonar() const
{
     return new Variable(*this);
}

void Variable::renombrarVariables(std::map<std::string,std::string>&renombre, int& comienzo)
{
     std::map<std::string,std::string>::const_iterator it = renombre.find(id);
     if (it == renombre.end()) {
	  std::stringstream s;
	  s << comienzo;	  
	  id = renombre[id] = "X" + s.str();
	  comienzo++;
     } else
	  id = it->second;
}

/* Funcion */
Funcion::Funcion(const std::string& id)
     : Termino(id, Termino::Func)
{
     args = new Argumentos();
}

Funcion::Funcion(const Funcion& f) : Termino(f)
{
     args = new Argumentos(*f.args);
}

const std::string Funcion::getString() const
{
     return id + args->getString();
}

bool Funcion::operator==(const Termino& otro) const
{
     if (otro.getTipo() != Termino::Func || otro.getId() != id)
     	  return false;
     
     return static_cast<const Funcion*>(&otro)->args == args;
}

bool Funcion::contieneVariable(const std::string& id) const
{
     return args->contieneVariable(id);
}

bool Funcion::unificar(Sustitucion& s, const Termino& otro) const
{
     if (otro.getTipo() == Var)
	  return otro.unificar(s, *this);
     else if (otro.getId() == id)
	  return args->unificar(s, *static_cast<const Funcion*>(&otro)->args);
     else 
	  return false;
}

Termino* Funcion::aplicarSustitucion(const Sustitucion& s) const
{
     Funcion* f = new Funcion(*this);
     f->args->aplicarSustitucion(s);
     return f;
}

Termino* Funcion::clonar() const
{
     return new Funcion(*this);
}

void Funcion::renombrarVariables(std::map<std::string,std::string>& renombre, int& comienzo)
{
     args->renombrarVariables(renombre, comienzo);
}

int Funcion::aridad() const
{
     return args->aridad();
}

bool Funcion::esConstante() const
{
     return args->aridad() == 0;
}

void Funcion::agregarArgumento(const Termino& t)
{
     args->agregarArgumento(t);
}

Funcion::~Funcion()
{
     delete args;
}
