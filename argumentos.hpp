#ifndef ARGUMENTOS_HPP
#define ARGUMENTOS_HPP

#include <list>
#include <map>
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

     bool contieneVariable(const std::string& id) const;

     bool unificar(Sustitucion& s, const Argumentos& otro) const;

     void agregarArgumento(const Termino& t);

     void aplicarSustitucion(const Sustitucion& s);

     void renombrarVariables(std::map<std::string,std::string>& renombre, int& comienzo);

     ~Argumentos();

private:
     std::list<Termino*> args;
};

#endif     
