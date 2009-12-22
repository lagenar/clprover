/*
* clprover
* Copyright (C) 2009 Lucas Moauro
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "literal.hpp"

Literal::Literal(const std::string& id, bool signo)
     : signo(signo), id(id)
{
}

Literal::Literal(const std::string& id, const Argumentos& args, bool signo)
     : args(args), signo(signo), id(id)
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

void Literal::renombrarVariables(int& comienzo)
{
     std::map<std::string,std::string> ren;
     args.renombrarVariables(ren, comienzo);
}
