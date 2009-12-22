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

#ifndef QCLPROVER_H
#define QCLPROVER_H

#include <QtGui/QMainWindow>
#include "parser.hpp"
#include "threadresolucion.h"

namespace Ui
{
    class Qclprover;
}

class Qclprover : public QMainWindow
{
    Q_OBJECT

public slots:
    void agregarClausula();
    void eliminarClausula();
    void eliminarClausulas();
    void verificarSatisfacibilidad();
    void mostrarResultados();
    void detenerResolucion();
    void limpiarTexto();
    void abrirArchivo();
    void guardarClausulas();
    void unificarLiterales();
    void simplificarConjunto();

public:
    Qclprover(QWidget *parent = 0);
    ~Qclprover();

private:
    void mostrarInferencia(int i, const Inferencia& inf);
    void cargarArchivo(const QString& nombre);
    void agregarClausula(const std::string& cl);
    void mostrarConjunto(const ConjuntoClausulas<ClausComp>& claus);

private:
    Ui::Qclprover *ui;

    Parser parser;
    int id;
    ThreadResolucion *thread_res;
};

#endif // QCLPROVER_H
