bool Clausula::subsume(const Clausula& otra) const
{
     Sustitucion s;
     if (!predicados().subconjunto(otra.predicados()))
	  return false;
     return subsume(s, literales.begin(), otra);
}

bool Clausula::subsume(Sustitucion& s, const_iterator lit1, const Clausula& otra) const
{
     if (lit1 == literales.end())
	  return true;
     
     bool subsum = false;
     const_iterator it_otra = otra.begin();
     while (it_otra != otra.end() && !subsum) {
	  Sustitucion s1 = s;
	  if (lit1->unificarSubsuncion(*it_otra, s1)) {
	       const_iterator sig = lit1;
	       ++sig;
	       subsum = subsume(s, sig, otra);
	  }
	  ++it_otra;
     }
     return subsum;
}
