void Clausula::factores(std::list<Clausula>& fact) const
{
     for (const_iterator l1 = literales.begin(); l1 != literales.end(); ++l1) {
	  const_iterator l2 = l1;
	  ++l2;
	  while (l2 != literales.end() && l2->getId() == l1->getId()) {
	       Sustitucion s;
	       if (l1->unificar(*l2, s)) {
		    Clausula f;
		    for (const_iterator it = literales.begin(); it != literales.end();
			 ++it)
			 if (it != l2) {
			      Literal l = *it;
			      l.aplicarSustitucion(s);
			      f.agregarLiteral(l);
			 }
		    fact.push_back(f);
	       }
	       ++l2;
	  }
     }
}
