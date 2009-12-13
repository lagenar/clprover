#ifndef INFERENCIA_HPP
#define INFERENCIA_HPP
#include "clausula.hpp"
#include <list>

class Inferencia {
public:
     Inferencia(const Clausula& cl) :
	  claus(cl) { }
          
     virtual std::string getId() const = 0;
     
     const Clausula& getClausula() const {
	  return claus;
     }
     
     virtual void getPadres(std::list<Clausula>& p) const {
     }
protected:
     Clausula claus;
};

class InferenciaHipotesis : public Inferencia {
public:
     InferenciaHipotesis(const Clausula& cl) :
	  Inferencia(cl) { }

     std::string getId() const {
	  return "Hipo";
     }    
};

class InferenciaResolucion : public Inferencia {
public:
     InferenciaResolucion(Clausula res1, Clausula res2, const Clausula& cl) :
	  Inferencia(cl), res1(res1), res2(res2) { }
     
     std::string getId() const {
	  return "Res";
     }

     void getPadres(std::list<Clausula>& p) const {	 
	  p.push_back(res1);
	  p.push_back(res2);
     }
private:
     Clausula res1, res2;
};

class InferenciaFactorizacion : public Inferencia {
public:
     InferenciaFactorizacion(Clausula fact, const Clausula& cl) :
	  Inferencia(cl), fact(fact){ }

     std::string getId() const {
	  return "Fact";
     }

     void getPadres(std::list<Clausula>& p) const {
	  p.push_back(fact);
     }
private:
     Clausula fact;
};

#endif
