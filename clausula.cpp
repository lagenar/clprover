#include "clausula.hpp"

int Clausula::cantLiterales() const
{
     return literales.size();
}

bool Clausula::esUnitaria() const
{
     return cantLiterales() == 1;
}

bool Clausula::esDeHorn(t_horn& t) const
{
     int num_pos = 0;
     const_iterator it = literales.begin();

     while (it != literales.end() && num_pos < 2) {
	  if (it->getSigno())
	       ++num_pos;
	  ++it;
     }

     if (num_pos < 2) {
	  int c = cantLiterales();
	  if (num_pos == 0)
	       t = OBJ;
	  else if (num_pos == c == 1)
	       t = HECHO;
	  else
	       t = REGLA;
	  return true;
     }
     return false;
}

bool Clausula::esDeHorn() const
{
     t_horn t;
     return esDeHorn(t);
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
     const_iterator it = literales.begin();
     s += it->getString();
     ++it;
     while (it != literales.end()) {
	  s += " | " + it->getString();
	  ++it;
     }
     return s;
}

bool Clausula::contieneLiteral(const std::string& id, bool signo) const
{
     bool cont = false;
     bool paso = false;
     const_iterator it = literales.begin();

     while (it != literales.end() && !cont && !paso) {
	  int cmp = id.compare(it->getId());
	  if (cmp > 0)
	       paso = true;
	  else if (cmp == 0) {
	       paso = !signo && it->getSigno();
	       cont = signo == it->getSigno();
	  }
	  ++it;
     }

     return cont;	  
}

const_iterator begin() const
{
     return literales.begin();
}

const_iterator end() const
{
     return literales.end();
}

void Clausula::agregarLiteral(const Literal& lit)
{
     if (!tautologica) {
	  Literal l = lit;
	  l.setSigno(!l.getSigno());
	  const_iterator it = literales.find(l);

	  if (it == literales.end())
	       literales.insert(lit);
	  else
	       tautologica = true;
     }
}
