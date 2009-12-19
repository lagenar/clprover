#include "qtclausula.h"
#include "ui_qtclausula.h"
#include <QListWidget>
#include <QMessageBox>

QtClausula::QtClausula(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::QtClausula), id(QListWidgetItem::UserType), thread_res(0)
{
    ui->setupUi(this);
}

void QtClausula::agregarClausula()
{
    if (ui->lineaClausula->text().size() > 0) {
        std::string cl = ui->lineaClausula->text().toStdString();
        bool error;
        std::pair<Parser::t_error, std::string> E;
        parser.parseClausula(id, cl, error, E);
        if (error) {
            QString msg;
            QString ident(E.second.c_str());
            if (E.first == Parser::Aridad)
                msg = QString("Error de aridad para el identificador %1").arg(ident);
            else if (E.first == Parser::TipoId)
                msg = QString("Error de tipo para el identificador %1").arg(ident);
            else
                msg = "Error sintactico";
            QMessageBox::warning(this, "Error", msg);
        } else {
            QString cl_s(parser.getClausula(id).getString().c_str());
            QListWidgetItem * claus = new QListWidgetItem(cl_s, ui->listaClausulas, id);
            ui->listaClausulas->addItem(claus);
            ui->lineaClausula->clear();
            ++id;
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
    ui->botonDetener->setEnabled(false);
}

void QtClausula::limpiarTexto()
{
    ui->textoInfo->clear();
    ui->labelSatis->setText(QString("..."));
}

QtClausula::~QtClausula()
{
    delete ui;
}
