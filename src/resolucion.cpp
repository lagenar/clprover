/*
* clprover
* Copyright (C) 2009 Lucas Moauro
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "resolucion.hpp"
#include <list>
#include <map>

void Resolucion::resolverPredicadosEliminables(ConjClaus& claus, t_prueba& prueba, bool& resolvio_vacia)
{
     resolvio_vacia = false;
     std::set<std::string> eliminables = claus.predicadosEliminables();
     while (!eliminables.empty()) {
	  std::set<std::string>::const_iterator it_elim = eliminables.begin();
	  eliminarPredicado(claus, prueba, *it_elim, resolvio_vacia);
	  eliminables = claus.predicadosEliminables();
     }
}

void Resolucion::eliminarPredicado(ConjClaus& claus, t_prueba& prueba, const std::string& id_pred, bool& resolvio_vacia)
{
     ConjuntoClausulas<> simp;
     for (ConjuntoClausulas<>::const_iterator it = claus.begin(); it != claus.end(); ++it) {
	  if (it->contienePredicado(id_pred)) {
	       ConjuntoClausulas<>::const_iterator it2 = it;
	       ++it2;
	       for ( ; it2 != claus.end(); ++it2) {
		    Clausula res;		    
		    if (it->resolventeUsandoPred(*it2, id_pred, res)) {
			 res.setIdResolucion(id_resolucion);
			 id_resolucion++;
			 simp.agregarClausula(res);
			 if (!resolvio_vacia) {			      
			      boost::shared_ptr<Inferencia> p(new InferenciaResolucion(it->getIdResolucion(),
										       it2->getIdResolucion(), res));
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

void Resolucion::agregarUsadas(const t_prueba& prueba, int id, std::set<int>& usadas)
{
     usadas.insert(prueba[id]->getClausula().getIdResolucion());
     std::list<int> padres;
     prueba[id]->getPadres(padres);
     for (std::list<int>::const_iterator it = padres.begin(); it != padres.end(); ++it)
	  agregarUsadas(prueba, *it, usadas);
}

void Resolucion::simplificarPrueba(t_prueba& Prueba)
{
     std::set<int> usadas;
     t_prueba::const_iterator it_p = Prueba.begin();
     while (it_p != Prueba.end() && (*it_p)->getTipo() == Inferencia::Hipot) {
	  usadas.insert((*it_p)->getClausula().getIdResolucion());
	  ++it_p;
     }
     agregarUsadas(Prueba, Prueba.size() - 1, usadas);
     t_prueba simp;
     std::map<int, int> nueva_id;
     int id = 0;
     for (std::set<int>::const_iterator it = usadas.begin(); it != usadas.end(); ++it) {
	  nueva_id[*it] = id;
	  ++id;
	  boost::shared_ptr<Inferencia> p = Prueba[*it];
	  std::list<int> padres;
	  p->getPadres(padres);
	  for (std::list<int>::iterator it_pad = padres.begin(); it_pad != padres.end(); ++it_pad)
	       *it_pad = nueva_id[*it_pad];
	  p->setPadres(padres);
	  simp.push_back(p);
     }
     Prueba = simp;
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
     
     ConjClaus combinables(claus.begin(), claus.end(), true);
     id_resolucion += combinables.cantidadClausulas();
     for (ConjClaus::const_iterator it = combinables.begin(); it != combinables.end(); ++it) {
	  boost::shared_ptr<Inferencia> p(new InferenciaHipotesis(*it));
	  Prueba.push_back(p);
     }
     bool resolvio_vacia;
     resolverPredicadosEliminables(combinables, Prueba, resolvio_vacia);
     
     if (resolvio_vacia)
     	  return false;

     bool satisfacible = true;     
     ConjClaus::iterator itComb = combinables.begin();
     ConjClaus procesadas;

     while (satisfacible && itComb != combinables.end() && seguir_busqueda) {
     	  list<Clausula> factores;
     	  itComb->factores(factores);
     	  for (list<Clausula>::iterator it = factores.begin(); it != factores.end(); ++it)
     	       if (!combinables.contieneClausula(*it)) {
		    it->setIdResolucion(id_resolucion);
		    ++id_resolucion;
     		    combinables.agregarClausula(*it);
     		    boost::shared_ptr<Inferencia> p(new InferenciaFactorizacion(itComb->getIdResolucion(), *it));
     		    Prueba.push_back(p);
     	       }
     	  ConjClaus::const_iterator itProc = procesadas.begin();
     	  while (satisfacible && itProc != procesadas.end() && seguir_busqueda) {
     	       list<Clausula> res;
     	       itComb->resolventes(*itProc, res);
     	       list<Clausula>::iterator itRes = res.begin();
     	       while (satisfacible && itRes != res.end()) {
		    itRes->setIdResolucion(id_resolucion);
     		    boost::shared_ptr<Inferencia> p(new InferenciaResolucion(itComb->getIdResolucion(),
									     itProc->getIdResolucion(), *itRes));			
     		    if (itRes->esVacia()) {
     			 satisfacible = false;
     			 Prueba.push_back(p);
     		    }
     		    else if (!itRes->esTautologica() &&
     			     !procesadas.contieneClausula(*itRes) &&
     			     !combinables.contieneClausula(*itRes)) {
     			 combinables.agregarClausula(*itRes);
     			 Prueba.push_back(p);
			 ++id_resolucion;
     		    }
     		    ++itRes;
     	       }
     	       ++itProc;
     	  }
     	  procesadas.agregarClausula(*itComb);
     	  combinables.eliminar(itComb);
     	  itComb = combinables.begin();
     }
     if (!satisfacible)
	  simplificarPrueba(Prueba);
     return satisfacible;
}

bool ResolucionUnitaria::esSatisfacible(t_prueba& Prueba)
{
     return Resolucion::esSatisfacible(Prueba);
}

bool ResolucionUnitaria::esSatisfacible(t_prueba& Prueba, const bool& seguir_busqueda)
{
     using std::list;
     
     ConjClaus combinables(claus.begin(), claus.end(), true);
     id_resolucion += combinables.cantidadClausulas();
     for (ConjClaus::const_iterator it = combinables.begin(); it != combinables.end(); ++it) {
	  boost::shared_ptr<Inferencia> p(new InferenciaHipotesis(*it));
	  Prueba.push_back(p);
     }
     bool resolvio_vacia;
     resolverPredicadosEliminables(combinables, Prueba, resolvio_vacia);
     
     if (resolvio_vacia)
     	  return false;

     bool satisfacible = true;     
     ConjClaus::iterator itComb = combinables.begin();
     ConjClaus procesadas;

     //basta con verificar para los primeros elementos
     //porque las cláusulas estan ordenadas por cantidad de literales
     bool hay_unitarias = combinables.begin()->esUnitaria() || procesadas.begin()->esUnitaria();
     while (satisfacible && itComb != combinables.end()
	    && hay_unitarias && seguir_busqueda) {	  
	  list<Clausula> factores;
	  itComb->factores(factores);
	  for (list<Clausula>::iterator it = factores.begin(); it != factores.end(); ++it)
	       if (!combinables.contieneClausula(*it)) {
		    it->setIdResolucion(id_resolucion);
		    ++id_resolucion;
     		    combinables.agregarClausula(*it);
     		    boost::shared_ptr<Inferencia> p(new InferenciaFactorizacion(itComb->getIdResolucion(), *it));
     		    Prueba.push_back(p);
     	       }
	  ConjClaus::const_iterator itProc = procesadas.begin();
	  while (satisfacible && itProc != procesadas.end() && seguir_busqueda) {
	       if (itComb->esUnitaria() || itProc->esUnitaria()) {
		    list<Clausula> res;
		    itComb->resolventes(*itProc, res);
		    list<Clausula>::iterator itRes = res.begin();
		    while (satisfacible && itRes != res.end()) {
			 itRes->setIdResolucion(id_resolucion);
			 boost::shared_ptr<Inferencia> p(new InferenciaResolucion(itComb->getIdResolucion(),
										  itProc->getIdResolucion(), *itRes));			
			 if (itRes->esVacia()) {
			      satisfacible = false;
			      Prueba.push_back(p);
			 }
			 else if (!itRes->esTautologica() &&
				  !procesadas.contieneClausula(*itRes) &&
				  !combinables.contieneClausula(*itRes)) {
			      combinables.agregarClausula(*itRes);
			      Prueba.push_back(p);
			      ++id_resolucion;
			 }
			 ++itRes;
		    }
	       }
	       ++itProc;
	  }
	  procesadas.agregarClausula(*itComb);
	  combinables.eliminar(itComb);
	  itComb = combinables.begin();
	  hay_unitarias = combinables.begin()->esUnitaria() || procesadas.begin()->esUnitaria();
     }
     if (!satisfacible)
	  simplificarPrueba(Prueba);
     return satisfacible;
}
