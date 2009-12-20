#ifndef PARSER_HPP
#define PARSER_HPP

#include <map>
#include <memory>
#include "gramatica.hpp"
#include "clausula.hpp"

class Parser {
public:
     typedef enum {Sintactico, Aridad, TipoId} t_error;
     
     void parseClausula(int id, const std::string& clausula,
			bool& error, std::pair<t_error, std::string>& E);

     void eliminarClausula(int id);

     Clausula getClausula(int id) const;

     void getClausulas(std::list<Clausula>& l) const;

     static Literal* parseLiteral(const std::string& lit, bool& error,
				  std::pair<t_error, std::string>& E);
private:   
     std::map<int, Clausula> clausulas;
     
     //para manejo de errores semánticos
     typedef std::map<std::string, std::set<int> > t_apar;
     typedef std::map<std::string, std::pair<int, client::t_id> > t_attr;
     
     t_apar aparicionesId;
     t_attr atributosId;

     typedef client::gramatica_clausula<std::string::const_iterator> gramatica_clausula;
     typedef client::gramatica_literal<std::string::const_iterator> gramatica_literal;

     void verificarAtributos(bool& error,
			     std::pair<t_error, std::string>& E, const client::t_attrs& at);
     
     void agregarAtributos(int id, const client::t_attrs& atr_clausula);
};

#endif
