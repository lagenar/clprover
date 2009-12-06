#include "literal.hpp"

Literal::Literal(const std::string& id, bool signo)
     : id(id), signo(signo)
{
}

Literal::Literal(const std::string& id, const Argumentos& args, bool signo)
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
     return args.aridad();
}

bool Literal::operator==(const Literal& otro) const
{
     if (otro.id != id || otro.signo != signo)
	  return false;
     return otro.args == args;
}

bool Literal::operator!=(const Literal& otro) const
{
     return !(*this == otro);
}

const std::string Literal::getString() const
{
     return (signo ? "" : "~") + id + args.getString();
}

void Literal::agregarArgumento(const Termino& t)
{
     args.agregarArgumento(t);
}

void Literal::setSigno(bool s)
{
     signo = s;
}
