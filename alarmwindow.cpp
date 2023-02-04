#include "alarmwindow.h"
#include "ui_alarmwindow.h"

extern user c_user;

AlarmWindow::AlarmWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AlarmWindow)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose,true);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));
    timer->start(500);

    anTimer = new QTimer(this);
    connect(anTimer, SIGNAL(timeout()),this, SLOT(timeout_announcement()));
//    system(QString("gst-launch-1.0 playbin uri=file:///home/root/papis/sounds/attention.mp3 &").toLocal8Bit());
    anTimer->start(15000);

}

void AlarmWindow::timeout_announcement()
{
    system(QString("gst-launch-1.0 playbin uri=file:///home/root/papis/sounds/attention.mp3 &").toLocal8Bit());
}

AlarmWindow::~AlarmWindow()
{
    delete ui;
}

void AlarmWindow::timeout()
{
    ui->label_t1->setVisible(toggle);
    ui->label_t2->setVisible(toggle);
    ui->label_t3->setVisible(toggle);
    ui->label_t4->setVisible(toggle);

    update();
    toggle^=true;
}

void AlarmWindow::on_pushButton_clicked()
{
    ui->pushButton->setDisabled(true);
    ui->label->setText("PLEASE WAIT");
    verify_password();
//    this->close();
}

void AlarmWindow::verify_password()
{
    LoginDialog *loginDialog = new LoginDialog();
    loginDialog->setWindowFlags(Qt::FramelessWindowHint);
    connect(loginDialog,SIGNAL(pass()),this,SLOT(pass()));
    loginDialog->setskip(true);
    loginDialog->show();
}

void AlarmWindow::setNum(QList <int> *list)
{
    ui->pushButton->setEnabled(true);
    for(int idx=0;idx<list->count();idx++)
    {
        switch(list->at(idx))
        {
        case 1:
        {
            num=list->at(idx);
            ui->label_t1->setStyleSheet("image: url(:/images/icons/icon_siren.png);");
            w_log(",,Emergency Alarm Generated from Toilet Unit-1");
        }
        break;
        case 2:
        {
            num=list->at(idx);
            ui->label_t2->setStyleSheet("image: url(:/images/icons/icon_siren.png);");
            w_log(",,Emergency Alarm Generated from Toilet Unit-2");
        }
        break;
        case 3:
        {
            num=list->at(idx);
            ui->label_t3->setStyleSheet("image: url(:/images/icons/icon_siren.png);");
            w_log(",,Emergency Alarm Generated from Toilet Unit-3");
        }
            break;
        case 4:
        {
            num=list->at(idx);
            ui->label_t4->setStyleSheet("image: url(:/images/icons/icon_siren.png);");
            w_log(",,Emergency Alarm Generated from Toilet Unit-4");
        }
            break;
        }
    }
}

void AlarmWindow::pass()
{
    emit acknowledge(num);
    w_log(",,Emergency Alarm Acknowledged by user: "+c_user.name);
}
