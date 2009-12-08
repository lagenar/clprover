#ifndef CLAUSULAS_HPP
#define CLAUSULAS_HPP

#include <set>
#include "clausula.hpp"

template <typename Compare>
class ConjuntoClausulas {
public:
     typedef typename std::set<Clausula, Compare>::const_iterator const_iterator;
     typedef typename std::set<Clausula, Compare>::iterator iterator;
     typedef typename std::set<Clausula, Compare>::size_type size_type;
     
     ConjuntoClausulas() { }

     ConjuntoClausulas(const std::list<Clausula>& cls)
     {
	  for (std::list<Clausula>::const_iterator it = cls.begin(); it != cls.end(); ++it)	   
	       agregarClausula(*it);
     }
     
     size_type cantidadClausulas() const { return clausulas.size(); }

     bool esVacio() const { return clausulas.size() == 0; }

     bool esDeHorn() const 
     {
	  for (const_iterator it = clausulas.begin(); it != clausulas.end(); ++it)
	       if (!it->esDeHorn())
		    return false;
	  return true;
     }
     
     bool esLogicaPrograma() const
     {
	  for (const_iterator it = clausulas.begin(); it != clausulas.end(); ++it) {
	       Clausula::t_horn t;
	       if (!it->esDeHorn(t) || t == Clausula::OBJ)
		    return false;
	  }
	  return true;
     }

     void agregarClausula(const Clausula& C)
     {
	  clausulas.insert(C);
     }
     
     bool simplificarLiteralesPuros()
     {
	  bool simp = false;
	  iterator claus = clausulas.begin();	  
	  while (claus != clausulas.end()) {
	       bool claus_eliminada = false;
	       Clausula::const_iterator lit = claus->begin();	       
	       while (!claus_eliminada && lit != claus->end()) {		    
		    bool contieneComp = false;
		    const_iterator claus2 = clausulas.begin();
		    while (!contieneComp && claus2 != clausulas.end()) {
			 contieneComp = claus2->contieneComplementario(*lit);
			 ++claus2;
		    }		    
		    if (!contieneComp) {
			 simp = true;
			 clausulas.erase(claus++);
			 claus_eliminada = true;
		    }
		    ++lit;
	       }
	       if (!claus_eliminada)
		    ++claus;
	  }
	  return simp;
     }

     const_iterator begin() const
     {
	  return clausulas.begin();
     }

     const_iterator end() const
     {
	  return clausulas.end();
     }

private:
     std::set<Clausula, Compare> clausulas;
};

#endif
