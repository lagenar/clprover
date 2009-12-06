#include "clausula.hpp"

int Clausula::cantLiterales() const
{
     return literales.size();
}

bool Clausula::esUnitaria() const
{
     return cantLiterales() == 1;
}

bool Clausula::esDeHorn() const
{
     int num_pos = 0;
     std::set<Literal, LitComp>::const_iterator it = literales.begin();

     while (it != literales.end() && num_pos < 2) {
	  if (it->getSigno())
	       ++num_pos;
	  ++it;
     }

     return num_pos < 2;
}

bool Clausula::esVacia() const
{
     return cantLiterales() == 0;
}

bool Clausula::esTautologica() const
{
     return tautologica;
}

const std::string Clausula::getString() const
{
     if (tautologica)
	  return "T";
     else if (cantLiterales() == 0)
	  return "_|_";

     std::string s;
     std::set<Literal, LitComp>::const_iterator it = literales.begin();
     s += it->getString();
     ++it;
     while (it != literales.end()) {
	  s += " | " + it->getString();
	  ++it;
     }
     return s;
}

void Clausula::agregarLiteral(const Literal& lit)
{
     if (!tautologica) {
	  Literal l = lit;
	  l.setSigno(!l.getSigno());
	  std::set<Literal, LitComp>::const_iterator it = literales.find(l);

	  if (it == literales.end())
	       literales.insert(lit);
	  else
	       tautologica = true;
     }
}
