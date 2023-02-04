#include "systemdeviceswindow.h"
#include "ui_systemdeviceswindow.h"

#include "menuwindow.h"
extern MenuWindow *mnw;
extern LoginDialog *ld;
extern user c_user;

extern QList <slave*> papis_slaves;
extern SlaveSyncThread *slaveSyncThread;
extern int inactive;
extern  slave_tasks_t    slave_task;
extern MainWindow *mw;
extern QString base_dir;

#define OP_ON           1
#define OP_OFF          0
#define PA_SWITCH       6

SystemDevicesWindow::SystemDevicesWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SystemDevicesWindow)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose,true);
    render_table();

    ui->tableView->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
    ui->tableView->scrollToTop();
    highlight_row(selectedIndex);
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
    ui->label_datetime->setText(QDateTime::currentDateTime().toString("ddd MMMM d yy hh:mm"));
    timer->start(60000);
    update();
}

void SystemDevicesWindow::showTime()
{
    ui->label_datetime->setText(QDateTime::currentDateTime().toString("ddd MMMM d yy hh:mm"));
    update();
}

void SystemDevicesWindow::highlight_row(int row)
{
    selectedIndex=row;
    QModelIndex index = ui->tableView->model()->index(row, 0);
    ui->tableView->setCurrentIndex(index);
}

SystemDevicesWindow::~SystemDevicesWindow()
{
    delete ui;
}

void SystemDevicesWindow::render_table()
{
    w_log(c_user.name+",Opened System Devices");
    QStringList labels = {"HW ID","Description","Address","Conn Mode","Installed","Activity"};

    dModel = new QStandardItemModel(papis_slaves.count(),5);
    dModel->setHorizontalHeaderLabels(labels);

    for (int row = 0; row < dModel->rowCount(); ++row) {

        QStandardItem *item_hw_id = new QStandardItem(papis_slaves.at(row)->hw_id);
        dModel->setItem(row,0,item_hw_id);

        QStandardItem *item_device_type = new QStandardItem(QString(papis_slaves.at(row)->device_type_desc));
        dModel->setItem(row,1,item_device_type);

        if(papis_slaves.at(row)->conn_mode=="ETH")
        {
            QStandardItem *item_ip_addr = new QStandardItem(papis_slaves.at(row)->ip_addr);
            dModel->setItem(row,2,item_ip_addr);
        }
        else
        {
            QStandardItem *item_rs485_addr = new QStandardItem(QString::number(papis_slaves.at(row)->rs485_addr));
            dModel->setItem(row,2,item_rs485_addr);
        }

        QStandardItem *item_conn_mode = new QStandardItem(papis_slaves.at(row)->conn_mode);
        dModel->setItem(row,3,item_conn_mode);

        QStandardItem *item_installedStatus = new QStandardItem(papis_slaves.at(row)->installStatus==INSTALLED?"YES":"NO");
        dModel->setItem(row,4,item_installedStatus);

        QStandardItem *item_activeStatus = new QStandardItem(papis_slaves.at(row)->activeStatus==ACTIVE?"YES":"NO");
        dModel->setItem(row,5,item_activeStatus);
    }

    ui->tableView->setModel(dModel);
}


void SystemDevicesWindow::slave_scan_progress(int pc)
{
    pd->setValue(pc);
    if(pc==100)
    {
        ui->pushButton_10->setEnabled(true);
        ui->pushButton_7->setEnabled(true);
        ui->pushButton_11->setEnabled(true);
        ui->pushButton_14->setEnabled(true);


        w_log(c_user.name+",Slave Scan Completed");
        mw->render_dashboard();
        pd->close();
        render_table();
        update();
        disconnect(slaveSyncThread,SIGNAL(slave_scan_progress(int)),this,SLOT(slave_scan_progress(int)));

        if(test_all)
        {
            test_all=false;
            DeviceTestDialog *eDialog = new DeviceTestDialog();
            eDialog->setWindowFlags(Qt::FramelessWindowHint);
            eDialog->setModal(true);
#ifndef LAPTOP
            eDialog->show();
#else
            eDialog->showFullScreen();
#endif
        }
    }
}

void SystemDevicesWindow::on_pushButton_menu_clicked()
{
    this->close();
    mnw = new MenuWindow();
    mnw->setWindowFlags(Qt::FramelessWindowHint);
    mnw->showFullScreen();
}

void SystemDevicesWindow::on_tableView_clicked(const QModelIndex &index)
{
    if(papis_slaves.at(index.row())->installStatus==INSTALLED)
    {
        ui->pushButton_7->setText("UNINSTALL");
    }
    else
    {
        ui->pushButton_7->setText("INSTALL");
    }

    selectedIndex = index.row();
    update();
}

void SystemDevicesWindow::on_pushButton_7_clicked()
{
    if(c_user.access_level==ACCESS_LEVEL_2)
    {
        if(ui->pushButton_7->text()=="INSTALL")
        {
            dCnt=10;
            msgBox = new QMessageBox(this);
            msgBox->setIcon(QMessageBox::Question);
            msgBox->setWindowTitle("Confirm Installation");
            msgBox->setText("Slave Device "+papis_slaves.at(selectedIndex)->device_type_desc+" will be installed. ");
            msgBox->setInformativeText("Closing in "+QString::number(dCnt)+"s");
            msgBox->setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
            popupTimer= new QTimer();
            connect(popupTimer, SIGNAL(timeout()), this, SLOT(closeMessageBox()));
            popupTimer->start(1000);
            msgBox->setModal(true);
            int ret = msgBox->exec();
            update();
            if(ret == QMessageBox::Yes)
            {
                w_log(c_user.name+",Slave "+papis_slaves.at(selectedIndex)->device_type_desc+" Installed");
                papis_slaves.at(selectedIndex)->installStatus=INSTALLED;
                write_config();

                w_log(c_user.name+",Scan Slave "+papis_slaves.at(selectedIndex)->device_type_desc);
                pd = new QProgressDialog("Scanning Slave", "Cancel", 0, 100);
                pd->setWindowFlags(Qt::FramelessWindowHint);
                pd->setModal(true);
                update();
                connect(slaveSyncThread,SIGNAL(slave_scan_progress(int)),this,SLOT(slave_scan_progress(int)));
                slaveSyncThread->setIndex(selectedIndex);
                slave_task = SLAVE_SCAN_IP;
            }
            else
            {
                disconnect(popupTimer, SIGNAL(timeout()), this, SLOT(closeMessageBox()));
                popupTimer->stop();
                msgBox->close();
            }
        }
        else
        {
            dCnt=10;
            msgBox = new QMessageBox(this);
            msgBox->setIcon(QMessageBox::Question);
            msgBox->setWindowTitle("Confirm Uninstallation");
            msgBox->setText("Slave Device "+papis_slaves.at(selectedIndex)->device_type_desc+" will be uninstalled. ");
            msgBox->setInformativeText("Closing in "+QString::number(dCnt)+"s");
            msgBox->setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
            popupTimer= new QTimer();
            connect(popupTimer, SIGNAL(timeout()), this, SLOT(closeMessageBox()));
            popupTimer->start(1000);
            msgBox->setModal(true);
            int ret = msgBox->exec();
            update();
            if(ret == QMessageBox::Yes)
            {
                disconnect(popupTimer, SIGNAL(timeout()), this, SLOT(closeMessageBox()));
                popupTimer->stop();
                msgBox->close();

                w_log(c_user.name+",Slave "+papis_slaves.at(selectedIndex)->device_type_desc+" Uninstalled");
                papis_slaves.at(selectedIndex)->installStatus=NOT_INSTALLED;
                write_config();
                render_table();
            }
            else
            {
                disconnect(popupTimer, SIGNAL(timeout()), this, SLOT(closeMessageBox()));
                popupTimer->stop();
                msgBox->close();
            }
        }
    }
    else
    {
        w_log(c_user.name+",Entered Incorrect Password, Install/Uninstalled access denied!");

        dCnt=10;
        msgBox = new QMessageBox(this);
        msgBox->setIcon(QMessageBox::Warning);
        msgBox->setWindowTitle("Restricted");
        msgBox->setText("Access Restricted for Level-1 User.");
        msgBox->setInformativeText("Closing in "+QString::number(dCnt)+"s");
        msgBox->setStandardButtons(QMessageBox::Ok);
        popupTimer= new QTimer();
        connect(popupTimer, SIGNAL(timeout()), this, SLOT(closeMessageBox()));
        popupTimer->start(1000);
        msgBox->setModal(true);
        int ret = msgBox->exec();
        update();
        if(ret == QMessageBox::Ok)
        {
            disconnect(popupTimer, SIGNAL(timeout()), this, SLOT(closeMessageBox()));
            popupTimer->stop();
            msgBox->close();
        }
    }
}

void SystemDevicesWindow::closeMessageBox()
{
    dCnt--;
    msgBox->setInformativeText("Closing in "+QString::number(dCnt)+"s");
    update();
    if(dCnt==0)
    {
        disconnect(popupTimer, SIGNAL(timeout()), this, SLOT(closeMessageBox()));
        popupTimer->stop();
        msgBox->close();
    }
}


void SystemDevicesWindow::on_pushButton_10_clicked()
{        
    ui->pushButton_10->setDisabled(true);
    ui->pushButton_7->setDisabled(true);
    ui->pushButton_11->setDisabled(true);
    ui->pushButton_14->setDisabled(true);

    test_all=false;
    w_log(c_user.name+",Started Slave Scanning");
    pd = new QProgressDialog("Scanning Slaves", "Cancel", 0, 100);
    pd->setWindowFlags(Qt::FramelessWindowHint);
//    connect(pd, &QProgressDialog::canceled, this, &SystemDevicesWindow::cancel);
    update();
    connect(slaveSyncThread,SIGNAL(slave_scan_progress(int)),this,SLOT(slave_scan_progress(int)));
    slave_task = SLAVE_SCAN_ALL;
}


void SystemDevicesWindow::on_pushButton_12_clicked()
{
    //scroll up
    selectedIndex = scrlIdx;
    if(scrlIdx>0)
    {
        scrlIdx--;
    }
    ui->tableView->verticalScrollBar()->setValue(scrlIdx);
    highlight_row(scrlIdx);
    update();
}

void SystemDevicesWindow::on_pushButton_13_clicked()
{
    //scroll down

    if(scrlIdx<papis_slaves.count()-1)
    {
        scrlIdx++;
    }
    ui->tableView->verticalScrollBar()->setValue(scrlIdx);
    highlight_row(scrlIdx);
    update();
}

void SystemDevicesWindow::on_pushButton_11_clicked()
{
    //test run
    dCnt=10;
    msgBox = new QMessageBox(this);
    msgBox->setIcon(QMessageBox::Question);
    msgBox->setWindowTitle("Slave Test");
    msgBox->setText("Confirm Testing of PA System.");
    msgBox->setInformativeText("Closing in "+QString::number(dCnt)+"s");
    msgBox->setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    popupTimer= new QTimer();
    connect(popupTimer, SIGNAL(timeout()), this, SLOT(closeMessageBox()));
    popupTimer->start(1000);
    msgBox->setModal(true);
    int ret = msgBox->exec();
    update();
    if(ret == QMessageBox::Yes)
    {
        test_pa();
        disconnect(popupTimer, SIGNAL(timeout()), this, SLOT(closeMessageBox()));
        popupTimer->stop();
        msgBox->close();
    }
    else
    {
        w_log(c_user.name+",PA System Test Cancelled");
        disconnect(popupTimer, SIGNAL(timeout()), this, SLOT(closeMessageBox()));
        popupTimer->stop();
        msgBox->close();
    }
}

void SystemDevicesWindow::test_pa()
{
    w_log(c_user.name+",Tested PA System");
    gpio.gpio_set_value(PA_SWITCH,OP_ON);
    system(QString("gst-launch-1.0 playbin uri=file://"+base_dir+"sounds/chime.mp3").toLocal8Bit());
    system(QString("gst-launch-1.0 playbin uri=file://"+base_dir+"sounds/w_msg_hi.mp3").toLocal8Bit());
    system(QString("gst-launch-1.0 playbin uri=file://"+base_dir+"sounds/w_msg_en.mp3").toLocal8Bit());
    gpio.gpio_set_value(PA_SWITCH,OP_OFF);
}

void SystemDevicesWindow::on_pushButton_14_clicked()
{
    //test run       

    dCnt=10;
    msgBox = new QMessageBox(this);
    msgBox->setIcon(QMessageBox::Question);
    msgBox->setWindowTitle("Slave Test");
    msgBox->setText("Confirm Testing of All Slave Devices");
    msgBox->setInformativeText("Closing in "+QString::number(dCnt)+"s");
    msgBox->setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    popupTimer= new QTimer();
    connect(popupTimer, SIGNAL(timeout()), this, SLOT(closeMessageBox()));
    popupTimer->start(1000);
    msgBox->setModal(true);
    int ret = msgBox->exec();
    update();
    if(ret == QMessageBox::Yes)
    {
        disconnect(popupTimer, SIGNAL(timeout()), this, SLOT(closeMessageBox()));
        popupTimer->stop();
        msgBox->close();

        w_log(c_user.name+",Started Slave Scanning");
        test_all=true;
        pd = new QProgressDialog("Scanning Slaves", "Cancel", 0, 100);
        pd->setWindowFlags(Qt::FramelessWindowHint);
    //    connect(pd, &QProgressDialog::canceled, this, &SystemDevicesWindow::cancel);
        update();
        connect(slaveSyncThread,SIGNAL(slave_scan_progress(int)),this,SLOT(slave_scan_progress(int)));
        slave_task = SLAVE_SCAN_ALL;
    }
    else
    {
        w_log(c_user.name+",All Slave Test Cancelled");
        disconnect(popupTimer, SIGNAL(timeout()), this, SLOT(closeMessageBox()));
        popupTimer->stop();
        msgBox->close();
    }
}

void SystemDevicesWindow::on_pushButton_16_clicked()
{
    //scroll top
    if(scrlIdx>0)
    {
        scrlIdx=0;
    }
    ui->tableView->verticalScrollBar()->setValue(scrlIdx);
    highlight_row(scrlIdx);
    update();
}

void SystemDevicesWindow::on_pushButton_18_clicked()
{
    //scoll bottom
    if(scrlIdx<papis_slaves.count())
    {
        scrlIdx=papis_slaves.count()-1;
    }

    ui->tableView->verticalScrollBar()->setValue(scrlIdx);
    highlight_row(scrlIdx);
    update();
}
