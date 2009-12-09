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
	  else if (num_pos == 1 && c == 1)
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
     if (cantLiterales() == 0)
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

bool Clausula::contieneComplementario(const Literal& lit) const
{
     const_iterator it = literales.begin();
     Literal r_lit = lit;
     int i = 0;
     r_lit.renombrarVariables(i);

     while (it != literales.end() && it->getId() <= r_lit.getId()) {
	  Literal lit2 = *it;
	  int k = i;
	  lit2.renombrarVariables(k);
	  Sustitucion s;
	  if (r_lit.unificarComplementario(lit2, s))
	       return true;
	  ++it;
     }
     return false;
}

void Clausula::agregarLiteral(const Literal& lit)
{
     if (!tautologica) {
	  Literal l = lit;
	  l.setSigno(!l.getSigno());
	  const_iterator it = literales.find(l);
	  tautologica = it != literales.end();
     }
     literales.insert(lit);
}

void Clausula::resolventes(const Clausula& claus, std::list<Clausula>& res) const
{
     int com = 1;
     Clausula C1 = renombrarVariables(com);
     Clausula C2 = claus.renombrarVariables(com);

     for (const_iterator lit = C1.begin(); lit != C1.end(); ++lit) {
	  const_iterator lit2 = C2.begin();
	  while (lit2 != C2.end() && lit->getId() >= lit2->getId()) {
	       Sustitucion s;
	       if (lit->unificarComplementario(*lit2, s)) {
		    Clausula nueva_res;
		    nueva_res.agregarLitsResolvente(C1, lit, s);
		    nueva_res.agregarLitsResolvente(C2, lit2, s);
		    res.push_back(nueva_res);
	       }
	       ++lit2;
	  }
     }	  
}

bool Clausula::equivalente(const Clausula& C) const
{
     if (C.cantLiterales() != cantLiterales())
	  return false;
     int i = 0;
     Clausula ren = this->renombrarVariables(i);
     i = 0;
     Clausula ren_C = C.renombrarVariables(i);
     bool iguales = true;
     const_iterator it = ren.begin();
     const_iterator it2 = ren_C.begin();
     while (iguales && it != ren.end() && it2 != ren_C.end()) {
	  iguales = *it == *it2;
	  ++it;
	  ++it2;
     }
     return iguales;
}

Clausula Clausula::renombrarVariables(int& comienzo) const
{
     Clausula C;
     std::map<std::string, std::string> ren;
     for (iterator it = literales.begin(); it != literales.end(); ++it) {
	  Literal l = *it;
	  l.renombrarVariables(ren, comienzo);
	  C.agregarLiteral(l);
     }
     return C;
}

void Clausula::agregarLitsResolvente(const Clausula& c, const const_iterator& lit,
				     const Sustitucion& s)
{
     for (const_iterator it = c.begin(); it != c.end(); ++it)
	  if (it != lit) {
	       Literal nuevo = *it;
	       nuevo.aplicarSustitucion(s);
	       agregarLiteral(nuevo);
	  }
}
