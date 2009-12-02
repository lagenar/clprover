#ifndef LITERAL_HPP
#define LITERAL_HPP

#include <list>
#include <iostream>
#include "termino.hpp"

class Literal {
public:
     typedef std::list<Termino*>::const_iterator const_iterator;

     Literal(const std::string& id, const std::list<Termino*>& args, bool signo);

     bool getSigno() const;

     const std::string& getId() const;

     const std::string getString() const;
     
     int aridad() const;
     
     bool operator==(const Literal& otro) const;

     const_iterator begin() const;

     const_iterator end() const;

     ~Literal();
private:
     std::list<Termino*> args;
     bool signo;
     std::string id;
};

#endif
