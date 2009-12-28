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

#include "dialogumg.h"
#include "ui_dialogumg.h"
#include "mensajeserror.h"
#include "parser.hpp"
#include "sustitucion.hpp"
#include <QMessageBox>

DialogUmg::DialogUmg(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::DialogUmg)
{
    m_ui->setupUi(this);
}

DialogUmg::~DialogUmg()
{
    delete m_ui;
}

void DialogUmg::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void DialogUmg::unificar()
{
    std::string s_lit1 = m_ui->lineaLit1->text().toStdString();
    std::string s_lit2 = m_ui->lineaLit2->text().toStdString();

    bool error1, error2;
    std::pair<Parser::t_error, std::string> E1, E2;
    Literal *lit1;
    Literal *lit2;
    lit1 = Parser::parseLiteral(s_lit1, error1, E1);
    lit2 = Parser::parseLiteral(s_lit2, error2, E2);
    if (error1)
        QMessageBox::critical(this, trUtf8("Unificación de literales"),
                              trUtf8("El primer literal no está bien definido\n"));
    else if (error2)
        QMessageBox::critical(this, trUtf8("Unificación de literales"),
                              trUtf8("El segundo literal no está bien definido\n"));
    else {
        Sustitucion s;
        m_ui->textoUnificacion->clear();
        if (lit1->unificar(*lit2, s)) {
            lit1->aplicarSustitucion(s);
            m_ui->textoUnificacion->insertHtml("<b>" + trUtf8("Resultado de la unificación") +
                                               "</b><br>");
            m_ui->textoUnificacion->insertHtml(lit1->getString().c_str());
            m_ui->textoUnificacion->insertHtml("<br><br><b>umg</b><br>");
            m_ui->textoUnificacion->insertHtml(s.getString().c_str());
        } else
            m_ui->textoUnificacion->insertHtml("<b>" + trUtf8("Los literales no son unificables")
                                               + "</b>");
    }
    delete lit1;
    delete lit2;
}
