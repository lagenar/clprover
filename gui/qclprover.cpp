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

#include "qclprover.h"
#include "ui_qclprover.h"
#include "mensajeserror.h"
#include "dialogumg.h"
#include "dialogsimplificacion.h"
#include "dialogformula.h"
#include <QListWidget>
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>

Qclprover::Qclprover(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::Qclprover), id(QListWidgetItem::UserType), thread_res(0)
{
    ui->setupUi(this);
}

void Qclprover::agregarClausula(const std::string& cl)
{
    QString cl_s(cl.c_str());
    QListWidgetItem * claus = new QListWidgetItem(cl_s, ui->listaClausulas, id);
    ui->listaClausulas->addItem(claus);
    id++;
}

void Qclprover::agregarClausula()
{
    if (ui->lineaClausula->text().size() > 0) {
        std::string cl = ui->lineaClausula->text().toStdString();
        bool error;
        std::pair<Parser::t_error, std::string> E;
        parser.parseClausula(id, cl, error, E);
        if (error) {
            QMessageBox::warning(this, "No se pudo agregar la clausula", getMensajeErrorParser(E));
        } else {
            agregarClausula(parser.getClausula(id).getString());
            ui->lineaClausula->clear();
        }
    }
}

void Qclprover::eliminarClausula()
{
    QListWidgetItem* item = ui->listaClausulas->currentItem();
    if (item) {
        int id_e = item->type();
        parser.eliminarClausula(id_e);
        int row = ui->listaClausulas->row(item);
        delete ui->listaClausulas->takeItem(row);
    }
}

void Qclprover::eliminarClausulas()
{
    for (int i = 0; i < ui->listaClausulas->count(); ++i) {
        QListWidgetItem* item = ui->listaClausulas->item(i);
        parser.eliminarClausula(item->type());
    }
    ui->listaClausulas->clear();
}

void Qclprover::verificarSatisfacibilidad()
{
    limpiarTexto();
    ConjuntoClausulas<> conj;
    parser.getClausulas(conj);
    bool simp;
    simp = conj.simplificarPorTautologicas();
    simp = conj.simplificarPorEquivalentes() ? true : simp;
    simp = conj.simplificarLiteralesPuros() ? true : simp;
    if (simp) {
        ui->textoInfo->insertHtml("<b>Conjunto de clausulas simplificado</b><br>");
        mostrarConjunto(conj);
    }

    thread_res = new ThreadResolucion(conj);
    QObject::connect(thread_res, SIGNAL(finished()), this, SLOT(mostrarResultados()));
    ui->botonVerificarSatis->setEnabled(false);
    ui->botonDetener->setEnabled(true);
    ui->actionVerificar_Satisfacibilidad->setEnabled(false);
    thread_res->start();
}

void Qclprover::mostrarConjunto(const ConjuntoClausulas<ClausComp>& claus)
{
    if (claus.esVacio())
        ui->textoInfo->insertHtml("&Oslash;<br>");
    for (ConjuntoClausulas<ClausComp>::const_iterator it = claus.begin(); it != claus.end(); ++it) {
        ui->textoInfo->insertHtml(it->getString().c_str());
        ui->textoInfo->insertHtml("<br>");
    }
}

void Qclprover::mostrarInferencia(int i, const Inferencia& inf)
{
    QString inf_id(inf.getId().c_str());
    ui->textoInfo->insertHtml(QString("%1) %2").arg(QString::number(i), inf_id));
    std::list<Clausula> padres;
    inf.getPadres(padres);
    if (padres.size() > 0) {
        ui->textoInfo->insertHtml("(");
        std::list<Clausula>::const_iterator it = padres.begin();
        ui->textoInfo->insertHtml(it->getString().c_str());
        ++it;
        while (it != padres.end()) {
            ui->textoInfo->insertHtml(QString(", %1").arg(it->getString().c_str()));
            ++it;
        }
        ui->textoInfo->insertHtml(")");
    }
    QString cl;
    if (inf.getClausula().esVacia())
        cl = "&perp;"; // _|_
    else
        cl = inf.getClausula().getString().c_str();
    ui->textoInfo->insertHtml(QString(" = %1<br>").arg(cl));
}

void Qclprover::mostrarResultados()
{
    if (thread_res->esSatisfacible())
        ui->labelSatis->setText(QString("Satisfacible"));
    else
        ui->labelSatis->setText(QString("Insatisfacible"));
    ui->textoInfo->insertHtml("<b>Pasos de resolucion<b><br>");
    Resolucion::t_prueba prueba = thread_res->getPrueba();
    int i = 1;
    for (Resolucion::t_prueba::const_iterator it = prueba.begin(); it != prueba.end(); ++it) {
        mostrarInferencia(i, **it);
        ++i;
    }
    delete thread_res;
    thread_res = 0;
    ui->botonVerificarSatis->setEnabled(true);
    ui->actionVerificar_Satisfacibilidad->setEnabled(true);
    ui->botonDetener->setEnabled(false);
}

void Qclprover::detenerResolucion()
{
    QObject::disconnect(thread_res, SIGNAL(finished()), this, SLOT(mostrarResultados()));
    thread_res->terminar();
    thread_res->wait();
    delete thread_res;
    thread_res = 0;
    ui->botonVerificarSatis->setEnabled(true);
    ui->actionVerificar_Satisfacibilidad->setEnabled(true);
    ui->botonDetener->setEnabled(false);
}

void Qclprover::limpiarTexto()
{
    ui->textoInfo->clear();
    ui->labelSatis->setText(QString("..."));
}

void Qclprover::cargarArchivo(const QString& nombre)
{
    QFile file(nombre);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error al abrir el archivo", "No se pudo abrir el archivo");
        return;
    }
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString linea = in.readLine();
        bool error;
        std::pair<Parser::t_error, std::string> E;
        parser.parseClausula(id, linea.toStdString(), error, E);
        if (error) {
            eliminarClausulas();
            QMessageBox::warning(this, "Error", "Archivo corrupto");
            parser = Parser();
            return;
        }
        agregarClausula(parser.getClausula(id).getString());
    }
}

void Qclprover::abrirArchivo()
{
    QString nombre = QFileDialog::getOpenFileName(this, "Abrir Archivo");
    if (!nombre.isNull())
        cargarArchivo(nombre);
}

void Qclprover::guardarClausulas()
{
    QString nombre = QFileDialog::getSaveFileName(this, "Guardar");
    if (!nombre.isNull()) {
        QFile file(nombre);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::warning(this, "Error", "No se pudo crear el archivo para escritura");
            return;
        }
        QTextStream out(&file);
        std::list<Clausula> l;
        parser.getClausulas(l);
        for (std::list<Clausula>::const_iterator it = l.begin(); it != l.end(); ++it)
            out << it->getString().c_str() << "\n";
    }
}

void Qclprover::unificarLiterales()
{
    DialogUmg diag;
    diag.exec();
}

void Qclprover::simplificarConjunto()
{
    ConjuntoClausulas<> conj;
    parser.getClausulas(conj);
    DialogSimplificacion diag(conj);
    diag.exec();

}

void Qclprover::cargarFormula()
{
    DialogFormula diag;
    diag.exec();
    if (diag.cargoFormula()) {
        eliminarClausulas();
        cargarArchivo(diag.getNombreArchivoClausulas());
    }
}

Qclprover::~Qclprover()
{
    delete ui;
}
