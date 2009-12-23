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

#ifndef DIALOGFORMULA_H
#define DIALOGFORMULA_H

#include <QtGui/QDialog>

namespace Ui {
    class DialogFormula;
}

class DialogFormula : public QDialog {
    Q_OBJECT
public:
    DialogFormula(QWidget *parent = 0);
    ~DialogFormula();
    bool cargoFormula();
    QString getNombreArchivoClausulas() const;

public slots:
    void cargarFormula();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::DialogFormula *m_ui;

    QString archivo;
    bool cargo_formula;
};

#endif // DIALOGFORMULA_H
