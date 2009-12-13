#include "resolucion.hpp"
#include <list>

bool ResolucionGeneral::esSatisfacible(std::list<boost::shared_ptr<Inferencia> >& Prueba) const
{
     using std::list;     
     bool satisfacible = true;
     list<Clausula> combinables;
     claus.getClausulas(combinables);
     list<Clausula>::iterator itComb = combinables.begin();
     list<Clausula> procesadas;

     for (list<Clausula>::const_iterator it = combinables.begin(); it != combinables.end(); ++it) {
	  boost::shared_ptr<Inferencia> p(new InferenciaHipotesis(*it));
	  Prueba.push_back(p);
     }
     while (satisfacible && itComb != combinables.end()) {
	  list<Clausula>::const_iterator itProc = procesadas.begin();
	  if (!contieneClausula(*itComb, procesadas.begin(), procesadas.end())) {
	       list<Clausula> factores;
	       itComb->factores(factores);
	       for (list<Clausula>::const_iterator it = factores.begin(); it != factores.end(); ++it)
		    if (!contieneClausula(*it, combinables.begin(), combinables.end())) {
			 combinables.push_back(*it);
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
				  !contieneClausula(*itRes, procesadas.begin(), procesadas.end()) &&
				  !contieneClausula(*itRes, combinables.begin(), combinables.end())) {
			      combinables.push_back(*itRes);
			      Prueba.push_back(p);
			 }
			 ++itRes;
		    }
		    ++itProc;
	       }
	  }
	  procesadas.push_back(*itComb);
	  combinables.erase(itComb);
	  itComb = combinables.begin();
     }
     return satisfacible;
}
