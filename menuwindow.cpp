#include "menuwindow.h"
#include "ui_menuwindow.h"

extern MainWindow *mw;
extern LoginDialog *ld;
extern SecurityWindow *sw;
extern ServiceDataWindow *sdw;
extern MaintenanceWindow *mtw;
extern SystemDevicesWindow *sdvw;
extern SystemInfoWindow *siw;
extern SystemSettingsWindow *ssw;
extern user c_user;
extern bool route_ongoing;

MenuWindow::MenuWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MenuWindow)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose,true);
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
    ui->label_datetime->setText(QDateTime::currentDateTime().toString("ddd MMMM d yy hh:mm"));
    timer->start(60000);
    update();

    ui->pushButton_start->setDisabled(route_ongoing);
    ui->pushButton_stop->setDisabled(!route_ongoing);
}

void MenuWindow::route_ops(bool ongoing)
{
    ui->pushButton_start->setDisabled(ongoing);
    ui->pushButton_stop->setDisabled(!ongoing);
}

void MenuWindow::showTime()
{
    ui->label_datetime->setText(QDateTime::currentDateTime().toString("ddd MMMM d yy hh:mm"));
    update();
}

MenuWindow::~MenuWindow()
{
    delete ui;
}

void MenuWindow::on_pushButton_menu_clicked()
{
    w_log(c_user.name+",Closed system menu");
    this->close();
    mw->showFullScreen();
    update();
}

void MenuWindow::on_pushButton_sysinfo_clicked()
{
    if(c_user.access_level==ACCESS_LEVEL_2)
    {
        w_log(c_user.name+",Opened System Info");
        siw = new SystemInfoWindow();
        this->close();
        siw->showFullScreen();
        update();
    }
    else
    {
        w_log(c_user.name+",Tried to access System Info, denied!");
        QMessageBox::information(this, tr("Restricted"),
        "Access Restricted for Level-1 User.", QMessageBox::Ok);
    }
}

void MenuWindow::on_pushButton_devices_clicked()
{
    w_log(c_user.name+",Opened System Devices");
    sdvw = new SystemDevicesWindow();
    this->close();
    sdvw->showFullScreen();
}

void MenuWindow::on_pushButton_service_data_clicked()
{
    w_log(c_user.name+",Opened Service Data (Routes)");
    sdw = new ServiceDataWindow();
    this->close();
    sdw->showFullScreen();
    update();
}

void MenuWindow::on_pushButton_settings_clicked()
{
    if(c_user.access_level==ACCESS_LEVEL_2)
    {
        w_log(c_user.name+",Opened System Settings");
        ssw = new SystemSettingsWindow();
        this->close();
        ssw->showFullScreen();
        update();
    }
    else
    {
        w_log(c_user.name+",Tried to access System Settings, denied!");
        QMessageBox::information(this, tr("Restricted"),
        "Access Restricted for Level-1 User.", QMessageBox::Ok);
    }
}

void MenuWindow::on_pushButton_maintenance_clicked()
{    
    if(c_user.access_level==ACCESS_LEVEL_2)
    {
        w_log(c_user.name+",Opened System Logs");
        mtw = new MaintenanceWindow();
        this->close();
        mtw->showFullScreen();
        update();
    }
    else
    {
        w_log(c_user.name+",Tried to access System Logs, denied!");
        QMessageBox::information(this, tr("Restricted"),
        "Access Restricted for Level-1 User.", QMessageBox::Ok);
    }
}

void MenuWindow::on_pushButton_security_clicked()
{    
    if(c_user.access_level==ACCESS_LEVEL_2)
    {
        w_log(c_user.name+",Opened System Maintenance");
        sw = new SecurityWindow();
        this->close();
        sw->showFullScreen();
        update();
    }
    else
    {
        w_log(c_user.name+",Tried to access System Maitenance, denied!");
        QMessageBox::information(this, tr("Restricted"),
        "Access Restricted for Level-1 User.", QMessageBox::Ok);
    }
}

void MenuWindow::on_pushButton_menu_3_clicked()
{    
    if(c_user.access_level==ACCESS_LEVEL_2)
    {
        //restart        
        system_menu_op=SYSTEM_RESTART;
        verify_password();
    }
    else
    {
        w_log(c_user.name+",Tried to reboot System, denied!");
        QMessageBox::information(this, tr("Restricted"),
        "Access Restricted for Level-1 User.", QMessageBox::Ok);
    }
}

void MenuWindow::on_pushButton_menu_2_clicked()
{    
    if(c_user.access_level==ACCESS_LEVEL_2)
    {
        //shutdown        
        system_menu_op=SYSTEM_POWEROFF;
        verify_password();
    }
    else
    {
        w_log(c_user.name+",Tried to Shutdown System, denied!");
        QMessageBox::information(this, tr("Restricted"),
        "Access Restricted for Level-1 User.", QMessageBox::Ok);
    }
}

bool MenuWindow::verify_password()
{
    passwordDialog = new LineEditDialog();
    passwordDialog->setModal(true);
    passwordDialog->setWindowFlags(Qt::FramelessWindowHint);
    passwordDialog->setLabel("Enter Password");
    passwordDialog->setEchoMode(QLineEdit::Password);
    passwordDialog->show();
    connect(passwordDialog,SIGNAL(text_entered(QString)),this,SLOT(receive_password(QString)));
}

void MenuWindow::receive_password(QString password)
{
    disconnect(passwordDialog,SIGNAL(text_entered(QString)),this,SLOT(receive_password(QString)));
    if(c_user.password!=password)
    {
        w_log(c_user.name+",Entered incorrect password, denied!");
        QMessageBox::warning(this, tr("Error"),
            "Password entered is incorrect, Pls try again!",
            QMessageBox::Ok);
    }
    else
    {
        w_log(c_user.name+",Entered correct password");
        switch(system_menu_op)
        {
        case SYSTEM_POWEROFF:
        {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, tr("Confirm Shutdown"),
                                            "Please confirm if you wish to shutdown the MPU",
                                            QMessageBox::Yes | QMessageBox::Cancel);
            if (reply == QMessageBox::Yes)
            {
                w_log(c_user.name+",System Shutdown");
                system(QString("shutdown -r").toLocal8Bit());
            }
        }
            break;
        case SYSTEM_RESTART:
        {
            QMessageBox::StandardButton reply;

            reply = QMessageBox::question(this, tr("Confirm Reboot"),
                                            "Please confirm if you wish to reboot the MPU",
                                            QMessageBox::Yes | QMessageBox::Cancel);
            if (reply == QMessageBox::Yes)
            {
                w_log(c_user.name+",Rebooted System");
                system(QString("reboot now").toLocal8Bit());
            }
        }
            break;

        case START_ROUTE:
        {
            w_log(c_user.name+",Opened Route Execution Dialog");
            emit start_route();
            this->close();
        }
            break;
        case STOP_ROUTE:
        {
            w_log(c_user.name+",Stopped Route Execution");
            emit stop_route();
        }
            break;
        }
    }
}

void MenuWindow::on_pushButton_start_clicked()
{
    //start
    system_menu_op=START_ROUTE;
    verify_password();
}

void MenuWindow::on_pushButton_stop_clicked()
{
    //stop
    system_menu_op=STOP_ROUTE;
    verify_password();
}
