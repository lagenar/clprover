#ifndef RESOLUCION_HPP
#define RESOLUCION_HPP

#include "clausulas.hpp"
#include "inferencia.hpp"
#include <list>
#include <boost/shared_ptr.hpp>

class ClausComp {
public:
     bool operator()(const Clausula& c1, const Clausula& c2) const
     {
	  if (c1.cantLiterales() < c2.cantLiterales())
	       return true;
	  else if (c1.cantLiterales() > c2.cantLiterales())
	       return false;
	  else if (!c1.equivalente(c2))
	       return c1.getString() < c2.getString();
	  else
	       return false;
     }
};

class Resolucion {
public:
     typedef ConjuntoClausulas<ClausComp> ConjClaus;

     Resolucion(const ConjClaus& claus) :
	  claus(claus) { }

     virtual bool esSatisfacible(std::list<boost::shared_ptr<Inferencia> >& Prueba) const = 0;

protected:
     const ConjuntoClausulas<ClausComp> claus;
};

class ResolucionGeneral : public Resolucion {
public:
     ResolucionGeneral(const ConjClaus& claus) :
	  Resolucion(claus) { }

     bool esSatisfacible(std::list<boost::shared_ptr<Inferencia> >& Prueba) const;
};

#endif
