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

bool todosAscii(const QString& s)
{
    for (QString::const_iterator it = s.begin(); it != s.end(); ++it)
        if (it->unicode() > 127)
            return false;
    return true;
}

void Qclprover::agregarClausula()
{
    QString simp = ui->lineaClausula->text().simplified();
    std::string cl = simp.toStdString();
    /*si la cadena tiene caracteres no-ascii
      se rechaza porque no son aceptados por spirit*/
    if (!todosAscii(simp)) {
        std::pair<Parser::t_error, std::string> E;
        E.first = Parser::Sintactico;
        QMessageBox::critical(this, trUtf8("Carga de cláusula"), getMensajeErrorParser(E));
    }
    else if (cl.size() > 0) {
        bool error;
        std::pair<Parser::t_error, std::string> E;
        parser.parseClausula(id, cl, error, E);
        if (error) {
            QMessageBox::critical(this, trUtf8("Carga de cláusula"), getMensajeErrorParser(E));
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
    std::list<Clausula> l;
    parser.getClausulas(l);
    ui->textoInfo->insertHtml("<b>" + trUtf8("Conjunto original de cláusulas:") + "</b><br>");
    mostrarConjunto(l.begin(), l.end());
    ui->textoInfo->insertHtml("<br>");
    ConjuntoClausulas<> conj(l);
    bool simp;
    simp = conj.simplificarPorTautologicas();
    simp = conj.simplificarPorEquivalentes() || simp;
    simp = conj.simplificarLiteralesPuros() || simp;
    if (simp) {
        ui->textoInfo->insertHtml("<b>" + trUtf8("Conjunto de cláusulas simplificado:") + "</b><br>");
        mostrarConjunto(conj.begin(), conj.end());
        ui->textoInfo->insertHtml("<br>");
    }

    if (conj.esVacio()) {
        ui->textoInfo->insertHtml("<b>" + tr("Propiedad utilizada:") + "</b><br>");
        ui->textoInfo->insertHtml(trUtf8("El conjunto vacio es satisfacible por definición"));
        ui->textoInfo->insertHtml("<br><br>");
        mostrarSatisfacibilidad(true);
    }
    else if (conj.esLogicaPrograma()) {
        ui->textoInfo->insertHtml("<b>" + tr("Propiedad utilizada:") + "</b><br>");
        ui->textoInfo->insertHtml("<b>" + trUtf8("El conjunto es una lógica de programa.\n"
                                                 "Toda lógica de programa es satisfacible."));
        ui->textoInfo->insertHtml("<br><br>");
        mostrarSatisfacibilidad(true);
    }
    else {
        thread_res = new ThreadResolucion(conj);
        QObject::connect(thread_res, SIGNAL(finished()), this, SLOT(mostrarResultados()));
        ui->botonVerificarSatis->setEnabled(false);
        ui->botonDetener->setEnabled(true);
        ui->actionVerificar_Satisfacibilidad->setEnabled(false);
        thread_res->start();
    }
}

void Qclprover::mostrarSatisfacibilidad(bool satis)
{
    QString s = satis ? tr("Satisfacible") : tr("Insatisfacible");
    ui->textoInfo->insertHtml("<b>" + tr("Satisfacibilidad: ") + "</b>" + s);
    ui->labelSatis->setText(s);
}

template <typename Iterator>
void Qclprover::mostrarConjunto(Iterator begin, Iterator end)
{
    if (begin == end)
        ui->textoInfo->insertHtml("&Oslash;<br>");
    for (Iterator it = begin; it != end; ++it) {
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
    ui->textoInfo->insertHtml("<b>" + trUtf8("Método de resolución: ") + "</b>");
    if (thread_res->getTipoResolucion() == ThreadResolucion::General)
        ui->textoInfo->insertHtml(trUtf8("Resolución general"));
    else
        ui->textoInfo->insertHtml(trUtf8("Resolución unitaria"));
    ui->textoInfo->insertHtml("<br><br><b>" + trUtf8("Pasos de resolución:") + "</b><br>");
    Resolucion::t_prueba prueba = thread_res->getPrueba();
    int i = 1;
    for (Resolucion::t_prueba::const_iterator it = prueba.begin(); it != prueba.end(); ++it) {
        mostrarInferencia(i, **it);
        ++i;
    }
    ui->textoInfo->insertHtml("<br>");
    if (thread_res->esSatisfacible())
        mostrarSatisfacibilidad(true);
    else
        mostrarSatisfacibilidad(false);
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
        QMessageBox::critical(this, tr("Abrir archivo"),
                              tr("No se pudo abrir el archivo \n"
                                 "Verifique si tiene acceso de lectura al directorio"));
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
            QMessageBox::critical(this, tr("Abrir archivo"),
                                  tr("El archivo no tiene es formato correcto"));
            parser = Parser();
            return;
        }
        agregarClausula(parser.getClausula(id).getString());
    }
}

void Qclprover::abrirArchivo()
{
    QString nombre = QFileDialog::getOpenFileName(this, tr("Abrir Archivo"));
    if (!nombre.isNull())
        cargarArchivo(nombre);
}

void Qclprover::guardarClausulas()
{
    QString nombre = QFileDialog::getSaveFileName(this, tr("Guardar"));
    if (!nombre.isNull()) {
        QFile file(nombre);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::critical(this, tr("Guardar"),
                                  tr("No se pudo crear el archivo"
                                     "\nVerifique si tiene acceso de escritura al directorio"));
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
