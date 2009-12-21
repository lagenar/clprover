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
     
     bool simplificarPorTautologicas()
     {
	  bool simp = false;
	  iterator claus = clausulas.begin();
	  while (claus != clausulas.end()) {
	       if (claus->esTautologica()) {
		    simp = true;
		    clausulas.erase(claus++);		   
	       } else
		    ++claus;
	  }
	  return simp;
     }

     bool simplificarPorEquivalentes()
     {
	  bool simp = false;
	  for (iterator it1 = clausulas.begin(); it1 != clausulas.end(); ++it1) {
	       iterator it2 = it1;
	       ++it2;
	       while (it2 != clausulas.end()) {
		    if (it1->equivalente(*it2)) {
			 simp = true;
			 clausulas.erase(it2++);
		    } else
			 ++it2;
	       }
	  }
	  return simp;
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
		    } else
			 ++lit;
	       }
	       if (!claus_eliminada)
		    ++claus;
	  }
	  return simp;
     }
     
     bool contieneClausula(const Clausula& c) const
     {
	  const_iterator it = clausulas.find(c);
	  return it != clausulas.end();
     }

     void getClausulas(std::list<Clausula>& l) const
     {
	  for (const_iterator it = clausulas.begin(); it != clausulas.end(); ++it)
	       l.push_back(*it);
     }
     
     std::set<std::string> predicadosEliminables() const
     {
	  std::map<std::string, bool> eliminable;
	  for (const_iterator it = clausulas.begin(); it != clausulas.end(); ++it) {
	       std::map<std::string, int> apar = it->aparicionesPredicados();
	       std::map<std::string, int>::const_iterator it_apar;
	       for (it_apar = apar.begin(); it_apar != apar.end(); ++it_apar)
		    if (it_apar->second > 1)
			 eliminable[it_apar->first] = false;
		    else {
			 std::map<std::string, bool>::const_iterator it_elim = eliminable.find(it_apar->first);
			 if (it_elim == eliminable.end())
			      eliminable[it_apar->first] = true;
		    }
	  }
	  std::set<std::string> eliminables;
	  for (std::map<std::string, bool>::const_iterator it_elim = eliminable.begin();
	       it_elim != eliminable.end(); ++it_elim)
	       if (it_elim->second)
		    eliminables.insert(it_elim->first);

	  return eliminables;
     }

     const_iterator begin() const
     {
	  return clausulas.begin();
     }

     const_iterator end() const
     {
	  return clausulas.end();
     }
     
     iterator begin()
     {
	  return clausulas.begin();
     }

     iterator end()
     {
	  return clausulas.end();
     }
     
     void eliminar(iterator c)
     {
	  clausulas.erase(c);
     }
private:
     std::set<Clausula, Compare> clausulas;
};

#endif
