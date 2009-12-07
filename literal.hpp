#ifndef LITERAL_HPP
#define LITERAL_HPP

#include <list>
#include <iostream>
#include <map>

#include "termino.hpp"
#include "argumentos.hpp"
#include "sustitucion.hpp"

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

     bool unificar(const Literal& otro, Sustitucion& s) const;
     
     bool unificable(const Literal& otro) const;

     bool unificarComplementario(const Literal& otro, Sustitucion& s) const;
     
     void aplicarSustitucion(const Sustitucion& s);

     void agregarArgumento(const Termino& t);

     void setSigno(bool s);
     
     void renombrarVariables(std::map<std::string,std::string>& renombre, int& comienzo);
private:
     Argumentos args;
     bool signo;
     std::string id;
};

#endif
