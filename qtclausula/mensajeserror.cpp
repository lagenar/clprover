#include "mensajeserror.h"

QString getMensajeErrorParser(const std::pair<Parser::t_error, std::string>& E)
{
    QString msg;
    QString ident = E.second.c_str();
    if (E.first == Parser::Aridad)
        msg = QString("El identificador '%1' es usado con diferentes aridades").arg(ident);
    else if (E.first == Parser::TipoId)
        msg = QString("El identificador '%1' es usado con diferentes tipos").arg(ident);
    else
        msg = "Error sintactico";
    return msg;
}
