#include "argumentos.hpp"
#include <cassert>

Argumentos::Argumentos(const Argumentos& Args)
{
     const_iterator it;
     for (it = Args.begin(); it != Args.end(); ++it)
	  args.push_back((*it)->clonar());
}


int Argumentos::aridad() const
{
     return args.size();
}

const std::string Argumentos::getString() const
{
     //sólo debe invocarse cuando hay 1 o más argumentos
     assert(aridad() > 0);

     const_iterator it = args.begin();
     std::string s((*it)->getString());
     ++it;
     while (it != args.end()) {
	  s += ", " + (*it)->getString();
	  ++it;
     }

     return "(" + s + ")";
}

Argumentos::const_iterator Argumentos::begin() const
{
     return args.begin();
}

Argumentos::const_iterator Argumentos::end() const
{
     return args.end();
}

bool Argumentos::operator==(const Argumentos& otro) const
{
     if (otro.aridad() != aridad())
	  return false;
     
     bool igual = true;
     const_iterator it = args.begin();
     const_iterator it_otro = otro.begin();
     while (igual && it != args.end() && it_otro != otro.end()) {
	  igual = (**it == **it_otro);
	  ++it;
	  ++it_otro;
     }
     return igual;
}

void Argumentos::agregarArgumento(const Termino& t)
{
     args.push_back(t.clonar());
}

Argumentos::~Argumentos()
{
     for (iterator it = args.begin(); it != args.end(); ++it)
	  delete *it;
}
