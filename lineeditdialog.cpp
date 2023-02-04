#include "lineeditdialog.h"
#include "ui_lineeditdialog.h"

LineEditDialog::LineEditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LineEditDialog)
{
    ui->setupUi(this);

    le = new customle(this);
    connect(le,SIGNAL(entered_text(QString)),this,SLOT(entered(QString)));
    le->setStyleSheet("background-color: rgb(255, 255, 255); color: rgb(0, 0, 0);");
    ui->horizontalLayout->addWidget(le);
    update();
}

LineEditDialog::~LineEditDialog()
{
    delete ui;
}

void LineEditDialog::setEchoMode(QLineEdit::EchoMode echomode)
{
    le->setEchoMode(echomode);
}

void LineEditDialog::setLabel(QString string)
{
    ui->label->setText(string);
}

void LineEditDialog::setLabel2(QString string)
{
    ui->label_2->setText(string);
}

void LineEditDialog::entered(QString string)
{
    str=string;
}

void LineEditDialog::on_buttonBox_accepted()
{
    emit text_entered(str);
}
