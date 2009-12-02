#ifndef LITERAL_HPP
#define LITERAL_HPP

#include <list>
#include <iostream>
#include "termino.hpp"

class Literal {
public:
     Literal(const std::string& id, const std::list<Termino*>& args, bool signo);

     bool getSigno() const;

     const std::string& getId() const;

     const std::string getString() const;
     
     int aridad() const;
     
     const std::list<Termino*> getArgumentos() const;
     
     bool operator==(const Literal& otro) const;

     ~Literal();
private:
     std::list<Termino*> args;
     bool signo;
     std::string id;
};

#endif
