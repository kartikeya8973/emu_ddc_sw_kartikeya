#include "servicedatawindow.h"
#include "ui_servicedatawindow.h"

extern MainWindow *mw;
extern MenuWindow *mnw;
extern LoginDialog *ld;
extern data papis_data;
extern user c_user;
extern routes routesInfo;

ServiceDataWindow::ServiceDataWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ServiceDataWindow)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose,true);

    render_routes();
    w_log(c_user.name+",Opened Service Data");

    highlight_row(selectedRouteIndex);
//    connect(this,SIGNAL(update_info()),mw,SLOT(reload_info()));
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
    ui->label_datetime->setText(QDateTime::currentDateTime().toString("ddd MMMM d yy hh:mm"));
    timer->start(60000);
    update();
}

void ServiceDataWindow::showTime()
{
    ui->label_datetime->setText(QDateTime::currentDateTime().toString("ddd MMMM d yy hh:mm"));
    update();
}

ServiceDataWindow::~ServiceDataWindow()
{
    delete ui;
}

void ServiceDataWindow::highlight_row(int row)
{
    selectedRouteIndex = row;
    QModelIndex index = ui->tableView_routes->model()->index(row, 0);
    ui->tableView_routes->setCurrentIndex(index);
}

void ServiceDataWindow::on_pushButton_menu_clicked()
{
//    disconnect(this,SIGNAL(update_info()),mw,SLOT(reload_info()));
    this->close();
    mnw = new MenuWindow();
    mnw->setWindowFlags(Qt::FramelessWindowHint);
    mnw->showFullScreen();
}

void ServiceDataWindow::render_selection()
{
//    ui->label_r1_2->setText("");
//    ui->label_r2_2->setText("");
//    ui->label_r3_2->setText("");
//    ui->label_r4_2->setText("");
//    ui->label_r5_2->setText("");
//    ui->label_r6_2->setText("");
    for(int idx=0;idx<routesInfo.routeSel.count();idx++)
    {
        switch(idx)
        {
        case 0:
            ui->label_r1_2->setText(routesInfo.routeList.at(routesInfo.routeSel.at(idx)).tr_no_en);
            break;
        case 1:
            ui->label_r2_2->setText(routesInfo.routeList.at(routesInfo.routeSel.at(idx)).tr_no_en);
            break;
        case 2:
            ui->label_r3_2->setText(routesInfo.routeList.at(routesInfo.routeSel.at(idx)).tr_no_en);
            break;
        case 3:
            ui->label_r4_2->setText(routesInfo.routeList.at(routesInfo.routeSel.at(idx)).tr_no_en);
            break;
        case 4:
            ui->label_r5_2->setText(routesInfo.routeList.at(routesInfo.routeSel.at(idx)).tr_no_en);
            break;
        case 5:
            ui->label_r6_2->setText(routesInfo.routeList.at(routesInfo.routeSel.at(idx)).tr_no_en);
            break;
        }

//        ui->label_r1->setText(routesInfo.routeList.at(idx).tr_no_en);
    }
}

void ServiceDataWindow::render_routes()
{
    render_selection();
    QStringList labels = {"TRAIN No.","TRAIN NAME","SRC-DEST", "VIA"};

    rModel = new QStandardItemModel(routesInfo.routeList.count(),4);
    rModel->setHorizontalHeaderLabels(labels);

    for (int row = 0; row < rModel->rowCount(); ++row)
    {
        QStandardItem *item_train_number = new QStandardItem(routesInfo.routeList.at(row).tr_no_en);
        rModel->setItem(row,0,item_train_number);

        QStandardItem *item_train_name = new QStandardItem(routesInfo.routeList.at(row).tr_nm_en);
        rModel->setItem(row,1,item_train_name);

        QStandardItem *item_train_route = new QStandardItem(routesInfo.routeList.at(row).tr_route_en);
        rModel->setItem(row,2,item_train_route);

        QStandardItem *item_train_via = new QStandardItem(routesInfo.routeList.at(row).tr_via_en);
        rModel->setItem(row,3,item_train_via);
    }

    ui->tableView_routes->setModel(rModel);
    ui->tableView_routes->setColumnWidth(0,80);
    ui->tableView_routes->setColumnWidth(1,200);
    ui->tableView_routes->setColumnWidth(2,200);
    ui->tableView_routes->setColumnWidth(3,220);
    ui->tableView_routes->setShowGrid(true);
    ui->tableView_routes->setSortingEnabled(true);
    selectedRouteIndex=0;

    update();
    w_log(c_user.name+",Opened Route List");


}

void ServiceDataWindow::closeMessageBox()
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

void ServiceDataWindow::on_pushButton_6_clicked()
{    
    if(c_user.access_level==ACCESS_LEVEL_2)
    {
        //delete route
        route_op=ROUTE_DEL;
        verify_password();;
        update();
    }
    else
    {
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

void ServiceDataWindow::verify_password()
{
    passwordDialog = new LineEditDialog();
    passwordDialog->setModal(true);
    passwordDialog->setWindowFlags(Qt::FramelessWindowHint);
    passwordDialog->setLabel("Enter Password");
    passwordDialog->show();
    connect(passwordDialog,SIGNAL(text_entered(QString)),this,SLOT(receive_password(QString)));

}

void ServiceDataWindow::receive_password(QString password)
{
    disconnect(passwordDialog,SIGNAL(text_entered(QString)),this,SLOT(receive_password(QString)));
    if(c_user.password!=password)
    {
        w_log(c_user.name+",Entered Incorrect Password, Denied Access to Service Data");        
        dCnt=10;
        msgBox = new QMessageBox(this);
        msgBox->setIcon(QMessageBox::Warning);
        msgBox->setWindowTitle("Error");
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
    else
    {
        switch(route_op)
        {
        case ROUTE_SET_1:
        case ROUTE_SET_2:
        case ROUTE_SET_3:
        case ROUTE_SET_4:
        case ROUTE_SET_5:
        case ROUTE_SET_6:
        {
            dCnt=10;
            msgBox = new QMessageBox(this);
            msgBox->setIcon(QMessageBox::Question);
            msgBox->setWindowTitle("Set Route "+QString::number((int)route_op).toLocal8Bit());
            msgBox->setText("Confirm Selection of Route: "+routesInfo.routeList.at(selectedRouteIndex).tr_no_en);
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

                int opidx=(int)route_op-1;
                if(opidx<routesInfo.routeSel.count())
                {
                    routesInfo.routeSel.replace(opidx, selectedRouteIndex);
                    w_log(c_user.name+",Selected "+routesInfo.routeList.at(selectedRouteIndex).tr_no_en+" as Active Route "+QString::number(opidx));
                    QMessageBox::information(this, tr("Success"),
                    routesInfo.routeList.at(selectedRouteIndex).tr_no_en+" selected as Active Route "+QString::number(opidx),
                    QMessageBox::Ok);
                }
                else
                if(opidx==routesInfo.routeSel.count())
                {
                    routesInfo.routeSel.append(selectedRouteIndex);
                    w_log(c_user.name+",Selected "+routesInfo.routeList.at(selectedRouteIndex).tr_no_en+" as Active Route "+QString::number(opidx));
                    QMessageBox::information(this, tr("Success"),
                    routesInfo.routeList.at(selectedRouteIndex).tr_no_en+" selected as Active Route "+QString::number(opidx),
                    QMessageBox::Ok);
                }
                else
                {
                    QMessageBox::information(this, tr("Error"),
                    " Set previous route first!", QMessageBox::Ok);
                }
            }
            else
            {
                disconnect(popupTimer, SIGNAL(timeout()), this, SLOT(closeMessageBox()));
                popupTimer->stop();
                msgBox->close();
            }
        }
            break;
        case ROUTE_DEL:
            {
                dCnt=10;
                msgBox = new QMessageBox(this);
                msgBox->setIcon(QMessageBox::Question);
                msgBox->setWindowTitle("Delete Route");
                msgBox->setText("Confirm Deletion of Route: "+routesInfo.routeList.at(selectedRouteIndex).tr_no_en);
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

                    if((int)route_op<routesInfo.routeSel.count())
                    {
                        w_log(c_user.name+",Deleted "+routesInfo.routeList.at(selectedRouteIndex).tr_no_en);
                        routesInfo.routeList.removeAt(selectedRouteIndex);
                        QMessageBox::information(this, tr("Success"),
                        "Route Deleted Succesfully!",
                        QMessageBox::Ok);
                    }
                }
                else
                {
                    disconnect(popupTimer, SIGNAL(timeout()), this, SLOT(closeMessageBox()));
                    popupTimer->stop();
                    msgBox->close();
                }
            }
            break;
        }

        //write
        if(write_routes())
        {
            w_log(c_user.name+",Saved Route List to Disk");
            render_routes();
        }
    }
}

void ServiceDataWindow::on_tableView_routes_clicked(const QModelIndex &index)
{    
    selectedRouteIndex=index.row();
}

void ServiceDataWindow::on_pushButton_15_clicked()
{
    //scroll up
    if(scrlIdx>0)
    {
        scrlIdx--;
    }
    ui->tableView_routes->verticalScrollBar()->setValue(scrlIdx);
    highlight_row(scrlIdx);
    update();
}


void ServiceDataWindow::on_pushButton_14_clicked()
{
    //scroll down
    if(scrlIdx<routesInfo.routeList.count()-1)
    {
        scrlIdx++;
    }
    ui->tableView_routes->verticalScrollBar()->setValue(scrlIdx);
    highlight_row(scrlIdx);
    update();
}

void ServiceDataWindow::on_pushButton_r1_clicked()
{
    route_op=ROUTE_SET_1;
    verify_password();
}

void ServiceDataWindow::on_pushButton_r2_clicked()
{
    route_op=ROUTE_SET_2;
    verify_password();
}

void ServiceDataWindow::on_pushButton_r3_clicked()
{
    route_op=ROUTE_SET_3;
    verify_password();
}

void ServiceDataWindow::on_pushButton_r4_clicked()
{
    route_op=ROUTE_SET_4;
    verify_password();
}

void ServiceDataWindow::on_pushButton_r5_clicked()
{
    route_op=ROUTE_SET_5;
    verify_password();
}

void ServiceDataWindow::on_pushButton_r6_clicked()
{
    route_op=ROUTE_SET_6;
    verify_password();
}

void ServiceDataWindow::on_pushButton_d1_clicked()
{
    if(routesInfo.routeSel.count()>0)
    {
        w_log(c_user.name+",Removed Active Route at Position 1");
        routesInfo.routeSel.removeAt(0);
        render_selection();
    }
    update();
}

void ServiceDataWindow::on_pushButton_d2_clicked()
{
    if(routesInfo.routeSel.count()>1)
    {
        w_log(c_user.name+",Removed Active Route at Position 2");
        routesInfo.routeSel.removeAt(1);
        render_selection();
    }
    update();
}

void ServiceDataWindow::on_pushButton_d3_clicked()
{
    if(routesInfo.routeSel.count()>2)
    {
        w_log(c_user.name+",Removed Active Route at Position 3");
        routesInfo.routeSel.removeAt(2);
        render_selection();
    }
    update();
}

void ServiceDataWindow::on_pushButton_d4_clicked()
{
    if(routesInfo.routeSel.count()>3)
    {
        w_log(c_user.name+",Removed Active Route at Position 4");
        routesInfo.routeSel.removeAt(3);
        render_selection();
    }
    update();
}

void ServiceDataWindow::on_pushButton_d5_clicked()
{
    if(routesInfo.routeSel.count()>4)
    {
        w_log(c_user.name+",Removed Active Route at Position 5");
        routesInfo.routeSel.removeAt(4);
        render_selection();
    }
    update();
}

void ServiceDataWindow::on_pushButton_d6_clicked()
{
    if(routesInfo.routeSel.count()>5)
    {
        w_log(c_user.name+",Removed Active Route at Position 6");
        routesInfo.routeSel.removeAt(5);
        render_selection();
    }
    update();
}

void ServiceDataWindow::on_pushButton_16_clicked()
{
    //top
    if(scrlIdx>0)
    {
        scrlIdx=0;
    }
    ui->tableView_routes->verticalScrollBar()->setValue(scrlIdx);
    highlight_row(scrlIdx);
    update();
}

void ServiceDataWindow::on_pushButton_18_clicked()
{
    //bottom
    if(scrlIdx<routesInfo.routeList.count())
    {
        scrlIdx=routesInfo.routeList.count()-1;
    }

    ui->tableView_routes->verticalScrollBar()->setValue(scrlIdx);
    highlight_row(scrlIdx);
    update();
}


void ServiceDataWindow::on_pushButton_7_clicked()
{
    //open route
//    ui->stackedWidget->setCurrentIndex(1);
    dCnt=10;
    msgBox = new QMessageBox(this);
    msgBox->setIcon(QMessageBox::Information);
    msgBox->setWindowTitle("Open Route");
    msgBox->setText("Feature not enabled in this version");
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

void ServiceDataWindow::on_pushButton_8_clicked()
{
    //search route
    lineEditDialog = new LineEditDialog();
    lineEditDialog->setModal(true);
    lineEditDialog->setWindowFlags(Qt::FramelessWindowHint);
    lineEditDialog->setLabel("Search Route");
    lineEditDialog->setLabel2("Enter Route Number Here");
#ifndef LAPTOP
    lineEditDialog->show();
#else
    lineEditDialog->showFullScreen();
#endif
    connect(lineEditDialog,SIGNAL(text_entered(QString)),this,SLOT(receive_text(QString)));
}

void ServiceDataWindow::receive_text(QString text)
{
    bool found=false;
    int idx;
    //search by route number
    for(idx=0;idx<routesInfo.routeList.count();idx++)
    {
        if(routesInfo.routeList.at(idx).tr_no_en==text)
        {
            found=true;
            highlight_row(idx);
            break;
        }
    }
    if(!found)
    {
        dCnt=10;
        msgBox = new QMessageBox(this);
        msgBox->setIcon(QMessageBox::Information);
        msgBox->setWindowTitle("Route Search");
        msgBox->setText("Route not found in the list!");
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
