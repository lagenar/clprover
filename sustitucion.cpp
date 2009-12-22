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

#include "sustitucion.hpp"

const std::string Sustitucion::getString() const
{
     if (susts.empty())
	  return "{ }";

     std::string s("{");
     t_sust::const_iterator it = susts.begin();

     s += " { " + it->first + " / " + it->second->getString() + " }";
     ++it;
     while (it != susts.end()) {
	  s += ", { " + it->first + " / " + it->second->getString() + " }";
	  ++it;
     }

     return s + " }";
}

void Sustitucion::componer(const Sustitucion& s)
{
     t_sust::iterator it = susts.begin();
     while (it != susts.end()) {
	  Termino* r = it->second->aplicarSustitucion(s);
	  delete it->second;
	  it->second = r;
	  if (r->getTipo() == Termino::Var &&
	      r->getId() == it->first) {
	       delete r;
	       susts.erase(it++);
	  } else 
	       ++it;
     }

     for (t_sust::const_iterator it = s.susts.begin(); it != s.susts.end(); ++it)
	  if (susts.find(it->first) == susts.end())
	       susts[it->first] = it->second->clonar();
}

const Termino* Sustitucion::getSustitucion(const std::string& id) const
{
     t_sust::const_iterator it = susts.find(id);
     if (it != susts.end())
	  return it->second;

     return NULL;
}

void Sustitucion::agregarSustitucion(const std::string& id, const Termino& t)
{
     susts[id] = t.clonar();
}

Sustitucion::~Sustitucion()
{
     for (t_sust::iterator it = susts.begin(); it != susts.end(); ++it)
	  delete it->second;
}
