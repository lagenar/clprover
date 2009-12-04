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
     if (args->aridad() == 0)
	  return id;
     
     return id + args->getString();
}

bool Funcion::operator==(const Termino& otro) const
{
     if (otro.getTipo() != Termino::Func || otro.getId() != id)
     	  return false;
     
     return static_cast<const Funcion*>(&otro)->args == args;
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
