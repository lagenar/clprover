#ifndef RESOLUCION_HPP
#define RESOLUCION_HPP

#include "clausulas.hpp"
#include "inferencia.hpp"
#include <list>
#include <boost/shared_ptr.hpp>
#include <iostream>

class ClausComp {
public:
     bool operator()(const Clausula& c1, const Clausula& c2) const
     {
	  if (c1.cantLiterales() < c2.cantLiterales())
	       return true;
	  else if (c1.cantLiterales() > c2.cantLiterales())
	       return false;
	  else
	       return c1.getString() < c2.getString();
	      
     }
};

class Resolucion {
public:
     Resolucion(const ConjuntoClausulas<ClausComp>& claus) :
	  claus(claus) { }

     virtual bool esSatisfacible(std::list<boost::shared_ptr<Inferencia> >& Prueba) const = 0;

protected:
     const ConjuntoClausulas<ClausComp> claus;
     
     template<typename Iterator>
     bool contieneClausula(const Clausula& c, Iterator begin, Iterator end) const
     {
	  for (Iterator it = begin; it != end; ++it)
	       if (it->equivalente(c))	   
		    return true;
	  return false;
     }
};

class ResolucionGeneral : public Resolucion {
public:
     ResolucionGeneral(const ConjuntoClausulas<ClausComp>& claus) :
	  Resolucion(claus) { }

     bool esSatisfacible(std::list<boost::shared_ptr<Inferencia> >& Prueba) const;
};

#endif
