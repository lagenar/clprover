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

bool Literal::operator==(const Literal& otro) const
{
     if (otro.getId() != id || otro.aridad() != args.size())
	  return false;

     Literal::const_iterator it = args.begin();
     Literal::const_iterator it_otro = otro.begin();

     return find_if
}

const std::string Literal::getString() const
{
     std::string s;
     for (std::list<Termino*>::const_iterator it = args.begin(); it != args.end(); it++)
	  s += (*it)->getString();
     return (signo ? "" : "~") + id + "(" + s + ")";
}

Literal::const_iterator Literal::begin() const
{
     return args.begin();
}

Literal::const_iterator Literal::end() const
{
     return args.end();
}

Literal::~Literal()
{
     for (std::list<Termino*>::iterator it = args.begin(); it != args.end(); it++)
	  delete *it;
}
