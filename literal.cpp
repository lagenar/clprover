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

Literal::const_iterator Literal::begin() const
{
     return args.begin();
}

Literal::const_iterator Literal::end() const
{
     return args.end();
}

const std::string Literal::getString() const
{
     return (signo ? "" : "~") + id + args.getString();
}

bool Literal::unificar(const Literal& otro, Sustitucion& s) const
{
     if (otro.aridad() != aridad() || otro.signo != signo || otro.id != id)
	  return false;
     return args.unificar(s, otro.args);
}

bool Literal::unificable(const Literal& otro) const
{
     Sustitucion s;
     return unificar(otro, s);
}

bool Literal::unificarComplementario(const Literal& otro, Sustitucion& s) const
{
     if (otro.aridad() != aridad() || otro.signo == signo || otro.id != id)
	  return false;
     return args.unificar(s, otro.args);
}

void Literal::aplicarSustitucion(const Sustitucion& s)
{
     args.aplicarSustitucion(s);
}

void Literal::agregarArgumento(const Termino& t)
{
     args.agregarArgumento(t);
}

void Literal::setSigno(bool s)
{
     signo = s;
}

void Literal::renombrarVariables(std::map<std::string,std::string>& renombre, int& comienzo)
{
     args.renombrarVariables(renombre, comienzo);
}
