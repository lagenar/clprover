#ifndef CLAUSULA_HPP
#define CLAUSULA_HPP

#include <list>
#include <set>
#include "literal.hpp"

class Clausula {
public:
     Clausula() : tautologica(false) { }
     
     int cantLiterales() const;

     bool esUnitaria() const;

     bool esDeHorn() const;

     bool esVacia() const;

     bool esTautologica() const;
     
     const std::string getString() const;

     void agregarLiteral(const Literal& lit);

private:
     class LitComp {
     public:
	  bool operator()(const Literal& l1, const Literal& l2) {
	       int r = l1.getId().compare(l2.getId());
	       if (r == 0)
		    return !l1.getSigno() && l2.getSigno()
			 || l1 != l2;			 
	       else
		    return r == -1;
	  }
     };

     std::set<Literal, LitComp> literales;
     bool tautologica;
};

#endif
