#ifndef QTCLAUSULA_H
#define QTCLAUSULA_H

#include <QtGui/QMainWindow>
#include "parser.hpp"
#include "threadresolucion.h"
#include "dialogounificacion.h"

namespace Ui
{
    class QtClausula;
}

class QtClausula : public QMainWindow
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

public:
    QtClausula(QWidget *parent = 0);
    ~QtClausula();

private:
    void mostrarInferencia(int i, const Inferencia& inf);
    void cargarArchivo(const QString& nombre);
    void agregarClausula(const std::string& cl);

private:
    Ui::QtClausula *ui;

    Parser parser;
    int id;
    ThreadResolucion *thread_res;
};

#endif // QTCLAUSULA_H