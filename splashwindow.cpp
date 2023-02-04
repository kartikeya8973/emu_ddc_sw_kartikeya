#include "splashwindow.h"
#include "ui_splashwindow.h"

#include <QNetworkInterface>
#include <QFont>
#include <QFontDatabase>

SplashWindow::SplashWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SplashWindow)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose,true);

    statusBar()->showMessage("Initializing System");

    //set font
    int id = QFontDatabase::addApplicationFont(":/fonts/fSakalBharati_N_Ship.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont fontSB(family);

    fontSB.setPointSize(10);
    ui->label_pwr->setFont(fontSB);
    ui->label_ver->setFont(fontSB);
    ui->label_hwid->setFont(fontSB);

    fontSB.setPointSize(28);
    fontSB.setBold(true);
    ui->label_hin->setFont(fontSB);
    ui->label_eng->setFont(fontSB);

    fontSB.setPointSize(10);
    ui->label_hwidval->setFont(fontSB);
    ui->label_verval->setFont(fontSB);

    //test network
    QTextStream out(stdout);
    QList<QNetworkInterface> allInterfaces = QNetworkInterface::allInterfaces();
    QNetworkInterface eth;

    foreach(eth, allInterfaces) {
        QList<QNetworkAddressEntry> allEntries = eth.addressEntries();
        QNetworkAddressEntry entry;
        foreach (entry, allEntries) {
            out << entry.ip().toString() << "/" << entry.netmask().toString();
        }
    }

    //test connectivity

    //wait

    //initialize main ui
    statusBar()->showMessage("System IP: ");
    statusBar()->showMessage("Searching Peripherals");

//    system("gst-launch-1.0 filesrc location=/home/root/welcome.wav ! wavparse ! audioconvert ! audioresample ! alsasink");

    ui->pushButton_menu->setVisible(false);

//    //start windows
//    mw = new MainWindow();
//    mw->setWindowFlags(Qt::FramelessWindowHint);
//    mw->showFullScreen();

//    LoginDialog *loginDialog = new LoginDialog();
//    loginDialog->setWindowFlags(Qt::FramelessWindowHint);
//    loginDialog->showFullScreen();
}

void SplashWindow::setError()
{
    ui->label_eng->setText("WELCOME TO INDIAN RAILWAYS");
    ui->label_hin->setText("");
    ui->pushButton_menu->setVisible(true);
}

SplashWindow::~SplashWindow()
{
    delete ui;
}

void SplashWindow::on_pushButton_menu_clicked()
{
    this->close();
}

void SplashWindow::update_status(QString message)
{
    ui->statusbar->showMessage(message);
}

void SplashWindow::close_window()
{
    this->close();
}
