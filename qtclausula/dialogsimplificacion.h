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
