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
 * Clase abstracta que representa un termino de lógica de primer
 * orden.
 */

class Termino {
public:
     /**
      * Tipo del término(variable o función).
      * Las constantes son funciones de aridad cero
      */
     typedef enum { Var, Func } t;
     
     /**
      * Constructor de la clase Termino.
      * @param id Identificador del termino.
      * @param t Tipo del termino.
      */
     Termino(const std::string& id, Termino::t tipo);     

     /**
      * @returns El identificador del termino.
      */
     const std::string& getId() const;
     
     /**
      * @returns El tipo del termino.
      */
     t getTipo() const;     

     /**
      * @returns La representación del termino.
      */
     virtual const std::string getString() const = 0;

     /**
      * Sobrecarga de operador == para comparar terminos por
      * igualdad.
      * @param otro Término a comparar.
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
      * @param s Sustitución a aplicar.
      * @returns Un puntero a un nuevo término con las sustituciones
      *  aplicadas. El cliente debe liberar la memoria.
      * @see Sustitucion
      */
     virtual Termino* aplicarSustitucion(const Sustitucion& s) const = 0;
     
     /**
      * Calcula, si es posible, el unificador más general con otro término.
      * La sustitución s es el contexto en el que se unifica, es decir, las sustituciones
      * en s se aplican a los terminos a unificar. Las sustituciones del unificador más general
      * se agregan a s.
      * @param s Contexto de unificación.
      * @param otro Termino con el que se unifica.
      * @returns true si la unificación fue posible, false en caso contrario.
      */
     virtual bool unificar(Sustitucion& s, const Termino& otro) const = 0;

     /**
      * @returns Un puntero a un clon del término. El cliente debe liberarlo.
      */
     virtual Termino* clonar() const = 0;

     /**
      * Renombra las variables para que queden de la forma Xcomienzo, Xcomienzo+1, ..., xcomienzo+n.
      * El valor de comienzo termina siendo el último valor utilizado para el renombre.
      * Se actualiza el mapa de renombres con los renombres realizados.
      * @param comienzo Número de comienzo del renombre.
      * @param renombre mapa con los renombres que se hicieron hasta el momento.
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
      * @returns Cantidad de argumentos de la función.
      */
     int aridad() const;
     
     /**
      * @returns true si es una constante, false en caso contrario.
      */
     bool esConstante() const;

     /**
      * Agrega un argumento a la función.
      * @param t Argumento a agregar.
      */
     void agregarArgumento(const Termino& t);

     ~Funcion();

private:
     Argumentos * args;
};

#endif   
