#ifndef CLAUSULA_HPP
#define CLAUSULA_HPP

#include <list>
#include <set>
#include "literal.hpp"

class LitComp 
{
public:
     bool operator()(const Literal& l1, const Literal& l2) 
     {
	  int r = l1.getId().compare(l2.getId());
	  if (r == 0)
	       return !l1.getSigno() && l2.getSigno()
		    || l1.getString() < l2.getString();
	  else
	       return r < 0;
     }
};

class Clausula {
public:
     typedef std::set<Literal, LitComp>::const_iterator const_iterator;
     typedef std::set<Literal, LitComp>::iterator iterator;
     typedef enum { REGLA, HECHO, OBJ } t_horn;

     Clausula() : tautologica(false) { }
     
     int cantLiterales() const;

     bool esUnitaria() const;

     bool esDeHorn(t_horn& t) const;

     bool esDeHorn() const;

     bool esVacia() const;

     bool esTautologica() const;
     
     const std::string getString() const;
     
     const_iterator begin() const;

     const_iterator end() const;

     bool contieneComplementario(const Literal& lit) const;

     void resolventes(const Clausula& claus, std::list<Clausula>& res) const;

     Clausula renombrarVariables(int& comienzo) const;

     void agregarLiteral(const Literal& lit);
private:
     std::set<Literal, LitComp> literales;
     bool tautologica;
     
     void agregarLitsResolvente(const Clausula& c, const const_iterator& lit,
				const Sustitucion& s);
};

#endif
