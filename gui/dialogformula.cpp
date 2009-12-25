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

#include "dialogformula.h"
#include "ui_dialogformula.h"
#include "mensajeserror.h"
#include <QProcess>
#include <QDir>
#include <QFile>
#include <QTemporaryFile>
#include <QMessageBox>
#include <QApplication>
#include "parser.hpp"

DialogFormula::DialogFormula(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::DialogFormula),
    cargo_formula(false)
{
    m_ui->setupUi(this);
}

DialogFormula::~DialogFormula()
{
    delete m_ui;
}

void DialogFormula::changeEvent(QEvent *e)
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

void DialogFormula::cargarFormula()
{
    const int error_sintactico = 1;
    const int error_aridad = 2;
    const int error_tipo = 3;

    QString formula = m_ui->lineaFormula->text();
    QTemporaryFile entrada;
    entrada.open();
    entrada.write(formula.toAscii());
    entrada.write("\n");
    entrada.close();

    QString salida = QDir::tempPath() + "/salidafol.tmp";
    salida = QDir::toNativeSeparators(salida);
    QString path = QApplication::applicationDirPath();
#ifdef Q_WS_X11
    QString comando = path + "/fol < " + entrada.fileName() + " > " + salida;
    int c = QProcess::execute("sh -c \"" + comando + "\"");
#else
    QString comando = path + "\fol.exe < " + entrada.fileName() + " > " + salida;
    int c = QProcess::execute("cmd /C \"" + comando + "\"");
#endif
    if (c != 0) {
        std::pair<Parser::t_error, std::string> E;
        if (c != error_sintactico) {
            QFile s(salida);
            s.open(QIODevice::ReadOnly | QIODevice::Text);
            QString id = s.readLine();
            s.close();
            E.second = id.toStdString();
            if (c == error_aridad)
                E.first = Parser::Aridad;
            else if (c == error_tipo)
                E.first = Parser::TipoId;
        } else
            E.first = Parser::Sintactico;

        QMessageBox::warning(this, "Error", getMensajeErrorParser(E));
    } else {
        cargo_formula = true;
        archivo = salida;
        accept();
    }
}

bool DialogFormula::cargoFormula()
{
    return cargo_formula;
}

QString DialogFormula::getNombreArchivoClausulas() const
{
    return archivo;
}
