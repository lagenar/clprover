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

const std::string Clausula::getString() const
{
     std::string s;
     std::set<Literal, LitComp>::const_iterator it;
     
     for (it = literales.begin(); it != literales.end(); ++it)
	  s += it->getString();

     return s;
}

void Clausula::agregarLiteral(const Literal& lit)
{
     literales.insert(lit);
}
