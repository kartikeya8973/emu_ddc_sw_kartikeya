#include "logindialog.h"
#include "ui_logindialog.h"
#include "common.h"

extern access_level_t access_level;
extern MainWindow *mw;
extern MenuWindow *mnw;

extern QList <user> users;
extern user c_user;

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose,true);    

    le = new customle(this);
    le->setStyleSheet("background-color: rgb(255, 255, 255); color: rgb(0, 0, 0);");
    le->setEchoMode(QLineEdit::Password);
    ui->horizontalLayout->addWidget(le);

    w_log(c_user.name+",Attempted login.");

    reload();
    update();
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_buttonBox_accepted()
{
    if(users.at(ui->comboBox->currentIndex()).password == le->text())
    {
        c_user = users.at(ui->comboBox->currentIndex());
        if(!skip)
        {
            mnw->showFullScreen();
        }
        else
        {
            w_log(c_user.name+",Acknowledged Toilet Alarm");
        }
        w_log(c_user.name+",Password Accepted");
        emit pass();
        this->close();
    }
    else
    {
        w_log(c_user.name+",Password Incorrect.");
        QMessageBox::critical(this, tr("Error "),
                                        "Password Entered in Incorrect, Pls retry!", QMessageBox::Ok);
    }

//    this->close();
    update();
}

void LoginDialog::set_title(QString title)
{
    ui->label_2->setText(title);
}

void LoginDialog::setskip(bool skp)
{
    skip=skp;
}

void LoginDialog::on_buttonBox_rejected()
{
    w_log(",Login Cancelled");
    this->close();
}

void LoginDialog::reload()
{
    if(users.count()>0)
    {
        for(user usr : users)
        {
            ui->comboBox->addItem(usr.name+" : "+getlevel(usr.access_level));
        }
    }
    else
    {
        w_log(",,Fail to login, system has no users installed");
        w_fault(",,Failed login: System has no users installed");
        QMessageBox::critical(this, tr("Error "),
                                        "System has no users installed", QMessageBox::Ok);
    }
}

QString LoginDialog::getlevel(access_level_t access_level)
{
    switch(access_level)
    {
    case ACCESS_LEVEL_1:
        return QString("LEVEL-1");
        break;
    case ACCESS_LEVEL_2:
        return QString("LEVEL-2");
        break;
    case NO_ACCESS:
        return QString("NO ACCESS");
        break;
    }
}
