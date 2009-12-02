#include "literal.hpp"

Literal::Literal(const std::string& id, const std::list<Termino*>& args, bool signo)
     : id(id), args(args), signo(signo)
{
}

bool Literal::getSigno() const
{
     return signo;
}

const std::string& Literal::getId() const
{
     return id;
}

int Literal::aridad() const
{
     return args.size();
}

const std::list<Termino*> Literal::getArgumentos() const
{
     return args;
}

bool Literal::operator==(const Literal& otro) const
{
     if (otro.getId() != id || otro.aridad() != args.size())
	  return false;

     bool igual = true;
     const std::list<Termino*>& args_otro = otro.getArgumentos();
     std::list<Termino*>::const_iterator it = args.begin();
     std::list<Termino*>::const_iterator it_otro = args_otro.begin();

     while (igual && it != args.end() && it_otro != args_otro.end()) {
	  igual = (**it == *it_otro);
	  it++;
	  it_otro++;
     }
     
     return igual;
}

Literal::~Literal()
{
     for (std::list<Termino*>::iterator it = args.begin(); it != args.end(); it++)
	  delete *it;
}
