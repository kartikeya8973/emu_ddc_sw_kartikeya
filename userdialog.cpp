#include "userdialog.h"
#include "ui_userdialog.h"

UserDialog::UserDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserDialog)
{
    ui->setupUi(this);
    le_un = new customle(this);
    le_pw = new customle(this);
    le_un->setStyleSheet("background-color: rgb(255, 255, 255); color: rgb(0, 0, 0);");
    le_pw->setStyleSheet("background-color: rgb(255, 255, 255); color: rgb(0, 0, 0);");

    ui->horizontalLayout_un->addWidget(le_un);
    ui->horizontalLayout_pw->addWidget(le_pw);

    connect(le_un,SIGNAL(entered_text(QString)),this,SLOT(entered_un(QString)));
    connect(le_pw,SIGNAL(entered_text(QString)),this,SLOT(entered_pw(QString)));
}

UserDialog::~UserDialog()
{
    delete ui;
}

void UserDialog::on_buttonBox_accepted()
{
    usr.access_level=(access_level_t)ui->comboBox_al->currentIndex();
    usr.activeStatus=(activeStatus_t)ui->comboBox_ac->currentIndex();
    emit user_entered(usr);
    disconnect(le_un,SIGNAL(entered_text(QString)),this,SLOT(entered_un(QString)));
    disconnect(le_pw,SIGNAL(entered_text(QString)),this,SLOT(entered_pw(QString)));
}

void UserDialog::entered_un(QString str)
{
    usr.name=str;
    le_un->setText(usr.name);
}

void UserDialog::entered_pw(QString str)
{
    usr.password=str;
    le_pw->setText(usr.password);
}

void UserDialog::setUser(user u)
{
    usr=u;
    le_un->setText(usr.name);
    le_pw->setText(usr.password);
    ui->comboBox_al->setCurrentIndex((int)usr.access_level);
    ui->comboBox_ac->setCurrentIndex((int)usr.activeStatus);
}

void UserDialog::on_buttonBox_rejected()
{
    disconnect(le_un,SIGNAL(entered_text(QString)),this,SLOT(entered_un(QString)));
    disconnect(le_pw,SIGNAL(entered_text(QString)),this,SLOT(entered_pw(QString)));
}

void UserDialog::setTitle(QString str)
{
    ui->label_title->setText(str);
}
