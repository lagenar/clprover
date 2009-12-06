#include "termino.hpp"

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

void Termino::aplicarSustitucion(const Sustitucion& s)
{
     assert(tipo == Var);
     const Termino* t;

     if ((t = s.getSustitucion(id)) != NULL) {
	  if (t->getTipo() == Var)
	       id = t->getId();
	  else
	       *this = Funcion(*static_cast<const Funcion*>(t));
     }
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

Termino* Variable::clonar() const
{
     return new Variable(*this);
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

Termino* Funcion::clonar() const
{
     return new Funcion(*this);
}

void Funcion::aplicarSustitucion(const Sustitucion& s)
{
     args->aplicarSustitucion(s);
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
