#ifndef LITERAL_HPP
#define LITERAL_HPP

#include <list>
#include <iostream>
#include "termino.hpp"
#include "argumentos.hpp"

class Literal {
public:
     Literal(const std::string& id, bool signo);

     Literal(const std::string& id, const Argumentos& args, bool signo);

     bool getSigno() const;

     const std::string& getId() const;

     const std::string getString() const;
     
     int aridad() const;
     
     bool operator==(const Literal& otro) const;
     
     bool operator!=(const Literal& otro) const;

     void agregarArgumento(const Termino& t);

     void setSigno(bool s);
private:
     Argumentos args;
     bool signo;
     std::string id;
};

#endif
