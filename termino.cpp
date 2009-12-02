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
     const Funcion * f_otro = static_cast<const Funcion*>(otro);
     Funcion::const_iterator it = args.begin();
     Funcion::const_iterator it_otro = f_otro->begin();

     while (igual && it != args.end() && it_otro != f_otro->end()) {
	  igual = (**it == *it_otro);
	  it++;
	  it_otro++;
     }

     return igual;
}

int Funcion::aridad() const
{
     return args.size();
}

bool Funcion::esConstante() const
{
     return aridad() == 0;
}

Funcion::const_iterator Funcion::begin() const
{
     return args.begin();
}

Funcion::const_iterator Funcion::end() const
{
     return args.end();
}

Funcion::~Funcion()
{
     for (std::list<Termino*>::iterator it = args.begin(); it != args.end(); it++)
	  delete *it;
}
