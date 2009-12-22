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
