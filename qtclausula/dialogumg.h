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

#ifndef DIALOGUMG_H
#define DIALOGUMG_H

#include <QtGui/QDialog>

namespace Ui {
    class DialogUmg;
}

class DialogUmg : public QDialog {
    Q_OBJECT
public:
    DialogUmg(QWidget *parent = 0);
    ~DialogUmg();

public slots:
    void unificar();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::DialogUmg *m_ui;
};

#endif // DIALOGUMG_H
