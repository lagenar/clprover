#include "resolucion.hpp"
#include <list>

bool ResolucionGeneral::esSatisfacible(std::list<boost::shared_ptr<Inferencia> >& Prueba) const
{
     using std::list;     
     bool satisfacible = true;
     ConjClaus combinables = claus;
     ConjClaus::iterator itComb = combinables.begin();
     ConjClaus procesadas;

     for (ConjClaus::const_iterator it = combinables.begin(); it != combinables.end(); ++it) {
	  boost::shared_ptr<Inferencia> p(new InferenciaHipotesis(*it));
	  Prueba.push_back(p);
     }
     while (satisfacible && itComb != combinables.end()) {
	  ConjClaus::const_iterator itProc = procesadas.begin();
	  if (combinables.contieneClausula(*itComb)) {
	       list<Clausula> factores;
	       itComb->factores(factores);
	       for (list<Clausula>::const_iterator it = factores.begin(); it != factores.end(); ++it)
		    if (!combinables.contieneClausula(*it)) {
			 combinables.agregarClausula(*it);
			 boost::shared_ptr<Inferencia> p(new InferenciaFactorizacion(*itComb, *it));
			 Prueba.push_back(p);
		    }    
	       while (satisfacible && itProc != procesadas.end()) {
		    list<Clausula> res;
		    itComb->resolventes(*itProc, res);
		    list<Clausula>::const_iterator itRes = res.begin();
		    while (satisfacible && itRes != res.end()) {
			 boost::shared_ptr<Inferencia> p(new InferenciaResolucion(*itComb, *itProc, *itRes));			
			 if (itRes->esVacia()) {
			      satisfacible = false;
			      Prueba.push_back(p);
			 }
			 else if (!itRes->esTautologica() &&
				  !procesadas.contieneClausula(*itRes) &&
				  !combinables.contieneClausula(*itRes)) {
			      combinables.agregarClausula(*itRes);
			      Prueba.push_back(p);
			 }
			 ++itRes;
		    }
		    ++itProc;
	       }
	  }
	  procesadas.agregarClausula(*itComb);
	  combinables.eliminar(itComb);
	  itComb = combinables.begin();
     }
     return satisfacible;
}
