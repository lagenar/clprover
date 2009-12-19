#include "resolucion.hpp"
#include <list>

void Resolucion::resolverPredicadosEliminables(t_prueba& prueba, bool& resolvio_vacia)
{
     resolvio_vacia = false;
     std::set<std::string> eliminables = claus.predicadosEliminables();
     while (!eliminables.empty()) {
	  std::set<std::string>::const_iterator it_elim = eliminables.begin();
	  eliminarPredicado(prueba, *it_elim, resolvio_vacia);
	  eliminables = claus.predicadosEliminables();
     }
}

void Resolucion::eliminarPredicado(t_prueba& prueba, const std::string& id_pred, bool& resolvio_vacia)
{
     ConjuntoClausulas<ClausComp> simp;
     for (ConjuntoClausulas<ClausComp>::const_iterator it = claus.begin();
	  it != claus.end(); ++it) {
	  if (it->contienePredicado(id_pred)) {
	       ConjuntoClausulas<ClausComp>::const_iterator it2 = it;
	       ++it2;
	       for ( ; it2 != claus.end(); ++it2) {
		    Clausula res;
		    if (it->resolventeUsandoPred(*it2, id_pred, res)) {
			 simp.agregarClausula(res);
			 if (!resolvio_vacia) {
			      boost::shared_ptr<Inferencia> p(new InferenciaResolucion(*it, *it2, res));
			      prueba.push_back(p);
			      resolvio_vacia = res.esVacia();
			 }				   
		    }
	       }
	  } else
	       simp.agregarClausula(*it);
     }
     claus = simp;
}

bool Resolucion::esSatisfacible(t_prueba& Prueba)
{
     bool seguir = true;
     return esSatisfacible(Prueba, seguir);
}

bool ResolucionGeneral::esSatisfacible(t_prueba& Prueba)
{
     return Resolucion::esSatisfacible(Prueba);
}

bool ResolucionGeneral::esSatisfacible(t_prueba& Prueba, const bool& seguir_busqueda)
{
     using std::list;
    
     for (ConjClaus::const_iterator it = claus.begin(); it != claus.end(); ++it) {
	  boost::shared_ptr<Inferencia> p(new InferenciaHipotesis(*it));
	  Prueba.push_back(p);
     }
     bool resolvio_vacia;
     resolverPredicadosEliminables(Prueba, resolvio_vacia);
     
     if (resolvio_vacia)
	  return false;

     bool satisfacible = true;
     ConjClaus combinables = claus;
     ConjClaus::iterator itComb = combinables.begin();
     ConjClaus procesadas;

     while (satisfacible && itComb != combinables.end() && seguir_busqueda) {
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
	       while (satisfacible && itProc != procesadas.end() && seguir_busqueda) {
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
