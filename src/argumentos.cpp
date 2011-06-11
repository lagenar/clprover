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

#include "argumentos.hpp"
#include <cassert>

Argumentos::Argumentos(const Argumentos& Args)
{
     const_iterator it;
     for (it = Args.begin(); it != Args.end(); ++it)
	  args.push_back((*it)->clonar());
}


int Argumentos::aridad() const
{
     return args.size();
}

const std::string Argumentos::getString() const
{
     if (aridad() == 0)
	  return "";

     const_iterator it = args.begin();
     std::string s((*it)->getString());
     ++it;
     while (it != args.end()) {
	  s += ", " + (*it)->getString();
	  ++it;
     }

     return "(" + s + ")";
}

Argumentos::const_iterator Argumentos::begin() const
{
     return args.begin();
}

Argumentos::const_iterator Argumentos::end() const
{
     return args.end();
}

bool Argumentos::operator==(const Argumentos& otro) const
{
     if (otro.aridad() != aridad())
	  return false;
     
     bool igual = true;
     const_iterator it = args.begin();
     const_iterator it_otro = otro.begin();
     while (igual && it != args.end() && it_otro != otro.end()) {
	  igual = (**it == **it_otro);
	  ++it;
	  ++it_otro;
     }
     return igual;
}

bool Argumentos::contieneVariable(const std::string& id) const
{
     for (const_iterator it = args.begin(); it != args.end(); ++it)
	  if ((*it)->contieneVariable(id))
	       return true;
     return false;
}

bool Argumentos::unificar(Sustitucion& s, const Argumentos& otro) const
{
     if (args.size() != otro.args.size())
	  return false;

     const_iterator it = args.begin();
     const_iterator it_otro = otro.args.begin();
     bool unif = true;
     while (it != args.end() && it_otro != otro.args.end() && unif) {
	  unif = (*it)->unificar(s, **it_otro);	  
	  ++it;
	  ++it_otro;
     }
     return unif;
}

bool Argumentos::unificarSubsuncion(Sustitucion& s, const Argumentos& otro) const
{
     if (args.size() != otro.args.size())
	  return false;

     const_iterator it = args.begin();
     const_iterator it_otro = otro.args.begin();
     bool unif = true;
     while (it != args.end() && unif) {
	  unif = (*it)->unificarSubsuncion(s, **it_otro);
	  ++it;
	  ++it_otro;
     }
     return unif;
}

void Argumentos::agregarArgumento(const Termino& t)
{
     args.push_back(t.clonar());
}

void Argumentos::aplicarSustitucion(const Sustitucion& s)
{
     for (iterator it = args.begin(); it != args.end(); ++it) {
	  Termino* t = (*it)->aplicarSustitucion(s);
	  delete *it;
	  *it = t;
     }
}

void Argumentos::renombrarVariables(std::map<std::string,std::string>& renombre, int& comienzo)
{
     for (iterator it = args.begin(); it != args.end(); ++it)
	  (*it)->renombrarVariables(renombre, comienzo);
}

Argumentos::~Argumentos()
{
     for (iterator it = args.begin(); it != args.end(); ++it)
	  delete *it;
}
