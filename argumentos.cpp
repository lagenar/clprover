#include "argumentos.hpp"

Argumentos::Argumentos(const Argumentos& Args)
{
     const_iterator it;
     for (it = Args.begin(); it != Args.end(); ++it)
	  switch ((*it)->getTipo()) {
	  case Termino::Var:
	       args.push_back(new Variable(*static_cast<const Variable*>(*it)));
	       break;
	  case Termino::Func:
	       args.push_back(new Funcion(*static_cast<const Funcion*>(*it)));
	       break;
	  }
}


int Argumentos::aridad() const
{
     return args.size();
}

const std::string Argumentos::getString() const
{
     std::string s;
     for (const_iterator it = args.begin(); it != args.end(); ++it)
	  s += (*it)->getString();

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
     switch (t.getTipo()) {
     case Termino::Var:
	  args.push_back(new Variable(*static_cast<const Variable*>(&t)));
	  break;
     case Termino::Func:
	  args.push_back(new Funcion(*static_cast<const Funcion*>(&t)));
	  break;
     }
}

Argumentos::~Argumentos()
{
     for (iterator it = args.begin(); it != args.end(); ++it)
	  delete *it;
}
