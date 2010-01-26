#include <iostream>
#include <cstdlib>

#include "parser.hpp"
#include "resolucion.hpp"

using namespace std;

void mostrarConjunto(const ConjuntoClausulas<>& conj)
{
     for (ConjuntoClausulas<>::const_iterator it = conj.begin(); it != conj.end(); ++it)
	  cout << it->getString() << endl;
}

void mostrarInferencia(int i, const Inferencia& inf)
{
     cout << i << ") " << inf.getClausula().getString();
     cout << "     [";
     if (inf.getTipo() == Inferencia::Hipot)
	  cout << "Hipotesis";
     else if (inf.getTipo() == Inferencia::Res)
	  cout << "Res";
     else
	  cout << "Factor";
     list<int> padres;
     inf.getPadres(padres);
     if (padres.size() > 0) {
	  cout << "(";
	  list<int>::const_iterator it = padres.begin();
	  cout << *it + 1;
	  ++it;
	  while (it != padres.end()) {
	       cout << ", " << *it + 1;
	       ++it;
	  }
	  cout << ")";
     }
     cout << "]" << endl;
}

int main()
{
     int i = 0;
     string cl;
     Parser parser;
     while (getline(cin, cl)) {
	  bool error;
	  pair<Parser::t_error, string> E;
	  parser.parseClausula(i, cl, error, E);
	  if (error) {
	       cout << "Error en la clausula " << i << std::endl;
	       exit(1);
	  }
	  ++i;
     }
     ConjuntoClausulas<> Conj;
     parser.getClausulas(Conj);
     cout << "Conjunto original de clausulas: " << endl;
     mostrarConjunto(Conj);
     bool simp = Conj.simplificarPorTautologicas();
     simp = Conj.simplificarPorEquivalentes() || simp;
     simp = Conj.simplificarLiteralesPuros() || simp;
     if (simp) {
	  cout << endl << "Conjunto Simplificado:";
	  mostrarConjunto(Conj);
     }
     Resolucion::t_prueba prueba;
     bool s;
     if (Conj.esDeHorn()) {
	  ResolucionUnitaria res(Conj);
	  s = res.esSatisfacible(prueba);
     } else {
	  ResolucionGeneral res(Conj);
	  s = res.esSatisfacible(prueba);
     }
     if (s)
	  cout << "El Conjunto es satisfacible" << endl;
     else {
	  cout << "El Conjunto es insatisfacible" << endl;
	  cout << endl << "Pasos de resolucion" << endl;
	  int i = 1;
	  for (Resolucion::t_prueba::const_iterator it = prueba.begin(); it != prueba.end(); ++it) {
	       mostrarInferencia(i, **it);
	       ++i;
	  }
     }
     return 0;
}
