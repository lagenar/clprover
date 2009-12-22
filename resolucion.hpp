#ifndef RESOLUCION_HPP
#define RESOLUCION_HPP

#include "clausulas.hpp"
#include "inferencia.hpp"
#include <list>
#include <boost/shared_ptr.hpp>

class Resolucion {
public:
     typedef ConjuntoClausulas<> ConjClaus;
     typedef std::list<boost::shared_ptr<Inferencia> > t_prueba;

     Resolucion(const ConjClaus& claus) :
	  claus(claus) { }
 
     virtual bool esSatisfacible(t_prueba& Prueba, const bool& seguir_busqueda) = 0;

     virtual bool esSatisfacible(t_prueba& Prueba);
protected:
     ConjuntoClausulas<> claus;

     void resolverPredicadosEliminables(t_prueba& prueba, bool& resolvio_vacia);

     void eliminarPredicado(t_prueba& prueba, const std::string& p, bool& resolvio_vacia);
};

class ResolucionGeneral : public Resolucion {
public:
     ResolucionGeneral(const ConjClaus& claus) :
	  Resolucion(claus) { }

     bool esSatisfacible(t_prueba& Prueba, const bool& seguir_busqueda);

     bool esSatisfacible(t_prueba& Prueba);
};

#endif
