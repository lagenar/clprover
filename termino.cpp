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

bool Variable::operator==(const Termino * otro) const
{
     return otro->getTipo() == Termino::Var && otro->getId() == id;
}

/* Funcion */
Funcion::Funcion(const std::string& id, const std::list<Termino*>& args)
  : args(args), Termino(id, Termino::Func)
{

}

const std::string Funcion::getString() const
{
     if (aridad() == 0)
	  return id;

     std::string s;
     for (std::list<Termino*>::const_iterator it = args.begin(); it != args.end(); it++)
	  s += (*it)->getString();

     return id + "(" + s + ")";
}

bool Funcion::operator==(const Termino * otro) const
{
     if (otro->getTipo() != Termino::Func || otro->getId() != id
	 || static_cast<const Funcion*>(otro)->aridad() != args.size())
	  return false;
     
     bool igual = true;
     const std::list<Termino*> args_otro = static_cast<const Funcion*>(otro)->getArgumentos();
     std::list<Termino*>::const_iterator it = args.begin();
     std::list<Termino*>::const_iterator it_otro = args_otro.begin();

     while (igual && it != args.end() && it_otro != args_otro.end()) {
	  igual = (**it == *it_otro);
	  it++;
	  it_otro++;
     }

     return igual;
}

const std::list<Termino*>& Funcion::getArgumentos() const
{
     return args;
}

int Funcion::aridad() const
{
     return args.size();
}

bool Funcion::esConstante() const
{
     return aridad() == 0;
}

Funcion::~Funcion()
{
     for (std::list<Termino*>::iterator it = args.begin(); it != args.end(); it++)
	  delete *it;
}
