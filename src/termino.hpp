/*
* clprover
* Copyright (C) 2009 Lucas Moauro
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef TERMINO_HPP
#define TERMINO_HPP

#include <list>
#include <map>
#include <iostream>

#include "argumentos.hpp"
#include "sustitucion.hpp"
#include <cassert>

class Sustitucion;

/**
 * Clase abstracta que representa un término de lógica de primer
 * orden.
 */

class Termino {
public:
     /**
      * Tipo del término(variable o función).
      * Las constantes son funciones de aridad cero.
      */
     typedef enum { Var, Func } t;
     
     /**
      * Constructor de la clase Termino.
      * @param id identificador del término.
      * @param t tipo del término.
      */
     Termino(const std::string& id, Termino::t tipo);     

     /**
      * @returns identificador del término.
      */
     const std::string& getId() const;
     
     /**
      * @returns tipo del término.
      */
     t getTipo() const;     

     /**
      * @returns representación en forma de cadena del término.
      */
     virtual const std::string getString() const = 0;

     /**
      * Sobrecarga de operador == para comparar terminos por
      * igualdad.
      * @param otro término a comparar.
      * @returns true si son iguales, false en caso contrario.
      */
     virtual bool operator==(const Termino& otro) const = 0;
     
     /**
      * @param id Identificador de la variable a buscar.
      * @returns true si el término contiene una variable con el
      * identificador id, false en caso contrario.
      */
     virtual bool contieneVariable(const std::string& id) const = 0;

     /**
      * Aplica una sustitución de variables a terminos.
      * @param s sustitución a aplicar.
      * @returns un puntero a un nuevo término con las sustituciones
      *  aplicadas. El cliente debe liberar la memoria.
      * @see Sustitucion
      */
     virtual Termino* aplicarSustitucion(const Sustitucion& s) const = 0;
     
     /**
      * Calcula, si es posible, el unificador más general con otro término.
      * La sustitución s es el contexto en el que se unifica, es decir, las sustituciones
      * en s se aplican a los terminos a unificar. Las sustituciones del unificador más general
      * se agregan a s.
      * @param s contexto de unificación.
      * @param otro término con el que se unifica.
      * @returns true si la unificación fue posible, false en caso contrario.
      */
     virtual bool unificar(Sustitucion& s, const Termino& otro) const = 0;

     /**
      * @returns un puntero a un clon del término. El cliente debe liberarlo.
      */
     virtual Termino* clonar() const = 0;

     /**
      * Renombra las variables del término.
      * Para las variables que tienen un renombre definido en el mapa <EM>renombre</EM>,
      * se usa ese valor. A las variables que no tienen un renombre definido se les
      * asigna identificadores de la forma Xcomienzo, Xcomienzo+1, ..., Xcomienzo+n, y
      * se insertan los renombres en el mapa.
      * El valor de <EM>comienzo</EM> termina siendo comienzo + n + 1.
      */
     virtual void renombrarVariables(std::map<std::string,std::string>& renombre, int& comienzo) = 0;

     virtual ~Termino() { }

protected:
     std::string id;
     t tipo;
};

/**
 * Clase que representa una variable de lógica de primer
 * orden.
 */
class Variable : public Termino {
public:
     Variable(const std::string& id);

     const std::string getString() const;

     bool operator==(const Termino& otro) const;

     bool contieneVariable(const std::string& id) const;

     Termino* aplicarSustitucion(const Sustitucion& s) const;

     bool unificar(Sustitucion& s, const Termino& otro) const;

     Termino* clonar() const;     
     
     void renombrarVariables(std::map<std::string,std::string>& renombre, int& comienzo);	  
};

class Argumentos;

/**
 * Representa una función de lógica de primer orden, las constantes
 * son consideradas funciones de aridad cero.
 */
class Funcion : public Termino {
public:
     typedef std::list<Termino*>::const_iterator const_iterator;

     Funcion(const std::string& id);

     Funcion(const Funcion& f);

     const std::string getString() const;

     bool operator==(const Termino& otro) const;

     bool contieneVariable(const std::string& id) const;

     Termino* aplicarSustitucion(const Sustitucion& s) const;

     bool unificar(Sustitucion& s, const Termino& otro) const;

     Termino* clonar() const;

     void renombrarVariables(std::map<std::string,std::string>&renombre, int& comienzo);

     /**
      * @returns cantidad de argumentos de la función.
      */
     int aridad() const;
     
     /**
      * @returns true si es una constante, false en caso contrario.
      */
     bool esConstante() const;

     /**
      * Agrega un argumento a la función.
      * @param t argumento a agregar.
      */
     void agregarArgumento(const Termino& t);

     ~Funcion();

private:
     Argumentos * args;
};

#endif   
