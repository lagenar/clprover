#ifndef CLAUSULAS_HPP
#define CLAUSULAS_HPP

#include <set>
#include "clausula.hpp"

template <typename Compare>
class ConjuntoClausulas {
public:
     typedef std::set<Clausula, Compare>::const_iterator const_iterator;
     typedef std::set<Clausula, Compare>::iterator iterator;
     typedef std::set<Clausula, Compare>::size_type size_type;
     
     ConjuntoClausulas() { }
     
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
	  if (!C.esTautologica())
	       clausulas.insert(C);
     }
     
     bool simplificarLiteralesPuros()
     {
	  // typedef std::set<std::pair<std::string, bool> t_simp;
	  // mapa_simp lit_simplificado;
	  // bool simp = false;
	  
	  // for (const_iterator claus = clausulas.begin(); claus != clausulas.end(); ++claus) {
	       
     }

private:
     std::set<Clausula, Compare> clausulas;
};

#endif
