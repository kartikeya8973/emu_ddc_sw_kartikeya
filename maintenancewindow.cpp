#include "maintenancewindow.h"
#include "ui_maintenancewindow.h"

#include "menuwindow.h"
extern MenuWindow *mnw;
extern LoginDialog *ld;
extern QString base_dir;
extern QList <run*> kmRun;
MaintenanceWindow::MaintenanceWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MaintenanceWindow)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose,true);
    maintenance = MAINTENANCE_RUN;
    ui->label_8->setText("1/3");
    ui->stackedWidget->setCurrentIndex(maintenance);  
    render_logs();
    render_faults();
    render_km_run();
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
    ui->label_datetime->setText(QDateTime::currentDateTime().toString("ddd MMMM d yy hh:mm"));
    timer->start(60000);
    update();
}

void MaintenanceWindow::showTime()
{
    ui->label_datetime->setText(QDateTime::currentDateTime().toString("ddd MMMM d yy hh:mm"));
    update();
}

void MaintenanceWindow::render_logs()
{
    QFile file(base_dir+"logs/system.log");
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }

    QStringList labels = {"Timestamp","MPU ID","Coach ID/No","User","Message"};

    logModel = new QStandardItemModel();
    logModel->setColumnCount(5);
//    logModel = new QStandardItemModel(logList.count(),5);
    logModel->setHorizontalHeaderLabels(labels);

    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        QString str = QString::fromLocal8Bit(line);

        if(!str.isEmpty())
        {
            QStringList cols =str.split(',');
            if(cols.count()==5)
            {
                QList <QStandardItem*> sList;
                for(int col=0;col<cols.count();col++)
                {
                    sList.append(new QStandardItem(cols.at(col)));
                }
                logModel->appendRow(sList);
            }
        }
        lLines++;
    }

    file.close();

    ui->tableView_logs->setModel(logModel);
    ui->tableView_logs->setColumnWidth(0,150);
    ui->tableView_logs->setColumnWidth(1,100);
    ui->tableView_logs->setColumnWidth(2,120);
    ui->tableView_logs->setColumnWidth(3,80);
    ui->tableView_logs->setColumnWidth(4,290);

    update();
}

void MaintenanceWindow::render_faults()
{
    QFile file(base_dir+"faults/fault.log");
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }

    QStringList labels = {"Timestamp","MPU ID","Coach ID/No","User","Message"};

    faultModel = new QStandardItemModel();
    faultModel->setColumnCount(5);
    faultModel->setHorizontalHeaderLabels(labels);

    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        QString str = QString::fromLocal8Bit(line);

        if(!str.isEmpty())
        {
            QStringList cols =str.split(',');
            if(cols.count()==5)
            {
                QList <QStandardItem*> sList;
                for(int col=0;col<cols.count();col++)
                {
                    sList.append(new QStandardItem(cols.at(col)));
                }
                faultModel->appendRow(sList);
            }
        }
        fLines++;
    }

    file.close();

    ui->tableView_faults->setModel(faultModel);
    ui->tableView_faults->setColumnWidth(0,150);
    ui->tableView_faults->setColumnWidth(1,100);
    ui->tableView_faults->setColumnWidth(2,120);
    ui->tableView_faults->setColumnWidth(3,80);
    ui->tableView_faults->setColumnWidth(4,290);
    update();
}

void MaintenanceWindow::render_km_run()
{
    if(load_runs())
    {
        QStringList labels = {"Date","MPU ID","Coach ID","Route","Kilometers"};

        rModel = new QStandardItemModel(kmRun.count(),5);
        rModel->setHorizontalHeaderLabels(labels);

        for (int row = 0; row < rModel->rowCount(); ++row)
        {
            QStandardItem *item_date = new QStandardItem(kmRun.at(row)->date.toString());
            rModel->setItem(row,0,item_date);

            QStandardItem *item_mid = new QStandardItem(kmRun.at(row)->mpuId);
            rModel->setItem(row,1,item_mid);

            QStandardItem *item_cid = new QStandardItem(kmRun.at(row)->coachId);
            rModel->setItem(row,2,item_cid);

            QStandardItem *item_route = new QStandardItem(kmRun.at(row)->route);
            rModel->setItem(row,3,item_route);

            QStandardItem *item_kilometers = new QStandardItem(QString::number(kmRun.at(row)->km,'_f',2));
            rModel->setItem(row,4,item_kilometers);
        }

        ui->tableView_running->setModel(rModel);
        ui->tableView_running->setColumnWidth(0,150);
        ui->tableView_running->setColumnWidth(1,150);
        ui->tableView_running->setColumnWidth(2,150);
        ui->tableView_running->setColumnWidth(3,150);
        ui->tableView_running->setColumnWidth(4,150);
        update();
    }
}

bool MaintenanceWindow::load_runs()
{
    double kmR=0;
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
            kmRun.reserve(rArray.size());

            for(int rIdx=0;rIdx<rArray.size();rIdx++)
            {
                QJsonObject rObject = rArray[rIdx].toObject();
                run *r=new run();
                r->read(rObject);
                kmRun.append(r);
                kmR+=r->km;
            }
        }
        loadFile.close();
        ui->label_hw_id->setText(QString::number(kmR,'_f',2)+" km");
    }
    return true;
}

MaintenanceWindow::~MaintenanceWindow()
{
    delete ui;
}

void MaintenanceWindow::on_pushButton_menu_clicked()
{
    this->close();
    mnw = new MenuWindow();
    mnw->setWindowFlags(Qt::FramelessWindowHint);
    mnw->showFullScreen();
    update();
}

void MaintenanceWindow::on_pushButton_14_clicked()
{
    //previous
    index--;
    if(index<(int)MAINTENANCE_RUN)
    {
        index=(int)MAINTENANCE_LOGS;
    }
    maintenance = (maintenance_t)index;
    ui->stackedWidget->setCurrentIndex(index);

    switch(maintenance)
    {
    case MAINTENANCE_RUN:
        ui->label_8->setText("1/3");
        ui->label->setText("KILOMETER AGE");
        break;
    case MAINTENANCE_FAULTS:
        ui->label_8->setText("2/3");
        ui->label->setText("SYSTEM FAULTS");
        break;
    case MAINTENANCE_LOGS:
        ui->label_8->setText("3/3");
        ui->label->setText("SYSTEM LOGS");
        break;
    }
}

void MaintenanceWindow::on_pushButton_13_clicked()
{
    //next
    index++;
    if(index>(int)MAINTENANCE_LOGS)
    {
        index=(int)MAINTENANCE_RUN;
    }
    maintenance = (maintenance_t)index;
    ui->stackedWidget->setCurrentIndex(index);

    switch(maintenance)
    {
    case MAINTENANCE_RUN:
        ui->label_8->setText("1/3");
        ui->label->setText("KILOMETER AGE");
        break;
    case MAINTENANCE_FAULTS:
        ui->label_8->setText("2/3");
        ui->label->setText("SYSTEM FAULTS");
        break;
    case MAINTENANCE_LOGS:
        ui->label_8->setText("3/3");
        ui->label->setText("SYSTEM LOGS");
        break;
    }
}

void MaintenanceWindow::on_pushButton_17_clicked()
{
    //scroll up

    switch(maintenance)
    {
    case MAINTENANCE_RUN:
    {
        if(scrlIdx_r>0)
        {
            scrlIdx_r--;
        }
        ui->tableView_running->verticalScrollBar()->setValue(scrlIdx_r);
    }
        break;
    case MAINTENANCE_LOGS:
    {
        if(scrlIdx_l>0)
        {
            scrlIdx_l--;
        }
        ui->tableView_logs->verticalScrollBar()->setValue(scrlIdx_l);
    }
        break;
    case MAINTENANCE_FAULTS:
    {
        if(scrlIdx_f>0)
        {
            scrlIdx_f--;
        }
        ui->tableView_faults->verticalScrollBar()->setValue(scrlIdx_f);
    }
        break;
    }
    update();
}

void MaintenanceWindow::on_pushButton_15_clicked()
{
    //scroll down

    switch(maintenance)
    {
    case MAINTENANCE_RUN:
    {
        if(scrlIdx_r<1080)
        {
            scrlIdx_r++;
        }
        ui->tableView_running->verticalScrollBar()->setValue(scrlIdx_r);
    }
        break;
    case MAINTENANCE_LOGS:
    {
        if(scrlIdx_l<lLines-10)
        {
            scrlIdx_l++;
        }
        ui->tableView_logs->verticalScrollBar()->setValue(scrlIdx_l);
    }
        break;
    case MAINTENANCE_FAULTS:
    {
        if(scrlIdx_f<fLines)
        {
            scrlIdx_f++;
        }
        ui->tableView_faults->verticalScrollBar()->setValue(scrlIdx_f);
    }
        break;
    }
    update();
}

void MaintenanceWindow::on_pushButton_16_clicked()
{
    //goto top

    switch(maintenance)
    {
    case MAINTENANCE_RUN:
    {
        if(scrlIdx_r>0)
        {
            scrlIdx_r--;
        }
        ui->tableView_running->verticalScrollBar()->setValue(scrlIdx_r);
    }
        break;
    case MAINTENANCE_LOGS:
    {
        if(scrlIdx_l>0)
        {
            scrlIdx_l=0;
        }
        ui->tableView_logs->verticalScrollBar()->setValue(scrlIdx_l);
    }
        break;
    case MAINTENANCE_FAULTS:
    {
        if(scrlIdx_f>0)
        {
            scrlIdx_f=0;
        }
        ui->tableView_faults->verticalScrollBar()->setValue(scrlIdx_f);
    }
        break;
    }
    update();
}

void MaintenanceWindow::on_pushButton_18_clicked()
{
    //goto bottom
    switch(maintenance)
    {
    case MAINTENANCE_RUN:
    {
        if(scrlIdx_r<1080)
        {
            scrlIdx_r++;
        }
        ui->tableView_running->verticalScrollBar()->setValue(scrlIdx_r);
    }
        break;
    case MAINTENANCE_LOGS:
    {
        if(scrlIdx_l<lLines)
        {
            scrlIdx_l=lLines-10;
        }
        ui->tableView_logs->verticalScrollBar()->setValue(scrlIdx_l);
    }
        break;
    case MAINTENANCE_FAULTS:
    {
        if(scrlIdx_f<fLines)
        {
            scrlIdx_f=fLines-1;
        }
        ui->tableView_faults->verticalScrollBar()->setValue(scrlIdx_f);
    }
        break;
    }
    update();
}
