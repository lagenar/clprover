#ifndef CLAUSULA_HPP
#define CLAUSULA_HPP

#include <list>
#include <set>
#include "literal.hpp"

class Clausula {
public:
     Clausula() { }
     
     int cantLiterales() const;

     bool esUnitaria() const;

     bool esDeHorn() const;
     
     const std::string getString() const;

     void agregarLiteral(const Literal& lit);

private:
     class LitComp {
     public:
	  bool operator()(const Literal& l1, const Literal& l2) {
	       return l1.getId() < l2.getId();
	  }
     };

     std::set<Literal, LitComp> literales;
};

#endif
