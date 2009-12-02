#ifndef TERMINO_HPP
#define TERMINO_HPP

#include <list>
#include <iostream>

class Termino {
public:
     // Tipo de termino (las constantes son funciones de aridad 0)
     typedef enum { Var, Func } t;
     
     Termino(const std::string& id, Termino::t tipo);

     const std::string& getId() const;
     
     t getTipo() const;

     virtual const std::string getString() const = 0;

     virtual bool operator==(const Termino * otro) const;

     virtual ~Termino() { }

protected:
     std::string id;
     t tipo;
};


class Variable : public Termino {
public:
     Variable(const std::string& id);

     const std::string getString() const;

     bool operator==(const Termino * otro) const;
     
};

class Funcion : public Termino {
public:
     Funcion(const std::string& id, const std::list<Termino*>& args);

     const std::string getString() const;

     bool operator==(const Termino * otro) const;

     int aridad() const;
     
     bool esConstante() const;

     const std::list<Termino*>& getArgumentos() const;

     ~Funcion();

private:
     std::list<Termino*> args;
};

#endif   
