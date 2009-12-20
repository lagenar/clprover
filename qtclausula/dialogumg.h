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
