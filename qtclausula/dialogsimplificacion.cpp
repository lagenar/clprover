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

#include "dialogsimplificacion.h"
#include "ui_dialogsimplificacion.h"

DialogSimplificacion::DialogSimplificacion(const ConjuntoClausulas<ClausComp>& claus, QWidget *parent) :
    QDialog(parent),
    claus(claus),
    m_ui(new Ui::DialogSimplificacion)
{
    m_ui->setupUi(this);
}

DialogSimplificacion::~DialogSimplificacion()
{
    delete m_ui;
}

void DialogSimplificacion::changeEvent(QEvent *e)
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

void DialogSimplificacion::simplificarConjunto()
{
    ConjuntoClausulas<ClausComp> c = claus;
    if (m_ui->checkTautologicas->isChecked())
        c.simplificarPorTautologicas();
    if (m_ui->checkEquivalentes->isChecked())
        c.simplificarPorEquivalentes();
    if (m_ui->checkLiteralesPuros->isChecked())
        c.simplificarLiteralesPuros();

    m_ui->textoSimplificado->clear();
    if (c.esVacio())
        m_ui->textoSimplificado->insertHtml("&Oslash;<br>");
    for (ConjuntoClausulas<ClausComp>::const_iterator it = c.begin(); it != c.end(); ++it) {
        m_ui->textoSimplificado->insertHtml(it->getString().c_str());
        m_ui->textoSimplificado->insertHtml("<br>");
    }
}
