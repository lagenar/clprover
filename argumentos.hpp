#ifndef ARGUMENTOS_HPP
#define ARGUMENTOS_HPP

#include <list>
#include <iostream>
#include "termino.hpp"
#include "sustitucion.hpp"

class Termino;
class Sustitucion;

class Argumentos {

public:
     typedef std::list<Termino*>::iterator iterator;
     typedef std::list<Termino*>::const_iterator const_iterator;

     Argumentos() { }

     Argumentos(const Argumentos& Args);

     int aridad() const;

     const std::string getString() const;

     const_iterator begin() const;

     const_iterator end() const;

     bool operator==(const Argumentos& otro) const;

     void agregarArgumento(const Termino& t);

     void aplicarSustitucion(const Sustitucion& s);

     ~Argumentos();

private:
     std::list<Termino*> args;
};

#endif     
