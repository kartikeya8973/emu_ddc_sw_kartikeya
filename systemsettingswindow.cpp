#include "systemsettingswindow.h"
#include "ui_systemsettingswindow.h"

extern MainWindow *mw;
extern MenuWindow *mnw;
extern LoginDialog *ld;
extern systemconfig systemconfig;
extern QList <user> users;
extern user c_user;

SystemSettingsWindow::SystemSettingsWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SystemSettingsWindow)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose,true);

    w_log(c_user.name+",Opened System Settings");

    connect(this,SIGNAL(settings_updated()),mw,SLOT(settings_updated()));

    ui->stackedWidget->setCurrentIndex(index);
    render_config();
    render_user_table();
    highlight_row(selectedIndex);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
    ui->label_datetime->setText(QDateTime::currentDateTime().toString("ddd MMMM d yy hh:mm"));
    timer->start(60000);
    update();
}

void SystemSettingsWindow::showTime()
{
    ui->label_datetime->setText(QDateTime::currentDateTime().toString("ddd MMMM d yy hh:mm"));
    update();
}

SystemSettingsWindow::~SystemSettingsWindow()
{
    delete ui;
}

void SystemSettingsWindow::render_user_table()
{
    QStringList labels = {"Username","Access-Level","Password","Status"};

    uModel = new QStandardItemModel(users.count(),4);
    uModel->setHorizontalHeaderLabels(labels);

    for (int row = 0; row < uModel->rowCount(); ++row){

        if(users.at(row).access_level==ACCESS_LEVEL_1)
        {
            QStandardItem *item_al = new QStandardItem(QString("Level-1"));
            uModel->setItem(row,1,item_al);
        }
        else
        if(users.at(row).access_level==ACCESS_LEVEL_2)
        {
            QStandardItem *item_al = new QStandardItem(QString("Level-2"));
            uModel->setItem(row,1,item_al);
        }
        else
        {
            QStandardItem *item_al = new QStandardItem(QString("No Access"));
            uModel->setItem(row,1,item_al);
        }

        QStandardItem *item_un = new QStandardItem(QString(users.at(row).name));
        uModel->setItem(row,0,item_un);

        QStandardItem *item_ps = new QStandardItem(QString(users.at(row).password));
        uModel->setItem(row,2,item_ps);

        if(users.at(row).activeStatus==ACTIVE)
        {
            QStandardItem *item_as = new QStandardItem(QString("Active"));
            uModel->setItem(row,3,item_as);
        }
        else
        {
            QStandardItem *item_as = new QStandardItem(QString("Disabled"));
            uModel->setItem(row,3,item_as);
        }
    }

    ui->tableView->setModel(uModel);
    w_log(c_user.name+",Opened System Users");
}

void SystemSettingsWindow::render_config()
{   
    ui->comboBox_gps_src->setCurrentText(systemconfig.gps_source);
    ui->comboBox_dvi_2->setCurrentText(systemconfig.dvi_active);

    ui->horizontalSlider_pa_vol->setValue(QString(systemconfig.pa_vol).toInt());

    ui->label_coach_id->setText(systemconfig.coach_id);
    ui->label_coach_no->setText(systemconfig.coach_no);

    speedVal = systemconfig.led_pis_speed;
    ui->label_scroll_speed->setText(QString::number(speedVal)+" col/s");

    ui->label_gsm_status->setText(systemconfig.gsm_status);
    ui->label_gsm_imei->setText(systemconfig.gsm_imei);
    ui->label_gsm_msisdn->setText(systemconfig.gsm_mdisdn);
    ui->label_gsm_apn->setText(systemconfig.gsm_apn);


    //ETHERNET
    ui->label_47->setText("ETHERNET: Error");
    ui->label_eth_active->setText("Disconnected");
    ui->label_ip_mode_2->setText("");
    ui->label_ip_addr_2->setText("");
    ui->label_netmask_2->setText("");
    ui->label_gateway_2->setText("");
    ui->label_dns_2->setText("");

    foreach(QNetworkInterface netInterface, QNetworkInterface::allInterfaces())
    {
        // Return only the first non-loopback MAC Address
        if (!(netInterface.flags() & QNetworkInterface::IsLoopBack))
        {
            if(netInterface.type()==QNetworkInterface::Ethernet)
            {
                eth = netInterface;

                ui->label_47->setText("ETHERNET: "+eth.hardwareAddress());
                ui->label_ip_mode_2->setText(systemconfig.eth_ip_mode);

                QList<QNetworkAddressEntry> entries = eth.addressEntries();
                if (!entries.isEmpty()) {
                    ui->label_eth_active->setText("Connected");
                    QNetworkAddressEntry entry = entries.first();
                    QHostAddress ip = entry.ip();
                    ui->label_ip_addr_2->setText(ip.toString());
                    ui->label_netmask_2->setText(systemconfig.eth_net_mask);
                    ui->label_gateway_2->setText(systemconfig.eth_gateway);
                    ui->label_dns_2->setText(systemconfig.eth_dns);
                }
            }
        }
    }

    //Wi-Fi
    ui->label_wifi_ssid_2->setText(systemconfig.wifi_st_ssid);
    ui->label_wifi_pswd_2->setText(systemconfig.wifi_st_pswd);
    ui->label_wifi_sec_2->setText("WPA/WPA2");
    ui->label_wifi_ip_mode_2->setText(systemconfig.wifi_ip_mode);
    ui->label_wifi_ipv4_2->setText(systemconfig.wifi_ip_addr);
    ui->label_wifi_nm_2->setText(systemconfig.wifi_net_mask);
    ui->label_wifi_gw_2->setText(systemconfig.wifi_gateway);
    ui->label_wifi_dns_2->setText(systemconfig.wifi_dns);

    ui->label_wifi_ssid_3->setText(systemconfig.wifi_ap_pswd);
    ui->label_wifi_pswd_3->setText(systemconfig.wifi_ap_ssid);

    w_log(c_user.name+",System Configuration Loaded");
}

void SystemSettingsWindow::on_pushButton_menu_clicked()
{
    w_log(c_user.name+",Closed System Settings");
    disconnect(this,SIGNAL(settings_updated()),mw,SLOT(settings_updated()));
    this->close();    
    mnw = new MenuWindow();
    mnw->setWindowFlags(Qt::FramelessWindowHint);
    mnw->showFullScreen();
}

void SystemSettingsWindow::on_pushButton_14_clicked()
{
    index--;
    if(index<(int)SETTING_P1)
    {
        index=(int)SETTING_P3;
    }
    settings = (settings_t)index;
    ui->stackedWidget->setCurrentIndex(index);

    switch(settings)
    {
        case SETTING_P1:
    {
        ui->groupBox->setTitle("GENERAL SETTINGS");
    }
        break;
    case SETTING_P2:
    {
        ui->groupBox->setTitle("ACCESS MANAGEMENT");
    }
        break;
    case SETTING_P3:
    {
        ui->groupBox->setTitle("NETWORK SETTINGS");
    }
        break;
    }
}

void SystemSettingsWindow::on_pushButton_13_clicked()
{
    index++;
    if(index>(int)SETTING_P3)
    {
        index=(int)SETTING_P1;
    }
    settings = (settings_t)index;
    ui->stackedWidget->setCurrentIndex(index);

    switch(settings)
    {
        case SETTING_P1:
    {
        ui->groupBox->setTitle("GENERAL SETTINGS");
    }
        break;
    case SETTING_P2:
    {
        ui->groupBox->setTitle("ACCESS MANAGEMENT");
    }
        break;
    case SETTING_P3:
    {
        ui->groupBox->setTitle("NETWORK SETTINGS");
    }
        break;
    }
}

void SystemSettingsWindow::iTimerTimeout()
{
    infoBox->setInformativeText("Auto close in "+QString::number(dCnt--)+"s");
    if(dCnt==0)
    {
        iTimer->stop();
    }
}

void SystemSettingsWindow::showInfoBox(QMessageBox::Icon icon, QString title, QString msg, int to)
{
    infoBox =  new QMessageBox();
    infoBox->setWindowTitle(title);
    infoBox->setIcon(icon);
    dCnt=to;
    infoBox->setText(msg);
    infoBox->setInformativeText("Auto close in "+QString::number(dCnt)+" s");
    iTimer = new QTimer();
    connect(iTimer,SIGNAL(timeout()),this,SLOT(iTimerTimeout()));
    iTimer->start(1000);
    infoBox->addButton(QMessageBox::Ok);
}

void SystemSettingsWindow::on_pushButton_19_toggled(bool checked)
{
    if(checked)
    {
        w_log(c_user.name+",Wi-Fi Switched ON");
        showInfoBox(QMessageBox::Information,"Wi-Fi "+ui->comboBox->currentText()+" Turned ON", "WI-Fi", 10);
        ui->pushButton_19->setText("Wi-Fi ON");
    }
    else
    {
        w_log(c_user.name+",Wi-Fi Switched OFF");
        showInfoBox(QMessageBox::Information,"Wi-Fi "+ui->comboBox->currentText()+" Turned OFF", "WI-Fi", 10);
        ui->pushButton_19->setText("Wi-Fi OFF");
    }
}

void SystemSettingsWindow::on_pushButton_clicked()
{
    setting_op=SETTING_COACH_ID;
    verify_password();
}

void SystemSettingsWindow::highlight_row(int row)
{
    selectedIndex=row;
    QModelIndex index = ui->tableView->model()->index(row, 0);
    ui->tableView->setCurrentIndex(index);
}


void SystemSettingsWindow::on_pushButton_12_clicked()
{
    //scroll  up
    if(scrlIdx>0)
    {
        scrlIdx--;
    }
    ui->tableView->verticalScrollBar()->setValue(scrlIdx);
    highlight_row(scrlIdx);
    update();
}

void SystemSettingsWindow::on_pushButton_15_clicked()
{
    //scroll  down
    if(scrlIdx<users.count())
    {
        scrlIdx++;
    }
    ui->tableView->verticalScrollBar()->setValue(scrlIdx);
    highlight_row(scrlIdx);
    update();
}

void SystemSettingsWindow::on_pushButton_10_clicked()
{
    // modify user
    setting_op=SETTING_USER_MODIFY;
    verify_password();
}

bool SystemSettingsWindow::verify_password()
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

void SystemSettingsWindow::receive_user(user usr)
{
    switch(setting_op)
    {
    case SETTING_USER_MODIFY:
        w_log(c_user.name+",Modified User: "+users.at(selectedIndex).name);
        users.replace(selectedIndex,usr);
        render_user_table();
        //save user to database
        write_config();
        emit settings_updated();
        showInfoBox(QMessageBox::Information,"Success", "User has been successfully modified!",10);
        break;
    case SETTING_USER_CREATE:
        w_log(c_user.name+",Modified User: "+usr.name);
        users.append(usr);
        render_user_table();
        //save user to database
        write_config();
        emit settings_updated();
        showInfoBox(QMessageBox::Information,"Success", "User has been successfully added!",10);
        break;
    case SETTING_USER_DEL:
    case SETTING_GPS_SRC:
    case SETTING_COACH_ID:
    case SETTING_COACH_NO:
    case SETTING_LED_DISPLAY:
    case SETTING_DISPLAY:
    case SETTING_VOLUME:
    case SETTING_WIFI:
        break;
    }
}

void SystemSettingsWindow::receive_text(QString str)
{
    disconnect(lineEditDialog,SIGNAL(text_entered(QString)),this,SLOT(receive_text(QString)));

    if(setting_op==SETTING_COACH_ID)
    {
        ui->label_coach_id->setText(str);
        w_log(c_user.name+",Modified Coach ID from "+systemconfig.coach_id+" to "+str);
        systemconfig.coach_id=str;
        showInfoBox(QMessageBox::Information,"Success", "Coach ID has been modified to "+str,10);
    }
    else
    if(setting_op==SETTING_COACH_NO)
    {
        ui->label_coach_no->setText(str);
        w_log(c_user.name+",Modified Coach No from "+systemconfig.coach_no+" to "+str);
        systemconfig.coach_no=str;
        showInfoBox(QMessageBox::Information,"Success", "Coach No has been modified to "+str,10);
    }

    write_config();
    emit settings_updated();
}

void SystemSettingsWindow::receive_password(QString password)
{
    disconnect(passwordDialog,SIGNAL(text_entered(QString)),this,SLOT(receive_password(QString)));
    if(c_user.password!=password)
    {     
        showInfoBox(QMessageBox::Warning,"Error", "Password entered is incorrect, Pls try again!",10);
    }
    else
    {
        // open option
        switch(setting_op)
        {
        case SETTING_USER_MODIFY:
        {
            userDialog = new UserDialog();
            userDialog->setModal(true);
            userDialog->setWindowFlags(Qt::FramelessWindowHint);
            userDialog->setTitle("Modify User");
            userDialog->setUser(users.at(selectedIndex));
#ifndef LAPTOP
            userDialog->show();
#else
            userDialog->showFullScreen();
#endif
            connect(userDialog,SIGNAL(user_entered(user)),this,SLOT(receive_user(user)));
        }
            break;
        case SETTING_USER_DEL:
        {
            write_config();
            emit settings_updated();
            update();

            dCnt=10;
            msgBox = new QMessageBox(this);
            msgBox->setIcon(QMessageBox::Question);
            msgBox->setWindowTitle("Delete User");
            msgBox->setText("Confirm Deletion of User: "+users.at(selectedIndex).name);
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

                w_log(c_user.name+",Deleted User "+ users.at(selectedIndex).name);
                users.removeAt(selectedIndex);
                render_user_table();
                showInfoBox(QMessageBox::Information,"Success", "User Deleted Succesfully!",10);
            }
            else
            {
                disconnect(popupTimer, SIGNAL(timeout()), this, SLOT(closeMessageBox()));
                popupTimer->stop();
                msgBox->close();
            }
        }
            break;
        case SETTING_USER_CREATE:
        {
            userDialog = new UserDialog();
            userDialog->setModal(true);
            userDialog->setWindowFlags(Qt::FramelessWindowHint);
            userDialog->setTitle("Create new user");
#ifndef LAPTOP
            userDialog->show();
#else
            userDialog->showFullScreen();
#endif
            connect(userDialog,SIGNAL(user_entered(user)),this,SLOT(receive_user(user)));

        }
            break;
        case SETTING_GPS_SRC:
        {
            systemconfig.gps_source = ui->comboBox_gps_src->currentText();
            w_log(c_user.name+",Changed GPS Source to "+ systemconfig.gps_source);
            write_config();
            showInfoBox(QMessageBox::Information,"GPS Source has been successfully changed", "Success", 10);
        }
            break;
        case SETTING_COACH_ID:
        {
            lineEditDialog = new LineEditDialog();
            lineEditDialog->setModal(true);
            lineEditDialog->setWindowFlags(Qt::FramelessWindowHint);
            lineEditDialog->setLabel("Enter Coach ID");
#ifndef LAPTOP
            lineEditDialog->show();
#else
            lineEditDialog->showFullScreen();
#endif
            connect(lineEditDialog,SIGNAL(text_entered(QString)),this,SLOT(receive_text(QString)));
        }
            break;
        case SETTING_COACH_NO:
        {
            lineEditDialog = new LineEditDialog();
            lineEditDialog->setModal(true);
            lineEditDialog->setWindowFlags(Qt::FramelessWindowHint);
            lineEditDialog->setLabel("Enter Coach No");
#ifndef LAPTOP
            lineEditDialog->show();
#else
            lineEditDialog->showFullScreen();
#endif
            connect(lineEditDialog,SIGNAL(text_entered(QString)),this,SLOT(receive_text(QString)));
        }
            break;
        case SETTING_LED_DISPLAY:
        {         
            switch((int)speedVal)
            {
            case 100:
                systemconfig.led_pis_speed = 1;
                break;
            case 50:
                systemconfig.led_pis_speed = 2;
                break;
            case 25:
                systemconfig.led_pis_speed = 3;
                break;
            case 12:
                systemconfig.led_pis_speed = 4;
                break;
            case 0:
                systemconfig.led_pis_speed = 5;
                break;
            }

            w_log(c_user.name+",Changed LED Scroll Speed: "+QString::number(systemconfig.led_pis_speed)+" col/s");
            write_config();
            emit settings_updated();
        }
            break;
        case SETTING_DISPLAY:
        {
            w_log(c_user.name+",Changed DVI Output to: "+ ui->comboBox_dvi_2->currentText());
            systemconfig.dvi_active = ui->comboBox_dvi_2->currentText();
            write_config();
            emit settings_updated();
        }
            break;
        case SETTING_VOLUME:
        {
            systemconfig.pa_vol = QString::number( ui->horizontalSlider_pa_vol->value());
            w_log(c_user.name+",Changed PA Volume to : "+ QString::number(ui->horizontalSlider_pa_vol->value()));
            system(QString(("amixer set 'Headphone' ")+QString::number(ui->horizontalSlider_pa_vol->value())).toLocal8Bit());
            write_config();
            emit settings_updated();
        }
            break;
        case SETTING_WIFI:
        {
            systemconfig.wifi_mode = ui->comboBox->currentText();
            w_log(c_user.name+",Changed Wi-Fi Mode to : "+ systemconfig.wifi_mode);
            write_config();
            emit settings_updated();
        }
            break;
        }
    }
    disconnect(passwordDialog,SIGNAL(text_entered(QString)),this,SLOT(receive_password(QString)));
}

void SystemSettingsWindow::on_pushButton_11_clicked()
{
    // delete user
    setting_op=SETTING_USER_DEL;
    verify_password();
}

void SystemSettingsWindow::on_tableView_clicked(const QModelIndex &index)
{
//    ui->pushButton_10->setDisabled(true);
//    ui->pushButton_11->setDisabled(true);
//    ui->pushButton_7->setDisabled(true);

    selectedIndex= index.row();
    selectedCol = index.column();
}

void SystemSettingsWindow::on_pushButton_8_clicked()
{
    //create user
    setting_op=SETTING_USER_CREATE;
    verify_password();
}

void SystemSettingsWindow::on_pushButton_2_clicked()
{
    //save gps source
    setting_op=SETTING_GPS_SRC;
    verify_password();
}

void SystemSettingsWindow::on_pushButton_3_clicked()
{
    // save display
    setting_op=SETTING_DISPLAY;
    verify_password();
}

void SystemSettingsWindow::on_pushButton_4_clicked()
{
    // save volume
    setting_op=SETTING_VOLUME;
    verify_password();
}

void SystemSettingsWindow::on_pushButton_5_clicked()
{
    //save font
    setting_op=SETTING_LED_DISPLAY;
    verify_password();
}

void SystemSettingsWindow::on_pushButton_6_clicked()
{
    // pa -
    int val =   ui->horizontalSlider_pa_vol->value();
    if(val>0)
    {
        val--;
    }
    ui->horizontalSlider_pa_vol->setValue(val);
}

void SystemSettingsWindow::on_pushButton_7_clicked()
{
    // pa +
    int val =   ui->horizontalSlider_pa_vol->value();
    if(val<127)
    {
        val++;
    }
    ui->horizontalSlider_pa_vol->setValue(val);
}

void SystemSettingsWindow::on_pushButton_16_clicked()
{
    if(speedVal>12.5)
    {
        speedVal=speedVal/2;
    }
    else
    {
        speedVal = 0;
    }
    ui->label_scroll_speed->setText(QString::number((int)speedVal)+"col/s");
}

void SystemSettingsWindow::on_pushButton_9_clicked()
{
    if(speedVal==0)
    {
       speedVal=12.5;
    }
    else
    if(speedVal<100)
    {
        speedVal=speedVal*2;
    }
    ui->label_scroll_speed->setText(QString::number((int)speedVal)+"col/s");
}

void SystemSettingsWindow::on_pushButton_17_clicked()
{
    setting_op=SETTING_COACH_NO;
    verify_password();
}

void SystemSettingsWindow::on_pushButton_20_clicked()
{
    // scroll to top
    if(scrlIdx>0)
    {
        scrlIdx=0;
    }
    ui->tableView->verticalScrollBar()->setValue(scrlIdx);
    highlight_row(scrlIdx);
    update();
}

void SystemSettingsWindow::on_pushButton_18_clicked()
{
    // scroll to bottom
    if(scrlIdx<users.count())
    {
        scrlIdx=users.count()-1;
    }
    ui->tableView->verticalScrollBar()->setValue(scrlIdx);
    highlight_row(scrlIdx);
    update();
}
