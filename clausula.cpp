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

Clausula::const_iterator Clausula::begin() const
{
     return literales.begin();
}

Clausula::const_iterator Clausula::end() const
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


void Clausula::resolventes(const Clausula& claus, std::list<Clausula>& res) const
{
     for (const_iterator lit = literales.begin(); lit != literales.end(); ++lit) {
	  const_iterator lit2 = claus.begin();
	  while (lit2 != claus.end() && lit->getId() >= lit2->getId()) {
	       Sustitucion s;
	       if (lit->unificarComplementario(*lit2, s)) {
		    Clausula nueva_res;
		    for (const_iterator it = literales.begin(); it != literales.end(); ++it)
			 if (it != lit) {
			      Literal l = *it;
			      l.aplicarSustitucion(s);
			      nueva_res.agregarLiteral(l);
			 }
		    for (const_iterator it = claus.begin(); it != claus.end(); ++it)
			 if (it != lit2) {			     
			      Literal l = *it;
			      l.aplicarSustitucion(s);
			      nueva_res.agregarLiteral(l);
			 }
		    res.push_back(nueva_res);
	       }
	       ++lit2;
	  }
     }	  
}
