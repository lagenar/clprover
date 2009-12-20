#include "qtclausula.h"
#include "ui_qtclausula.h"
#include "mensajeserror.h"
#include "dialogumg.h"
#include <QListWidget>
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>

QtClausula::QtClausula(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::QtClausula), id(QListWidgetItem::UserType), thread_res(0)
{
    ui->setupUi(this);
}

void QtClausula::agregarClausula(const std::string& cl)
{
    QString cl_s(cl.c_str());
    QListWidgetItem * claus = new QListWidgetItem(cl_s, ui->listaClausulas, id);
    ui->listaClausulas->addItem(claus);
    id++;
}

void QtClausula::agregarClausula()
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

void QtClausula::eliminarClausula()
{
    QListWidgetItem* item = ui->listaClausulas->currentItem();
    if (item) {
        int id_e = item->type();
        parser.eliminarClausula(id_e);
        int row = ui->listaClausulas->row(item);
        delete ui->listaClausulas->takeItem(row);
    }
}

void QtClausula::eliminarClausulas()
{
    for (int i = 0; i < ui->listaClausulas->count(); ++i) {
        QListWidgetItem* item = ui->listaClausulas->item(i);
        parser.eliminarClausula(item->type());
    }
    ui->listaClausulas->clear();
}

void QtClausula::verificarSatisfacibilidad()
{
    std::list<Clausula> l;
    parser.getClausulas(l);
    ConjuntoClausulas<ClausComp> conj(l);
    thread_res = new ThreadResolucion(conj);
    QObject::connect(thread_res, SIGNAL(finished()), this, SLOT(mostrarResultados()));
    ui->botonVerificarSatis->setEnabled(false);
    ui->botonDetener->setEnabled(true);
    ui->actionVerificar_Satisfacibilidad->setEnabled(false);
    thread_res->start();
}

void QtClausula::mostrarInferencia(int i, const Inferencia& inf)
{
    QString inf_id(inf.getId().c_str());
    ui->textoInfo->insertPlainText(QString("%1) %2").arg(QString::number(i), inf_id));
    std::list<Clausula> padres;
    inf.getPadres(padres);
    if (padres.size() > 0) {
        ui->textoInfo->insertPlainText("(");
        std::list<Clausula>::const_iterator it = padres.begin();
        ui->textoInfo->insertPlainText(it->getString().c_str());
        ++it;
        while (it != padres.end()) {
            ui->textoInfo->insertPlainText(QString(", %1").arg(it->getString().c_str()));
            ++it;
        }
        ui->textoInfo->insertPlainText(")");
    }
    QString cl = inf.getClausula().getString().c_str();
    ui->textoInfo->insertPlainText(QString(" = %1\n").arg(cl));
}

void QtClausula::mostrarResultados()
{
    limpiarTexto();
    if (thread_res->esSatisfacible())
        ui->labelSatis->setText(QString("Satisfacible"));
    else
        ui->labelSatis->setText(QString("Insatisfacible"));
    ui->textoInfo->insertPlainText("Pasos de resolucion:\n");
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

void QtClausula::detenerResolucion()
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

void QtClausula::limpiarTexto()
{
    ui->textoInfo->clear();
    ui->labelSatis->setText(QString("..."));
}

void QtClausula::cargarArchivo(const QString& nombre)
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
            QMessageBox::warning(this, "Error", "Archivo corrupto");
            parser = Parser();
            eliminarClausulas();
            return;
        }
        agregarClausula(parser.getClausula(id).getString());
    }
}

void QtClausula::abrirArchivo()
{
    QString nombre = QFileDialog::getOpenFileName(this, "Abrir Archivo");
    if (!nombre.isNull())
        cargarArchivo(nombre);
}

void QtClausula::guardarClausulas()
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

void QtClausula::unificarLiterales()
{
    DialogUmg diag;
    diag.exec();
}

QtClausula::~QtClausula()
{
    delete ui;
}
