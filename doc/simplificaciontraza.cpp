void Resolucion::agregarInferenciasUsadas(const t_prueba& prueba, int id,
					  std::set<int>& inferencias_usadas) const
{
     inferencias_usadas.insert(prueba[id]->getClausula().getIdResolucion());
     std::list<int> padres;
     prueba[id]->getPadres(padres);
     for (std::list<int>::const_iterator it = padres.begin(); it != padres.end(); ++it)
	  agregarInferenciasUsadas(prueba, *it, inferencias_usadas);
}

void Resolucion::simplificarPrueba(t_prueba& prueba) const
{
     std::set<int> inferencias_usadas;
     t_prueba::const_iterator it_p = prueba.begin();
     //todas las hipotesis se incluyen en la prueba aunque no sean
     //usadas
     while (it_p != prueba.end() && (*it_p)->getTipo() == Inferencia::Hipot) {
	  inferencias_usadas.insert((*it_p)->getClausula().getIdResolucion());
	  ++it_p;
     }
     agregarInferenciasUsadas(prueba, prueba.size() - 1, inferencias_usadas);
     t_prueba simp;
     //se asigna un nuevo identificador a cada clausula
     //usada para que queden numeradas secuencialmente.
     std::map<int, int> nueva_id;
     int id = 0;
     for (std::set<int>::const_iterator it = inferencias_usadas.begin();
	  it != inferencias_usadas.end(); ++it) {
	  nueva_id[*it] = id;
	  ++id;
	  boost::shared_ptr<Inferencia> p = prueba[*it];
	  std::list<int> padres;
	  p->getPadres(padres);
	  for (std::list<int>::iterator it_pad = padres.begin(); it_pad != padres.end();
	       ++it_pad)
	       *it_pad = nueva_id[*it_pad];
	  p->setPadres(padres);
	  simp.push_back(p);
     }
     prueba = simp;
}
