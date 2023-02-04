#include "securitywindow.h"
#include "ui_securitywindow.h"

extern MenuWindow *mnw;
extern LoginDialog *ld;
extern user c_user;
extern systemconfig systemconfig;
extern QList <servicerecord*> sRecords;
extern QString base_dir;

SecurityWindow::SecurityWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SecurityWindow)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose,true);

    ui->stackedWidget->setCurrentIndex(0);
    render_table();
//    render_fs();
    render_records();
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
    ui->label_datetime->setText(QDateTime::currentDateTime().toString("ddd MMMM d yy hh:mm"));
    timer->start(60000);
    update();
}

void SecurityWindow::showTime()
{
    ui->label_datetime->setText(QDateTime::currentDateTime().toString("ddd MMMM d yy hh:mm"));
    update();
}

SecurityWindow::~SecurityWindow()
{
    delete ui;
}

void SecurityWindow::render_fs()
{
//    QFileSystemModel *model = new QFileSystemModel(this);
//    model->setRootPath(QString("/"));
//    ui->treeView_fs->setModel(model);
//    ui->treeView_fs->setRootIndex(model->setRootPath(QString("/")));
}

void SecurityWindow::render_table()
{
    QStringList labels = {"Device","Component","Operation"};

    sModel = new QStandardItemModel(17,3);
    sModel->setHorizontalHeaderLabels(labels);

    sModel->setItem(0,0,new QStandardItem("MPU"));
    sModel->setItem(0,1,new QStandardItem("Software"));
    sModel->setItem(0,2,new QStandardItem("Update from USB"));

    sModel->setItem(1,0,new QStandardItem("MPU"));
    sModel->setItem(1,1,new QStandardItem("Software"));
    sModel->setItem(1,2,new QStandardItem("Update from BMU"));

    sModel->setItem(2,0,new QStandardItem("MPU"));
    sModel->setItem(2,1,new QStandardItem("Routes"));
    sModel->setItem(2,2,new QStandardItem("Import from USB"));

    sModel->setItem(3,0,new QStandardItem("MPU"));
    sModel->setItem(3,1,new QStandardItem("Routes"));
    sModel->setItem(3,2,new QStandardItem("Download from BMU"));

    sModel->setItem(4,0,new QStandardItem("MPU"));
    sModel->setItem(4,1,new QStandardItem("Settings"));
    sModel->setItem(4,2,new QStandardItem("Import from USB"));

    sModel->setItem(5,0,new QStandardItem("MPU"));
    sModel->setItem(5,1,new QStandardItem("Settings"));
    sModel->setItem(5,2,new QStandardItem("Download from BMU"));

    sModel->setItem(6,0,new QStandardItem("MPU"));
    sModel->setItem(6,1,new QStandardItem("Logs"));
    sModel->setItem(6,2,new QStandardItem("Export to USB"));

    sModel->setItem(7,0,new QStandardItem("MPU"));
    sModel->setItem(7,1,new QStandardItem("Faults"));
    sModel->setItem(7,2,new QStandardItem("Export to USB"));

    sModel->setItem(8,0,new QStandardItem("MPU"));
    sModel->setItem(8,1,new QStandardItem("Running Data"));
    sModel->setItem(8,2,new QStandardItem("Export to USB"));

    sModel->setItem(9,0,new QStandardItem("MPU"));
    sModel->setItem(9,1,new QStandardItem("Logs"));
    sModel->setItem(9,2,new QStandardItem("Upload to BMU"));

    sModel->setItem(10,0,new QStandardItem("MPU"));
    sModel->setItem(10,1,new QStandardItem("Faults"));
    sModel->setItem(10,2,new QStandardItem("Upload to BMU"));

    sModel->setItem(11,0,new QStandardItem("MPU"));
    sModel->setItem(11,1,new QStandardItem("Running Data"));
    sModel->setItem(11,2,new QStandardItem("Upload to BMU"));
    ui->tableView->setModel(sModel);

    update();
}

void SecurityWindow::render_records()
{
     QStringList labels = {"Operation","Source","Issued On","Issued by","Status"};
     QString o_p;

     rModel = new QStandardItemModel(sRecords.count(),labels.count());
     rModel->setHorizontalHeaderLabels(labels);

     for (int row = 0; row < rModel->rowCount(); ++row)
     {
         switch(sRecords.at(row)->op)
         {
         case UPDATE_MPU_USB:
             o_p="UPDATE_MPU_USB";
             break;
         case UPDATE_MPU_BMU:
             o_p="UPDATE_MPU_USB";
             break;
         case IMPORT_ROUTES_USB:
             o_p="IMPORT_ROUTES_USB";
             break;
         case IMPORT_ROUTES_BMU:
             o_p="IMPORT_ROUTES_BMU";
             break;
         case IMPORT_SETTINGS_USB:
             o_p="IMPORT_SETTINGS_USB";
             break;
         case IMPORT_SETTINGS_BMU:
             o_p="IMPORT_SETTINGS_BMU";
             break;
         case EXPORT_LOGS_USB:
             o_p="EXPORT_LOGS_USB";
             break;
         case EXPORT_FAULTS_USB:
             o_p="EXPORT_FAULTS_USB";
             break;
         case EXPORT_RUNNING_USB:
             o_p="EXPORT_RUNNING_USB";
             break;
         case EXPORT_LOGS_BMU:
             o_p="EXPORT_LOGS_BMU";
             break;
         case EXPORT_FAULTS_BMU:
             o_p="EXPORT_FAULTS_BMU";
             break;
         case EXPORT_RUNNING_BMU:
             o_p="EXPORT_RUNNING_BMU";
             break;
         }

         QStandardItem *operation = new QStandardItem(o_p);
         rModel->setItem(row,0,operation);

         QStandardItem *source = new QStandardItem(sRecords.at(row)->issue_src);
         rModel->setItem(row,1,source);

         QStandardItem *issue_on = new QStandardItem(sRecords.at(row)->issue_dt.toString("dd.MM.yy hh:mm:ss"));
         rModel->setItem(row,2,issue_on);

         QStandardItem *issue_by = new QStandardItem(sRecords.at(row)->user);
         rModel->setItem(row,3,issue_by);

         QStandardItem *stat = new QStandardItem(sRecords.at(row)->status);
         rModel->setItem(row,4,stat);
     }

     ui->tableView_2->setModel(rModel);
     ui->tableView_2->setColumnWidth(0,150);
     ui->tableView_2->setColumnWidth(1,100);
     ui->tableView_2->setColumnWidth(2,150);
     ui->tableView_2->setColumnWidth(3,100);
     ui->tableView_2->setColumnWidth(4,150);

     update();
}

void SecurityWindow::on_pushButton_menu_clicked()
{
    if(mask_close)
    {
        dCnt=3;
        msgBox = new QMessageBox(this);
        msgBox->setIcon(QMessageBox::Information);
        msgBox->setWindowTitle("Download/Upload in Process");
        msgBox->setText("Please wait for File  Transfer to end");
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
    else
    {
        this->close();
        mnw = new MenuWindow();
        mnw->setWindowFlags(Qt::FramelessWindowHint);
        mnw->showFullScreen();
        update();
    }
}

void SecurityWindow::on_pushButton_13_clicked()
{
    //scroll up
    if(scrlIdx>0)
    {
        scrlIdx--;
    }
    ui->tableView->verticalScrollBar()->setValue(scrlIdx);
    update();
}

void SecurityWindow::on_pushButton_14_clicked()
{
    //scroll down
    if(scrlIdx<17)
    {
        scrlIdx++;
    }
    ui->tableView->verticalScrollBar()->setValue(scrlIdx);
    update();
}

void SecurityWindow::popupTimeout()
{
    dCnt--;
    msgBox->setInformativeText("Closing in "+QString::number(dCnt)+"s");
    update();
    if(dCnt==0)
    {
        msgBox->close();
        disconnect(popupTimer, SIGNAL(timeout()), this, SLOT(popupTimeout()));
        popupTimer->stop();
        update();
    }
}

void SecurityWindow::on_pushButton_7_clicked()
{    
    switch(op)
    {
    case UPDATE_MPU_USB:
    case IMPORT_ROUTES_USB:
    case IMPORT_SETTINGS_USB:
    case EXPORT_LOGS_USB:
    case EXPORT_FAULTS_USB:
    case EXPORT_RUNNING_USB:
    {
        if(u1_mnt||u2_mnt)
        {
            verify_password();
        }
        else
        {
            msgBox = new QMessageBox(this);
            msgBox->setIcon(QMessageBox::NoIcon);
            msgBox->setWindowTitle("USB Not Mounted");
            dCnt=30;
            msgBox->setText("Please Mount USB first, using buttons below");
            msgBox->setInformativeText("Closing in "+QString::number(dCnt)+"s");
            msgBox->setStandardButtons(QMessageBox::Ok);
            popupTimer= new QTimer();
            connect(popupTimer, SIGNAL(timeout()), this, SLOT(popupTimeout()));
            popupTimer->start(1000);
            msgBox->setModal(true);
            int ret = msgBox->exec();
            update();
            if(ret == QMessageBox::Ok)
            {

            }
        }
    }
        break;
    case UPDATE_MPU_BMU:
    case IMPORT_ROUTES_BMU:
    case IMPORT_SETTINGS_BMU:
    case EXPORT_FAULTS_BMU:
    case EXPORT_RUNNING_BMU:
    case EXPORT_LOGS_BMU:
        verify_password();
        break;
    }
}

void SecurityWindow::verify_password()
{
    passwordDialog = new LineEditDialog();
    passwordDialog->setModal(true);
    passwordDialog->setWindowFlags(Qt::FramelessWindowHint);
    passwordDialog->setLabel("Enter Password");
    passwordDialog->setEchoMode(QLineEdit::Password);
#ifndef LAPTOP
    passwordDialog->show();
#else
    passwordDialog->showFullScreen();
#endif
    connect(passwordDialog,SIGNAL(text_entered(QString)),this,SLOT(receive_password(QString)));
}

void SecurityWindow::download_message(QString message)
{
    mask_close=false;
    disconnect(dManager,SIGNAL(message(QString)),this,SLOT(download_message(QString)));
    update();
    dCnt=30;
    msgBox = new QMessageBox(this);
    msgBox->setIcon(QMessageBox::Information);
    msgBox->setWindowTitle("FTP Download");
    msgBox->setText(message);
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

void SecurityWindow::upload_message(QString message)
{
    mask_close=false;
    disconnect(dManager,SIGNAL(message(QString)),this,SLOT(upload_message(QString)));
    update();
    dCnt=30;
    msgBox = new QMessageBox(this);
    msgBox->setIcon(QMessageBox::Information);
    msgBox->setWindowTitle("FTP Upload");
    msgBox->setText(message);
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

void SecurityWindow::wait_prompt()
{
    waitPrompt = new QMessageBox(this);
    waitPrompt->setIcon(QMessageBox::Warning);
    waitPrompt->setWindowTitle("FTP Download");
    waitPrompt->setText("Please wait while the file is being downloaded");
    waitPrompt->setStandardButtons(QMessageBox::NoButton/*Cancel*/);
    waitPrompt->setModal(true);
    waitPrompt->exec();
    update();
}

void SecurityWindow::receive_password(QString password)
{
    disconnect(passwordDialog,SIGNAL(text_entered(QString)),this,SLOT(receive_password(QString)));
    if(c_user.password!=password)
    {
        dCnt=10;
        msgBox = new QMessageBox(this);
        msgBox->setIcon(QMessageBox::Question);
        msgBox->setWindowTitle("Error");
        msgBox->setText("Password entered is incorrect, Pls try again!");
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
    else
    {
        //create entry
        servicerecord *sr = new servicerecord(sRecords.count(),op,"MPU",c_user.name,"Completed");
        sRecords.append(sr);
        write_services();
        render_records();

        switch(op)
        {
        case UPDATE_MPU_USB:
        {
            w_log(c_user.name+",Copying Upd file from USB Drive");
            system(QString("rm -r /home/root/papis/update/*").toLocal8Bit());
            system(QString("cp /mnt/sdn/mpu.upd  /home/root/papis/update/").toLocal8Bit());
            w_log(c_user.name+",File Copied, System will be updated after reboot");

            msgBox = new QMessageBox(this);
            msgBox->setIcon(QMessageBox::Information);
            msgBox->setWindowTitle("USB Update");
            msgBox->setText("Update Done! Press OK to restart your System");
            msgBox->setStandardButtons(QMessageBox::Ok);
            msgBox->setModal(true);
            int ret = msgBox->exec();
            update();
            if(ret == QMessageBox::Ok)
            {
                w_log(c_user.name+",Rebooting System");
                system(QString("reboot now").toLocal8Bit());
            }
        }
            break;
        case UPDATE_MPU_BMU:
        {
            mask_close=true;
            QUrl url("ftp://"+systemconfig.bmu_ftp_user+":"+
                     systemconfig.bmu_ftp_pswd+"@"+systemconfig.bmu_ftp_ip+
                     "/"+systemconfig.mpu_id+"/export/mpu.upd");

            dManager = new DownloadManager();
            connect(dManager,SIGNAL(message(QString)),this,SLOT(download_message(QString)));
            dManager->doDownload(url,base_dir+"update/mpu.upd");
        }
            break;
        case IMPORT_ROUTES_USB:
        {
            system(QString("cp -r /mnt/sdn/*"+base_dir+"routes/").toLocal8Bit());
            w_log(c_user.name+",Routes Copied from USB Drive.");

            dCnt=30;
            msgBox = new QMessageBox(this);
            msgBox->setIcon(QMessageBox::Information);
            msgBox->setWindowTitle("USB Import");
            msgBox->setText("Done! Restart MPU for Settings to take effect");
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
            break;
        case IMPORT_ROUTES_BMU:
        {
            mask_close=true;
            QUrl url("ftp://"+systemconfig.bmu_ftp_user+":"+
                     systemconfig.bmu_ftp_pswd+"@"+systemconfig.bmu_ftp_ip+
                     "/"+systemconfig.mpu_id+"/export/routes.zip");

            dManager = new DownloadManager();
            connect(dManager,SIGNAL(message(QString)),this,SLOT(download_message(QString)));
            dManager->doDownload(url,base_dir+"import/routes.zip");

            dCnt=30;
            msgBox = new QMessageBox(this);
            msgBox->setIcon(QMessageBox::Information);
            msgBox->setWindowTitle("FTP Routes Import");
            msgBox->setText("Done!");
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
            break;
        case IMPORT_SETTINGS_USB:
        {
            system(QString("cp /mnt/sdn/config.json "+base_dir+"config/config.json").toLocal8Bit());
            w_log(c_user.name+",Settings Copied from USB Drive.");

            dCnt=30;
            msgBox = new QMessageBox(this);
            msgBox->setIcon(QMessageBox::Information);
            msgBox->setWindowTitle("USB Import");
            msgBox->setText("Done! Restart MPU for Settings to take effect");
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
            break;
        case IMPORT_SETTINGS_BMU:
        {
            mask_close=true;
            QUrl url("ftp://"+systemconfig.bmu_ftp_user+":"+
                     systemconfig.bmu_ftp_pswd+"@"+systemconfig.bmu_ftp_ip+
                     "/"+systemconfig.mpu_id+"/config/config.json");

            dManager = new DownloadManager();
            connect(dManager,SIGNAL(message(QString)),this,SLOT(download_message(QString)));
            dManager->doDownload(url,base_dir+"config/config.json");

            dCnt=30;
            msgBox = new QMessageBox(this);
            msgBox->setIcon(QMessageBox::Information);
            msgBox->setWindowTitle("FTP Settings Import");
            msgBox->setText("Done! Restart MPU for changes to take effect");
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
            break;
        case EXPORT_LOGS_USB:
        {
            QString  expFile = "systemlog_"+systemconfig.mpu_id+"_"+QDateTime::currentDateTime().toString("dd_MM_yyyy_hh_mm_ss")+".txt";
            system(QString("cp "+base_dir+"logs/system.log"+" "+base_dir+"export/"+expFile).toLocal8Bit());
            system(QString("cp "+base_dir+"export/"+expFile+" "+"/mnt/sdn/"+expFile).toLocal8Bit());
            w_log(c_user.name+",Logs Copied to pendrive");

            dCnt=30;
            msgBox = new QMessageBox(this);
            msgBox->setIcon(QMessageBox::Information);
            msgBox->setWindowTitle("USB Export");
            msgBox->setText("Done!");
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
            break;
        case EXPORT_FAULTS_USB:
        {
            QString  expFile = "faults_"+systemconfig.mpu_id+"_"+QDateTime::currentDateTime().toString("dd_MM_yyyy_hh_mm_ss")+".txt";
            system(QString("cp "+base_dir+"faults/fault.log"+" "+base_dir+"export/"+expFile).toLocal8Bit());
            system(QString("cp "+base_dir+"export/"+expFile+" "+"/mnt/sdn/"+expFile).toLocal8Bit());
            w_log(c_user.name+",Faults Copied to pendrive");

            dCnt=30;
            msgBox = new QMessageBox(this);
            msgBox->setIcon(QMessageBox::Information);
            msgBox->setWindowTitle("USB Export");
            msgBox->setText("Done!");
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
            break;
        case EXPORT_RUNNING_USB:
        {
            if(load_runs())
            {
                if(dump_runs())
                {
                    QString  expFile = "km_age_"+systemconfig.mpu_id+"_"+QDateTime::currentDateTime().toString("dd_MM_yyyy_hh_mm_ss")+".txt";
                    system(QString("cp "+base_dir+"export/km_age.txt"+" "+"/mnt/sdn/"+expFile).toLocal8Bit());
                    w_log(c_user.name+",KM-age to pendrive");
                    //system(QString("rm -r "+base_dir+"export/*").toLocal8Bit());
                    dCnt=30;
                    msgBox = new QMessageBox(this);
                    msgBox->setIcon(QMessageBox::Information);
                    msgBox->setWindowTitle("USB Export");
                    msgBox->setText("Done!");
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
        }
            break;
        case EXPORT_LOGS_BMU:
        {        
            mask_close=true;
            QString  expFile = "systemlog_"+systemconfig.mpu_id+"_"+QDateTime::currentDateTime().toString("dd_MM_yyyy_hh_mm_ss")+".txt";
            system(QString("cp "+base_dir+"logs/system.log"+" "+base_dir+"export/"+expFile).toLocal8Bit());

            QUrl url("ftp://"+systemconfig.bmu_ftp_user+":"+
                     systemconfig.bmu_ftp_pswd+"@"+systemconfig.bmu_ftp_ip+
                     "/"+systemconfig.mpu_id+"/import/"+expFile);

            dManager = new DownloadManager();
            connect(dManager,SIGNAL(message(QString)),this,SLOT(upload_message(QString)));
            dManager->doUpload(url, base_dir+"logs/system.log");

            dCnt=30;
            msgBox = new QMessageBox(this);
            msgBox->setIcon(QMessageBox::Information);
            msgBox->setWindowTitle("FTP LOGS Export");
            msgBox->setText("Done!");
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
            break;
        case EXPORT_FAULTS_BMU:
        {
            mask_close=true;
            QString  expFile = "faults_"+systemconfig.mpu_id+"_"+QDateTime::currentDateTime().toString("dd_MM_yyyy_hh_mm_ss")+".txt";
            system(QString("cp "+base_dir+"faults/fault.log"+" "+base_dir+"export/"+expFile).toLocal8Bit());

            QUrl url("ftp://"+systemconfig.bmu_ftp_user+":"+
                     systemconfig.bmu_ftp_pswd+"@"+systemconfig.bmu_ftp_ip+
                     "/"+systemconfig.mpu_id+"/import/"+expFile);

            dManager = new DownloadManager();
            connect(dManager,SIGNAL(message(QString)),this,SLOT(upload_message(QString)));
            dManager->doUpload(url, base_dir+"faults/fault.log");

            dCnt=30;
            msgBox = new QMessageBox(this);
            msgBox->setIcon(QMessageBox::Information);
            msgBox->setWindowTitle("FTP FAULTS Export");
            msgBox->setText("Done!");
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
            break;
        case EXPORT_RUNNING_BMU:
        {
            if(load_runs())
            {
                if(dump_runs())
                {
                    mask_close=true;
                    QString  expFile = "km_age_"+systemconfig.mpu_id+"_"+QDateTime::currentDateTime().toString("dd_MM_yyyy_hh_mm_ss")+".txt";

                    QUrl url("ftp://"+systemconfig.bmu_ftp_user+":"+
                             systemconfig.bmu_ftp_pswd+"@"+systemconfig.bmu_ftp_ip+
                             "/"+systemconfig.mpu_id+"/import/"+expFile);

                    dManager = new DownloadManager();
                    connect(dManager,SIGNAL(message(QString)),this,SLOT(upload_message(QString)));
                    dManager->doUpload(url, base_dir+"export/km_age.txt");

                    dCnt=30;
                    msgBox = new QMessageBox(this);
                    msgBox->setIcon(QMessageBox::Information);
                    msgBox->setWindowTitle("FTP KM-Age Export");
                    msgBox->setText("Done!");
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
        }
            break;
        }

    }
}

bool SecurityWindow::load_runs()
{
    //load runs
    QFile loadFile(QString(base_dir+"sdata/km_age.json"));
    if (!loadFile.open(QIODevice::ReadOnly)) {
        w_fault("KM age record file not found on Disk!");
        return false;
    }
    else
    {
        QByteArray saveData = loadFile.readAll();
        QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
        QJsonObject json = loadDoc.object();

        if (json.contains("runs") && json["runs"].isArray())
        {
            QJsonArray rArray = json["runs"].toArray();
            kmRun.clear();

            for(int rIdx=0;rIdx<rArray.size();rIdx++)
            {
                QJsonObject rObject = rArray[rIdx].toObject();
                run *r=new run();
                r->read(rObject);
                kmRun.append(r);
            }
        }
        loadFile.close();
    }
    return true;
}

bool SecurityWindow::dump_runs()
{
    QString path = base_dir+"export/km_age.txt";
    QFile saveFile(path);

    if (!saveFile.open(QIODevice::WriteOnly)) {
        w_fault("Unable to save km_age records to Disk!");
        return false;
    }

    saveFile.write(QString("Date,Coach Id, MPU Id, Route, Distance(km)\n").toLocal8Bit());
    for(run *r : kmRun)
    {
        QJsonObject rObject;
        r->write(rObject);
        saveFile.write(QString(r->date.toString()+","+r->coachId+","+
                               r->mpuId+","+r->route+","+QString::number(r->km,'_f',2)+"\n").toLocal8Bit());
    }
    saveFile.close();
    return true;
}

void SecurityWindow::job_finished()
{
    dCnt=30;
    msgBox = new QMessageBox(this);
    msgBox->setIcon(QMessageBox::Information);
    msgBox->setWindowTitle("USB Export");
    msgBox->setText("Done!");
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

void SecurityWindow::closeMessageBox()
{
    dCnt--;
    msgBox->setInformativeText("Closing in "+QString::number(dCnt)+"s");
    update();
    if(dCnt==0)
    {
        disconnect(popupTimer, SIGNAL(timeout()), this, SLOT(closeMessageBox()));
        popupTimer->stop();
        msgBox->close();
        update();
    }
}

void SecurityWindow::on_pushButton_15_clicked()
{
    //previous
    index--;
    if(index<(int)SYNC_LIST)
    {
        index=(int)SYNC_STATUS;
    }
    datasync = (datasync_t)index;
    ui->stackedWidget->setCurrentIndex(index);

    switch(datasync)
    {
    case SYNC_LIST:
        ui->groupBox_route->setTitle("MAINTENANCE OPERATIONS");
        break;
    case SYNC_STATUS:
        ui->groupBox_route->setTitle("SERVICE REQUESTS");
        break;
    }
}

void SecurityWindow::on_pushButton_19_clicked()
{
    //right
    index++;
    if(index>(int)SYNC_STATUS)
    {
        index=(int)SYNC_LIST;
    }
    datasync = (datasync_t)index;
    ui->stackedWidget->setCurrentIndex(index);

    switch(datasync)
    {
    case SYNC_LIST:
        ui->groupBox_route->setTitle("MAINTENANCE OPERATIONS");
        break;
    case SYNC_STATUS:
        ui->groupBox_route->setTitle("SERVICE REQUESTS");
        break;
    }
}

void SecurityWindow::on_tableView_2_clicked(const QModelIndex &index)
{

}

void SecurityWindow::on_tableView_clicked(const QModelIndex &index)
{
    selectedIndex = index.row();
    op = (maintenance_ops_t)selectedIndex;
}

void SecurityWindow::on_pushButton_8_clicked()
{
    //schedule
}

void SecurityWindow::on_pushButton_21_clicked()
{
    //delete entry
}

void SecurityWindow::on_pushButton_23_clicked()
{
    //clear all entries
}

void SecurityWindow::on_pushButton_25_toggled(bool checked)
{
    if(checked)
    {
        if(!u1_mnt)
        {
            msgBox = new QMessageBox(this);
            msgBox->setIcon(QMessageBox::Information);
            msgBox->setWindowTitle("USB Plug-in");
            msgBox->setText("Please Plug-in USB Pendrive to USB-1 and Press OK. Ignore if already done");
            msgBox->setStandardButtons(QMessageBox::Ok);
            msgBox->setModal(true);
            int ret = msgBox->exec();
            update();
            if(ret == QMessageBox::Ok)
            {
                msgBox->close();
            }

            //mount usb-1
            system(QString("echo 1 > /sys/bus/usb/devices/1-1.1.3/bConfigurationValue").toLocal8Bit());

            dCnt=10;
            msgBox = new QMessageBox(this);
            msgBox->setIcon(QMessageBox::Information);
            msgBox->setWindowTitle("USB Detect");
            msgBox->setText("Please Wait Detecting USB");
            msgBox->setInformativeText("Closing in "+QString::number(dCnt)+"s");
            msgBox->setStandardButtons(QMessageBox::Ok);
            popupTimer= new QTimer();
            connect(popupTimer, SIGNAL(timeout()), this, SLOT(closeMessageBox()));
            popupTimer->start(1000);
            msgBox->setModal(true);
            ret = msgBox->exec();
            update();


            if(QFile::exists("/dev/sdc1"))
            {
                u1_mnt=true;
                usb_path = QString("/dev/sdc1");
                system(QString("rm -r /mnt/sdn").toLocal8Bit());
                system(QString("mkdir /mnt/sdn").toLocal8Bit());
                system(QString("mount "+ usb_path+" /mnt/sdn").toLocal8Bit());
            }
            else
            if(QFile::exists("/dev/sdd1"))
            {
                u1_mnt=true;
                usb_path = QString("/dev/sdd1");
                system(QString("rm -r /mnt/sdn").toLocal8Bit());
                system(QString("mkdir /mnt/sdn").toLocal8Bit());
                system(QString("mount "+ usb_path+" /mnt/sdn").toLocal8Bit());
            }
            else
            {
                dCnt=15;
                msgBox = new QMessageBox(this);
                msgBox->setIcon(QMessageBox::Warning);
                msgBox->setWindowTitle("USB Error");
                msgBox->setText("Failed to Detect USB Drive");
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
    }
    else
    {
        if(u1_mnt)
        {
            system(QString("umount /mnt/sdn").toLocal8Bit());

            dCnt=15;
            msgBox = new QMessageBox(this);
            msgBox->setIcon(QMessageBox::Warning);
            msgBox->setWindowTitle("USB Unmount");
            msgBox->setText("USB Drive unmounted");
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

            system(QString("rm -r /mnt/sdn").toLocal8Bit());
            u1_mnt=false;
        }
    }
}

void SecurityWindow::on_pushButton_24_toggled(bool checked)
{
    if(checked)
    {
        if(!u2_mnt)
        {
            msgBox = new QMessageBox(this);
            msgBox->setIcon(QMessageBox::Information);
            msgBox->setWindowTitle("USB Plug-in");
            msgBox->setText("Please Plug-in USB Pendrive to USB-2 and Press OK. Ignore if already done");
            msgBox->setStandardButtons(QMessageBox::Ok);
            msgBox->setModal(true);
            int ret = msgBox->exec();
            update();
            if(ret == QMessageBox::Ok)
            {
                msgBox->close();
            }

            //mount usb-1
            system(QString("echo 1 > /sys/bus/usb/devices/1-1.1.4/bConfigurationValue").toLocal8Bit());

            dCnt=10;
            msgBox = new QMessageBox(this);
            msgBox->setIcon(QMessageBox::Information);
            msgBox->setWindowTitle("USB Detect");
            msgBox->setText("Please Wait Detecting USB");
            msgBox->setInformativeText("Closing in "+QString::number(dCnt)+"s");
            msgBox->setStandardButtons(QMessageBox::Ok);
            popupTimer= new QTimer();
            connect(popupTimer, SIGNAL(timeout()), this, SLOT(closeMessageBox()));
            popupTimer->start(1000);
            msgBox->setModal(true);
            ret = msgBox->exec();
            update();
            if(ret == QMessageBox::Ok)
            {
                msgBox->close();
                disconnect(popupTimer, SIGNAL(timeout()), this, SLOT(closeMessageBox()));
                popupTimer->stop();
            }

            if(QFile::exists("/dev/sdc1"))
            {
                u2_mnt=true;
                usb_path = QString("/dev/sdc1");
                system(QString("rm -r /mnt/sdn").toLocal8Bit());
                system(QString("mkdir /mnt/sdn").toLocal8Bit());
                system(QString("mount "+ usb_path+" /mnt/sdn").toLocal8Bit());
            }
            else
            if(QFile::exists("/dev/sdd1"))
            {
                u2_mnt=true;
                usb_path = QString("/dev/sdd1");
                system(QString("rm -r /mnt/sdn").toLocal8Bit());
                system(QString("mkdir /mnt/sdn").toLocal8Bit());
                system(QString("mount "+ usb_path+" /mnt/sdn").toLocal8Bit());
            }
            else
            {
                dCnt=15;
                msgBox = new QMessageBox(this);
                msgBox->setIcon(QMessageBox::Warning);
                msgBox->setWindowTitle("USB Error");
                msgBox->setText("Failed to Detect USB Drive");
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
    }
    else
    {
        if(u2_mnt)
        {
            system(QString("umount /mnt/sdn").toLocal8Bit());

            dCnt=15;
            msgBox = new QMessageBox(this);
            msgBox->setIcon(QMessageBox::Warning);
            msgBox->setWindowTitle("USB Unmount");
            msgBox->setText("USB Drive unmounted");
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

            system(QString("rm -r /mnt/sdn").toLocal8Bit());
            u2_mnt=false;
        }
    }
}

void SecurityWindow::on_pushButton_25_clicked()
{

}

