#ifndef SUSTITUCION_HPP
#define SUSTITUCION_HPP

#include <map>
#include <iostream>
#include "termino.hpp"

class Termino;

class Sustitucion {
public:     
     const std::string getString() const;

     const Termino* getSustitucion(const std::string& id) const;

     void agregarSustitucion(const std::string& id, const Termino& t);
         
     void componer(const Sustitucion& s);

     ~Sustitucion();
private:
     typedef std::map<std::string, Termino*> t_sust;
     t_sust sustituciones;
};

#endif
