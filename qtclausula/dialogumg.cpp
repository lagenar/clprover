#include "dialogumg.h"
#include "ui_dialogumg.h"
#include "mensajeserror.h"
#include "parser.hpp"
#include "sustitucion.hpp"
#include <QMessageBox>

DialogUmg::DialogUmg(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::DialogUmg)
{
    m_ui->setupUi(this);
}

DialogUmg::~DialogUmg()
{
    delete m_ui;
}

void DialogUmg::changeEvent(QEvent *e)
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

void DialogUmg::unificar()
{
    std::string s_lit1 = m_ui->lineaLit1->text().toStdString();
    std::string s_lit2 = m_ui->lineaLit2->text().toStdString();

    bool error1, error2;
    std::pair<Parser::t_error, std::string> E1, E2;
    Literal *lit1;
    Literal *lit2;
    lit1 = Parser::parseLiteral(s_lit1, error1, E1);
    lit2 = Parser::parseLiteral(s_lit2, error2, E2);
    if (error1)
        QMessageBox::warning(this, "Error en el literal 1", getMensajeErrorParser(E1));
    else if (error2)
        QMessageBox::warning(this, "Error en el literal 2", getMensajeErrorParser(E2));
    else {
        Sustitucion s;
        m_ui->textoUnificacion->clear();
        if (lit1->unificar(*lit2, s)) {
            m_ui->textoUnificacion->insertHtml("<b>Resultado de la unificacion</b><br>");
            m_ui->textoUnificacion->insertHtml(lit1->getString().c_str());
            m_ui->textoUnificacion->insertHtml("<br><b>umg</b><br>");
            m_ui->textoUnificacion->insertHtml(s.getString().c_str());
        } else
            m_ui->textoUnificacion->insertHtml("<b>Los literales no son unificables</b>");
    }
    delete lit1;
    delete lit2;
}
