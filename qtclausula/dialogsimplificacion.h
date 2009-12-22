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

#ifndef DIALOGSIMPLIFICACION_H
#define DIALOGSIMPLIFICACION_H

#include <QtGui/QDialog>
#include "clausulas.hpp"

namespace Ui {
    class DialogSimplificacion;
}

class DialogSimplificacion : public QDialog {
    Q_OBJECT
public:
    DialogSimplificacion(const ConjuntoClausulas<ClausComp>& claus, QWidget *parent = 0);
    ~DialogSimplificacion();

public slots:
    void simplificarConjunto();

protected:
    void changeEvent(QEvent *e);

private:
    ConjuntoClausulas<ClausComp> claus;
    Ui::DialogSimplificacion *m_ui;
};

#endif // DIALOGSIMPLIFICACION_H
