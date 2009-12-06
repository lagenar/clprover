#ifndef TERMINO_HPP
#define TERMINO_HPP

#include <list>
#include <iostream>
#include "argumentos.hpp"

class Termino {
public:
     // Tipo de termino (las constantes son funciones de aridad 0)
     typedef enum { Var, Func } t;
     
     Termino(const std::string& id, Termino::t tipo);     

     const std::string& getId() const;
     
     t getTipo() const;

     virtual const std::string getString() const = 0;

     virtual bool operator==(const Termino& otro) const = 0;

     virtual Termino* clonar() const = 0;

     virtual ~Termino() { }

protected:
     std::string id;
     t tipo;
};


class Variable : public Termino {
public:
     Variable(const std::string& id);

     const std::string getString() const;

     bool operator==(const Termino& otro) const;

     Termino* clonar() const;     
};

class Argumentos;

class Funcion : public Termino {
public:
     typedef std::list<Termino*>::const_iterator const_iterator;

     Funcion(const std::string& id);

     Funcion(const Funcion& f);

     const std::string getString() const;

     bool operator==(const Termino& otro) const;

     Termino* clonar() const;

     int aridad() const;
     
     bool esConstante() const;

     void agregarArgumento(const Termino& t);

     ~Funcion();

private:
     Argumentos * args;
};

#endif   
