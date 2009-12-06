#include "sustitucion.hpp"

const std::string Sustitucion::getString() const
{
     return "";
}

void Sustitucion::componer(const Sustitucion& s)
{
}

const Termino* Sustitucion::getSustitucion(const std::string& id) const
{
     t_sust::const_iterator it = sustituciones.find(id);
     if (it != sustituciones.end())
	  return it->second;

     return NULL;
}

void Sustitucion::agregarSustitucion(const std::string& id, const Termino& t)
{
     sustituciones[id] = t.clonar();
}

Sustitucion::~Sustitucion()
{
     for (t_sust::iterator it = sustituciones.begin(); it != sustituciones.end(); ++it)
	  delete it->second;
}
