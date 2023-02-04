#include "systeminfowindow.h"
#include "ui_systeminfowindow.h"

#include "menuwindow.h"
extern MenuWindow *mnw;
extern LoginDialog *ld;
extern user c_user;

SystemInfoWindow::SystemInfoWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SystemInfoWindow)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose,true);
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
    ui->label_datetime->setText(QDateTime::currentDateTime().toString("ddd MMMM d yy hh:mm"));
    timer->start(60000);
    update();
}

void SystemInfoWindow::showTime()
{
    ui->label_datetime->setText(QDateTime::currentDateTime().toString("ddd MMMM d yy hh:mm"));
    update();
}

SystemInfoWindow::~SystemInfoWindow()
{
    delete ui;
}

void SystemInfoWindow::on_pushButton_menu_clicked()
{
    w_log(c_user.name+",Closed System Info");
    this->close();
    mnw = new MenuWindow();
    mnw->setWindowFlags(Qt::FramelessWindowHint);
    mnw->showFullScreen();
}
