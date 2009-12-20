#ifndef MENSAJESERROR_H
#define MENSAJESERROR_H
#include "parser.hpp"
#include <QString>

QString getMensajeErrorParser(const std::pair<Parser::t_error, std::string>& E);

#endif // MENSAJESERROR_H
