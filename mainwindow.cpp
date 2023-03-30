#include "mainwindow.h"
#include "ui_mainwindow.h"
extern access_level_t access_level;
extern LoginDialog *ld;
extern MenuWindow *mnw;
extern SplashWindow *spw;
extern AlarmWindow *aw;
extern data papis_data;
extern QList <slave*> papis_slaves;
extern toiletmanagerthread  *toiletThread;
extern SlaveSyncThread *slaveSyncThread;
extern slaveDriverThread *slaveDriver;
extern subsystemthread *subsysThread;
extern  slave_tasks_t    slave_task;
extern const char* ptr_ddb_sl;
extern const char* ptr_ddb_dl;
extern const char* ptr_led_pis;
extern QString lcd_pis_text;
extern systemconfig systemconfig;
//extern int inactive;
extern papis_sequence_t papis_sequence;

//extern QList <run*> kmRun;
extern routes routesInfo;
extern QList <routeData*> route_data;
extern QString base_dir;

extern QString l_log;
extern QString l_fault;


extern ddc_mode_t ddc_mode;
extern QList <QString> keypad_arr;

audioplayerthread *aplayer;

#define RS485_IO        38
#define OP_ON           1
#define OP_OFF          0
#define PA_SWITCH       6

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    sw_ver="0.01.004";
    //    ui->label_version->setText(sw_ver);
    w_log(",,MPU Software Version: "+sw_ver);

    slaveSyncThread = new SlaveSyncThread();
    connect(slaveSyncThread,SIGNAL(eth_present()),this,SLOT(eth_present()));
    connect(slaveSyncThread,SIGNAL(eth_absent()),this,SLOT(eth_absent()));
    connect(slaveSyncThread,SIGNAL(eth_up()),this,SLOT(eth_up()));
    connect(slaveSyncThread,SIGNAL(eth_down()),this,SLOT(eth_down()));
    connect(slaveSyncThread,SIGNAL(eth_con()),this,SLOT(eth_con()));
    connect(slaveSyncThread,SIGNAL(eth_dis()),this,SLOT(eth_dis()));
    connect(slaveSyncThread,SIGNAL(init_done()),this,SLOT(slave_init_done()));
    slaveSyncThread->update_config(systemconfig);
    slave_task = SLAVE_INIT;
    //    slaveSyncThread->start();//

    //    slaveDriver = new slaveDriverThread();
    //    connect(this,SIGNAL(rs485_drive_matrix(s_driver_tasks_t,const char*,int,dispType_t,scrlDir_t,day_night_mode_t )),
    //            slaveDriver,SLOT(rs485_drive_matrix(s_driver_tasks_t,const char*,int,dispType_t,scrlDir_t,day_night_mode_t )));

    //    connect(this,SIGNAL(rs485_emergency_ack(int)),slaveDriver,SLOT(rs485_emergency_ack(int)));
    //    connect(this, SIGNAL(rs485_scan_all(QList <slave*>)),slaveDriver, SLOT(rs485_scan_all(QList <slave*>)));
    //    connect(this, SIGNAL(rs485_scan(int)),slaveDriver, SLOT(rs485_scan(int)));
    //    connect(slaveDriver,SIGNAL(message_pass(QString)),this,SLOT(message_pass(QString)));
    //    connect(slaveDriver,SIGNAL(toilet_status(toilet_stat)),this,SLOT(toilet_status(toilet_stat)),Qt::BlockingQueuedConnection);
    //    slaveDriver->start();


    subsysThread = new subsystemthread();
    connect(subsysThread,SIGNAL(ss_present()),this,SLOT(ss_present()));
    connect(subsysThread,SIGNAL(ss_absent()),this,SLOT(ss_absent()));
    connect(subsysThread,SIGNAL(lte_present()),this,SLOT(lte_present()));
    connect(subsysThread,SIGNAL(lte_absent()),this,SLOT(lte_absent()));

    connect(subsysThread,SIGNAL(pow_lo()),this,SLOT(pow_lo()));
    connect(subsysThread,SIGNAL(pow_hi()),this,SLOT(pow_hi()));
    connect(subsysThread,SIGNAL(pow_ok()),this,SLOT(pow_ok()));
    connect(subsysThread,SIGNAL(mic_in()),this,SLOT(mic_in()));
    connect(subsysThread,SIGNAL(mic_out()),this,SLOT(mic_out()));
    //    subsysThread->start();

    toiletThread = new toiletmanagerthread();

    connect(toiletThread,SIGNAL(message_pass(QString)),this,SLOT(message_pass(QString)));
    connect(toiletThread,SIGNAL(toilet_status(int,toilet_packet_t)),this,SLOT(toilet_status(int,toilet_packet_t)),Qt::BlockingQueuedConnection);

    wake=true;

#ifndef LAPTOP
    //    wifi
    system(QString("echo 1 > /sys/bus/usb/devices/1-1.1.1/bConfigurationValue").toLocal8Bit());
    //    gsm
    system(QString("echo 1 > /sys/bus/usb/devices/1-1.1.2/bConfigurationValue").toLocal8Bit());
#endif

    load_info();

    day_night_mode=MODE_DAY;
    //    ui->label_mode->setStyleSheet("image: url(:/images/icons/icon_day.png);border-color: rgb(255, 255, 255);");

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
    ui->label_datetime->setText(QDateTime::currentDateTime().toString("ddd dd.MM.yy hh:mm:ss"));
    //    timer->start(1000);

    connect(this,SIGNAL(slave_route_send()),this,SLOT(slave_send_route()));
    connect(this,SIGNAL(slave_distance_send(double)),this,SLOT(slave_send_distance(double)));
    connect(this,SIGNAL(slave_alert_send(int)),this,SLOT(slave_send_alert(int)));
    connect(this,SIGNAL(slave_clear_send(int)),this,SLOT(slave_send_clear(int)));
    connect(this,SIGNAL(slave_return_send()),this,SLOT(slave_send_return()));
    connect(this,SIGNAL(slave_error_send()),this,SLOT(slave_send_error()));
    connect(this,SIGNAL(slave_mode_send(QString)),this,SLOT(slave_send_mode(QString)));
    connect(this,SIGNAL(slave_message_send()),this,SLOT(slave_send_message()));

    //initialize font objects
    font_hi =new QFont(QFontDatabase::applicationFontFamilies(
                           QFontDatabase::addApplicationFont(":/fonts/HINDI/NotoSans-Regular.ttf")).at(0),10);
    font_en =new QFont(QFontDatabase::applicationFontFamilies(
                           QFontDatabase::addApplicationFont(":/fonts/ENGLISH/OpenSans-Light.ttf")).at(0),10);
    font_ban=new QFont(QFontDatabase::applicationFontFamilies(
                           QFontDatabase::addApplicationFont(":/fonts/BANGLA/HindSiliguri-Regular.ttf")).at(0),10);
    font_guj=new QFont(QFontDatabase::applicationFontFamilies(
                           QFontDatabase::addApplicationFont(":/fonts/GUJARATI/HindVadodara-Regular.ttf")).at(0),9);
    font_kan=new QFont(QFontDatabase::applicationFontFamilies(
                           QFontDatabase::addApplicationFont(":/fonts/KANNADA/BalooTamma2-Regular.ttf")).at(0),10);
    font_mal=new QFont(QFontDatabase::applicationFontFamilies(
                           QFontDatabase::addApplicationFont(":/fonts/MALAYALAM/BalooChettan2-Regular.ttf")).at(0),10);
    font_odia=new QFont(QFontDatabase::applicationFontFamilies(
                            QFontDatabase::addApplicationFont(":/fonts/ODIA/BalooBhaina2-Regular.ttf")).at(0),10);
    font_pun=new QFont(QFontDatabase::applicationFontFamilies(
                           QFontDatabase::addApplicationFont(":/fonts/PUNJABI/MuktaMahee-Regular.ttf")).at(0),11);
    font_tam=new QFont(QFontDatabase::applicationFontFamilies(
                           QFontDatabase::addApplicationFont(":/fonts/TAMIL/BalooThambi2-Regular.ttf")).at(0),10);
    font_tel=new QFont(QFontDatabase::applicationFontFamilies(
                           QFontDatabase::addApplicationFont(":/fonts/TELUGU/HindGuntur-Regular.ttf")).at(0),10);
    font_urdu=new QFont(QFontDatabase::applicationFontFamilies(
                            QFontDatabase::addApplicationFont(":/fonts/URDU/asunaskh.ttf")).at(0),10);

    font_hi->setStyleStrategy(QFont::NoAntialias);
    font_en->setStyleStrategy(QFont::NoAntialias);
    font_ban->setStyleStrategy(QFont::NoAntialias);
    font_guj->setStyleStrategy(QFont::NoAntialias);
    font_kan->setStyleStrategy(QFont::NoAntialias);
    font_mal->setStyleStrategy(QFont::NoAntialias);
    font_odia->setStyleStrategy(QFont::NoAntialias);
    font_pun->setStyleStrategy(QFont::NoAntialias);
    font_tam->setStyleStrategy(QFont::NoAntialias);
    font_tel->setStyleStrategy(QFont::NoAntialias);
    font_urdu->setStyleStrategy(QFont::NoAntialias);

    //    ui->label_ddb_dl_p1->setFont(*font_en);
    //    ui->label_ddb_sl_p1->setFont(*font_en);

    //    ui->label_ddb_dl_p2->setFont(*font_en);
    //    ui->label_ddb_sl_p2->setFont(*font_en);
    //    //ui->label_led_pis->setFont(*font_en);

    //    ui->label_ddb_dl_p1->setText("");
    //    ui->label_ddb_dl_p2->setText("");
    //    ui->label_ddb_sl_p1->setText("");
    //    ui->label_ddb_sl_p2->setText("");
    //    //ui->label_led_pis->setText("");
    ui->stackedWidget->setCurrentIndex(0);

    aplayer = new audioplayerthread();
    connect(this,SIGNAL(audio_add(QString)),aplayer,SLOT(add(QString)));
    connect(this,SIGNAL(audio_add(QString, announcementType_t)),aplayer,SLOT(add(QString, announcementType_t)));
    connect(this,SIGNAL(audio_play()),aplayer,SLOT(play()));
    aplayer->start();

    alarmSrcList=new QList<int>();
    update();

    //try to connect with bmu

    //tcpserver for lcd pis
    //    tcpServer = new QTcpServer(this);
    //    tcpServer->listen(QHostAddress::Any,3700);
    //    connect(tcpServer,SIGNAL(newConnection()),this,SLOT(client_connected()));


    //    sock = new QTcpSocket();
    //    QString ip="192.168.1.161";
    //    sock->connectToHost(ip,3670);

    //    if(sock->waitForConnected(3000))
    //    {
    //        QString msg = "*dt#";
    //        sock->write(msg.toLocal8Bit());
    //        connect(sock,SIGNAL(readyRead()),this,SLOT(serverReadyRead()));
    //    }
    //    else
    //    {

    //    }

    //emu-memu specific
    pjsipListener = new QTcpServer(this);
    pjsipListener->listen(QHostAddress::Any,1999);
    connect(pjsipListener,SIGNAL(newConnection()),this,SLOT(pjsip_connected()));

    callTimer = new QTimer(this);
    connect(callTimer, SIGNAL(timeout()), this, SLOT(callTimerTimeout()));
    callTimeCtr = 0;
    ui->label_call_duration->setText("");

    //pjsip will connect with this itself. when pjsip connects, system connects back using pjsipSocket



    //TBD: this has to be automted
    ui->pushButton_select_etb1->setDisabled(false);
    ui->pushButton_select_etb2->setDisabled(false);
    ui->pushButton_select_etb3->setDisabled(true);
    ui->pushButton_select_etb4->setDisabled(true);
    ui->pushButton_select_etb5->setDisabled(true);
    ui->pushButton_select_etb6->setDisabled(true);
    ui->pushButton_select_etb7->setDisabled(true);
    ui->pushButton_select_etb8->setDisabled(true);
    ui->pushButton_select_etb9->setDisabled(true);
    ui->pushButton_select_etb10->setDisabled(true);
    ui->pushButton_select_etb11->setDisabled(true);
    ui->pushButton_select_etb12->setDisabled(true);
    ui->pushButton_select_etb13->setDisabled(true);
    ui->pushButton_select_etb14->setDisabled(true);
    ui->pushButton_select_etb15->setDisabled(true);
    ui->pushButton_select_etb16->setDisabled(true);
    ui->pushButton_select_etb17->setDisabled(true);
    ui->pushButton_select_etb18->setDisabled(true);
    ui->pushButton_select_etb19->setDisabled(true);
    ui->pushButton_select_etb20->setDisabled(true);
    ui->pushButton_select_etb_dg->setDisabled(false);
    ui->pushButton_select_etb_occ->setDisabled(true);

    //TBD: temporarily hiding sip status
    ui->label_sip_server_status->hide();
    ui->label_sip_peers->hide();
    ui->label_sip_status->hide();

    //volume controls
    ui->label_cabmic_vol->setText(QString::number(cab_mic_vol)+"%");
    ui->label_cabspk_vol->setText(QString::number(cab_spk_vol)+"%");
    ui->label_paspkr_vol->setText(QString::number(pa_spk_vol)+"%");

    ui->stackedWidget_announcements->setCurrentIndex(1);

    load_prerec_files();

    this->installEventFilter(this);
    connect(this,SIGNAL(keypressed(keypad_press_t)),this,SLOT(keypressed_slot(keypad_press_t)));
}


void MainWindow::keypressed_slot(keypad_press_t keypress)
{
    switch(keypress)
    {
    case KEY_MS:
    {
        switch(ddc_mode)
        {
        case DDC_MODE_MASTER:
            ddc_mode = DDC_MODE_SLAVE;
            ui->label_ms->setText(" DDC SLAVE");
            break;
        case DDC_MODE_SLAVE:
            ddc_mode = DDC_MODE_MASTER;
            ui->label_ms->setText(" DDC MASTER");
            break;
        }
    }
        break;
    case KEY_AS: {}break;
    case KEY_AD: {}break;
    case KEY_CALL: {}break;
    case KEY_CALL_DG: {}break;
    case KEY_CALL_OC: {}break;
    case KEY_CALL_INT: {}break;
    case KEY_CALL_VU: {}break;
    case KEY_CALL_VD: {}break;
    case KEY_CALL_MUTE: {}break;
    case KEY_CALL_REDIAL: {}break;
    case KEY_NUM_1: {}break;
    case KEY_NUM_2: {}break;
    case KEY_NUM_3: {}break;
    case KEY_NUM_4: {}break;
    case KEY_NUM_5: {}break;
    case KEY_NUM_6: {}break;
    case KEY_NUM_7: {}break;
    case KEY_NUM_8: {}break;
    case KEY_NUM_9: {}break;
    case KEY_NUM_0: {}break;
    case KEY_NUM_AST: {}break;
    case KEY_NUM_HASH: {}break;
    case KEY_PLAY: {}break;
    case KEY_UP: {}break;
    case KEY_PAUSE: {}break;
    case KEY_LEFT: {}break;
    case KEY_OK: {}break;
    case KEY_RIGHT: {}break;
    case KEY_STOP: {}break;
    case KEY_DOWN: {}break;
    case KEY_BACK: {}break;
    case KEY_VU: {}break;
    case KEY_VD: {}break;
    case KEY_MENU: {}break;
    case KEY_SPEAK: {}break;
    case KEY_ALARM: {}break;
    case KEY_SOS: {}break;
    case KEY_DELAY: {}break;
    case KEY_PR:
    {
        ui->label_platform->setText("PLATFORM RIGHT");
        emit audio_add(base_dir+"/prerec_audio/7_eng_platform_right.mp3",ANN_NETWORK);
        emit audio_add(base_dir+"/prerec_audio/8_hin_platform_right.mp3",ANN_NETWORK);
        emit audio_play();
    }
        break;
    case KEY_PL:
    {
        ui->label_platform->setText("PLATFORM LEFT");
        emit audio_add(base_dir+"/prerec_audio/5_eng_platform_left.mp3",ANN_NETWORK);
        emit audio_add(base_dir+"/prerec_audio/6_hin_platform_left.mp3",ANN_NETWORK);
        emit audio_play();
    }
        break;
    default: {}break;
    }
}

bool MainWindow::eventFilter( QObject* object, QEvent* event)
{
    if ( event->type() == QEvent::KeyPress ) {
        // special processing for key press
        QKeyEvent *k = (QKeyEvent *)event;
        qDebug( "Ate key press %d", k->key() );

        if(k->key()==16777248)
        {

        }
        else
            if(keystr.size()==0 && k->key()=='#')
            {
                keystr.append(QChar(k->key()));
            }
            else
                if(keystr.size()>0)
                {
                    keystr.append(QChar(k->key()));

                    if(k->key()=='*')
                    {
                        //catch key here
                        int sz = keypad_arr.count();

                        for(int idx=0;idx<keypad_arr.count();idx++)
                        {
                            if(keystr==keypad_arr[idx])
                            {
                                emit keypressed((keypad_press_t)idx);
                                break;
                            }
                        }

                        ui->label_keypad_debug->setText(keystr);
                        keystr="";
                    }
                }

        //        ui->label_keypad_debug->setText(keystr);
        return TRUE; // eat event
    } else {
        // standard event processing
        return FALSE;
    }
}

void MainWindow::pjsip_connected()
{
    pjsipLocalConnection = pjsipListener->nextPendingConnection();
    connect(pjsipLocalConnection, &QAbstractSocket::disconnected,
            pjsipLocalConnection, &QObject::deleteLater);

    connect(pjsipLocalConnection,SIGNAL(readyRead()),this,SLOT(pjsip_ready_read()));


    //    pjsipSocket = new QTcpSocket();
    //    pjsipSocket->connectToHost(QHostAddress(QHostAddress::LocalHost),1998);
    //    if(pjsipSocket->waitForConnected(3000))
    //    {
    //        //
    //        QString msg = "*ak#";
    //        sock->write(msg.toLocal8Bit());
    //    }
    //    else
    //    {
    //        //no connection available
    //    }
}

void MainWindow::pjsip_ready_read()
{
    QByteArray block = pjsipLocalConnection->readAll();

    if(block.contains("Hello"))
    {
        QString msg = "World";
        pjsipLocalConnection->write(msg.toLocal8Bit());

        //        QString str = QString::fromStdString(block.toStdString());
        //        w_log(",LCD_PIS:"+str);
    }
    else if(block.contains("Incoming"))
    {

        ui->label_call_status->setText(QString(block));
        ui->label_emergency->setStyleSheet(QString("image: url(:/images/icons/icon_siren.png);background-color: rgb(238, 238, 236);"));
        ui->pushButton_select_etb1->setStyleSheet(QString("background-color: rgb(255, 255, 0);"));

        //Sending String to START CCTV Feed on HMI
        etbCallSocket = new QTcpSocket();
        etbCallSocket->connectToHost("192.168.1.70",1999);
        if(etbCallSocket->waitForConnected(3000))
        {
            QString msg = "ST:";
            etbCallSocket->write(msg.toLocal8Bit());
        }

        etbCallSocket->disconnect();
        etbCallSocket->close();
    }
}

void MainWindow::serverReadyRead()
{
    QByteArray datas=sock->readAll();

    sock->disconnect();
    sock->close();
}

void MainWindow::client_connected()
{
    clientConnection = tcpServer->nextPendingConnection();
    connect(clientConnection, &QAbstractSocket::disconnected,
            clientConnection, &QObject::deleteLater);

    connect(clientConnection,SIGNAL(readyRead()),this,SLOT(server_ready_read()));
}

void MainWindow::server_ready_read()
{
    QByteArray block = clientConnection->readAll();

    if(block.contains("log="))
    {
        QString str = QString::fromStdString(block.toStdString());
        w_log(",LCD_PIS:"+str);
    }
}

void MainWindow::slave_init_done()
{
    if(papis_slaves.count()>0)
    {
        pd = new QProgressDialog("Scanning Ethernet Slaves", "Cancel", 0, 100);
        pd->setWindowFlags(Qt::FramelessWindowHint);
        pd->setModal(true);
        connect(slaveSyncThread,SIGNAL(slave_scan_progress_eth(int,QList <slave*>)),
                this,SLOT(slave_scan_progress_eth(int,QList <slave*>)),Qt::BlockingQueuedConnection);
        connect(this, SIGNAL(eth_scan_all(QList <slave*>)),slaveSyncThread, SLOT(eth_scan_all(QList <slave*>)));
        emit eth_scan_all(papis_slaves);
    }
    else
    {
        gain_consciousness();
    }
}

void MainWindow::tmsTimeout()
{
#ifndef LAPTOP
    //    gpio.gpio_set_value(RS485_IO,OP_OFF);
#endif
    offTimer->start(1);
}

void MainWindow::toilet_status(int toilet, toilet_packet_t t_stat)
{    
    switch(toilet)
    {
    case 1:
        if(t_stat.emergency1)
        {
            //            ui->label_t1A_emergency->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
            alarmSource=1;
        }
        else
        {
            //            ui->label_t1A_emergency->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
        }

        if(t_stat.emergency2)
        {
            //            ui->label_t1B_emergency->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
            alarmSource=2;
        }
        else
        {
            //            ui->label_t1B_emergency->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
        }

        if(t_stat.occupancy1)
        {
            //            ui->label_t1A_occupancy->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
            tstat1a="1";
        }
        else
        {
            //            ui->label_t1A_occupancy->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
            tstat1a="0";
        }

        if(t_stat.occupancy2)
        {
            //            ui->label_t1B_occupancy->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
            tstat1b="1";
        }
        else
        {
            //            ui->label_t1B_occupancy->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
            tstat1b="0";
        }

        break;
    case 2:
        if(t_stat.emergency1)
        {
            //            ui->label_t2A_emergency->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
            alarmSource=3;
        }
        else
        {
            //            ui->label_t2A_emergency->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
        }

        if(t_stat.emergency2)
        {
            //            ui->label_t2B_emergency->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
            alarmSource=4;
        }
        else
        {
            //            ui->label_t2B_emergency->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
        }

        if(t_stat.occupancy1)
        {
            //            ui->label_t2A_occupancy->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
            tstat2a="1";
        }
        else
        {
            //            ui->label_t2A_occupancy->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
            tstat2a="0";
        }

        if(t_stat.occupancy2)
        {
            //            ui->label_t2B_occupancy->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
            tstat2b="1";
        }
        else
        {
            //            ui->label_t2B_occupancy->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
            tstat2b="0";
        }
        break;
    }

    //SK: toilet stat sent to LCD Pis
    update_occ(tstat1a+tstat1b+tstat2a+tstat2b);

    if(!tms_alarm)
    {
        if(t_stat.emergency1||t_stat.emergency2)
        {
            //start alarm
            tms_alarm=true;

            //enqueue attention alarm
            emit audio_add(base_dir+"/sounds/attention.mp3");
            emit audio_play();

            aw = new AlarmWindow();
            alarmSrcList->append((int)alarmSource);
            aw->setNum(alarmSrcList);
            aw->setWindowFlags(Qt::FramelessWindowHint);
            connect(aw, SIGNAL(acknowledge(int)),this,SLOT(alarm_ack(int)));
            aw->showFullScreen();
            emit slave_alert_send((int)alarmSource);
            w_log(",,Alarm Indication Started in MPU for Toilet-"+QString::number(alarmSource));
        }
        else
        {
            //clear alarm

        }
    }

    //    if(!tms_alarm)
    //    {
    //        if(t_stat.sw==0/*1*/)
    //        {
    //            switch(t_stat.addr)
    //            {
    //            case 1:
    //                        ui->label_t1_emergency->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
    //                break;
    //            case 2:
    //                        ui->label_t2_emergency->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
    //                break;
    //            case 3:
    //                        ui->label_t3_emergency->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
    //                break;
    //            case 4:
    //                        ui->label_t4_emergency->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
    //                break;
    //            }

    //            if(tms_alarm==false)
    //            {
    //                aw = new AlarmWindow();
    //                alarmSrcList->append((int)t_stat.addr);
    //                aw->setNum(alarmSrcList);
    //                aw->setWindowFlags(Qt::FramelessWindowHint);
    //                connect(aw, SIGNAL(acknowledge(int)),this,SLOT(alarm_ack(int)));
    //                aw->showFullScreen();
    //                tms_alarm=true;
    //                emit slave_alert_send((int)t_stat.addr);
    //                w_log(",,Alarm Indication Started in MPU for Toilet-"+QString::number(t_stat.addr));
    //            }
    //            else
    //            {
    //                if(!alarmSrcList->contains((int)t_stat.addr))
    //                {
    //                    alarmSrcList->append((int)t_stat.addr);
    //                    aw->setNum(alarmSrcList);
    //                    w_log(",,Alarm Indication Started in MPU for Toilet-"+QString::number(t_stat.addr));
    //                }
    //            }
    //        }
    //        else
    //        {
    ////            if(tms_alarm==true)
    ////            {
    ////                for(int idx=0;idx<alarmSrcList->count();idx++)
    ////                {
    ////                    if((int)t_stat.addr==alarmSrcList->at(idx))
    ////                    {
    ////                        alarmSrcList->removeAt(idx);
    ////                        emit slave_clear_send((int)t_stat.addr);
    ////                        w_log(",,Emergency Alarm Stopped for Toilet-"+QString::number(t_stat.addr));
    ////                         aw->close();
    ////                         alarmSrcList->clear();
    ////                    }
    ////                }

    ////                if(alarmSrcList->count()==0)
    ////                {
    ////                    alarmSrcList->clear();
    ////                    tms_alarm=false;
    //////                    aw->close();
    ////                    w_log(",,All Emergency Alarm(s) Stopped");
    ////                }
    ////            }

    ////            tms_alarm=false;
    ////            aw->close();
    ////            alarmSrcList->clear();
    ////            switch(t_stat.addr)
    ////            {
    ////            case 1:
    ////                        ui->label_t1_emergency->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
    ////                break;
    ////            case 2:
    ////                        ui->label_t2_emergency->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
    ////                break;
    ////            case 3:
    ////                        ui->label_t3_emergency->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
    ////                break;
    ////            case 4:
    ////                        ui->label_t4_emergency->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
    ////                break;
    ////            }
    //        }
    //    }
}

void MainWindow::alarm_ack(int num)
{
    QString ip;
    for(int idx=0;idx<alarmSrcList->count();idx++)
    {
        if(alarmSrcList->at(idx)==1||alarmSrcList->at(idx)==2)
        {
            ip="192.168.1.161";
        }
        else
        {
            ip="192.168.1.162";
        }
        sock = new QTcpSocket();
        sock->connectToHost(ip,24);
        if(sock->waitForConnected(3000))
        {
            QString msg = "*ak#";
            sock->write(msg.toLocal8Bit());
        }
        else
        {

        }
    }

    //    for(int idx=0;idx<alarmSrcList->count();idx++)
    //    {
    //        emit rs485_emergency_ack(60+alarmSrcList->at(idx));
    //    }
    w_log(",Emergency Alarm Stopped");
    disconnect(aw, SIGNAL(acknowledge(int)),this,SLOT(alarm_ack(int)));

    tms_alarm=false;
    aw->close();
    alarmSrcList->clear();
    emit slave_clear_send((int)alarmSource);

    //    ui->label_t1A_emergency->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
    //    ui->label_t2B_emergency->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
    //    ui->label_t1A_emergency->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
    //    ui->label_t2B_emergency->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");

}

void MainWindow::showTime()
{
    ui->label_datetime->setText(QDateTime::currentDateTime().toString("ddd dd.MM.yy hh:mm:ss"));

    QTime ct = QTime::currentTime();
    if(!mask_dn)
    {
        if((ct.hour()>=22 && ct.minute()>=0)||(ct.hour()<6 && ct.minute()<=59))
        {
            if((day_night_mode==MODE_DAY)&&(!emergency))/*if((day_night_mode==MODE_DAY)||(!emergency))*/
            {
                day_night_mode=MODE_NIGHT;
                emit slave_mode_send("n");
                //                ui->label_mode->setStyleSheet("image: url(:/images/icons/icon_night.png);border-color: rgb(255, 255, 255);");
            }
        }
        else
        {
            if(day_night_mode==MODE_NIGHT)
            {
                day_night_mode=MODE_DAY;
                emit slave_mode_send("d");
                //                ui->label_mode->setStyleSheet("image: url(:/images/icons/icon_day.png);border-color: rgb(255, 255, 255);");
            }
        }
    }
    update();
}

void MainWindow::settings_updated()
{
    //    ui->label_coach->setText(systemconfig.coach_id+" , "+systemconfig.coach_no);
}

void MainWindow::ddbCycleTimeout()
{
    ai=(int)ddb_cycle;
    ai++;
    ddb_cycle=(ddb_cycle_t)ai;
    emit ddb_drive();
    if(ddb_cycle==DDB_RL_VIA)
    {
        ddb_cycle = DDB_HI_ROUTE_1;
    }

    if(slogan_play)
    {
        emit pis_slogan_drive();
        slogan_idx++;
        if(slogan_idx==aRouteData->slogans.count())
        {
            slogan_idx=0;
        }
    }
}

void MainWindow::pow_lo()
{
    w_log(",Power Low Detected");
    //    //ui->label_pwr->setText("L");
    //    //ui->label_pwr->setStyleSheet("color: rgb(255, 255, 255);border-color: rgb(255, 255, 255);background-color: rgb(255, 0, 0);");
    update();
}
void MainWindow::pow_hi()
{
    w_log(",Power High Detected");
    //ui->label_pwr->setText("H");
    //ui->label_pwr->setStyleSheet("color: rgb(255, 255, 255);border-color: rgb(255, 255, 255);background-color: rgb(255, 0, 0);");
    update();
}
void MainWindow::pow_ok()
{
    w_log(",Power OK Detected");
    //ui->label_pwr->setText("");
    //ui->label_pwr->setStyleSheet("color: rgb(255, 255, 255);image: url(:/images/icons/check.png);border-color: rgb(255, 255, 255);");
    update();
}
void MainWindow::mic_in()
{    
    //    #ifndef LAPTOP
    //    gpio.gpio_set_value(PA_SWITCH,OP_ON);
    //    #endif
    if(!tms_alarm)
        if(!emergency)
        {
            system("amixer set 'Headphone' 1");
            w_log(",Driver Started Emergency Announcement");
            ui->label_mic->setStyleSheet("color: rgb(255, 255, 255);image: url(:/images/icons/icon_driver.png);border-color: rgb(255, 255, 255);");
            //ui->label_led_pis->setFont(*font_hi);
            //ui->label_led_pis->setText("Attention: Emergency Message   कृपया ध्यान दें: आपातकालीन संदेश");
            // ui->label_lcd_pis->setText("Attention: Emergency Message   कृपया ध्यान दें: आपातकालीन संदेश");
            update();
            drive_pis();
            emit slave_message_send();
            emergency=true;
        }
}
void MainWindow::mic_out()
{
    if(!tms_alarm)
        if(emergency)
        {
            system("amixer set 'Headphone' 127");
            //ui->label_led_pis->setFont(*font_hi);
            w_log(",Driver Stopped Emergency Announcement");
            emergency=false;
            //ui->label_led_pis->setText("");
            // ui->label_lcd_pis->setText("");
            ui->label_mic->setStyleSheet("color: rgb(255, 255, 255);border-color: rgb(255, 255, 255);");
            update();
            drive_pis();
            emit slave_message_send();
        }
    if(!announcement_ongoing)
    {
        //  gpio.gpio_set_value(PA_SWITCH,OP_OFF);
    }
}

void MainWindow::ss_present()
{
    //ui->label_subsystem->setStyleSheet("image: url(:/images/icons/icon_ss.png);");
    //ui->label_rs485->setStyleSheet("image: url(:/images/icons/icon_rs485.png);");
    w_log(",Subsystem Initialized");
}

void MainWindow::ss_absent()
{
    //ui->label_subsystem->setStyleSheet("image: url(:/images/icons/icon_att.png);");
    //ui->label_rs485->setStyleSheet("image: url(:/images/icons/icon_att.png);");
    w_fault(",Subsystem Not Detected");
}

void MainWindow::lte_present()
{
    ui->label_lte->setStyleSheet("image: url(:/images/icons/icon_lte.png);");
    w_log(",LTE Detected");
}

void MainWindow::lte_absent()
{
    ui->label_lte->setStyleSheet("image: url(:/images/icons/icon_att.png);");
    w_fault(",LTE Not Detected");
}

void MainWindow::eth_present()
{
    ui->label_eth->setStyleSheet("image: url(:/images/icons/icon_eth_dis.png);");
}
void MainWindow::eth_absent()
{
    w_fault(",Ethernet Not Detected");
    ui->label_eth->setStyleSheet("");
}
void MainWindow::eth_up()
{
    ui->label_eth->setStyleSheet("image: url(:/images/icons/icon_eth_con.png);");
}
void MainWindow::eth_down()
{
    w_fault(",Ethernet Down");
    ui->label_eth->setStyleSheet("image: url(:/images/icons/icon_eth_dis.png);");
}
void MainWindow::eth_con()
{
    ui->label_eth->setStyleSheet("image: url(:/images/icons/icon_eth_con.png);");
}
void MainWindow::eth_dis()
{
    w_fault(",Ethernet Not Connected");
    ui->label_eth->setStyleSheet("image: url(:/images/icons/icon_eth_dis.png);");
}

void MainWindow::slave_scan_silent(int pc, QList <slave*> slaves)
{
    papis_slaves=slaves;
    if(pc==100)
    {
        render_dashboard();
        w_log(",Slave Scan Completed in Background");
        //show slave status
        //        ui->stackedWidget_2->setCurrentIndex(1);
        slaveTimer=new QTimer();
        connect(slaveTimer,SIGNAL(timeout()),this,SLOT(slaveTimeout()));
        slaveTimer->start(20000);
        disconnect(slaveSyncThread,SIGNAL(slave_scan_progress_eth(int,QList <slave*>)),this,SLOT(slave_scan_silent(int,QList <slave*> slaves)));
    }
}

void MainWindow::slave_scan_progress_eth(int pc,QList <slave*> slaves)
{
    papis_slaves=slaves;
    pd->setValue(pc);
    if(pc==100)
    {
        disconnect(slaveSyncThread,SIGNAL(slave_scan_progress_eth(int,QList <slave*>)),
                   this,SLOT(slave_scan_progress_eth(int,QList <slave*>)));

        pd->close();
        render_dashboard();

        if(!toiletThreadStarted)
        {
            toiletThreadStarted=true;
            toiletThread->start();
        }

#ifndef SKIP_RS485
        pd = new QProgressDialog("Scanning RS485 Slaves", "Cancel", 0, 100);
        pd->setWindowFlags(Qt::FramelessWindowHint);
        pd->setModal(true);
        is_pd=true;
        connect(slaveDriver,SIGNAL(slave_silent_scan(QList <slave*>)),this,SLOT(slave_silent_scan(QList <slave*>)),Qt::BlockingQueuedConnection);
        connect(slaveDriver,SIGNAL(slave_scan_progress(int,QList <slave*>)),this,SLOT(slave_scan_progress_rs485(int,QList <slave*>)),Qt::BlockingQueuedConnection);
        emit rs485_scan_all(papis_slaves);
#else
        gain_consciousness();
#endif
    }
}

void MainWindow::slave_silent_scan(QList <slave*> slaves)
{
    papis_slaves = slaves;
    render_dashboard();
}

void MainWindow::slave_scan_progress_rs485(int pc,QList <slave*> slaves)
{
    papis_slaves = slaves;
    if(is_pd)
    {
        pd->setValue(pc);
    }

    if(pc==100)
    {
        render_dashboard();
        if(is_pd)
        {
            pd->close();
            is_pd=false;
        }

        disconnect(slaveSyncThread,SIGNAL(slave_scan_progress_eth(int,QList <slave*>)),this,SLOT(slave_scan_progress_eth(int,QList <slave*>)));
        //show slave status
        //        ui->stackedWidget_2->setCurrentIndex(1);
        slaveTimer=new QTimer();
        connect(slaveTimer,SIGNAL(timeout()),this,SLOT(slaveTimeout()));
        slaveTimer->start(20000);
        gain_consciousness();
    }
}

void MainWindow::closeMessageBox()
{
    dCnt--;
    msgBox->setInformativeText("Do you wish to perform scan again? Closing in "+QString::number(dCnt)+"s");
    update();
    if(dCnt==0)
    {
        w_log(",Second Autoscan Timed out");
        //        ui->stackedWidget_2->setCurrentIndex(0);
        disconnect(popupTimer, SIGNAL(timeout()), this, SLOT(closeMessageBox()));
        popupTimer->stop();
        msgBox->close();
        disconnect(slaveSyncThread,SIGNAL(slave_scan_progress_eth(int,QList <slave*>)),this,SLOT(slave_scan_progress_eth(int,QList <slave*>)));
    }
}

void MainWindow::clearMessage()
{
    //ui->label_msg->clear();
}

void MainWindow::switch_route()
{
    dCnt--;
    msgBox->setInformativeText("Do you wish Switch Route now? Auto Switch in "+QString::number(dCnt/60)+" min, "+QString::number(dCnt%60)+" sec");
    update();
    if(dCnt==0)
    {
        disconnect(popupTimer, SIGNAL(timeout()), this, SLOT(switch_route()));
        popupTimer->stop();

        //load next route
        if((int)route_type < route_data.count()-1)
        {
            route_type = (route_type_t)((int)route_type+1);
        }
        else
            if(route_type == route_data.count()-1)
            {
                route_type = ROUTE_1;
            }

        w_log(",Route Auto switched to "+ routesInfo.routeList.at(routesInfo.routeSel.at((int)route_type)).tr_no_en);

        aRIdx = routesInfo.routeSel.at((int)route_type);
        aRouteData = route_data.at((int)route_type);
        aStnIdx = 0;
        wake=false;
        kmRt=0;
        emit slave_route_send();

        //ui->label_tr_name->setText(routesInfo.routeList.at(aRIdx).tr_no_en+", "+routesInfo.routeList.at(aRIdx).tr_nm_en);
        //ui->label_tr_route->setText(routesInfo.routeList.at(aRIdx).tr_route_en);
        //ui->label_tr_via->setText(routesInfo.routeList.at(aRIdx).tr_via_en);
        //ui->label_curr_stn->setText(aRouteData->st_list.at(aStnIdx).st_name_en);
        //ui->label_next_stn->setText(aRouteData->st_list.at(aStnIdx+1).st_name_en);

        w_log(",Update Active Route Details: Train "+ routesInfo.routeList.at(aRIdx).tr_no_en+", "+routesInfo.routeList.at(aRIdx).tr_nm_en);

        //put next route
        papis_sequence=PAPIS_GEN_WELCOME;
        msgBox->close();
    }
}

void MainWindow::render_dashboard()
{
    w_fault(",Slave Report");
    if((papis_slaves.at(0)->installStatus==INSTALLED&&papis_slaves.at(0)->activeStatus==ACTIVE))
    {
        //ui->label_status_lcd_pis18_1->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
        //  w_log(",lcd_pis18_1 Online");
    }
    else
        if((papis_slaves.at(0)->installStatus==INSTALLED&&papis_slaves.at(0)->activeStatus==INACTIVE))
        {
            //ui->label_status_lcd_pis18_1->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
            w_fault(",lcd_pis18_1 Offline");
        }

    if((papis_slaves.at(1)->installStatus==INSTALLED&&papis_slaves.at(1)->activeStatus==ACTIVE))
    {
        //ui->label_status_lcd_pis18_2->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
        //  w_log(",lcd_pis18_2 Online");
    }
    else
        if((papis_slaves.at(1)->installStatus==INSTALLED&&papis_slaves.at(1)->activeStatus==INACTIVE))
        {
            //ui->label_status_lcd_pis18_2->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
            w_fault(",lcd_pis18_2 Offline");
        }

    if((papis_slaves.at(2)->installStatus==INSTALLED&&papis_slaves.at(2)->activeStatus==ACTIVE))
    {
        //ui->label_status_lcd_pis18_3->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
    }
    else
        if((papis_slaves.at(2)->installStatus==INSTALLED&&papis_slaves.at(2)->activeStatus==INACTIVE))
        {
            //ui->label_status_lcd_pis18_3->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
        }

    if((papis_slaves.at(3)->installStatus==INSTALLED&&papis_slaves.at(3)->activeStatus==ACTIVE))
    {
        //ui->label_status_lcd_pis18_4->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
    }
    else
        if((papis_slaves.at(3)->installStatus==INSTALLED&&papis_slaves.at(3)->activeStatus==INACTIVE))
        {
            //ui->label_status_lcd_pis18_4->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
        }

    if((papis_slaves.at(4)->installStatus==INSTALLED&&papis_slaves.at(4)->activeStatus==ACTIVE))
    {
        // ui->label_status_lcd_pis10_1->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
        //        w_log(",lcd_pis10_1 Online");
    }
    else
        if((papis_slaves.at(4)->installStatus==INSTALLED&&papis_slaves.at(4)->activeStatus==INACTIVE))
        {
            // ui->label_status_lcd_pis10_1->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
            w_fault(",lcd_pis10_1 Offline");
        }

    if((papis_slaves.at(5)->installStatus==INSTALLED&&papis_slaves.at(5)->activeStatus==ACTIVE))
    {
        //ui->label_status_lcd_pis10_2->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
        //        w_log(",lcd_pis10_2 Online");
    }
    else
        if((papis_slaves.at(5)->installStatus==INSTALLED&&papis_slaves.at(5)->activeStatus==INACTIVE))
        {
            //ui->label_status_lcd_pis10_2->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
            w_fault(",lcd_pis10_2 Offline");
        }

    if((papis_slaves.at(6)->installStatus==INSTALLED&&papis_slaves.at(6)->activeStatus==ACTIVE))
    {
        //ui->label_status_lcd_pis10_3->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
    }
    else
        if((papis_slaves.at(6)->installStatus==INSTALLED&&papis_slaves.at(6)->activeStatus==INACTIVE))
        {
            //ui->label_status_lcd_pis10_3->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
        }

    if((papis_slaves.at(7)->installStatus==INSTALLED&&papis_slaves.at(7)->activeStatus==ACTIVE))
    {
        //ui->label_status_lcd_pis10_4->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
    }
    else
        if((papis_slaves.at(7)->installStatus==INSTALLED&&papis_slaves.at(7)->activeStatus==INACTIVE))
        {
            //ui->label_status_lcd_pis10_4->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
        }

    if((papis_slaves.at(8)->installStatus==INSTALLED&&papis_slaves.at(8)->activeStatus==ACTIVE))
    {
        //ui->label_status_lcd_pis10_5->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
    }
    else
        if((papis_slaves.at(8)->installStatus==INSTALLED&&papis_slaves.at(8)->activeStatus==INACTIVE))
        {
            //ui->label_status_lcd_pis10_5->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
        }

    if((papis_slaves.at(9)->installStatus==INSTALLED&&papis_slaves.at(9)->activeStatus==ACTIVE))
    {
        //ui->label_status_lcd_pis10_6->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
    }
    else
        if((papis_slaves.at(9)->installStatus==INSTALLED&&papis_slaves.at(9)->activeStatus==INACTIVE))
        {
            //ui->label_status_lcd_pis10_6->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
        }

    if((papis_slaves.at(10)->installStatus==INSTALLED&&papis_slaves.at(10)->activeStatus==ACTIVE))
    {
        //ui->label_status_lcd_pis10_7->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
    }
    else
        if((papis_slaves.at(10)->installStatus==INSTALLED&&papis_slaves.at(10)->activeStatus==INACTIVE))
        {
            //ui->label_status_lcd_pis10_7->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
        }

    if((papis_slaves.at(11)->installStatus==INSTALLED&&papis_slaves.at(11)->activeStatus==ACTIVE))
    {
        //ui->label_status_lcd_pis10_8->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
    }
    else
        if((papis_slaves.at(11)->installStatus==INSTALLED&&papis_slaves.at(11)->activeStatus==INACTIVE))
        {
            //ui->label_status_lcd_pis10_8->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
        }

    if((papis_slaves.at(12)->installStatus==INSTALLED&&papis_slaves.at(12)->activeStatus==ACTIVE))
    {
        //ui->label_status_lcd_pis10_9->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
    }
    else
        if((papis_slaves.at(12)->installStatus==INSTALLED&&papis_slaves.at(12)->activeStatus==INACTIVE))
        {
            //ui->label_status_lcd_pis10_9->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
        }

    if((papis_slaves.at(13)->installStatus==INSTALLED&&papis_slaves.at(13)->activeStatus==ACTIVE))
    {
        //        ui->label_status_ddb_sl_1->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
        //        w_log(",ddb_sl_1 Online");
    }
    else
        if((papis_slaves.at(13)->installStatus==INSTALLED&&papis_slaves.at(13)->activeStatus==INACTIVE))
        {
            //        ui->label_status_ddb_sl_1->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
            w_fault(",ddb_sl_1 Offline");
        }

    if((papis_slaves.at(14)->installStatus==INSTALLED&&papis_slaves.at(14)->activeStatus==ACTIVE))
    {
        //        ui->label_status_ddb_sl_2->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
        //        w_log(",ddb_sl_2 Online");
    }
    else
        if((papis_slaves.at(14)->installStatus==INSTALLED&&papis_slaves.at(14)->activeStatus==INACTIVE))
        {
            //        ui->label_status_ddb_sl_2->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
            w_fault(",ddb_sl_2 Offline");
        }

    if((papis_slaves.at(15)->installStatus==INSTALLED&&papis_slaves.at(15)->activeStatus==ACTIVE))
    {
        //        ui->label_status_ddb_sl_3->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
    }
    else
        if((papis_slaves.at(15)->installStatus==INSTALLED&&papis_slaves.at(15)->activeStatus==INACTIVE))
        {
            //        ui->label_status_ddb_sl_3->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
        }

    if((papis_slaves.at(16)->installStatus==INSTALLED&&papis_slaves.at(16)->activeStatus==ACTIVE))
    {
        //        ui->label_status_ddb_sl_4->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
    }
    else
        if((papis_slaves.at(16)->installStatus==INSTALLED&&papis_slaves.at(16)->activeStatus==INACTIVE))
        {
            //        ui->label_status_ddb_sl_4->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
        }

    if((papis_slaves.at(17)->installStatus==INSTALLED&&papis_slaves.at(17)->activeStatus==ACTIVE))
    {
        //        ui->label_status_ddb_dl_1->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
        //        w_log(",ddb_dl_1 Online");
    }
    else
        if((papis_slaves.at(17)->installStatus==INSTALLED&&papis_slaves.at(17)->activeStatus==INACTIVE))
        {
            //        ui->label_status_ddb_dl_1->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
            w_fault(",ddb_dl_1 Offline");
        }

    if((papis_slaves.at(18)->installStatus==INSTALLED&&papis_slaves.at(18)->activeStatus==ACTIVE))
    {
        //        ui->label_status_ddb_dl_2->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
        //        w_log(",ddb_dl_2 Online");
    }
    else
        if((papis_slaves.at(18)->installStatus==INSTALLED&&papis_slaves.at(18)->activeStatus==INACTIVE))
        {
            //        ui->label_status_ddb_dl_2->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
            w_fault(",ddb_dl_2 Offline");
        }

    if((papis_slaves.at(19)->installStatus==INSTALLED&&papis_slaves.at(19)->activeStatus==ACTIVE))
    {
        //        ui->label_status_ddb_dl_3->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
    }
    else
        if((papis_slaves.at(19)->installStatus==INSTALLED&&papis_slaves.at(19)->activeStatus==INACTIVE))
        {
            //        ui->label_status_ddb_dl_3->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
        }

    if((papis_slaves.at(20)->installStatus==INSTALLED&&papis_slaves.at(20)->activeStatus==ACTIVE))
    {
        //        ui->label_status_ddb_dl_4->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
    }
    else
        if((papis_slaves.at(20)->installStatus==INSTALLED&&papis_slaves.at(20)->activeStatus==INACTIVE))
        {
            //        ui->label_status_ddb_dl_4->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
        }

    if((papis_slaves.at(21)->installStatus==INSTALLED&&papis_slaves.at(21)->activeStatus==ACTIVE))
    {
        //        ui->label_status_led_pis_1->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
        //        w_log(",led_pis_1 Online");
    }
    else
        if((papis_slaves.at(21)->installStatus==INSTALLED&&papis_slaves.at(21)->activeStatus==INACTIVE))
        {
            //        ui->label_status_led_pis_1->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
            //        w_fault(",led_pis_1 Offline");
        }

    if((papis_slaves.at(22)->installStatus==INSTALLED&&papis_slaves.at(22)->activeStatus==ACTIVE))
    {
        //        ui->label_status_led_pis_2->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
        //        w_log(",led_pis_2 Online");
    }
    else
        if((papis_slaves.at(22)->installStatus==INSTALLED&&papis_slaves.at(22)->activeStatus==INACTIVE))
        {
            //        ui->label_status_led_pis_2->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
            //        w_fault(",led_pis_1 Offline");
        }

    if((papis_slaves.at(23)->installStatus==INSTALLED&&papis_slaves.at(24)->activeStatus==ACTIVE))
    {
        //        ui->label_status_led_pis_3->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
    }
    else
        if((papis_slaves.at(23)->installStatus==INSTALLED&&papis_slaves.at(24)->activeStatus==INACTIVE))
        {
            //        ui->label_status_led_pis_3->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
        }

    if((papis_slaves.at(24)->installStatus==INSTALLED&&papis_slaves.at(24)->activeStatus==ACTIVE))
    {
        //        ui->label_status_led_pis_4->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
    }
    else
        if((papis_slaves.at(24)->installStatus==INSTALLED&&papis_slaves.at(24)->activeStatus==INACTIVE))
        {
            //        ui->label_status_led_pis_4->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
        }

    if((papis_slaves.at(25)->installStatus==INSTALLED&&papis_slaves.at(25)->activeStatus==ACTIVE))
    {
        //        ui->label_status_tms_1->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
        //        w_log(",tms_1 Online");
    }
    else
        if((papis_slaves.at(25)->installStatus==INSTALLED&&papis_slaves.at(25)->activeStatus==INACTIVE))
        {
            //        ui->label_status_tms_1->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
            w_fault(",tms_1 Offline");
        }

    if((papis_slaves.at(26)->installStatus==INSTALLED&&papis_slaves.at(26)->activeStatus==ACTIVE))
    {
        //        ui->label_status_tms_2->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
        //        w_log(",tms_2 Online");
    }
    else
        if((papis_slaves.at(26)->installStatus==INSTALLED&&papis_slaves.at(26)->activeStatus==INACTIVE))
        {
            //        ui->label_status_tms_2->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
            w_fault(",tms_2 Offline");
        }
    if((papis_slaves.at(27)->installStatus==INSTALLED&&papis_slaves.at(27)->activeStatus==ACTIVE))
    {
        //        ui->label_status_tms_3->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
    }
    else
        if((papis_slaves.at(27)->installStatus==INSTALLED&&papis_slaves.at(27)->activeStatus==INACTIVE))
        {
            //        ui->label_status_tms_3->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
        }

    if((papis_slaves.at(28)->installStatus==INSTALLED&&papis_slaves.at(28)->activeStatus==ACTIVE))
    {
        //        ui->label_status_tms_4->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
    }
    else
        if((papis_slaves.at(28)->installStatus==INSTALLED&&papis_slaves.at(28)->activeStatus==INACTIVE))
        {
            //        ui->label_status_tms_4->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
        }

    //    w_log(",Rendered Slave Dashboard");
    update();
}

void MainWindow::gain_consciousness()
{
    QString mR;
    QList <int> skipL;
    //check if we have any routeData
    if(load_routes())
    {
        for(int idx=0;idx<routesInfo.routeSel.count();idx++)
        {
            route_ok=load_routeData(idx);
            if(!route_ok)
            {
                route r = routesInfo.routeList.at(routesInfo.routeSel.at(idx));
                r.d_av = false;
                w_fault(",No Data found for Route: "+r.tr_no_en);
                routesInfo.routeList.replace(routesInfo.routeSel.at(idx),r);
                mR+=routesInfo.routeList.at(routesInfo.routeSel.at(idx)).tr_no_en+" ";
                skipL.append(idx);
            }
        }
    }

    if(!route_ok)
    {
        //remove routes for which data is not found
        for(int idx=0;idx<skipL.count();idx++)
        {
            routesInfo.routeSel.removeAt(skipL.at(idx)-idx);
        }

        //ui->label_msg->setText(" Error: Data for configured route(s): "+mR+"NOT found!!");
        w_fault(",Data for configured route(s): "+mR+"NOT found!!");
        msgTimer = new QTimer();
        connect(msgTimer, SIGNAL(timeout()), this, SLOT(clearMessage()));
        msgTimer->setSingleShot(true);
        msgTimer->start(5000);
        update();
    }

    if(true)//route_ok)
    {
        announcement_idx=ANN_HINDI;
        if(!routesInfo.routeList.count())
        {
            w_fault(",No Routes and Route Data found in the system!");
            QMessageBox::critical(this, tr("Error "),
                                  "No Routes and Route Data found in the system!", QMessageBox::Ok);
            update();
        }
        else
        {
            route_decision();
        }
    }
}

void MainWindow::route_decision()
{
    ExecDialog *execDialog = new ExecDialog();
    execDialog->setWindowFlags(/*Qt::WindowStaysOnTopHint|*/Qt::FramelessWindowHint);
    connect(execDialog,SIGNAL(decision(exec_t)),this, SLOT(decision(exec_t)));
    execDialog->setModal(true);
#ifndef LAPTOP
    execDialog->show();
#else
    execDialog->showFullScreen();
#endif
}

void MainWindow::atTimeout()
{
    char nl[] = {0x0d, 0x0a};
    atSerialPort->write("at+qgps=1");
    atSerialPort->write(nl);
    atCnt++;
    //ui->label_msg->setText("Switching GPS ON: "+QString::number(atCnt));

    if(atCnt==10)
    {
        atTimer->stop();
        disconnect(atSerialPort,SIGNAL(readyRead()),this,SLOT(atReadyRead()));
        atSerialPort->close();
        //ui->label_msg->setText("");
        w_log(",Failed to Switch GPS ON!");
        w_fault(",Failed to Switch GPS ON!");
    }
}

void MainWindow::atReadyRead()
{
    const QByteArray data = atSerialPort->readAll();

    if(data.contains("OK")||data.contains("ok"))
    {
        atTimer->stop();
        disconnect(atSerialPort,SIGNAL(readyRead()),this,SLOT(atReadyRead()));
        atSerialPort->close();
        //ui->label_msg->setText("GPS ON");
        w_log(",GPS Switched ON!");
    }
}

void MainWindow::slave_send_mode(QString mode)
{
    for(int idx=0; idx< papis_slaves.count();idx++)
    {
        if((papis_slaves.at(idx)->installStatus==INSTALLED
            &&papis_slaves.at(idx)->activeStatus==ACTIVE
            &&papis_slaves.at(idx)->conn_mode=="ETH"))
        {
            switch(papis_slaves.at(idx)->device_type)
            {
            case DEV_LCD_PIS_18:
            case DEV_LCD_PIS_10:
            {
                papis_slaves.at(idx)->socket = new QTcpSocket();
                papis_slaves.at(idx)->socket->connectToHost(papis_slaves.at(idx)->ip_addr,3669);
                papis_slaves.at(idx)->socket->write(QString("mode="+mode).toLocal8Bit());
                papis_slaves.at(idx)->socket->disconnect();
                papis_slaves.at(idx)->socket->close();
            }
                break;
            case DEV_DDB_SL:

                break;
            case DEV_DDB_DL:

                break;
            case DEV_LED_PIS:

                break;
            case DEV_TMS:

                break;
            default:
                break;
            }
        }
    }
}

void MainWindow::slave_send_error()
{
    for(int idx=0; idx< papis_slaves.count();idx++)
    {
        if((papis_slaves.at(idx)->installStatus==INSTALLED
            &&papis_slaves.at(idx)->activeStatus==ACTIVE
            &&papis_slaves.at(idx)->conn_mode=="ETH"))
        {
            switch(papis_slaves.at(idx)->device_type)
            {
            case DEV_LCD_PIS_18:
            case DEV_LCD_PIS_10:
            {
                papis_slaves.at(idx)->socket = new QTcpSocket();
                papis_slaves.at(idx)->socket->connectToHost(papis_slaves.at(idx)->ip_addr,3669);
                papis_slaves.at(idx)->socket->write(QString("err").toLocal8Bit());
                papis_slaves.at(idx)->socket->disconnect();
                papis_slaves.at(idx)->socket->close();
            }
                break;
            case DEV_DDB_SL:

                break;
            case DEV_DDB_DL:

                break;
            case DEV_LED_PIS:

                break;
            case DEV_TMS:

                break;
            default:
                break;
            }
        }
    }
}

void MainWindow::slave_send_return()
{
    for(int idx=0; idx< papis_slaves.count();idx++)
    {
        if((papis_slaves.at(idx)->installStatus==INSTALLED
            &&papis_slaves.at(idx)->activeStatus==ACTIVE
            &&papis_slaves.at(idx)->conn_mode=="ETH"))
        {
            switch(papis_slaves.at(idx)->device_type)
            {
            case DEV_LCD_PIS_18:
            case DEV_LCD_PIS_10:
            {
                papis_slaves.at(idx)->socket = new QTcpSocket();
                papis_slaves.at(idx)->socket->connectToHost(papis_slaves.at(idx)->ip_addr,3669);
                papis_slaves.at(idx)->socket->write(QString("return").toLocal8Bit());
                papis_slaves.at(idx)->socket->disconnect();
                papis_slaves.at(idx)->socket->close();
            }
                break;
            case DEV_DDB_SL:

                break;
            case DEV_DDB_DL:

                break;
            case DEV_LED_PIS:

                break;
            case DEV_TMS:

                break;
            default:
                break;
            }
        }
    }
}


void MainWindow::slave_send_clear(int i)
{
    for(int idx=0; idx< papis_slaves.count();idx++)
    {
        if((papis_slaves.at(idx)->installStatus==INSTALLED
            &&papis_slaves.at(idx)->activeStatus==ACTIVE
            &&papis_slaves.at(idx)->conn_mode=="ETH"))
        {
            switch(papis_slaves.at(idx)->device_type)
            {
            case DEV_LCD_PIS_18:
            case DEV_LCD_PIS_10:
            {
                papis_slaves.at(idx)->socket = new QTcpSocket();
                papis_slaves.at(idx)->socket->connectToHost(papis_slaves.at(idx)->ip_addr,3669);
                papis_slaves.at(idx)->socket->write(QString("clear="+QString::number(i)).toLocal8Bit());
                papis_slaves.at(idx)->socket->disconnect();
                papis_slaves.at(idx)->socket->close();
            }
                break;
            case DEV_DDB_SL:

                break;
            case DEV_DDB_DL:

                break;
            case DEV_LED_PIS:

                break;
            case DEV_TMS:

                break;
            default:
                break;
            }
        }
    }
}

void MainWindow::slave_send_alert(int i)
{
    for(int idx=0; idx< papis_slaves.count();idx++)
    {
        if((papis_slaves.at(idx)->installStatus==INSTALLED
            &&papis_slaves.at(idx)->activeStatus==ACTIVE
            &&papis_slaves.at(idx)->conn_mode=="ETH"))
        {
            switch(papis_slaves.at(idx)->device_type)
            {
            case DEV_LCD_PIS_18:
            case DEV_LCD_PIS_10:
            {
                papis_slaves.at(idx)->socket = new QTcpSocket();
                papis_slaves.at(idx)->socket->connectToHost(papis_slaves.at(idx)->ip_addr,3669);
                papis_slaves.at(idx)->socket->write(QString("alert="+QString::number(i)).toLocal8Bit());
                papis_slaves.at(idx)->socket->disconnect();
                papis_slaves.at(idx)->socket->close();
            }
                break;
            case DEV_DDB_SL:

                break;
            case DEV_DDB_DL:

                break;
            case DEV_LED_PIS:

                break;
            case DEV_TMS:

                break;
            default:
                break;
            }
        }
    }
}

void MainWindow::slave_send_distance(double km)
{
    for(int idx=0; idx< papis_slaves.count();idx++)
    {
        if((papis_slaves.at(idx)->installStatus==INSTALLED
            &&papis_slaves.at(idx)->activeStatus==ACTIVE
            &&papis_slaves.at(idx)->conn_mode=="ETH"))
        {
            switch(papis_slaves.at(idx)->device_type)
            {
            case DEV_LCD_PIS_18:
            case DEV_LCD_PIS_10:
            {
                papis_slaves.at(idx)->socket = new QTcpSocket();
                papis_slaves.at(idx)->socket->connectToHost(papis_slaves.at(idx)->ip_addr,3669);
                papis_slaves.at(idx)->socket->write(QString("dist="+QString::number(km,'_f',2)).toLocal8Bit());
                papis_slaves.at(idx)->socket->disconnect();
                papis_slaves.at(idx)->socket->close();
            }
                break;
            case DEV_DDB_SL:

                break;
            case DEV_DDB_DL:

                break;
            case DEV_LED_PIS:

                break;
            case DEV_TMS:

                break;
            default:
                break;
            }
        }
    }

}

void MainWindow::slave_send_route()
{
    kmRt=0;
    for(int idx=0; idx< papis_slaves.count();idx++)
    {
        if((papis_slaves.at(idx)->installStatus==INSTALLED
            &&papis_slaves.at(idx)->activeStatus==ACTIVE
            &&papis_slaves.at(idx)->conn_mode=="ETH"))
        {
            switch(papis_slaves.at(idx)->device_type)
            {
            case DEV_LCD_PIS_18:
            case DEV_LCD_PIS_10:
            {
                papis_slaves.at(idx)->socket = new QTcpSocket();
                papis_slaves.at(idx)->socket->connectToHost(papis_slaves.at(idx)->ip_addr,3669);
                QJsonObject ri_json;
                routesInfo.routeList.at(aRIdx).write_s(ri_json);
                QJsonObject rd_json;
                aRouteData->write_s(rd_json);
                QJsonObject json;
                json["info"]=ri_json;
                json["data"]=rd_json;
                QJsonDocument rDoc(json);

                //               QFile saveFile(QString("./rinfo.json"));
                //               if (saveFile.open(QIODevice::WriteOnly))
                //               {
                //                   saveFile.write(rDoc.toJson());
                //                   saveFile.close();
                //               }

                //               QByteArray rArray = rDoc.toJson();
                QByteArray rArray = rDoc.toJson().replace("  ","");
                papis_slaves.at(idx)->socket->write(QString("rinfo="+rArray).toLocal8Bit());
                papis_slaves.at(idx)->socket->disconnect();
                papis_slaves.at(idx)->socket->close();
            }
                break;
            case DEV_DDB_SL:

                break;
            case DEV_DDB_DL:

                break;
            case DEV_LED_PIS:

                break;
            case DEV_TMS:

                break;
            default:
                break;
            }
        }
    }
}

void MainWindow:: decision(exec_t exec)
{
    if(exec.exec_type == EXEC_CANCEL)
    {
        //try to autostart after N min
        route_ongoing=false;
    }
    else
        if(exec.exec_type == EXEC_AUTO)
        {
            route_ongoing=true;
            w_log(",Auto Route Mode Selected");
            if(systemconfig.gps_source == "FILE")
            {
                w_log(",GPS Source: File");

                QString gfp = base_dir+"routes/"+exec.autoFile;
                w_log(",Loading NMEA Simulation File: "+exec.autoFile);
                //ui->label_msg->setText("File: "+gfp);
                if(QFile::exists(gfp))
                {
                    gpsFile = new QFile(gfp);
                    nmeaSource = new QNmeaPositionInfoSource(QNmeaPositionInfoSource::SimulationMode);
                    nmeaSource->setDevice(gpsFile);
                    connect(nmeaSource, SIGNAL(positionUpdated(QGeoPositionInfo)),this, SLOT(positionUpdated(QGeoPositionInfo)));
                    connect(nmeaSource, SIGNAL(updateTimeout()), this, SLOT(updateTimeout()));
                    connect(nmeaSource,SIGNAL(error(QGeoPositionInfoSource::Error)),this,SLOT(error(QGeoPositionInfoSource::Error)));
                    nmeaSource->startUpdates();
                    w_log(",NMEA Simulation Started");
                }
                else
                {
                    route_ongoing=false;
                    w_log(",NMEA Simulation File not Found for Auto Mode!");
                    w_fault(",NMEA Simulation File not Found for Auto Mode!");
                    QMessageBox::critical(this, tr("GPS Error "),
                                          "NMEA Simulation File not Found for Auto Mode!", QMessageBox::Ok);
                    update();
                    return;
                }
            }
            else
                if(systemconfig.gps_source == "SERIAL")
                {
                    w_log(",GPS Source: Serial");
                    atSerialPort = new QSerialPort(this);
                    atSerialPort->setPortName("/dev/ttyUSB2");
                    atSerialPort->setBaudRate(QSerialPort::Baud115200);
                    atSerialPort->setDataBits(QSerialPort::Data8);
                    atSerialPort->setParity(QSerialPort::NoParity);
                    atSerialPort->setStopBits(QSerialPort::OneStop);
                    atSerialPort->setFlowControl(QSerialPort::NoFlowControl);
                    connect(atSerialPort,SIGNAL(readyRead()),this,SLOT(atReadyRead()));
                    if(atSerialPort->open(QIODevice::ReadWrite))
                    {
                        atTimer = new QTimer();
                        w_log(",AT Serial Port Opened");
                        connect(atTimer, SIGNAL(timeout()),this, SLOT(atTimeout()));
                        atTimer->start(3000);
                    }
                    else
                    {
                        w_log(",Failed to Open AT Serial Port!");
                        w_fault(",Failed to Open AT Serial Port!");
                    }

                    serial = new QSerialPort(this);
                    serial->setPortName(systemconfig.gps_port);
                    serial->setBaudRate(QSerialPort::Baud115200);
                    serial->setDataBits(QSerialPort::Data8);
                    serial->setParity(QSerialPort::NoParity);
                    serial->setStopBits(QSerialPort::OneStop);
                    serial->setFlowControl(QSerialPort::NoFlowControl);
                    if(serial->open(QIODevice::ReadWrite))
                    {
                        //cavili
                        //                char nl[] = {0x0d, 0x0a};
                        //                serial->write("at+cgps=1");
                        //                serial->write(nl);
                        w_log(",GPS Serial Port Opened");
                        nmeaSource = new QNmeaPositionInfoSource(QNmeaPositionInfoSource::RealTimeMode);
                        nmeaSource->setDevice(serial);

                        wake=true;

                        connect(nmeaSource, SIGNAL(positionUpdated(QGeoPositionInfo)),this, SLOT(positionUpdated(QGeoPositionInfo)));
                        connect(nmeaSource, SIGNAL(updateTimeout()), this, SLOT(updateTimeout()));
                        connect(nmeaSource,SIGNAL(error(QGeoPositionInfoSource::Error)),this,SLOT(error(QGeoPositionInfoSource::Error)));
                        nmeaSource->startUpdates();
                    }
                    else
                    {
                        route_ongoing=false;
                        w_log(",Failed to Open GPS Serial Port!");
                        w_fault(",Failed to Open GPS Serial Port!");
                        QMessageBox::critical(this, tr("GPS Error "),
                                              "HERE: Failed to detect GPS! Please check system configuration.", QMessageBox::Ok);
                        update();
                        return;
                    }
                }

            error_mode();
            emit slave_error_send();
        }
        else
        {
            route_ongoing=true;
            aRIdx = routesInfo.routeSel.at((int)exec.exec_type);
            aRouteData = route_data.at((int)exec.exec_type);
            route_type = (route_type_t)exec.exec_type;
            aStnIdx = exec.st_idx;

            w_log(",Manual Route Mode Selected");

            wake=false;

            //ui->label_tr_name->setText(routesInfo.routeList.at(aRIdx).tr_no_en+", "+routesInfo.routeList.at(aRIdx).tr_nm_en);
            //ui->label_tr_route->setText(routesInfo.routeList.at(aRIdx).tr_route_en);
            //ui->label_tr_via->setText(routesInfo.routeList.at(aRIdx).tr_via_en);

            //        w_log(",Train Name: "+//ui->label_tr_name->text());
            //        w_log(",Train Route: "+//ui->label_tr_route->text());
            //        w_log(",Train Via: "+//ui->label_tr_via->text());

            draw_route();
            emit slave_route_send();

            if(aStnIdx==-1)
            {
                //ui->label_curr_stn->setText("");
                //ui->label_next_stn->setText("");
                //            ui->label_next_dist->setText("");
                //            ui->label_sch_time->setText("");
                w_log(",Unknown Station Selected for Manual Route");

                kmRt=0;
            }
            else
                if(aStnIdx==0)
                {
                    //ui->label_curr_stn->setText(aRouteData->st_list.at(aStnIdx).st_name_en);
                    //ui->label_next_stn->setText(aRouteData->st_list.at(aStnIdx+1).st_name_en);
                    //            ui->label_next_dist->setText(QString::number(aRouteData->st_list.at(aStnIdx+1).dist_km,'_f',1)+" km");
                    //            ui->label_sch_time->setText(QString::number(aRouteData->st_list.at(aStnIdx+1).time_arr_hh)+":"+QString::number(aRouteData->st_list.at(aStnIdx+1).time_arr_mm));
                    //            w_log(",Station Selected for Manual Route: "+//ui->label_curr_stn->text());
                    //            w_log(",Next: "+//ui->label_next_stn->text()+", Dist: "+ui->label_next_dist->text()+", Sch: "+ui->label_sch_time->text());

                    kmRt=0;
                }
                else
                    if(aStnIdx<aRouteData->st_list.count()-1)
                    {
                        //ui->label_curr_stn->setText(aRouteData->st_list.at(aStnIdx).st_name_en);
                        //ui->label_next_stn->setText(aRouteData->st_list.at(aStnIdx+1).st_name_en);
                        //            ui->label_next_dist->setText(QString::number(aRouteData->st_list.at(aStnIdx+1).dist_km-aRouteData->st_list.at(aStnIdx).dist_km,'_f',1)+" km");
                        //            ui->label_sch_time->setText(QString::number(aRouteData->st_list.at(aStnIdx+1).time_arr_hh)+":"+QString::number(aRouteData->st_list.at(aStnIdx+1).time_arr_mm));
                        //            w_log(",Station Selected for Manual Route: "+//ui->label_curr_stn->text());
                        //            w_log(",Next: "+//ui->label_next_stn->text()+", Dist: "+ui->label_next_dist->text()+", Sch: "+ui->label_sch_time->text());

                        kmRt = aRouteData->st_list.at(aStnIdx).dist_km;
                    }
                    else
                    {
                        //ui->label_curr_stn->setText("");
                        //ui->label_next_stn->setText("");
                        //            ui->label_next_dist->setText("");
                        //            ui->label_sch_time->setText("");
                    }

            ddb_cycle = DDB_HI_ROUTE_1;
            ddbCycler = new QTimer(this);
            connect(ddbCycler, SIGNAL(timeout()),this, SLOT(ddbCycleTimeout()));
            connect(this, SIGNAL(ddb_drive()),this, SLOT(drive_ddb()));
            connect(this, SIGNAL(pis_slogan_drive()),this, SLOT(pis_drive_slogan()));
            drive_ddb();
            w_log(",External DDB Update Process Started, Cycle time: 10s");

            ddbCycler->start(60000);
            update();

            if(systemconfig.gps_source == "FILE")
            {
                w_log(",GPS Source: File");
                QString gfp = base_dir+"routes/"+routesInfo.routeList.at(aRIdx).tr_no_en+"/"+
                        routesInfo.routeList.at(aRIdx).tr_no_en+".nmea";
                w_log(",Opening Route File: "+gfp);
                //ui->label_msg->setText("File: "+gfp);
                if(QFile::exists(gfp))
                {
                    gpsFile = new QFile(gfp);

                    double lat, lon;
                    QString line="";
                    gpsFile->open(QIODevice::ReadOnly);
                    if(aStnIdx!=-1)
                    {
                        int lines=0;
                        while(!gpsFile->atEnd())
                        {
                            line = QString::fromLocal8Bit(gpsFile->readLine());
                            if(line.contains("RMC"))
                            {
                                QStringList sl = line.split(",");
                                double tmpd = sl.at(3).toDouble();
                                int dd = tmpd/100;
                                double ss = tmpd - dd * 100;
                                lat = dd+ss/60;

                                if(sl.at(4)=="S")
                                {
                                    lat*=-1;
                                }

                                tmpd = sl.at(5).toDouble();
                                dd = tmpd/100;
                                ss = tmpd - dd * 100;
                                lon = dd+ss/60;

                                if(sl.at(4)=="W")
                                {
                                    lon*=-1;
                                }

                                lines++;

                                if(Calc_distance_between_locations(aRouteData->st_list.at(aStnIdx).gps_lon, aRouteData->st_list.at(aStnIdx).gps_lat, lon, lat)<0.1)
                                {
                                    break;
                                }
                            }
                        }

                    }
                    nmeaSource = new QNmeaPositionInfoSource(QNmeaPositionInfoSource::SimulationMode);
                    nmeaSource->setDevice(gpsFile);
                    connect(nmeaSource, SIGNAL(positionUpdated(QGeoPositionInfo)),this, SLOT(positionUpdated(QGeoPositionInfo)));
                    connect(nmeaSource, SIGNAL(updateTimeout()), this, SLOT(updateTimeout()));
                    connect(nmeaSource,SIGNAL(error(QGeoPositionInfoSource::Error)),this,SLOT(error(QGeoPositionInfoSource::Error)));
                    nmeaSource->startUpdates();
                    w_log(",NMEA Simulation Started");
                }
                else
                {
                    route_ongoing=false;
                    w_log(",Error: NMEA Simulation File not Found for the Selected route!");
                    w_fault(",Error: NMEA Simulation File not Found for the Selected route!");
                    QMessageBox::critical(this, tr("GPS Error "),
                                          "NMEA Simulation File not Found for the Selected route!", QMessageBox::Ok);
                    update();
                    return;
                }
            }
            else
                if(systemconfig.gps_source == "SERIAL")
                {
                    w_log(",GPS Source: Serial");
                    atSerialPort = new QSerialPort(this);
                    atSerialPort->setPortName("/dev/ttyUSB2");
                    atSerialPort->setBaudRate(QSerialPort::Baud115200);
                    atSerialPort->setDataBits(QSerialPort::Data8);
                    atSerialPort->setParity(QSerialPort::NoParity);
                    atSerialPort->setStopBits(QSerialPort::OneStop);
                    atSerialPort->setFlowControl(QSerialPort::NoFlowControl);
                    if(atSerialPort->open(QIODevice::ReadWrite))
                    {
                        atTimer = new QTimer();
                        w_log(",AT Serial Port Opened");
                        connect(atTimer, SIGNAL(timeout()),this, SLOT(atTimeout()));
                        //                atTimer->setSingleShot(true);
                        atTimer->start(3000);
                    }
                    else
                    {
                        w_log(",Failed to Open AT Serial Port!");
                        w_fault(",Failed to Open AT Serial Port!");
                    }

                    serial = new QSerialPort(this);
                    serial->setPortName(systemconfig.gps_port);
                    serial->setBaudRate(QSerialPort::Baud115200);
                    serial->setDataBits(QSerialPort::Data8);
                    serial->setParity(QSerialPort::NoParity);
                    serial->setStopBits(QSerialPort::OneStop);
                    serial->setFlowControl(QSerialPort::NoFlowControl);
                    if(serial->open(QIODevice::ReadWrite))
                    {
                        w_log(",GPS Serial Port Opened");
                        nmeaSource = new QNmeaPositionInfoSource(QNmeaPositionInfoSource::RealTimeMode);
                        nmeaSource->setDevice(serial);

                        connect(nmeaSource, SIGNAL(positionUpdated(QGeoPositionInfo)),this, SLOT(positionUpdated(QGeoPositionInfo)));
                        connect(nmeaSource, SIGNAL(updateTimeout()), this, SLOT(updateTimeout()));
                        connect(nmeaSource,SIGNAL(error(QGeoPositionInfoSource::Error)),this,SLOT(error(QGeoPositionInfoSource::Error)));
                        nmeaSource->startUpdates();
                    }
                    else
                    {
                        route_ongoing=false;
                        w_log(",Failed to Open GPS Serial Port!");
                        w_fault(",Failed to Open GPS Serial Port!");
                        QMessageBox::critical(this, tr("GPS Error "),
                                              "HERE: Failed to detect GPS! Please check system configuration.", QMessageBox::Ok);
                        update();
                        return;
                    }
                }
        }
}

void MainWindow::welcome_mode_eng(){
    //transmit welcome data
    w_log(",Transmitted: General Welcome message: English");
    //ui->label_led_pis->setText("Indian Railways Welcomes You");
    // ui->label_lcd_pis->setText("Indian Railways Welcomes You");

    update();
}

void MainWindow::welcome_mode_hin()
{
    w_log(",Transmitted: General Welcome message: Hindi");
    //transmit welcome data
    //ui->label_led_pis->setText("भारतीय रेल में सभी यात्रियों का स्वागत है");
    // ui->label_lcd_pis->setText("भारतीय रेल में सभी यात्रियों का स्वागत है");
    update();
}

void MainWindow::welcome_mode_rl(int idx)
{
    w_log(",Transmitted: General Welcome message: RL");
    lang = aRouteData->st_list.at(aStnIdx).rl;
    //ui->label_led_pis->setFont(*getRLFont(aRouteData->st_list.at(aStnIdx).rl));
    //ui->label_led_pis->setText(aRouteData->st_list.at(idx).w_msg_rl);
    // ui->label_lcd_pis->setText(aRouteData->st_list.at(idx).w_msg_rl);
    update();
}

void MainWindow::error_mode()
{
    //    ui->label_ddb_sl_p1->setText("");
    //    ui->label_ddb_sl_p2->setText("Welcome to Indian Railways");

    //    ui->label_ddb_dl_p1->setText("");
    //    ui->label_ddb_dl_p2->setText("Welcome to Indian Railways");
    //ui->label_led_pis->setText("Welcome to Indian Railways");
    //    ui->label_ddb_sl_p1->setText("");
    //    ui->label_ddb_sl_p2->setText("Welcome to Indian Railways");
    // ui->label_lcd_pis->setText("Welcome to Indian Railways");

    //ui->label_tr_name->setText("");
    //ui->label_tr_route->setText("");
    //ui->label_tr_via->setText("");
    //ui->label_curr_stn->setText("");
    //ui->label_next_stn->setText("");
    //    ui->label_next_dist->setText("");
    //    ui->label_eta->setText("");
    //    ui->label_sch_time->setText("");
    //    ui->label_late_stat->setText("");

    //transmit error messages
    update();
}

bool socketBusy=false;

void MainWindow::drive_slaves()
{
    //extract data
    QPixmap pmap_ddb_sl, pmap_ddb_dl_l1, pmap_ddb_dl_l2, pmap_led_pis;
    QImage img_ddb, imgL1, imgL2, imgL;

    //ddb_sl
    QRect rect_ddb_sl = QRect(QPoint(190+80,250+29),QSize(512,16));
    pmap_ddb_sl = this->grab(rect_ddb_sl);
    img_ddb = pmap_ddb_sl.toImage().convertToFormat(QImage::Format_Mono,Qt::MonoOnly);

    ushort ba_ddb_sl[512]={0};
    int ba_idx=0, ba_idx_ctr=15;
    uchar temp;

    for(int y=0;y<16;y++)
    {
        for(int x=0;x<512;x++)
        {
            if(img_ddb.pixel(x,y)==0xFF000000)
            {
                temp=1;
            }
            else
            {
                temp=0;
            }

            ba_ddb_sl[ba_idx] |= (ushort)(temp << ba_idx_ctr);
            ba_idx_ctr--;
            if(ba_idx_ctr<0)
            {
                ba_idx_ctr=15;
                ba_idx++;
            }
        }
    }

    //ddb_dl
    const QRect rL1 = QRect(QPoint(190+80,250+54),QSize(512,16));
    const QRect rL2 = QRect(QPoint(190+80,250+77),QSize(512,16));
    pmap_ddb_dl_l1 = this->grab(rL1);
    pmap_ddb_dl_l2 = this->grab(rL2);

    imgL1 = pmap_ddb_dl_l1.toImage().convertToFormat(QImage::Format_Mono,Qt::MonoOnly);
    imgL2 = pmap_ddb_dl_l2.toImage().convertToFormat(QImage::Format_Mono,Qt::MonoOnly);

    imgL1.mirrored();
    imgL2.mirrored();

    ushort ba_ddb_dl[1024]={0};
    ba_idx=0;
    ba_idx_ctr=15;

    for(int y=0;y<16;y++)
    {
        //L1
        for(int x=0;x<64;x++)
        {
            if(imgL1.pixel(x,y)==0xFF000000)
            {
                temp=1;
            }
            else
            {
                temp=0;
            }

            ba_ddb_dl[ba_idx] |= (ushort)(temp << ba_idx_ctr);
            ba_idx_ctr--;
            if(ba_idx_ctr<0)
            {
                ba_idx_ctr=15;
                ba_idx++;
            }
        }

        //L2
        for(int x=0;x<448;x++)
        {
            if(imgL2.pixel(x,y)==0xFF000000)
            {
                temp=1;
            }
            else
            {
                temp=0;
            }

            ba_ddb_dl[ba_idx] |= (ushort)(temp << ba_idx_ctr);
            ba_idx_ctr--;
            if(ba_idx_ctr<0)
            {
                ba_idx_ctr=15;
                ba_idx++;
            }
        }
    }

    //    //led_pis
    const QRect rl = QRect(QPoint(190+80,250+102),QSize(512,16));
    pmap_led_pis = this->grab(rl);
    imgL = pmap_led_pis.toImage().convertToFormat(QImage::Format_Mono,Qt::MonoOnly);

    ushort ba_led_pis[512]={0};
    ba_idx=0;
    ba_idx_ctr=15;

    for(int y=0;y<16;y++)
    {
        for(int x=0;x<512;x++)
        {
            if(imgL.pixel(x,y)==0xFF000000)
            {
                temp=1;
            }
            else
            {
                temp=0;
            }

            ba_led_pis[ba_idx] |= (ushort)(temp << ba_idx_ctr);
            ba_idx_ctr--;
            if(ba_idx_ctr<0)
            {
                ba_idx_ctr=15;
                ba_idx++;
            }
        }
    }

    ptr_ddb_sl = (char*)ba_ddb_sl;
    ptr_ddb_dl = (char*)ba_ddb_dl;
    ptr_led_pis = (char*)ba_led_pis;

    int idx;
    for(idx=0; idx< papis_slaves.count();idx++)
    {
        if((papis_slaves.at(idx)->installStatus==INSTALLED&&papis_slaves.at(idx)->activeStatus==ACTIVE))
        {
            papis_slaves.at(idx)->socket = new QTcpSocket();
            switch(papis_slaves.at(idx)->device_type)
            {
            case DEV_LCD_PIS_18:
            case DEV_LCD_PIS_10:
                //send data to lcd pis
                papis_slaves.at(idx)->socket->connectToHost(papis_slaves.at(idx)->ip_addr,3669);
                //               papis_slaves.at(idx)->socket->write(ui->label_led_pis->text().toLocal8Bit());
                papis_slaves.at(idx)->socket->disconnect();
                papis_slaves.at(idx)->socket->close();
                break;
            case DEV_DDB_SL:
                papis_slaves.at(idx)->socket->connectToHost(papis_slaves.at(idx)->ip_addr,1000);
                papis_slaves.at(idx)->socket->write(ptr_ddb_sl,1024);
                papis_slaves.at(idx)->socket->disconnect();
                papis_slaves.at(idx)->socket->close();
                break;
            case DEV_DDB_DL:
                papis_slaves.at(idx)->socket->connectToHost(papis_slaves.at(idx)->ip_addr,1000);
                papis_slaves.at(idx)->socket->write(ptr_ddb_dl,1024);
                papis_slaves.at(idx)->socket->disconnect();
                papis_slaves.at(idx)->socket->close();
                break;
            case DEV_LED_PIS:
                papis_slaves.at(idx)->socket->connectToHost(papis_slaves.at(idx)->ip_addr,1000);
                papis_slaves.at(idx)->socket->write(ptr_led_pis,1024);
                papis_slaves.at(idx)->socket->disconnect();
                papis_slaves.at(idx)->socket->close();
                break;
            case DEV_TMS:

                break;
            default:
                break;
            }
        }
    }
}

void MainWindow::pis_drive_slogan()
{
    QString str = "      "+aRouteData->slogans.at(slogan_idx);
    //ui->label_led_pis->setText(str);
    // ui->label_lcd_pis->setText(str);
    update();
    drive_pis();
    emit slave_message_send();
}

void MainWindow::update_occ(QString stat)
{
    for(int idx=0; idx< papis_slaves.count();idx++)
    {
        if((papis_slaves.at(idx)->installStatus==INSTALLED
            &&papis_slaves.at(idx)->activeStatus==ACTIVE
            &&papis_slaves.at(idx)->conn_mode=="ETH"))
        {
            switch(papis_slaves.at(idx)->device_type)
            {
            case DEV_LCD_PIS_18:
            case DEV_LCD_PIS_10:
            {
                papis_slaves.at(idx)->socket = new QTcpSocket();
                papis_slaves.at(idx)->socket->connectToHost(papis_slaves.at(idx)->ip_addr,3669);
                papis_slaves.at(idx)->socket->write(QString("occ="+stat).toLocal8Bit());
                papis_slaves.at(idx)->socket->disconnect();
                papis_slaves.at(idx)->socket->close();
            }
                break;
            case DEV_DDB_SL:

                break;
            case DEV_DDB_DL:

                break;
            case DEV_LED_PIS:

                break;
            case DEV_TMS:

                break;
            default:
                break;
            }
        }
    }
}

void MainWindow::drive_ddb()
{
    drive_ddb485();
    //extract data
    QPixmap pmap_ddb_sl, pmap_ddb_dl_l1, pmap_ddb_dl_l2, pmap_led_pis;
    QImage img_ddb, imgL1, imgL2;

    //    printf("\n %s %d \n",__FUNCTION__,__LINE__);

    if(papis_sequence == PAPIS_NEXT)
    {
        //        ui->label_ddb_sl_p1->setText("");
        //        ui->label_ddb_sl_p2->setText(QDateTime::currentDateTime().toString("ddd MMMM d yy hh:mm"));
        //        ui->label_ddb_dl_p1->setText("");
        //        ui->label_ddb_dl_p2->setText(QDateTime::currentDateTime().toString("ddd MMMM d yy hh:mm"));
    }
    else
    {
        int asidx = aStnIdx;
        if(aStnIdx==-1){asidx=0;}

        switch(ddb_cycle)
        {
        case DDB_HI_ROUTE_1:
        case DDB_HI_ROUTE_2:
            //            ui->label_ddb_dl_p2->setFont(*font_hi);
            //            ui->label_ddb_sl_p2->setFont(*font_hi);
            //                 ui->label_ddb_sl_p1->setText(systemconfig.coach_no);
            //                 ui->label_ddb_sl_p2->setText("    "+aRouteData->st_list.at(asidx).tr_route_hi);
            //                 ui->label_ddb_dl_p1->setText(aRouteData->st_list.at(asidx).tr_no_hi+" "+systemconfig.coach_no);
            //                 ui->label_ddb_dl_p2->setText("    "+aRouteData->st_list.at(asidx).tr_route_hi);
            break;
        case DDB_HI_TR_NM:
            //            ui->label_ddb_dl_p2->setFont(*font_hi);
            //            ui->label_ddb_sl_p2->setFont(*font_hi);
            //                 ui->label_ddb_sl_p1->setText(aRouteData->st_list.at(asidx).tr_no_hi);
            //                 ui->label_ddb_sl_p2->setText("    "+aRouteData->st_list.at(asidx).tr_nm_hi);
            //                 ui->label_ddb_dl_p1->setText(aRouteData->st_list.at(asidx).tr_no_hi+" "+systemconfig.coach_no);
            //                 ui->label_ddb_dl_p2->setText("    "+aRouteData->st_list.at(asidx).tr_nm_hi);
            break;
        case DDB_HI_VIA:
            //            ui->label_ddb_dl_p2->setFont(*font_hi);
            //            ui->label_ddb_sl_p2->setFont(*font_hi);
            //                 ui->label_ddb_sl_p1->setText(aRouteData->st_list.at(asidx).tr_no_hi);
            //                 ui->label_ddb_sl_p2->setText("    "+aRouteData->st_list.at(asidx).tr_via_hi);
            //                 ui->label_ddb_dl_p1->setText(aRouteData->st_list.at(asidx).tr_no_hi+" "+systemconfig.coach_no);
            //                 ui->label_ddb_dl_p2->setText("    "+aRouteData->st_list.at(asidx).tr_via_hi);
            break;
        case DDB_EN_ROUTE_1:
        case DDB_EN_ROUTE_2:
            //            ui->label_ddb_dl_p2->setFont(*font_en);
            //            ui->label_ddb_sl_p2->setFont(*font_en);
            //             ui->label_ddb_sl_p1->setText(systemconfig.coach_no);
            //             ui->label_ddb_sl_p2->setText("    "+aRouteData->st_list.at(asidx).tr_route_en);
            //             ui->label_ddb_dl_p1->setText(aRouteData->st_list.at(asidx).tr_no_en+" "+systemconfig.coach_no);
            //             ui->label_ddb_dl_p2->setText("    "+aRouteData->st_list.at(asidx).tr_route_en);
            break;
        case DDB_EN_TR_NM:
            //            ui->label_ddb_dl_p2->setFont(*font_en);
            //            ui->label_ddb_sl_p2->setFont(*font_en);
            //             ui->label_ddb_sl_p1->setText(aRouteData->st_list.at(asidx).tr_no_en);
            //             ui->label_ddb_sl_p2->setText("    "+aRouteData->st_list.at(asidx).tr_nm_en);
            //             ui->label_ddb_dl_p1->setText(aRouteData->st_list.at(asidx).tr_no_en+" "+systemconfig.coach_no);
            //             ui->label_ddb_dl_p2->setText("    "+aRouteData->st_list.at(asidx).tr_nm_en);
            break;

        case DDB_EN_VIA:
            //            ui->label_ddb_dl_p2->setFont(*font_en);
            //            ui->label_ddb_sl_p2->setFont(*font_en);
            //             ui->label_ddb_sl_p1->setText(aRouteData->st_list.at(asidx).tr_no_en);
            //             ui->label_ddb_sl_p2->setText("    "+aRouteData->st_list.at(asidx).tr_via_en);
            //             ui->label_ddb_dl_p1->setText(aRouteData->st_list.at(asidx).tr_no_en+" "+systemconfig.coach_no);
            //             ui->label_ddb_dl_p2->setText("    "+aRouteData->st_list.at(asidx).tr_via_en);
            break;
        case DDB_RL_ROUTE_1:
        case DDB_RL_ROUTE_2:
            //             ui->label_ddb_sl_p1->setText(systemconfig.coach_no);
            //             ui->label_ddb_sl_p2->setText("    "+aRouteData->st_list.at(asidx).tr_route_rl);
            //             ui->label_ddb_dl_p1->setText(aRouteData->st_list.at(asidx).tr_no_rl+" "+systemconfig.coach_no);
            //             ui->label_ddb_dl_p2->setText("    "+aRouteData->st_list.at(asidx).tr_route_rl);
            break;
        case DDB_RL_TR_NM:
            //             ui->label_ddb_sl_p1->setText(aRouteData->st_list.at(asidx).tr_no_rl);
            //             ui->label_ddb_sl_p2->setText("    "+aRouteData->st_list.at(asidx).tr_nm_rl);
            //             ui->label_ddb_dl_p1->setText(aRouteData->st_list.at(asidx).tr_no_rl+" "+systemconfig.coach_no);
            //             ui->label_ddb_dl_p2->setText("    "+aRouteData->st_list.at(asidx).tr_nm_rl);
            break;
        case DDB_RL_VIA:
            //             ui->label_ddb_sl_p1->setText(aRouteData->st_list.at(asidx).tr_no_rl);
            //             ui->label_ddb_sl_p2->setText("    "+aRouteData->st_list.at(asidx).tr_via_rl);
            //             ui->label_ddb_dl_p1->setText(aRouteData->st_list.at(asidx).tr_no_rl+" "+systemconfig.coach_no);
            //             ui->label_ddb_dl_p2->setText("    "+aRouteData->st_list.at(asidx).tr_via_rl);
            break;
        case DDB_ERROR:
            //            ui->label_ddb_dl_p2->setFont(*font_en);
            //            ui->label_ddb_sl_p2->setFont(*font_en);
            //            ui->label_ddb_sl_p1->setText("");
            //            ui->label_ddb_sl_p2->setText("Welcome to Indian Railways");
            //            ui->label_ddb_dl_p1->setText("");
            //            ui->label_ddb_dl_p2->setText("Welcome to Indian Railways");
            break;
        }
    }

    //    printf("\n %s %d \n",__FUNCTION__,__LINE__);

    update();

    //ddb_sl
    QRect rect_ddb_sl = QRect(QPoint(90,250+30),QSize(512,16));
    pmap_ddb_sl = this->grab(rect_ddb_sl);
    img_ddb = pmap_ddb_sl.toImage().convertToFormat(QImage::Format_Mono,Qt::MonoOnly);

    ushort ba_ddb_sl[513]={0};
    int ba_idx=0, ba_idx_ctr=15;
    uchar temp;

    for(int y=0;y<16;y++)
    {
        for(int x=0;x<512;x++)
        {
            if(img_ddb.pixel(x,y)==0xFF000000)
            {
                temp=1;
            }
            else
            {
                temp=0;
            }

            ba_ddb_sl[ba_idx] |= (ushort)(temp << ba_idx_ctr);
            ba_idx_ctr--;
            if(ba_idx_ctr<0)
            {
                ba_idx_ctr=15;
                ba_idx++;
            }
        }
    }
    ba_ddb_sl[0]=(short)systemconfig.led_pis_speed;

    //ddb_dl
    const QRect rL1 = QRect(QPoint(90,250+54),QSize(512,16));
    const QRect rL2 = QRect(QPoint(90,250+79),QSize(512,16));
    pmap_ddb_dl_l1 = this->grab(rL1);
    pmap_ddb_dl_l2 = this->grab(rL2);

    imgL1 = pmap_ddb_dl_l1.toImage().convertToFormat(QImage::Format_Mono,Qt::MonoOnly);
    imgL2 = pmap_ddb_dl_l2.toImage().convertToFormat(QImage::Format_Mono,Qt::MonoOnly);

    imgL1.mirrored();
    imgL2.mirrored();

    ushort ba_ddb_dl[1024]={0};
    ba_idx=0;
    ba_idx_ctr=15;

    for(int y=0;y<16;y++)
    {
        //L1
        for(int x=0;x<64;x++)
        {
            if(imgL1.pixel(x,y)==0xFF000000)
            {
                temp=1;
            }
            else
            {
                temp=0;
            }

            ba_ddb_dl[ba_idx] |= (ushort)(temp << ba_idx_ctr);
            ba_idx_ctr--;
            if(ba_idx_ctr<0)
            {
                ba_idx_ctr=15;
                ba_idx++;
            }
        }

        //L2
        for(int x=0;x<448;x++)
        {
            if(imgL2.pixel(x,y)==0xFF000000)
            {
                temp=1;
            }
            else
            {
                temp=0;
            }

            ba_ddb_dl[ba_idx] |= (ushort)(temp << ba_idx_ctr);
            ba_idx_ctr--;
            if(ba_idx_ctr<0)
            {
                ba_idx_ctr=15;
                ba_idx++;
            }
        }
    }

    ptr_ddb_sl = (char*)ba_ddb_sl;
    ptr_ddb_dl = (char*)ba_ddb_dl;

    int idx;
    for(idx=0; idx< papis_slaves.count();idx++)
    {
        if((papis_slaves.at(idx)->installStatus==INSTALLED&&papis_slaves.at(idx)->activeStatus==ACTIVE))
            //        if(papis_slaves.at(idx)->installStatus==INSTALLED)
        {
            papis_slaves.at(idx)->socket = new QTcpSocket();
            //            qDebug() << QString("Sending data to %1").arg(papis_slaves.at(idx)->ip_addr);

            switch(papis_slaves.at(idx)->device_type)
            {
            case DEV_LCD_PIS_18:
            case DEV_LCD_PIS_10:

                break;
            case DEV_DDB_SL:
                papis_slaves.at(idx)->socket->connectToHost(papis_slaves.at(idx)->ip_addr,1000);
                papis_slaves.at(idx)->socket->write(ptr_ddb_sl,1024);
                papis_slaves.at(idx)->socket->disconnect();
                papis_slaves.at(idx)->socket->close();
                break;
            case DEV_DDB_DL:
                papis_slaves.at(idx)->socket->connectToHost(papis_slaves.at(idx)->ip_addr,1000);
                papis_slaves.at(idx)->socket->write(ptr_ddb_dl,1024);
                papis_slaves.at(idx)->socket->disconnect();
                papis_slaves.at(idx)->socket->close();
                break;
            case DEV_LED_PIS:

                break;
            case DEV_TMS:

                break;
            default:
                break;
            }
        }
    }
}

QFont* MainWindow::getRLFont(QString rl)
{
    if(rl=="HINDI")
    {
        return font_hi;
    }else
        if(rl=="BANGLA")
        {
            return font_ban;
        }else
            if(rl=="GUJARATI")
            {
                return font_guj;
            }else
                if(rl=="KANNADA")
                {
                    return font_kan;
                }else
                    if(rl=="MALAYALAM")
                    {
                        return font_mal;
                    }else
                        if(rl=="MARATHI")
                        {
                            return font_hi;
                        }else
                            if(rl=="NEPALI")
                            {
                                return font_hi;
                            }else
                                if(rl=="ODIA")
                                {
                                    return font_odia;
                                }else
                                    if(rl=="PUNJABI")
                                    {
                                        return font_pun;
                                    }else
                                        if(rl=="SINDHI")
                                        {
                                            return font_urdu;
                                        }else
                                            if(rl=="TAMIL")
                                            {
                                                return font_tam;
                                            }else
                                                if(rl=="TELUGU")
                                                {
                                                    return font_tel;
                                                }else
                                                    if(rl=="URDU")
                                                    {
                                                        return font_urdu;
                                                    }
                                                    else
                                                    {
                                                        return font_hi;
                                                    }
}

//rs485
void MainWindow::drive_ddb485()
{
    if(papis_sequence == PAPIS_NEXT)
    {
        //        ui->label_ddb_sl_p1->setText("");
        //        ui->label_ddb_sl_p2->setText(QDateTime::currentDateTime().toString("ddd MMMM d yy hh:mm"));
        //        ui->label_ddb_dl_p1->setText("");
        //        ui->label_ddb_dl_p2->setText(QDateTime::currentDateTime().toString("ddd MMMM d yy hh:mm"));
    }
    else
    {
        int asidx = aStnIdx;
        if(aStnIdx==-1){asidx=0;}

        switch(ddb_cycle)
        {
        case DDB_HI_ROUTE_1:
        case DDB_HI_ROUTE_2:
            //            ui->label_ddb_dl_p2->setFont(*font_hi);
            //            ui->label_ddb_sl_p2->setFont(*font_hi);
            //                 ui->label_ddb_sl_p1->setText(systemconfig.coach_no);
            //                 ui->label_ddb_sl_p2->setText("    "+aRouteData->st_list.at(asidx).tr_route_hi);
            //                 ui->label_ddb_dl_p1->setText(aRouteData->st_list.at(asidx).tr_no_hi+","+systemconfig.coach_no);
            //                 ui->label_ddb_dl_p2->setText("    "+aRouteData->st_list.at(asidx).tr_route_hi);
            break;
        case DDB_HI_TR_NM:
            //            ui->label_ddb_dl_p2->setFont(*font_hi);
            //            ui->label_ddb_sl_p2->setFont(*font_hi);
            //                 ui->label_ddb_sl_p1->setText(aRouteData->st_list.at(asidx).tr_no_hi);
            //                 ui->label_ddb_sl_p2->setText("    "+aRouteData->st_list.at(asidx).tr_nm_hi);
            //                 ui->label_ddb_dl_p1->setText(aRouteData->st_list.at(asidx).tr_no_hi+","+systemconfig.coach_no);
            //                 ui->label_ddb_dl_p2->setText("    "+aRouteData->st_list.at(asidx).tr_nm_hi);
            break;
        case DDB_HI_VIA:
            //            ui->label_ddb_dl_p2->setFont(*font_hi);
            //            ui->label_ddb_sl_p2->setFont(*font_hi);
            //                 ui->label_ddb_sl_p1->setText(aRouteData->st_list.at(asidx).tr_no_hi);
            //                 ui->label_ddb_sl_p2->setText("    "+aRouteData->st_list.at(asidx).tr_via_hi);
            //                 ui->label_ddb_dl_p1->setText(aRouteData->st_list.at(asidx).tr_no_hi+","+systemconfig.coach_no);
            //                 ui->label_ddb_dl_p2->setText("    "+aRouteData->st_list.at(asidx).tr_via_hi);
            break;
        case DDB_EN_ROUTE_1:
        case DDB_EN_ROUTE_2:
            //            ui->label_ddb_dl_p2->setFont(*font_en);
            //            ui->label_ddb_sl_p2->setFont(*font_en);
            //             ui->label_ddb_sl_p1->setText(systemconfig.coach_no);
            //             ui->label_ddb_sl_p2->setText("    "+aRouteData->st_list.at(asidx).tr_route_en);
            //             ui->label_ddb_dl_p1->setText(aRouteData->st_list.at(asidx).tr_no_en+","+systemconfig.coach_no);
            //             ui->label_ddb_dl_p2->setText("    "+aRouteData->st_list.at(asidx).tr_route_en);
            break;
        case DDB_EN_TR_NM:
            //            ui->label_ddb_dl_p2->setFont(*font_en);
            //            ui->label_ddb_sl_p2->setFont(*font_en);
            //             ui->label_ddb_sl_p1->setText(aRouteData->st_list.at(asidx).tr_no_en);
            //             ui->label_ddb_sl_p2->setText("    "+aRouteData->st_list.at(asidx).tr_nm_en);
            //             ui->label_ddb_dl_p1->setText(aRouteData->st_list.at(asidx).tr_no_en+","+systemconfig.coach_no);
            //             ui->label_ddb_dl_p2->setText("    "+aRouteData->st_list.at(asidx).tr_nm_en);
            break;

        case DDB_EN_VIA:
            //            ui->label_ddb_dl_p2->setFont(*font_en);
            //            ui->label_ddb_sl_p2->setFont(*font_en);
            //             ui->label_ddb_sl_p1->setText(aRouteData->st_list.at(asidx).tr_no_en);
            //             ui->label_ddb_sl_p2->setText("    "+aRouteData->st_list.at(asidx).tr_via_en);
            //             ui->label_ddb_dl_p1->setText(aRouteData->st_list.at(asidx).tr_no_en+","+systemconfig.coach_no);
            //             ui->label_ddb_dl_p2->setText("    "+aRouteData->st_list.at(asidx).tr_via_en);
            break;
        case DDB_RL_ROUTE_1:
        case DDB_RL_ROUTE_2:
            //             ui->label_ddb_sl_p1->setText(systemconfig.coach_no);
            //             ui->label_ddb_sl_p2->setText("    "+aRouteData->st_list.at(asidx).tr_route_rl);
            //             ui->label_ddb_dl_p1->setText(aRouteData->st_list.at(asidx).tr_no_rl+","+systemconfig.coach_no);
            //             ui->label_ddb_dl_p2->setText("    "+aRouteData->st_list.at(asidx).tr_route_rl);
            break;
        case DDB_RL_TR_NM:
            //             ui->label_ddb_sl_p1->setText(aRouteData->st_list.at(asidx).tr_no_rl);
            //             ui->label_ddb_sl_p2->setText("    "+aRouteData->st_list.at(asidx).tr_nm_rl);
            //             ui->label_ddb_dl_p1->setText(aRouteData->st_list.at(asidx).tr_no_rl+","+systemconfig.coach_no);
            //             ui->label_ddb_dl_p2->setText("    "+aRouteData->st_list.at(asidx).tr_nm_rl);
            break;
        case DDB_RL_VIA:
            //             ui->label_ddb_sl_p1->setText(aRouteData->st_list.at(asidx).tr_no_rl);
            //             ui->label_ddb_sl_p2->setText("    "+aRouteData->st_list.at(asidx).tr_via_rl);
            //             ui->label_ddb_dl_p1->setText(aRouteData->st_list.at(asidx).tr_no_rl+","+systemconfig.coach_no);
            //             ui->label_ddb_dl_p2->setText("    "+aRouteData->st_list.at(asidx).tr_via_rl);
            break;
        case DDB_ERROR:
            //            ui->label_ddb_dl_p2->setFont(*font_en);
            //            ui->label_ddb_sl_p2->setFont(*font_en);
            //            ui->label_ddb_sl_p1->setText("");
            //            ui->label_ddb_sl_p2->setText("WELCOME TO INDIAN RAILWAYS");
            //            ui->label_ddb_dl_p1->setText("");
            //            ui->label_ddb_dl_p2->setText("WELCOME TO INDIAN RAILWAYS");
            if(error_displayed)
            {
                return;
            }
            else
            {
                error_displayed=true;
            }
            break;
        }
    }

    update();

    //ddb_sl
    QPixmap pmap_ddb_sl;
    QImage img_ddb;
    QRect rect_ddb_sl = QRect(QPoint(90,250+30),QSize(512,16));
    pmap_ddb_sl = this->grab(rect_ddb_sl);
    img_ddb = pmap_ddb_sl.toImage().convertToFormat(QImage::Format_Mono,Qt::MonoOnly);

    for(int idx=0;idx<512;idx++)
    {
        ba_ddb_sl[idx]=0;
    }

    int ba_idx=0, ba_idx_ctr=15;
    uchar temp;

    for(int y=0;y<16;y++)
    {
        for(int x=0;x<512;x++)
        {
            if(img_ddb.pixel(x,y)==0xFF000000)
            {
                temp=1;
            }
            else
            {
                temp=0;
            }

            ba_ddb_sl[ba_idx] |= (ushort)(temp << ba_idx_ctr);
            ba_idx_ctr--;
            if(ba_idx_ctr<0)
            {
                ba_idx_ctr=15;
                ba_idx++;
            }
        }
    }
    emit rs485_drive_matrix(SD_DRIVE_DDB_SL,(char*)ba_ddb_sl,1024,DISP_REP,SCRL_LEFT,day_night_mode);

    //ddb_dl
    QPixmap pmap_ddb_dl_l1, pmap_ddb_dl_l2;
    QImage imgL1, imgL2;

    //ddb_dl
    const QRect rL1 = QRect(QPoint(90,250+54),QSize(512,16));
    const QRect rL2 = QRect(QPoint(90,250+79),QSize(512,16));
    pmap_ddb_dl_l1 = this->grab(rL1);
    pmap_ddb_dl_l2 = this->grab(rL2);

    imgL1 = pmap_ddb_dl_l1.toImage().convertToFormat(QImage::Format_Mono,Qt::MonoOnly);
    imgL2 = pmap_ddb_dl_l2.toImage().convertToFormat(QImage::Format_Mono,Qt::MonoOnly);

    imgL1.mirrored();
    imgL2.mirrored();

    for(int idx=0;idx<1024;idx++)
    {
        ba_ddb_dl[idx]=0;
    }

    ba_idx=0, ba_idx_ctr=15;

    for(int y=0;y<16;y++)
    {
        //L1
        for(int x=0;x<64;x++)
        {
            if(imgL1.pixel(x,y)==0xFF000000)
            {
                temp=1;
            }
            else
            {
                temp=0;
            }

            ba_ddb_dl[ba_idx] |= (ushort)(temp << ba_idx_ctr);
            ba_idx_ctr--;
            if(ba_idx_ctr<0)
            {
                ba_idx_ctr=15;
                ba_idx++;
            }
        }

        //L2
        for(int x=0;x<448;x++)
        {
            if(imgL2.pixel(x,y)==0xFF000000)
            {
                temp=1;
            }
            else
            {
                temp=0;
            }

            ba_ddb_dl[ba_idx] |= (ushort)(temp << ba_idx_ctr);
            ba_idx_ctr--;
            if(ba_idx_ctr<0)
            {
                ba_idx_ctr=15;
                ba_idx++;
            }
        }
    }
    emit rs485_drive_matrix(SD_DRIVE_DDB_DL,(char*)ba_ddb_dl,1024,DISP_REP,SCRL_LEFT,day_night_mode);
}

//rs485
void MainWindow::drive_pis485()
{
    QPixmap pmap_led_pis;
    QImage imgL;

    const QRect rl = QRect(QPoint(90,250+104),QSize(512,16));
    pmap_led_pis = this->grab(rl);
    imgL = pmap_led_pis.toImage().convertToFormat(QImage::Format_Mono,Qt::MonoOnly);

    for(int idx=0;idx<512;idx++)
    {
        ba_led_pis[idx]=0;
    }

    int ba_idx=0, ba_idx_ctr=15;
    uchar temp;

    for(int y=0;y<16;y++)
    {
        for(int x=0;x<512;x++)
        {
            if(imgL.pixel(x,y)==0xFF000000)
            {
                temp=1;
            }
            else
            {
                temp=0;
            }

            ba_led_pis[ba_idx] |= (ushort)(temp << ba_idx_ctr);
            ba_idx_ctr--;
            if(ba_idx_ctr<0)
            {
                ba_idx_ctr=15;
                ba_idx++;
            }
        }
    }
    emit rs485_drive_matrix(SD_DRIVE_LED_PIS,(char*)ba_led_pis,1024,DISP_REP,SCRL_LEFT,day_night_mode);
}

void MainWindow::drive_pis()
{
    drive_pis485();
    //    //led_pis
    //extract data
    QPixmap pmap_led_pis;
    QImage imgL;

    const QRect rl = QRect(QPoint(90,250+104),QSize(512,16));
    pmap_led_pis = this->grab(rl);
    imgL = pmap_led_pis.toImage().convertToFormat(QImage::Format_Mono,Qt::MonoOnly);

    ushort ba_led_pis[513]={0};
    int ba_idx=0, ba_idx_ctr=15;
    uchar temp;

    for(int y=0;y<16;y++)
    {
        for(int x=0;x<512;x++)
        {
            if(imgL.pixel(x,y)==0xFF000000)
            {
                temp=1;
            }
            else
            {
                temp=0;
            }

            ba_led_pis[ba_idx] |= (ushort)(temp << ba_idx_ctr);
            ba_idx_ctr--;
            if(ba_idx_ctr<0)
            {
                ba_idx_ctr=15;
                ba_idx++;
            }
        }
    }

    ba_led_pis[0]=(short)systemconfig.led_pis_speed;
    ptr_led_pis = (char*)ba_led_pis;

    int idx;
    for(idx=0; idx< papis_slaves.count();idx++)
    {
        if((papis_slaves.at(idx)->installStatus==INSTALLED&&papis_slaves.at(idx)->activeStatus==ACTIVE))
        {
            papis_slaves.at(idx)->socket = new QTcpSocket();

            switch(papis_slaves.at(idx)->device_type)
            {
            case DEV_LCD_PIS_18:
            case DEV_LCD_PIS_10:
                break;
            case DEV_DDB_SL:

                break;
            case DEV_DDB_DL:

                break;
            case DEV_LED_PIS:
                papis_slaves.at(idx)->socket->connectToHost(papis_slaves.at(idx)->ip_addr,1000);
                papis_slaves.at(idx)->socket->write(ptr_led_pis,1024);
                papis_slaves.at(idx)->socket->disconnect();
                papis_slaves.at(idx)->socket->close();
                break;
            case DEV_TMS:

                break;
            default:
                break;
            }
        }
    }
}

//void MainWindow::img()
//{
//    QPixmap pmap_led_pis;
//    QImage imgL;

//    QPixmap pmap_ddb_sl, pmap_ddb_dl_l1, pmap_ddb_dl_l2;
//    QImage img_ddb, imgL1, imgL2;

//    const QRect rl = QRect(QPoint(90,250+104),QSize(512,16));
//    pmap_led_pis = this->grab(rl);
//    imgL = pmap_led_pis.toImage().convertToFormat(QImage::Format_Mono,Qt::MonoOnly);
//    imgL.save("./img_pis.png");

//    QRect rect_ddb_sl = QRect(QPoint(90,250+30),QSize(512,16));
//    pmap_ddb_sl = this->grab(rect_ddb_sl);
//    img_ddb = pmap_ddb_sl.toImage().convertToFormat(QImage::Format_Mono,Qt::MonoOnly);
//    img_ddb.save("./img_ddb_sl.png");

//    const QRect rL1 = QRect(QPoint(90,250+54),QSize(512,16));
//    const QRect rL2 = QRect(QPoint(90,250+79),QSize(512,16));
//    pmap_ddb_dl_l1 = this->grab(rL1);
//    pmap_ddb_dl_l2 = this->grab(rL2);

//    imgL1 = pmap_ddb_dl_l1.toImage().convertToFormat(QImage::Format_Mono,Qt::MonoOnly);
//    imgL2 = pmap_ddb_dl_l2.toImage().convertToFormat(QImage::Format_Mono,Qt::MonoOnly);

//    imgL1.mirrored();
//    imgL2.mirrored();

//    imgL1.save("./img_ddb_sl_1.png");
//    imgL2.save("./img_ddb_sl_2.png");
//}

void MainWindow::drive_pis_rs485(dispType_t dtype, scrlDir_t sdir, int scols)
{
    QPixmap pmap_led_pis;
    QImage imgL;

    const QRect rl = QRect(QPoint(90,250+104),QSize(512,16));
    pmap_led_pis = this->grab(rl);
    imgL = pmap_led_pis.toImage().convertToFormat(QImage::Format_Mono,Qt::MonoOnly);

    for(int idx=0;idx<512;idx++)
    {
        ba_led_pis[idx]=0;
    }

    int ba_idx=0, ba_idx_ctr=15;
    uchar temp;

    for(int y=0;y<16;y++)
    {
        for(int x=0;x<512;x++)
        {
            if(imgL.pixel(x,y)==0xFF000000)
            {
                temp=1;
            }
            else
            {
                temp=0;
            }

            ba_led_pis[ba_idx] |= (ushort)(temp << ba_idx_ctr);
            ba_idx_ctr--;
            if(ba_idx_ctr<0)
            {
                ba_idx_ctr=15;
                ba_idx++;
            }
        }
    }

    emit rs485_drive_matrix(SD_DRIVE_LED_PIS,(char*)ba_led_pis,1024,DISP_REP,SCRL_LEFT,day_night_mode);
}

void MainWindow::drive_ddb_sl_rs485(dispType_t dtype, scrlDir_t sdir, int scols)
{
    //extract data
    QPixmap pmap_ddb_sl;
    QImage img_ddb;

    //ddb_sl
    QRect rect_ddb_sl = QRect(QPoint(90,250+30),QSize(512,16));
    pmap_ddb_sl = this->grab(rect_ddb_sl);
    img_ddb = pmap_ddb_sl.toImage().convertToFormat(QImage::Format_Mono,Qt::MonoOnly);

    for(int idx=0;idx<512;idx++)
    {
        ba_ddb_sl[idx]=0;
    }

    int ba_idx=0, ba_idx_ctr=15;
    uchar temp;

    for(int y=0;y<16;y++)
    {
        for(int x=0;x<512;x++)
        {
            if(img_ddb.pixel(x,y)==0xFF000000)
            {
                temp=1;
            }
            else
            {
                temp=0;
            }

            ba_ddb_sl[ba_idx] |= (ushort)(temp << ba_idx_ctr);
            ba_idx_ctr--;
            if(ba_idx_ctr<0)
            {
                ba_idx_ctr=15;
                ba_idx++;
            }
        }
    }

    emit rs485_drive_matrix(SD_DRIVE_DDB_SL,(char*)ba_ddb_sl,1024,DISP_REP,SCRL_LEFT,day_night_mode);
}

void MainWindow::drive_ddb_dl_rs485(dispType_t dtype, scrlDir_t sdir, int scols)
{
    QPixmap pmap_ddb_dl_l1, pmap_ddb_dl_l2;
    QImage img_ddb, imgL1, imgL2;

    //ddb_dl
    const QRect rL1 = QRect(QPoint(90,250+54),QSize(512,16));
    const QRect rL2 = QRect(QPoint(90,250+79),QSize(512,16));
    pmap_ddb_dl_l1 = this->grab(rL1);
    pmap_ddb_dl_l2 = this->grab(rL2);

    imgL1 = pmap_ddb_dl_l1.toImage().convertToFormat(QImage::Format_Mono,Qt::MonoOnly);
    imgL2 = pmap_ddb_dl_l2.toImage().convertToFormat(QImage::Format_Mono,Qt::MonoOnly);

    imgL1.mirrored();
    imgL2.mirrored();

    for(int idx=0;idx<1024;idx++)
    {
        ba_ddb_dl[idx]=0;
    }

    int ba_idx=0, ba_idx_ctr=15;
    uchar temp;

    for(int y=0;y<16;y++)
    {
        //L1
        for(int x=0;x<64;x++)
        {
            if(imgL1.pixel(x,y)==0xFF000000)
            {
                temp=1;
            }
            else
            {
                temp=0;
            }

            ba_ddb_dl[ba_idx] |= (ushort)(temp << ba_idx_ctr);
            ba_idx_ctr--;
            if(ba_idx_ctr<0)
            {
                ba_idx_ctr=15;
                ba_idx++;
            }
        }

        //L2
        for(int x=0;x<448;x++)
        {
            if(imgL2.pixel(x,y)==0xFF000000)
            {
                temp=1;
            }
            else
            {
                temp=0;
            }

            ba_ddb_dl[ba_idx] |= (ushort)(temp << ba_idx_ctr);
            ba_idx_ctr--;
            if(ba_idx_ctr<0)
            {
                ba_idx_ctr=15;
                ba_idx++;
            }
        }
    }

    emit rs485_drive_matrix(SD_DRIVE_DDB_DL,(char*)ba_ddb_dl,1024,DISP_REP,SCRL_LEFT,day_night_mode);
}

void MainWindow::slave_send_message()
{
    int idx;
    for(idx=0; idx< papis_slaves.count();idx++)
    {
        if((papis_slaves.at(idx)->installStatus==INSTALLED&&papis_slaves.at(idx)->activeStatus==ACTIVE))
        {
            papis_slaves.at(idx)->socket = new QTcpSocket();

            switch(papis_slaves.at(idx)->device_type)
            {
            case DEV_LCD_PIS_18:
            case DEV_LCD_PIS_10:
            {
                QString ip = papis_slaves.at(idx)->ip_addr;

                QJsonObject json;
                json["msg"]=//ui->label_led_pis->text();
                        json["lang"]=lang;
                QJsonDocument rDoc(json);
                QByteArray rArray = rDoc.toJson();

                QString msg = "msg="+rArray;
                //               QString msg = "msg="+//ui->label_led_pis->text();
                papis_slaves.at(idx)->socket->connectToHost(papis_slaves.at(idx)->ip_addr,3669);
                papis_slaves.at(idx)->socket->write(msg.toLocal8Bit());
                papis_slaves.at(idx)->socket->disconnect();
                papis_slaves.at(idx)->socket->close();
            }
                break;
            case DEV_DDB_SL:

                break;
            case DEV_DDB_DL:

                break;
            case DEV_LED_PIS:

                break;
            case DEV_TMS:

                break;
            default:
                break;
            }
        }
    }

    //    slave_get_logs();
}

void MainWindow::slave_get_logs()
{
    int idx;
    for(idx=0; idx< papis_slaves.count();idx++)
    {
        if((papis_slaves.at(idx)->installStatus==INSTALLED&&papis_slaves.at(idx)->activeStatus==ACTIVE))
        {
            papis_slaves.at(idx)->socket = new QTcpSocket();

            switch(papis_slaves.at(idx)->device_type)
            {
            case DEV_LCD_PIS_18:
            case DEV_LCD_PIS_10:
            {
                papis_slaves.at(idx)->socket->connectToHost(papis_slaves.at(idx)->ip_addr,3669);
                papis_slaves.at(idx)->socket->write(QString("log="+systemconfig.eth_ip_addr).toLocal8Bit());
                papis_slaves.at(idx)->socket->disconnect();
                papis_slaves.at(idx)->socket->close();
            }
                break;
            case DEV_DDB_SL:

                break;
            case DEV_DDB_DL:

                break;
            case DEV_LED_PIS:

                break;
            case DEV_TMS:

                break;
            default:
                break;
            }
        }
    }
}

route_type_t MainWindow::wakeupMidRoute()
{
    for(int idx=0;idx<routesInfo.routeSel.count();idx++)
    {
        for(int jdx=0;jdx<route_data.at(idx)->st_list.count();jdx++)
        {
            if(Calc_distance_between_locations(route_data.at(idx)->st_list.at(jdx).gps_lon
                                               , route_data.at(idx)->st_list.at(jdx).gps_lat
                                               , curr_long, curr_lat)<= route_data.at(idx)->st_list.at(jdx).arr_r)
            {
                //parse probable list to see if thre is a similar entry existing
                bool match=false;

                for(int kdx=0;kdx<pList.count();kdx++)
                {
                    if(pList.at(kdx)->route == idx && pList.at(kdx)->stn == jdx)
                    {
                        match = true;
                    }
                }

                if(!match)
                {
                    //process
                    for(int kdx=0;kdx<pList.count();kdx++)
                    {
                        if(pList.at(kdx)->route == idx)
                        {
                            if(pList.at(kdx)->stn == jdx-1)
                            {
                                //match
                                w_log(",Route Auto Set to "+route_data.at(idx)->st_list.at(jdx).tr_no_en+ " Station: "+route_data.at(idx)->st_list.at(jdx).st_name_en);
                                aRIdx = routesInfo.routeSel.at(idx);
                                aRouteData = route_data.at(idx);
                                aStnIdx = jdx;
                                pList.clear();
                                return (route_type_t)idx;
                            }
                        }
                    }
                    pList.append(new probable(idx,jdx));
                }
            }
        }
    }

    return UNKNOWN;
}

route_type_t MainWindow::wakeupStart()
{        
    for(int idx=0;idx<routesInfo.routeSel.count();idx++)
    {
        //see if we are at the start of any route
        if(Calc_distance_between_locations(route_data.at(idx)->st_list.at(0).gps_lon
                                           , route_data.at(idx)->st_list.at(0).gps_lat
                                           , curr_long, curr_lat)<=1)
        {
            int dDiff = route_data.at(idx)->st_list.at(0).time_dep.msecsSinceStartOfDay()
                    - gpsDateTime.time().msecsSinceStartOfDay();

            if(dDiff>0)
            {
                lr=false;
            }
            else
            {
                lr=true;
                lateBy = QTime::fromMSecsSinceStartOfDay(-1*dDiff);
            }

            aRIdx = routesInfo.routeSel.at(idx);
            aRouteData = route_data.at(idx/*aRIdx*/);
            aStnIdx = 0;

            w_log(",Route Auto Set to "+route_data.at(idx)->st_list.at(0).tr_no_en+ " Station: "+route_data.at(idx)->st_list.at(0).st_name_en);
            return (route_type_t)idx;
        }
    }

    //we are not at any station
    pList.clear();
    return UNKNOWN;
}

void MainWindow::assign_route()
{
    pCtr=0;
    //ui->label_tr_name->setText(routesInfo.routeList.at(aRIdx).tr_no_en+", "+routesInfo.routeList.at(aRIdx).tr_nm_en);
    //ui->label_tr_route->setText(routesInfo.routeList.at(aRIdx).tr_route_en);
    //ui->label_tr_via->setText(routesInfo.routeList.at(aRIdx).tr_via_en);
    //ui->label_curr_stn->setText(aRouteData->st_list.at(0).st_name_en);
    //ui->label_next_stn->setText(aRouteData->st_list.at(1).st_name_en);
    //    ui->label_next_dist->setText(QString::number(aRouteData->st_list.at(1).dist_km,'_f',1)+" km");
    //    ui->label_sch_time->setText(QString::number(aRouteData->st_list.at(1).time_arr_hh)+":"+QString::number(aRouteData->st_list.at(1).time_arr_mm));

    w_log(",Update Active Route Details: Train "+ routesInfo.routeList.at(aRIdx).tr_no_en+", "+routesInfo.routeList.at(aRIdx).tr_nm_en);

    if(lr)
    {
        if(lateBy.hour()>0)
        {
            //            ui->label_late_stat->setText(QString::number(lateBy.hour())+" hr, "+QString::number(lateBy.minute())+" min");
        }
        else
        {
            //            ui->label_late_stat->setText(QString::number(lateBy.minute())+" min");
        }
    }
    emit slave_route_send();
    ddb_cycle = DDB_HI_ROUTE_1;
    ddbCycler = new QTimer(this);
    connect(ddbCycler, SIGNAL(timeout()),this, SLOT(ddbCycleTimeout()));
    connect(this, SIGNAL(ddb_drive()),this, SLOT(drive_ddb()));
    connect(this, SIGNAL(pis_slogan_drive()),this, SLOT(pis_drive_slogan()));
    drive_ddb();
    w_log(",External DDB Update Process Started, Cycle time: 10s");

    update();ddbCycler->start(60000);
    wake=false;
}

void MainWindow::start_route()
{
    route_decision();
}

void MainWindow::stop_route()
{
    route_ongoing=false;
    error_mode();
    nmeaSource->stopUpdates();
}

void MainWindow::positionUpdated(QGeoPositionInfo positionInfo)
{
    //if not fix, then we will wait
    if(positionInfo.isValid())
    {
        gps_pidx++;

#ifndef LAPTOP
        // IST Tzone
        gpsDateTime = positionInfo.timestamp().addSecs(19800);
        //3 sec tolerance is allowed
        int ms = gpsDateTime.msecsTo(QDateTime::currentDateTime());
        if(ms>3000 || ms<-3000)
        {
            system(QString("timedatectl set-time '"+gpsDateTime.toString("yyyy-MM-dd hh:mm:ss")+"'").toLocal8Bit());
            //system(QString("hwclock -w").toLocal8Bit());
            /*
            timedatectl set-time '2021-06-29 23:00:00'
            hwclock -w
            */
        }
#else
        gpsDateTime = QDateTime::currentDateTime();
#endif

        curr_lat = positionInfo.coordinate().latitude();
        curr_long = positionInfo.coordinate().longitude();
        cur_tim = gpsDateTime.time();

        //        ui->label_gps_sat->setText((positionInfo.isValid())?"Fix":"No-Fix");
        //        ui->label_gps_lat->setText(QString::number(curr_lat)+"°");
        //        ui->label_gps_lon->setText(QString::number(curr_long)+"°");
        //        ui->label_gps_ele->setText(QString::number(positionInfo.coordinate().altitude())+" m");

        if(!gpsFix)
        {
            ui->label_gps->setStyleSheet("color: rgb(255, 255, 255);image: url(:/images/icons/icon_gps.png); border-color: rgb(255, 255, 255);");
            w_log(",GPS Obtained Fix");
        }

        if(gps_ctr==0)
        {
            pCtr+=1.0;

            if(wake)
            {
                route_type = wakeupStart();
                switch(route_type)
                {
                case WAIT:
                {
                    //the system has been started prior to scheduled time and thus is waiting
                }
                    break;
                case ROUTE_1:
                case ROUTE_2:
                case ROUTE_3:
                case ROUTE_4:
                case ROUTE_5:
                case ROUTE_6:
                {
                    assign_route();
                }
                    break;
                case UNKNOWN:
                {
                    error_displayed=false;
                    ddb_cycle=DDB_ERROR;
                    wake=false;
                }
                    break;
                }
            }
            else
            {
                if(gps_fp)
                {
                    k_lat=curr_lat;
                    k_lon=curr_long;
                    gps_fp=false;
                }
                else
                {
                    //calculate speed
                    c_speed = (3600*Calc_distance_between_locations(curr_long, curr_lat, prev_long, prev_lat))/(prev_tim.msecsTo(cur_tim)/1000);
                    avg_speed = (avg_speed*(pCtr-1) + c_speed)/pCtr;

                    //                    ui->label_gps_speed->setText(QString::number(c_speed,'_f',1)+" km/h");
                    //                    ui->label_gps_aspeed->setText(QString::number(avg_speed,'_f',1)+" km/h");

                    //km run
                    kmD=Calc_distance_between_locations(curr_long, curr_lat, k_lon, k_lat);
                    if(kmD>1.0)
                    {
                        k_lat=curr_lat;
                        k_lon=curr_long;
                        kmR+=kmD;

                        if(route_type!=WAIT && route_type!=UNKNOWN)
                        {
                            if(aStnIdx==aRouteData->st_list.count()-1)
                            {
                                kmRt=aRouteData->st_list.at(aRouteData->st_list.count()-1).dist_km;
                            }
                            else
                                if(aStnIdx!=-1)
                                {
                                    // kmRt+=kmD;

                                    //dist from prev stn + dist to prev stn:
                                    kmRt = aRouteData->st_list.at(aStnIdx).dist_km + Calc_distance_between_locations(curr_long, curr_lat,
                                                                                                                     aRouteData->st_list.at(aStnIdx).gps_lon,
                                                                                                                     aRouteData->st_list.at(aStnIdx).gps_lat);
                                }
                        }

                        if(kmRun.count()>0)
                        {
                            if(kmRun.at(kmRun.count()-1)->date != QDate::currentDate())
                            {
                                kmT=kmD;
                                run *r = new run(kmRun.count(),QDate::currentDate(),systemconfig.coach_id,systemconfig.mpu_id,routesInfo.routeList.at(aRIdx).tr_no_en,kmT);
                                kmRun.append(r);
                            }
                            else
                            {
                                kmT+=kmD;
                                kmRun.at(kmRun.count()-1)->update_km(kmT);
                            }
                        }
                        else
                        {
                            kmT=kmD;
                            run *r = new run(kmRun.count(),QDate::currentDate(),systemconfig.coach_id,systemconfig.mpu_id,routesInfo.routeList.at(aRIdx).tr_no_en,kmT);
                            kmRun.append(r);
                        }

                        dump_runs();

                        w_log(",Kilometers Run: "+QString::number(kmR,'_f',1)+", Updated Kilometer age");
                        //                        ui->label_km_today->setText(QString::number(kmT,'_f',1)+"km");
                        //                        ui->label_km_age->setText(QString::number(kmR,'_f',1)+"km");

                        slave_task = SLAVE_SCAN_ALL;
                        connect(slaveSyncThread,SIGNAL(slave_scan_progress_eth(int,QList <slave*>)),this,SLOT(slave_scan_silent(int,QList <slave*>)));
                        emit eth_scan_all(papis_slaves);
                        w_log(",Auto Scan of Slaves Started");
                    }
                }

                //process
                if(route_type==UNKNOWN)
                {
                    prev_lat=curr_lat;
                    prev_long=curr_long;
                    prev_tim=cur_tim;

                    route_type = wakeupMidRoute();
                    if(route_type!=WAIT && route_type!=UNKNOWN)
                    {
                        assign_route();
                        kmRt=aRouteData->st_list.at(aStnIdx).dist_km;
                        //ui->label_curr_stn->setText(aRouteData->st_list.at(aStnIdx).st_name_en);

                        if(aStnIdx<aRouteData->st_list.count()-1)
                        {
                            //ui->label_next_stn->setText(aRouteData->st_list.at(aStnIdx+1).st_name_en);
                        }
                        //                        ui->label_sch_time->setText(QString::number(aRouteData->st_list.at(aStnIdx+1).time_arr_hh)+":"+QString::number(aRouteData->st_list.at(aStnIdx+1).time_arr_mm));
                    }
                }
                else
                {
                    //temporary, we need better method to find out if the train has started
                    if(c_speed>5)
                    {
                        train_started=true;
                    }

                    prev_lat=curr_lat;
                    prev_long=curr_long;
                    prev_tim=cur_tim;

                    double nDist=0, hr, min;
                    int eta_hr=0, eta_min=0;

                    if(aStnIdx==-1)
                    {
                        papis_sequence=PAPIS_GEN_WELCOME;
                        for(int astnidx=0;astnidx<aRouteData->st_list.count()-1;astnidx++)
                        {
                            nDist = Calc_distance_between_locations(aRouteData->st_list.at(astnidx).gps_lon, aRouteData->st_list.at(astnidx).gps_lat, curr_long, curr_lat);

                            if(nDist<=aRouteData->st_list.at(astnidx).arr_r || nDist<=aRouteData->st_list.at(astnidx).app_r)
                            {
                                aStnIdx=astnidx;
                                if(astnidx>0)
                                {
                                    papis_sequence=PAPIS_ARRIVAL;
                                }
                                kmRt = aRouteData->st_list.at(aStnIdx).dist_km;
                                emit slave_distance_send(kmRt);
                            }
                        }
                    }
                    else
                    {
                        if(papis_sequence==PAPIS_WAIT_ARRIVAL)
                        {
                            nDist = Calc_distance_between_locations(aRouteData->st_list.at(aStnIdx).gps_lon, aRouteData->st_list.at(aStnIdx).gps_lat, curr_long, curr_lat);
                            if(nDist>aRouteData->st_list.at(aStnIdx).dep_r)
                            {
                                papis_sequence=PAPIS_GEN_WELCOME;
                                mask_wc=false;
                                announcement_idx = ANN_HINDI;
                            }
                            else
                            {
                                //                                ui->label_eta->setText("");
                                //                                ui->label_next_dist->setText(QString::number(aRouteData->st_list.at(aStnIdx+1).dist_km-aRouteData->st_list.at(aStnIdx).dist_km,'_f',1)+" km");
                                //                                ui->label_sch_time->setText(QString::number(aRouteData->st_list.at(aStnIdx+1).time_arr_hh)+":"+QString::number(aRouteData->st_list.at(aStnIdx+1).time_arr_mm));
                            }
                        }
                        else
                        {
                            if(aStnIdx<aRouteData->st_list.count()-1)
                            {
                                nDist = Calc_distance_between_locations(aRouteData->st_list.at(aStnIdx+1).gps_lon, aRouteData->st_list.at(aStnIdx+1).gps_lat, curr_long, curr_lat);
                                hr= nDist/avg_speed;
                                min=hr*60;
                                eta_hr = (int)min/60;
                                eta_min = (int)min%60;

                                if(nDist>nDist_p)
                                {
                                    nDist_Ctr++;
                                    //distance to next station is increasing, we are on wrong route
                                    if(nDist_Ctr==10/*40*/)
                                    {
                                        nDist_Ctr=0;
                                        route_type=UNKNOWN;
                                        pList.clear();

                                        error_displayed=false;
                                        ddb_cycle=DDB_ERROR;
                                        ddbCycler->stop();

                                        error_mode();
                                        drive_ddb();
                                        drive_pis();
                                        emit slave_error_send();
                                        return;
                                    }
                                }
                                else
                                {
                                    nDist_Ctr=0;
                                }

                                nDist_p=nDist;

                                if(nDist<= aRouteData->st_list.at(aStnIdx+1).app_r && nDist>aRouteData->st_list.at(aStnIdx+1).arr_r)
                                {
                                    if(papis_sequence!=PAPIS_APPROACH)
                                    {
                                        announcement_idx=ANN_HINDI;
                                        papis_sequence=PAPIS_APPROACH;
                                        appCtr=0;
                                    }
                                    //                                    ui->label_next_dist->setText(QString::number(nDist,'_f',1)+" km");
                                    if(avg_speed>10)
                                    {
                                        //                                        ui->label_eta->setText(QString::number(eta_hr)+" hr, "+QString::number(eta_min)+" min");
                                    }
                                    else
                                    {
                                        //                                        ui->label_eta->setText("");
                                    }
                                }
                                else
                                    if(nDist<=aRouteData->st_list.at(aStnIdx+1).arr_r)
                                    {
                                        aStnIdx++;
                                        announcement_idx = ANN_HINDI;
                                        if(aStnIdx==aRouteData->st_list.count()-1)
                                        {
                                            papis_sequence=PAPIS_DESTINATION;

                                            //ui->label_curr_stn->setText(aRouteData->st_list.at(aStnIdx).st_name_en);
                                            //ui->label_next_stn->setText("");
                                            //                                        ui->label_next_dist->setText("");
                                            //                                        ui->label_eta->setText("");
                                        }
                                        else
                                        {
                                            papis_sequence=PAPIS_ARRIVAL;
                                            f_msg_idx=0;
                                            //ui->label_curr_stn->setText(aRouteData->st_list.at(aStnIdx).st_name_en);
                                            //ui->label_next_stn->setText(aRouteData->st_list.at(aStnIdx+1).st_name_en);
                                            //                                        ui->label_next_dist->setText(QString::number(aRouteData->st_list.at(aStnIdx+1).dist_km - aRouteData->st_list.at(aStnIdx).dist_km,'_f',1)+" km");
                                            //                                        ui->label_sch_time->setText(QString::number(aRouteData->st_list.at(aStnIdx+1).time_arr_hh)+":"+QString::number(aRouteData->st_list.at(aStnIdx+1).time_arr_mm));
                                            //                                        ui->label_eta->setText("");
                                        }
                                    }
                                    else
                                    {
                                        //                                    ui->label_next_dist->setText(QString::number(nDist,'_f',1)+" km");

                                        if(avg_speed>10)
                                        {
                                            //                                        ui->label_eta->setText(QString::number(eta_hr)+" hr, "+QString::number(eta_min)+" min");
                                        }
                                        else
                                        {
                                            //                                        ui->label_eta->setText("");
                                        }
                                    }
                            }
                        }
                    }


                    QString status=//ui->label_tr_name->text()+" "+//ui->label_tr_route->text()
                            +";"+QString::number(curr_lat,'_f',6)+";"+QString::number(curr_long,'_f',6);

                    sendMPUStatus(status);
                    //                   if(sendMPUStatus(status)==-1)
                    //                   {
                    //                        ui->label_bmu->setStyleSheet("color: rgb(255, 255, 255);image: url(:/images/icons/error.png);border-color: rgb(255, 255, 255);");
                    //                   }
                    //                   else
                    //                   {
                    //                        ui->label_bmu->setStyleSheet("color: rgb(255, 255, 255);image: url(:/images/icons/icon_bmu.png);border-color: rgb(255, 255, 255);");
                    //                   }

                    //                    papis_sequence=PAPIS_ARRIVAL;
                    switch(papis_sequence)
                    {
                    case PAPIS_GEN_WELCOME:
                    {
                        switch(announcement_idx)
                        {
                        case ANN_HINDI:
#ifndef LAPTOP
                            announcement_ongoing=true;
                            //                                gpio.gpio_set_value(PA_SWITCH,OP_ON);
#endif
                            welcome_mode_hin();
                            if(!emergency)
                            {
                                //ui->label_led_pis->setFont(*font_hi);
                                drive_pis();
                                emit slave_message_send();
                            }

                            if(!tms_alarm)
                                if(day_night_mode==MODE_DAY && !mask_wc && !emergency)
                                {
                                    emit audio_add(base_dir+"/sounds/chime.mp3");
                                    emit audio_add(base_dir+"/sounds/w_msg_hi.mp3");
                                    emit audio_play();
                                }

                            break;
                        case ANN_ENG:
                            //ui->label_led_pis->setFont(*font_en);
                            welcome_mode_eng();
                            if(!emergency)
                            {
                                //ui->label_led_pis->setFont(*font_en);
                                drive_pis();
                                emit slave_message_send();
                            }

                            if(!tms_alarm)
                                if(day_night_mode==MODE_DAY && !mask_wc && !emergency)
                                {
                                    emit audio_add(base_dir+"/sounds/chime.mp3");
                                    emit audio_add(base_dir+"/sounds/w_msg_en.mp3");
                                    emit audio_play();
                                }
                            break;
                        case ANN_RL:
                            welcome_mode_rl(aStnIdx);
                            if(!emergency)
                            {
                                lang = aRouteData->st_list.at(aStnIdx).rl;
                                //ui->label_led_pis->setFont(*getRLFont(aRouteData->st_list.at(aStnIdx).rl));
                                drive_pis();
                                emit slave_message_send();
                            }
#ifndef LAPTOP
                            announcement_ongoing=false;
                            //                                gpio.gpio_set_value(PA_SWITCH,OP_OFF);
#endif
                            break;
                        case ANN_DONE:

                            break;
                        }

                        ai=(int)announcement_idx;
                        ai++;
                        announcement_idx=(announcement_idx_t)ai;
                        if(announcement_idx==ANN_DONE)
                        {
                            announcement_idx = ANN_HINDI;
                            mask_wc=true;
                            if(train_started)
                            {
                                if(aStnIdx!=-1)
                                {
                                    papis_sequence=PAPIS_DEPARTURE;
                                    //calculate late stat here so that it remains same throughout
                                    int aDiff = aRouteData->st_list.at(aStnIdx+1).time_arr.msecsSinceStartOfDay()-QTime::currentTime().msecsSinceStartOfDay();
                                    lateBy = QTime::fromMSecsSinceStartOfDay(-1*aDiff);
                                }
                            }
                            w_msg_idx=0;
                            slogan_idx=0;
                        }
                    }
                        break;
                    case PAPIS_DEPARTURE:
                        if(train_started)
                        {
                            QString str="";
                            switch(announcement_idx)
                            {
                            case ANN_HINDI:
                                //ui->label_led_pis->setFont(*font_hi);
                                switch(w_msg_idx)
                                {
                                case 0:
#ifndef LAPTOP
                                    announcement_ongoing=true;
                                    //                                        gpio.gpio_set_value(PA_SWITCH,OP_ON);
#endif
                                    str = aRouteData->st_list.at(aStnIdx).w_msg1_hi;
                                    if(!tms_alarm)
                                        if((day_night_mode==MODE_DAY)&&(!emergency))/*if((day_night_mode==MODE_DAY)||(!emergency))*/
                                        {
                                            emit audio_add(base_dir+"/sounds/chime.mp3");
                                            emit audio_add(base_dir+"/sounds/ss_train_num_hi.mp3");
                                            emit audio_add(base_dir+"routes/"+routesInfo.routeList.at(aRIdx).tr_no_en+"/sounds/ss_train_no_hi.mp3");
                                            //                                            emit audio_add(base_dir+"routes/"+routesInfo.routeList.at(aRIdx).tr_no_en+"/sounds/ss_train_nm_hi.mp3");
                                            emit audio_add(base_dir+"/sounds/ss_welcome_hi.mp3");
                                            emit audio_play();
                                        }
                                    break;
                                case 1:
                                    str = aRouteData->st_list.at(aStnIdx).w_msg2_hi;
                                    if(!tms_alarm)
                                        if((day_night_mode==MODE_DAY)&&(!emergency))/*if((day_night_mode==MODE_DAY)||(!emergency))*/
                                        {
                                            emit audio_add(base_dir+"/sounds/chime.mp3");
                                            emit audio_add(base_dir+"/sounds/ss_ye_gaadi_hi.mp3");
                                            emit audio_add(base_dir+"routes/"+
                                                           routesInfo.routeList.at(aRIdx).tr_no_en+"/sounds/"+QString::number(aStnIdx+1)+
                                                           +"/st_name.mp3");
                                            emit audio_add(base_dir+"/sounds/ss_se_hi.mp3");
                                            emit audio_add(base_dir+"routes/"+
                                                           routesInfo.routeList.at(aRIdx).tr_no_en+"/sounds/"+QString::number(aRouteData->st_list.count())+
                                                           +"/st_name.mp3");
                                            emit audio_add(base_dir+"/sounds/ss_tak_hi.mp3");
                                            emit audio_play();
                                        }
                                    break;
                                case 2:
                                    str = "      "+aRouteData->ss_next_stn_hi+": "+ aRouteData->st_list.at(aStnIdx+1).st_name_hi;
                                    if(!tms_alarm)
                                        if((day_night_mode==MODE_DAY)&&(!emergency))/*if((day_night_mode==MODE_DAY)||(!emergency))*/
                                        {
                                            emit audio_add(base_dir+"/sounds/chime.mp3");
                                            emit audio_add(base_dir+"/sounds/ss_next_stn_hi.mp3");
                                            emit audio_add(base_dir+"routes/"+
                                                           routesInfo.routeList.at(aRIdx).tr_no_en+"/sounds/"+QString::number(aStnIdx+2)+
                                                           +"/st_name.mp3");
                                            emit audio_add(base_dir+"sounds/sup_hi_hai.mp3");
                                            emit audio_play();
                                        }
                                    break;
                                case 3:
                                    if(lateBy.hour()>0)
                                    {
                                        str = "     ट्रेन "+QString::number(lateBy.hour())+" hr, "+QString::number(lateBy.minute())+" min की देरी से चल रही है";
                                        //                                            ui->label_late_stat->setText(QString::number(lateBy.hour())+" hr, "+QString::number(lateBy.minute())+" min");
                                        if(!tms_alarm)
                                            if((day_night_mode==MODE_DAY)&&(!emergency))/*if((day_night_mode==MODE_DAY)||(!emergency))*/
                                            {
                                                emit audio_add(base_dir+"/sounds/chime.mp3");
                                                emit audio_add(base_dir+"/sounds/ss_late1_hi.mp3");
                                                emit audio_add(base_dir+"/sounds/nums/hi/"+
                                                               QString::number(lateBy.hour())+".mp3");

                                                if(lateBy.hour()>1)
                                                {
                                                    emit audio_add(base_dir+"/sounds/ss_hours_hi.mp3");
                                                }
                                                else
                                                {
                                                    emit audio_add(base_dir+"/sounds/ss_hour_hi.mp3");
                                                }

                                                emit audio_add(base_dir+"/sounds/nums/hi/"+
                                                               QString::number(lateBy.minute())+".mp3");
                                                emit audio_add(base_dir+"/sounds/ss_minute_hi.mp3");
                                                emit audio_add(base_dir+"/sounds/ss_late2_hi.mp3");
                                                emit audio_add(base_dir+"/sounds/ss_late3_hi.mp3");
                                                emit audio_play();
                                            }
                                    }
                                    else
                                        if(lateBy.minute()>0)
                                        {
                                            str = "     ट्रेन "+QString::number(lateBy.minute())+" min की देरी से चल रही है";
                                            if(!tms_alarm)
                                                if((day_night_mode==MODE_DAY)&&(!emergency))/*if((day_night_mode==MODE_DAY)||(!emergency))*/
                                                {
                                                    emit audio_add(base_dir+"/sounds/chime.mp3");
                                                    emit audio_add(base_dir+"/sounds/ss_late1_hi.mp3");
                                                    emit audio_add(base_dir+"/sounds/nums/hi/"+
                                                                   QString::number(lateBy.minute())+".mp3");
                                                    emit audio_add(base_dir+"/sounds/ss_minute_hi.mp3");
                                                    emit audio_add(base_dir+"/sounds/ss_late2_hi.mp3");
                                                    emit audio_add(base_dir+"/sounds/ss_late3_hi.mp3");
                                                    emit audio_play();
                                                }
                                        }
                                    break;
                                case 4:

                                    str = aRouteData->st_list.at(aStnIdx).w_msg3_hi;
                                    if(!tms_alarm)
                                        if((day_night_mode==MODE_DAY)&&(!emergency))/*if((day_night_mode==MODE_DAY)||(!emergency))*/
                                        {
                                            emit audio_add(base_dir+"/sounds/chime.mp3");
                                            emit audio_add(base_dir+"/sounds/w_msg3_hi.mp3");
                                            emit audio_play();
                                        }
                                    break;
                                }

                                break;
                            case ANN_ENG:

                                //ui->label_led_pis->setFont(*font_en);
                                switch(w_msg_idx)
                                {
                                case 0:
                                    str = aRouteData->st_list.at(aStnIdx).w_msg1_en;
                                    if(!tms_alarm)
                                        if((day_night_mode==MODE_DAY)&&(!emergency))/*if((day_night_mode==MODE_DAY)||(!emergency))*/
                                        {
                                            emit audio_add(base_dir+"/sounds/chime.mp3");
                                            emit audio_add(base_dir+"/sounds/w_train_no_en.mp3");
                                            emit audio_add(base_dir+"routes/"+routesInfo.routeList.at(aRIdx).tr_no_en+"/sounds/ss_train_no_en.mp3");
                                            //                                            emit audio_add(base_dir+"routes/"+routesInfo.routeList.at(aRIdx).tr_no_en+"/sounds/ss_train_nm_en.mp3");
                                            emit audio_play();
                                        }
                                    break;
                                case 1:
                                    str = aRouteData->st_list.at(aStnIdx).w_msg2_en;
                                    if(!tms_alarm)
                                        if((day_night_mode==MODE_DAY)&&(!emergency))/*if((day_night_mode==MODE_DAY)||(!emergency))*/
                                        {
                                            emit audio_add(base_dir+"/sounds/chime.mp3");
                                            emit audio_add(base_dir+"/sounds/ss_this_en.mp3");
                                            emit audio_add(base_dir+"routes/"+
                                                           routesInfo.routeList.at(aRIdx).tr_no_en+"/sounds/"+QString::number(aStnIdx+1)+
                                                           +"/st_name.mp3");
                                            emit audio_add(base_dir+"/sounds/ss_to_en.mp3");
                                            emit audio_add(base_dir+"routes/"+
                                                           routesInfo.routeList.at(aRIdx).tr_no_en+"/sounds/"+QString::number(aRouteData->st_list.count())+
                                                           +"/st_name.mp3");
                                            emit audio_play();
                                        }
                                    break;
                                case 2:
                                    str = "      "+aRouteData->ss_next_stn_en+": "+ aRouteData->st_list.at(aStnIdx+1).st_name_en;
                                    if(!tms_alarm)
                                        if((day_night_mode==MODE_DAY)&&(!emergency))/*if((day_night_mode==MODE_DAY)||(!emergency))*/
                                        {
                                            emit audio_add(base_dir+"/sounds/chime.mp3");
                                            emit audio_add(base_dir+"/sounds/ss_next_stn_en.mp3");
                                            emit audio_add(base_dir+"routes/"+
                                                           routesInfo.routeList.at(aRIdx).tr_no_en+"/sounds/"+QString::number(aStnIdx+2)+
                                                           +"/st_name.mp3");
                                            emit audio_play();
                                        }
                                    break;
                                case 3:
                                    if(lateBy.hour()>0)
                                    {
                                        str = "     Train is running late by "+QString::number(lateBy.hour())+" hr, "+QString::number(lateBy.minute())+" min";
                                        //                                            ui->label_late_stat->setText(QString::number(lateBy.hour())+" hr, "+QString::number(lateBy.minute())+" min");
                                        if(!tms_alarm)
                                            if((day_night_mode==MODE_DAY)&&(!emergency))/*if((day_night_mode==MODE_DAY)||(!emergency))*/
                                            {
                                                emit audio_add(base_dir+"/sounds/chime.mp3");
                                                emit audio_add(base_dir+"/sounds/ss_late1_en.mp3");
                                                emit audio_add(base_dir+"/sounds/nums/en/"+
                                                               QString::number(lateBy.hour())+".mp3");

                                                if(lateBy.hour()>1)
                                                {
                                                    emit audio_add(base_dir+"sounds/ss_hours_en.mp3");
                                                }
                                                else
                                                {
                                                    emit audio_add(base_dir+"/sounds/ss_hour_en.mp3");
                                                }

                                                emit audio_add(base_dir+"/sounds/nums/en/"+
                                                               QString::number(lateBy.minute())+".mp3");

                                                if(lateBy.minute()>1)
                                                {
                                                    emit audio_add(base_dir+"/sounds/ss_minutes_en.mp3");
                                                }
                                                else
                                                {
                                                    emit audio_add(base_dir+"/sounds/ss_minute_en.mp3");
                                                }

                                                emit audio_add(base_dir+"/sounds/ss_late3_en.mp3");
                                                emit audio_play();
                                            }
                                    }
                                    else
                                    {
                                        if(lateBy.minute()>0)
                                        {
                                            str = "     Train is running late by "+QString::number(lateBy.minute())+" min";
                                            if(!tms_alarm)
                                                if((day_night_mode==MODE_DAY)&&(!emergency))/*if((day_night_mode==MODE_DAY)||(!emergency))*/
                                                {
                                                    emit audio_add(base_dir+"/sounds/chime.mp3");
                                                    emit audio_add(base_dir+"/sounds/ss_late1_en.mp3");
                                                    emit audio_add(base_dir+"/sounds/nums/en/"+
                                                                   QString::number(lateBy.minute())+".mp3");
                                                    emit audio_add(base_dir+"/sounds/ss_minute_en.mp3");
                                                    emit audio_add(base_dir+"/sounds/ss_late3_en.mp3");
                                                    emit audio_play();
                                                }
                                        }
                                    }
                                    break;
                                case 4:
                                    str = aRouteData->st_list.at(aStnIdx).w_msg3_en;
                                    if(!tms_alarm)
                                        if((day_night_mode==MODE_DAY)&&(!emergency))/*if((day_night_mode==MODE_DAY)||(!emergency))*/
                                        {
                                            emit audio_add(base_dir+"/sounds/chime.mp3");
                                            emit audio_add(base_dir+"/sounds/"+QString::number(aStnIdx+1)+
                                                           +"/w_msg3_en.mp3");
                                            emit audio_play();
                                        }
                                    break;
                                }

                                break;
                            case ANN_RL:
                                lang = aRouteData->st_list.at(aStnIdx).rl;
                                //ui->label_led_pis->setFont(*getRLFont(aRouteData->st_list.at(aStnIdx).rl));
                                //ui->label_led_pis->setFont(*getRLFont(aRouteData->st_list.at(aStnIdx).rl));
                                if((aRouteData->st_list.at(aStnIdx).w_msg1_rl=="")&&(aRouteData->st_list.at(aStnIdx).w_msg2_rl=="")
                                        &&(aRouteData->st_list.at(aStnIdx).w_msg3_rl==""))
                                {
                                    w_msg_idx=3;
                                    skipSync=true;
                                }
                                else
                                {
                                    switch(w_msg_idx)
                                    {
                                    case 0:
                                        str = aRouteData->st_list.at(aStnIdx).w_msg1_rl;
                                        break;
                                    case 1:
                                        str = aRouteData->st_list.at(aStnIdx).w_msg2_rl;
                                        break;
                                    case 2:
                                        str = aRouteData->st_list.at(aStnIdx).w_msg3_rl;
                                        w_msg_idx++; // till we dont have late running message for RL
                                        break;
                                    }
                                }
#ifndef LAPTOP
                                announcement_ongoing=false;
                                //                                    gpio.gpio_set_value(PA_SWITCH,OP_OFF);
#endif
                                break;
                            }

                            if(!skipSync)
                            {
                                //ui->label_led_pis->setText(str);
                                // ui->label_lcd_pis->setText(str);
                                update();
                                if(!emergency)
                                {
                                    drive_pis();
                                    emit slave_message_send();
                                }
                                skipSync=false;
                            }

                            w_msg_idx++;

                            if(w_msg_idx==4)
                            {
                                w_msg_idx=0;
                                ai=(int)announcement_idx;
                                ai++;
                                announcement_idx=(announcement_idx_t)ai;
                                if(announcement_idx==ANN_DONE)
                                {
                                    announcement_idx = ANN_HINDI;
                                    papis_sequence=PAPIS_RUNNING;
                                    train_started=false;
                                }
                            }
                        }
                        break;
                    case PAPIS_RUNNING:
                    {
                        QString msg_led="";
                        QString msg_lcd="";
                        switch (pis_cycle)
                        {
                        case PIS_WELCOME:
                        {
                            switch(announcement_idx)
                            {
                            case ANN_HINDI:
                                //ui->label_led_pis->setFont(*font_hi);
                                msg_lcd = "     "+aRouteData->w_msg_hi;
                                msg_led = msg_lcd;
                                break;
                            case ANN_ENG:
                                //ui->label_led_pis->setFont(*font_en);
                                msg_lcd = "     "+aRouteData->w_msg_en;
                                msg_led = msg_lcd;
                                break;
                            case ANN_RL:
                                lang = aRouteData->st_list.at(aStnIdx).rl;
                                //ui->label_led_pis->setFont(*getRLFont(aRouteData->st_list.at(aStnIdx).rl));
                                if(aRouteData->st_list.at(aStnIdx).w_msg_rl=="")
                                {
                                    skipSync=true;
                                }
                                else
                                {
                                    msg_lcd = "     "+aRouteData->st_list.at(aStnIdx).w_msg_rl;
                                    msg_led = msg_lcd;
                                }
                                break;
                            case ANN_DONE:

                                break;
                            }
                            break;
                        }
                            break;
                        case PIS_ROUTE:
                        {
                            switch(announcement_idx)
                            {
                            case ANN_HINDI:
                                //                                    ui->stackedWidget->setCurrentIndex(1);
                                //ui->label_led_pis->setFont(*font_hi);
                                msg_lcd = "     ";
                                msg_led="      "+aRouteData->st_list.at(aStnIdx).tr_no_hi+" "+aRouteData->st_list.at(aStnIdx).tr_nm_hi+"      "+aRouteData->st_list.at(aStnIdx).tr_route_hi;
                                break;
                            case ANN_ENG:
                                //ui->label_led_pis->setFont(*font_en);
                                msg_lcd = "     ";
                                msg_led="      "+aRouteData->st_list.at(aStnIdx).tr_no_en+" "+aRouteData->st_list.at(aStnIdx).tr_nm_en+"      "+aRouteData->st_list.at(aStnIdx).tr_route_en;
                                break;
                            case ANN_RL:
                                lang = aRouteData->st_list.at(aStnIdx).rl;
                                //ui->label_led_pis->setFont(*getRLFont(aRouteData->st_list.at(aStnIdx).rl));
                                if(aRouteData->st_list.at(aStnIdx).tr_no_rl=="" && aRouteData->st_list.at(aStnIdx).tr_nm_rl==""
                                        && aRouteData->st_list.at(aStnIdx).tr_route_rl=="")
                                {
                                    skipSync=true;
                                }
                                else
                                {
                                    msg_lcd = "     ";
                                    msg_led="      "+aRouteData->st_list.at(aStnIdx).tr_no_rl+" "+aRouteData->st_list.at(aStnIdx).tr_nm_rl+"      "+aRouteData->st_list.at(aStnIdx).tr_route_rl;
                                }

                                //                                    ui->stackedWidget->setCurrentIndex(0);
                                break;
                            case ANN_DONE:

                                break;
                            }
                            break;
                        }
                            break;
                        case PIS_SPEED:
                        {
                            switch(announcement_idx)
                            {
                            case ANN_HINDI:
                                //ui->label_led_pis->setFont(*font_hi);
                                msg_lcd = "      "+aRouteData->ss_speed_hi+": "+QString::number(avg_speed,'_f',1)+" "+aRouteData->ss_km_hi+"/"+aRouteData->ss_hr_hi;
                                msg_led = msg_lcd;
                                break;
                            case ANN_ENG:
                                //ui->label_led_pis->setFont(*font_en);
                                msg_lcd = "      "+aRouteData->ss_speed_en+": "+QString::number(avg_speed,'_f',1)+" "+aRouteData->ss_km_en+"/"+aRouteData->ss_hr_en;
                                msg_led = msg_lcd;
                                break;
                            case ANN_RL:
                                lang = aRouteData->st_list.at(aStnIdx).rl;
                                //ui->label_led_pis->setFont(*getRLFont(aRouteData->st_list.at(aStnIdx).rl));
                                if(aRouteData->st_list.at(aStnIdx).ss_speed_rl=="")
                                {
                                    skipSync=true;
                                }
                                else
                                {
                                    msg_lcd = "      "+aRouteData->st_list.at(aStnIdx).ss_speed_rl+": "+QString::number(avg_speed,'_f',1)+" "+aRouteData->ss_km_en+"/"+aRouteData->ss_hr_en;
                                    msg_led = msg_lcd;
                                }

                                break;
                            case ANN_DONE:

                                break;
                            }
                            break;
                        }
                            break;
                        case PIS_NEXT:
                        {
                            switch(announcement_idx)
                            {
                            case ANN_HINDI:
                                //ui->label_led_pis->setFont(*font_hi);
                                msg_lcd = "      "+aRouteData->ss_next_stn_hi+": "+ aRouteData->st_list.at(aStnIdx+1).st_name_hi;
                                msg_led = msg_lcd;
                                break;
                            case ANN_ENG:
                                //ui->label_led_pis->setFont(*font_en);
                                msg_lcd = "      "+aRouteData->ss_next_stn_en+": "+ aRouteData->st_list.at(aStnIdx+1).st_name_en;
                                msg_led = msg_lcd;
                                break;
                            case ANN_RL:
                                lang = aRouteData->st_list.at(aStnIdx).rl;
                                //ui->label_led_pis->setFont(*getRLFont(aRouteData->st_list.at(aStnIdx).rl));
                                if(aRouteData->st_list.at(aStnIdx).ss_next_stn_rl=="")
                                {
                                    skipSync=true;
                                }
                                else
                                {
                                    msg_lcd = "      "+aRouteData->st_list.at(aStnIdx).ss_next_stn_rl+": "+ aRouteData->st_list.at(aStnIdx+1).st_name_rl;
                                    msg_led = msg_lcd;
                                }

                                break;
                            case ANN_DONE:

                                break;
                            }
                            break;
                        }
                            break;
                        case PIS_NEXT_DIST:
                        {
                            switch(announcement_idx)
                            {
                            case ANN_HINDI:
                                //ui->label_led_pis->setFont(*font_hi);
                                msg_lcd = "      "+aRouteData->ss_next_dist_hi+": "+ QString::number(Calc_distance_between_locations(aRouteData->st_list.at(aStnIdx+1).gps_lon, aRouteData->st_list.at(aStnIdx+1).gps_lat, curr_long, curr_lat),'_f',1) +" "+aRouteData->ss_km_hi;
                                msg_led = msg_lcd;
                                break;
                            case ANN_ENG:
                                //ui->label_led_pis->setFont(*font_en);
                                msg_lcd = "      "+aRouteData->ss_next_dist_en+": "+ QString::number(Calc_distance_between_locations(aRouteData->st_list.at(aStnIdx+1).gps_lon, aRouteData->st_list.at(aStnIdx+1).gps_lat, curr_long, curr_lat),'_f',1) +" "+aRouteData->ss_km_en;
                                msg_led = msg_lcd;
                                break;
                            case ANN_RL:
                                lang = aRouteData->st_list.at(aStnIdx).rl;
                                //ui->label_led_pis->setFont(*getRLFont(aRouteData->st_list.at(aStnIdx).rl));
                                if(aRouteData->st_list.at(aStnIdx).ss_next_dist_rl=="")
                                {
                                    skipSync=true;
                                }
                                else
                                {
                                    msg_lcd = "      "+aRouteData->st_list.at(aStnIdx).ss_next_dist_rl+": "+ QString::number(Calc_distance_between_locations(aRouteData->st_list.at(aStnIdx+1).gps_lon, aRouteData->st_list.at(aStnIdx+1).gps_lat, curr_long, curr_lat),'_f',1) +" "+aRouteData->ss_km_en;
                                    msg_led = msg_lcd;
                                }
                                break;
                            case ANN_DONE:

                                break;
                            }
                            break;
                        }
                            break;
                        case PIS_ETA:
                        {
                            switch(announcement_idx)
                            {
                            case ANN_HINDI:
                                //ui->label_led_pis->setFont(*font_hi);
                                msg_lcd = "      "+aRouteData->ss_eta_hi+": "+ QString::number(eta_hr) +" "+aRouteData->ss_hr_hi+", "+ QString::number(eta_min) +" "+aRouteData->ss_min_hi;
                                msg_led = msg_lcd;
                                break;
                            case ANN_ENG:
                                //ui->label_led_pis->setFont(*font_en);
                                msg_lcd = "      "+aRouteData->ss_eta_en+": "+ QString::number(eta_hr) +" "+aRouteData->ss_hr_en+", "+ QString::number(eta_min) +" "+aRouteData->ss_min_en;
                                msg_led = msg_lcd;
                                break;
                            case ANN_RL:
                                lang = aRouteData->st_list.at(aStnIdx).rl;
                                //ui->label_led_pis->setFont(*getRLFont(aRouteData->st_list.at(aStnIdx).rl));
                                if(aRouteData->st_list.at(aStnIdx).ss_eta_rl=="")
                                {
                                    skipSync=true;
                                }
                                else
                                {
                                    msg_lcd = "      "+aRouteData->st_list.at(aStnIdx).ss_eta_rl+": "+ QString::number(eta_hr) +" "+aRouteData->ss_hr_en+", "+ QString::number(eta_min) +" "+aRouteData->ss_min_en;
                                    msg_led = msg_lcd;
                                }
                                break;
                            case ANN_DONE:

                                break;
                            }
                            break;
                        }
                            break;
                        case PIS_LATE:
                        {
                            int aDiff = aRouteData->st_list.at(aStnIdx+1).time_arr.msecsSinceStartOfDay()-QTime::currentTime().msecsSinceStartOfDay();

                            if(aDiff<=-1800000)
                            {
                                lateBy = QTime::fromMSecsSinceStartOfDay(-1*aDiff);
                                msg_lcd = "";
                                switch(announcement_idx)
                                {
                                case ANN_HINDI:
                                    //ui->label_led_pis->setFont(*font_hi);
                                    if(lateBy.hour()>0)
                                    {
                                        msg_lcd = "     ट्रेन "+QString::number(lateBy.hour())+" hr, "+QString::number(lateBy.minute())+" min की देरी से चल रही है";
                                    }
                                    else
                                        if(lateBy.minute()>0)
                                        {
                                            msg_lcd = "     ट्रेन "+QString::number(lateBy.minute())+" min की देरी से चल रही है";
                                        }
                                    msg_led = msg_lcd;
                                    break;
                                case ANN_ENG:
                                    //ui->label_led_pis->setFont(*font_en);
                                    if(lateBy.hour()>0)
                                    {
                                        msg_lcd = "     Train is running late by "+QString::number(lateBy.hour())+" hr, "+QString::number(lateBy.minute())+" min";
                                    }
                                    else
                                        if(lateBy.minute()>0)
                                        {
                                            msg_lcd = "     Train is running late by "+QString::number(lateBy.minute())+" min";
                                        }
                                    msg_led = msg_lcd;
                                    break;
                                case ANN_RL:
                                    lang = aRouteData->st_list.at(aStnIdx).rl;
                                    //ui->label_led_pis->setFont(*getRLFont(aRouteData->st_list.at(aStnIdx).rl));
                                    if(lateBy.hour()>0)
                                    {
                                        msg_lcd = "     Train is running late by "+QString::number(lateBy.hour())+" hr, "+QString::number(lateBy.minute())+" min";
                                    }
                                    else
                                        if(lateBy.minute()>0)
                                        {
                                            msg_lcd = "     Train is running late by "+QString::number(lateBy.minute())+" min";
                                        }
                                    msg_led = msg_lcd;
                                    break;
                                case ANN_DONE:

                                    break;
                                }
                            }
                            else
                            {
                                break;
                            }
                            break;
                        }
                            break;
                        case PIS_SLOGAN:
                        {
                            switch(announcement_idx)
                            {
                            case ANN_HINDI:
                                //ui->label_led_pis->setFont(*font_hi);
                                msg_led = "     "+aRouteData->slogans.at(slogan_idx);
                                msg_lcd = msg_led;
                                break;
                            case ANN_ENG:
                                //ui->label_led_pis->setFont(*font_en);
                                msg_led = "     "+aRouteData->slogans.at(slogan_idx);
                                msg_lcd = msg_led;
                                slogan_idx++;
                                if(slogan_idx==aRouteData->slogans.count()-2)
                                    slogan_idx=0;
                                break;
                            case ANN_RL:
                                lang = aRouteData->st_list.at(aStnIdx).rl;
                                //ui->label_led_pis->setFont(*getRLFont(aRouteData->st_list.at(aStnIdx).rl));

                                break;
                            case ANN_DONE:

                                break;
                            }
                            break;
                        }
                            break;
                        case PIS_DONE:
                        {

                        }
                            break;
                        }
                        ai=(int)announcement_idx;
                        ai++;
                        announcement_idx=(announcement_idx_t)ai;
                        if(announcement_idx==ANN_DONE)
                        {
                            announcement_idx = ANN_HINDI;

                            pi=(int)pis_cycle;
                            pi++;
                            pis_cycle=(pis_cycle_t)pi;
                            if(pis_cycle == PIS_DONE)
                            {
                                pis_cycle = PIS_WELCOME;
                            }
                        }

                        if(!skipSync)
                        {
                            //ui->label_led_pis->setText(msg_led);
                            // ui->label_lcd_pis->setText(msg_lcd);
                            update();
                            if(!emergency)
                            {
                                drive_pis();
                                emit slave_message_send();
                            }
                            skipSync=false;
                        }
                    }
                        break;
                    case PAPIS_APPROACH:
                    {
                        QString msg_led="";
                        QString msg_lcd="";
                        appCtr++;
                        switch(announcement_idx)
                        {
                        case ANN_HINDI:
                            //ui->label_led_pis->setFont(*font_hi);
#ifndef LAPTOP
                            announcement_ongoing=true;
                            //                                gpio.gpio_set_value(PA_SWITCH,OP_ON);
#endif
                            msg_lcd = "      "+aRouteData->ss_next_stn_hi+": "+ aRouteData->st_list.at(aStnIdx+1).st_name_hi;
                            msg_led = msg_lcd;
                            if(appCtr<=2)
                            {
                                if(!tms_alarm)
                                    if((day_night_mode==MODE_DAY)&&(!emergency))/*if((day_night_mode==MODE_DAY)||(!emergency))*/
                                    {
                                        emit audio_add(base_dir+"sounds/chime.mp3");
                                        emit audio_add(base_dir+"/sounds/ss_att_hi.mp3");
                                        emit audio_add(base_dir+"/sounds/ss_appr1_hi.mp3");
                                        emit audio_add(base_dir+"routes/"+
                                                       routesInfo.routeList.at(aRIdx).tr_no_en+"/sounds/"+QString::number(aStnIdx+2)+
                                                       +"/st_name.mp3");
                                        emit audio_add(base_dir+"/sounds/ss_appr2_hi.mp3");
                                        emit audio_add(base_dir+"/sounds/ss_appr3_hi.mp3");
                                        emit audio_play();
                                    }
                            }
                            break;
                        case ANN_ENG:
                            //ui->label_led_pis->setFont(*font_en);
                            msg_lcd = "      "+aRouteData->ss_next_stn_en+": "+ aRouteData->st_list.at(aStnIdx+1).st_name_en;
                            msg_led = msg_lcd;
                            if(appCtr<=2)
                            {
                                if(!tms_alarm)
                                    if((day_night_mode==MODE_DAY)&&(!emergency))/*if((day_night_mode==MODE_DAY)||(!emergency))*/
                                    {
                                        emit audio_add(base_dir+"/sounds/chime.mp3");
                                        emit audio_add(base_dir+"/sounds/ss_att_en.mp3");
                                        emit audio_add(base_dir+"/sounds/ss_appr1_en.mp3");
                                        emit audio_add(base_dir+"routes/"+
                                                       routesInfo.routeList.at(aRIdx).tr_no_en+"/sounds/"+QString::number(aStnIdx+2)+
                                                       +"/st_name.mp3");
                                        emit audio_add(base_dir+"/sounds/ss_appr2_en.mp3");
                                        emit audio_add(base_dir+"/sounds/ss_appr3_en.mp3");
                                        emit audio_play();
                                    }
                            }
                            break;
                        case ANN_RL:
                            lang = aRouteData->st_list.at(aStnIdx).rl;
                            //ui->label_led_pis->setFont(*getRLFont(aRouteData->st_list.at(aStnIdx+1).rl));
                            if(aRouteData->st_list.at(aStnIdx+1).ss_next_stn_rl=="" && aRouteData->st_list.at(aStnIdx+1).st_name_rl=="")
                            {
                                skipSync=false;
                            }
                            else
                            {
                                msg_lcd = "      "+aRouteData->st_list.at(aStnIdx+1).ss_next_stn_rl+": "+ aRouteData->st_list.at(aStnIdx+1).st_name_rl;
                                msg_led = msg_lcd;
                            }
#ifndef LAPTOP
                            announcement_ongoing=false;
                            //                                gpio.gpio_set_value(PA_SWITCH,OP_OFF);
#endif
                            break;
                        case ANN_DONE:


                            break;
                        }

                        if(!skipSync)
                        {
                            //ui->label_led_pis->setText(msg_led);
                            // ui->label_lcd_pis->setText(msg_lcd);
                            update();
                            if(!emergency)
                            {
                                drive_pis();
                                emit slave_message_send();
                            }
                            skipSync=false;
                        }

                        ai=(int)announcement_idx;
                        ai++;
                        announcement_idx=(announcement_idx_t)ai;
                        if(announcement_idx==ANN_DONE)
                        {
                            announcement_idx = ANN_HINDI;
                            slogan_idx=0;
                        }
                    }
                        break;
                    case PAPIS_ARRIVAL:
                    {
                        QString str="";
                        switch(announcement_idx)
                        {
                        case ANN_HINDI:
                            //ui->label_led_pis->setFont(*font_hi);
#ifndef LAPTOP
                            announcement_ongoing=true;
                            //                                gpio.gpio_set_value(PA_SWITCH,OP_ON);
#endif
                            switch(f_msg_idx)
                            {
                            case 0:
                                str = aRouteData->st_list.at(aStnIdx).st_name_hi;
                                if(!tms_alarm)
                                    if((day_night_mode==MODE_DAY)&&(!emergency))/*if((day_night_mode==MODE_DAY)||(!emergency))*/
                                    {
                                        emit audio_add(base_dir+"/sounds/chime.mp3");
                                        emit audio_add(base_dir+"routes/"+
                                                       routesInfo.routeList.at(aRIdx).tr_no_en+"/sounds/"+QString::number(aStnIdx+1)+
                                                       +"/st_name.mp3");
                                        emit audio_add(base_dir+"/sounds/ss_arrv1_hi.mp3");
                                        emit audio_play();
                                    }
                                break;
                            case 1:
                                str = aRouteData->st_list.at(aStnIdx).f_msg2_hi;
                                if(!tms_alarm)
                                    if((day_night_mode==MODE_DAY)&&(!emergency))/*if((day_night_mode==MODE_DAY)||(!emergency))*/
                                    {
                                        emit audio_add(base_dir+"/sounds/f_msg2_hi.mp3");
                                        emit audio_play();
                                    }
                                break;
                            case 2:
                                str = aRouteData->st_list.at(aStnIdx).f_msg3_hi;
                                if(!tms_alarm)
                                    if((day_night_mode==MODE_DAY)&&(!emergency))/*if((day_night_mode==MODE_DAY)||(!emergency))*/
                                    {
                                        emit audio_add(base_dir+"sounds/f_msg3_hi.mp3");
                                        emit audio_play();
                                    }
                                break;
                            case 3:
                                str = aRouteData->st_list.at(aStnIdx).f_msg4_hi;
                                if(!tms_alarm)
                                    if((day_night_mode==MODE_DAY)&&(!emergency))/*if((day_night_mode==MODE_DAY)||(!emergency))*/
                                    {
                                        emit audio_add(base_dir+"/sounds/f_msg4_hi.mp3");
                                        emit audio_play();
                                    }
                                break;
                            }

                            break;
                        case ANN_ENG:
                            //ui->label_led_pis->setFont(*font_en);
                            switch(f_msg_idx)
                            {
                            case 0:
                                str = aRouteData->st_list.at(aStnIdx).st_name_en;
                                if(!tms_alarm)
                                    if((day_night_mode==MODE_DAY)&&(!emergency))/*if((day_night_mode==MODE_DAY)||(!emergency))*/
                                    {
                                        emit audio_add(base_dir+"/sounds/chime.mp3");
                                        emit audio_add(base_dir+"/sounds/ss_arrv1_en.mp3");
                                        emit audio_add(base_dir+"routes/"+
                                                       routesInfo.routeList.at(aRIdx).tr_no_en+"/sounds/"+QString::number(aStnIdx+1)+
                                                       +"/st_name.mp3");
                                        emit audio_play();
                                    }
                                break;
                            case 1:
                                str = aRouteData->st_list.at(aStnIdx).f_msg2_en;
                                if(!tms_alarm)
                                    if((day_night_mode==MODE_DAY)&&(!emergency))/*if((day_night_mode==MODE_DAY)||(!emergency))*/
                                    {
                                        emit audio_add(base_dir+"/sounds/f_msg2_en.mp3");
                                        emit audio_play();
                                    }
                                break;
                            case 2:
                                str = aRouteData->st_list.at(aStnIdx).f_msg3_en;
                                if(!tms_alarm)
                                    if((day_night_mode==MODE_DAY)&&(!emergency))/*if((day_night_mode==MODE_DAY)||(!emergency))*/
                                    {
                                        emit audio_add(base_dir+"/sounds/f_msg3_en.mp3");
                                        emit audio_play();
                                    }
                                break;
                            case 3:
                                str = aRouteData->st_list.at(aStnIdx).f_msg4_en;
                                if(!tms_alarm)
                                    if((day_night_mode==MODE_DAY)&&(!emergency))/*if((day_night_mode==MODE_DAY)||(!emergency))*/
                                    {
                                        emit audio_add(base_dir+"/sounds/f_msg4_en.mp3");
                                        emit audio_play();
                                    }
                                break;
                            }
                            break;
                        case ANN_RL:
                            lang = aRouteData->st_list.at(aStnIdx).rl;
                            //ui->label_led_pis->setFont(*getRLFont(aRouteData->st_list.at(aStnIdx).rl));
                            if(aRouteData->st_list.at(aStnIdx).st_name_rl=="" && aRouteData->st_list.at(aStnIdx).f_msg2_rl==""
                                    &&aRouteData->st_list.at(aStnIdx).f_msg3_rl==""&&aRouteData->st_list.at(aStnIdx).f_msg4_rl=="")
                            {
                                skipSync=true;
                                f_msg_idx=3;
                            }
                            switch(f_msg_idx)
                            {
                            case 0:
                                str = aRouteData->st_list.at(aStnIdx).st_name_rl;
                                break;
                            case 1:
                                str = aRouteData->st_list.at(aStnIdx).f_msg2_rl;
                                break;
                            case 2:
                                str = aRouteData->st_list.at(aStnIdx).f_msg3_rl;
                                break;
                            case 3:
                                str = aRouteData->st_list.at(aStnIdx).f_msg4_rl;
                                break;
                            }
#ifndef LAPTOP
                            announcement_ongoing=false;
                            //                                gpio.gpio_set_value(PA_SWITCH,OP_OFF);
#endif
                            break;
                        }

                        if(!skipSync)
                        {
                            //ui->label_led_pis->setText(str);
                            // ui->label_lcd_pis->setText(str);
                            update();
                            if(!emergency)
                            {
                                drive_pis();
                                emit slave_message_send();
                            }
                            skipSync=false;
                        }

                        f_msg_idx++;
                        if(f_msg_idx==4)
                        {
                            f_msg_idx=0;
                            ai=(int)announcement_idx;
                            ai++;
                            announcement_idx=(announcement_idx_t)ai;
                            if(announcement_idx==ANN_RL)
                            {
                                announcement_idx = ANN_HINDI;
                                papis_sequence=PAPIS_WAIT_ARRIVAL;
                            }
                        }
                    }
                        break;
                    case PAPIS_WAIT_ARRIVAL:
                    {
                        QString str="";
                        switch(announcement_idx)
                        {
                        case ANN_HINDI:
                            //ui->label_led_pis->setFont(*font_hi);
                            str = aRouteData->st_list.at(aStnIdx).st_name_hi;
                            break;
                        case ANN_ENG:
                            //ui->label_led_pis->setFont(*font_en);
                            str = aRouteData->st_list.at(aStnIdx).st_name_en;
                            break;
                        case ANN_RL:
                            lang = aRouteData->st_list.at(aStnIdx).rl;
                            //ui->label_led_pis->setFont(*getRLFont(aRouteData->st_list.at(aStnIdx).rl));
                            if(aRouteData->st_list.at(aStnIdx).st_name_rl=="")
                            {
                                skipSync=true;
                            }
                            else
                            {
                                str = aRouteData->st_list.at(aStnIdx).st_name_rl;
                            }

                            break;
                        case ANN_DONE:

                            break;
                        }

                        if(!skipSync)
                        {
                            //ui->label_led_pis->setText(str);
                            // ui->label_lcd_pis->setText(str);
                            update();
                            if(!emergency)
                            {
                                drive_pis();
                                emit slave_message_send();
                            }
                            skipSync=false;
                        }

                        ai=(int)announcement_idx;
                        ai++;
                        announcement_idx=(announcement_idx_t)ai;
                        if(announcement_idx==ANN_DONE)
                        {
                            announcement_idx = ANN_HINDI;
                        }
                    }
                        break;
                    case PAPIS_DESTINATION:
                    {
                        QString str="";
#ifndef LAPTOP
                        announcement_ongoing=true;
                        //                        gpio.gpio_set_value(PA_SWITCH,OP_ON);
#endif

                        switch(announcement_idx)
                        {
                        case ANN_HINDI:
                            //ui->label_led_pis->setFont(*font_hi);
                            switch(f_msg_idx)
                            {
                            case 0:
                                str = aRouteData->st_list.at(aStnIdx).st_name_hi;
                                if(!tms_alarm)
                                    if((day_night_mode==MODE_DAY)&&(!emergency))/*if((day_night_mode==MODE_DAY)||(!emergency))*/
                                    {
                                        emit audio_add(base_dir+"/sounds/chime.mp3");
                                        emit audio_add(base_dir+"routes/"+
                                                       routesInfo.routeList.at(aRIdx).tr_no_en+"/sounds/"+QString::number(aStnIdx+1)+
                                                       +"/st_name.mp3");
                                        emit audio_add(base_dir+"/sounds/ss_arrv1_hi.mp3");
                                        emit audio_play();
                                    }
                                break;
                            case 1:
                                str = aRouteData->st_list.at(aStnIdx).f_msg1_hi;
                                if(!tms_alarm)
                                    if((day_night_mode==MODE_DAY)&&(!emergency))/*if((day_night_mode==MODE_DAY)||(!emergency))*/
                                    {
                                        emit audio_add(base_dir+"/sounds/f_msg1_hi.mp3");
                                        emit audio_play();
                                    }
                                break;
                            case 2:
                                str = aRouteData->st_list.at(aStnIdx).f_msg2_hi;
                                if(!tms_alarm)
                                    if((day_night_mode==MODE_DAY)&&(!emergency))/*if((day_night_mode==MODE_DAY)||(!emergency))*/
                                    {
                                        emit audio_add(base_dir+"/sounds/f_msg2_hi.mp3");
                                        emit audio_play();
                                    }
                                break;
                            case 3:
                                str = aRouteData->st_list.at(aStnIdx).f_msg3_hi;
                                if(!tms_alarm)
                                    if((day_night_mode==MODE_DAY)&&(!emergency))/*if((day_night_mode==MODE_DAY)||(!emergency))*/
                                    {

                                        emit audio_add(base_dir+"/sounds/f_msg3_hi.mp3");
                                        emit audio_play();
                                    }
                                break;
                            case 4:
                                str = aRouteData->st_list.at(aStnIdx).f_msg4_hi;
                                if(!tms_alarm)
                                    if((day_night_mode==MODE_DAY)&&(!emergency))/*if((day_night_mode==MODE_DAY)||(!emergency))*/
                                    {

                                        emit audio_add(base_dir+"/sounds/f_msg4_hi.mp3");
                                        emit audio_play();
                                    }
                                break;
                            }
                            break;
                        case ANN_ENG:
                            //ui->label_led_pis->setFont(*font_en);
                            switch(f_msg_idx)
                            {
                            case 0:
                                str = aRouteData->st_list.at(aStnIdx).st_name_en;
                                if(!tms_alarm)
                                    if((day_night_mode==MODE_DAY)&&(!emergency))/*if((day_night_mode==MODE_DAY)||(!emergency))*/
                                    {

                                        emit audio_add(base_dir+"/sounds/chime.mp3");
                                        emit audio_add(base_dir+"/sounds/ss_arrv1_en.mp3");
                                        emit audio_add(base_dir+"routes/"+
                                                       routesInfo.routeList.at(aRIdx).tr_no_en+"/sounds/"+QString::number(aStnIdx+1)+
                                                       +"/st_name.mp3");
                                        emit audio_play();
                                    }
                                break;
                            case 1:
                                str = aRouteData->st_list.at(aStnIdx).f_msg1_en;
                                if(!tms_alarm)
                                    if((day_night_mode==MODE_DAY)&&(!emergency))/*if((day_night_mode==MODE_DAY)||(!emergency))*/
                                    {
                                        emit audio_add(base_dir+"/sounds/f_msg1_en.mp3");
                                        emit audio_play();
                                    }
                                break;
                            case 2:
                                str = aRouteData->st_list.at(aStnIdx).f_msg2_en;
                                if(!tms_alarm)
                                    if((day_night_mode==MODE_DAY)&&(!emergency))/*if((day_night_mode==MODE_DAY)||(!emergency))*/
                                    {
                                        emit audio_add(base_dir+"/sounds/f_msg2_en.mp3");
                                        emit audio_play();
                                    }
                                break;
                            case 3:
                                str = aRouteData->st_list.at(aStnIdx).f_msg3_en;
                                if(!tms_alarm)
                                    if((day_night_mode==MODE_DAY)&&(!emergency))/*if((day_night_mode==MODE_DAY)||(!emergency))*/
                                    {
                                        emit audio_add(base_dir+"/sounds/f_msg3_en.mp3");
                                        emit audio_play();
                                    }
                                break;
                            case 4:
                                str = aRouteData->st_list.at(aStnIdx).f_msg4_en;
                                if(!tms_alarm)
                                    if((day_night_mode==MODE_DAY)&&(!emergency))/*if((day_night_mode==MODE_DAY)||(!emergency))*/
                                    {

                                        emit audio_add(base_dir+"/sounds/f_msg4_en.mp3");
                                        emit audio_play();
                                    }
                                break;
                            }
                            break;
                        case ANN_RL:
                            lang = aRouteData->st_list.at(aStnIdx).rl;
                            //ui->label_led_pis->setFont(*getRLFont(aRouteData->st_list.at(aStnIdx).rl));
                            if(aRouteData->st_list.at(aStnIdx).st_name_rl=="" && aRouteData->st_list.at(aStnIdx).f_msg2_rl=="" && aRouteData->st_list.at(current_stn_idx).f_msg1_rl==""
                                    &&aRouteData->st_list.at(aStnIdx).f_msg3_rl==""&&aRouteData->st_list.at(aStnIdx).f_msg4_rl=="")
                            {
                                skipSync=true;
                                f_msg_idx=4;
                            }
                            else
                            {
                                switch(f_msg_idx)
                                {
                                case 0:
                                    str = aRouteData->st_list.at(current_stn_idx).st_name_rl;
                                    break;
                                case 1:
                                    str = aRouteData->st_list.at(current_stn_idx).f_msg1_rl;
                                    break;
                                case 2:
                                    str = aRouteData->st_list.at(current_stn_idx).f_msg2_rl;
                                    break;
                                case 3:
                                    str = aRouteData->st_list.at(current_stn_idx).f_msg3_rl;
                                    break;
                                case 4:
                                    str = aRouteData->st_list.at(current_stn_idx).f_msg4_rl;
                                    break;
                                }
                            }
#ifndef LAPTOP
                            announcement_ongoing=false;
                            //                                gpio.gpio_set_value(PA_SWITCH,OP_OFF);
#endif

                            break;
                        }

                        if(!skipSync)
                        {
                            //ui->label_led_pis->setText(str);
                            // ui->label_lcd_pis->setText(str);
                            update();
                            if(!emergency)
                            {
                                drive_pis();
                                emit slave_message_send();
                            }
                            skipSync=false;
                        }

                        f_msg_idx++;
                        if(f_msg_idx==5)
                        {
                            f_msg_idx=0;
                            ai=(int)announcement_idx;
                            ai++;
                            announcement_idx=(announcement_idx_t)ai;
                            if(announcement_idx==ANN_RL)
                            {
                                announcement_idx = ANN_HINDI;
                                papis_sequence=PAPIS_NEXT;
                            }
                        }
                    }
                        break;
                    case PAPIS_NEXT:
                    {
                        //ui->label_led_pis->setText("");
                        // ui->label_lcd_pis->setText("");
                        update();
                        if(!emergency)
                        {
                            drive_pis();
                        }

                        msgBox = new QMessageBox(this);
                        msgBox->setIcon(QMessageBox::Question);
                        msgBox->setWindowTitle("Route Complete");
                        dCnt=900;
                        msgBox->setText("Route "+routesInfo.routeList.at(aRIdx).tr_no_en+" is Completed");
                        msgBox->setInformativeText("Do you wish Switch Route now? Auto Switch in "+QString::number(dCnt/60)+" min, "+QString::number(dCnt%60)+" sec");
                        msgBox->setStandardButtons(QMessageBox::Yes);
                        popupTimer= new QTimer();
                        connect(popupTimer, SIGNAL(timeout()), this, SLOT(switch_route()));
                        popupTimer->start(1000);
                        msgBox->setModal(true);
                        update();
                        papis_sequence=PAPIS_NEXT_WAIT;
                        int ret = msgBox->exec();
                        pnext_popup=true;

                        if(ret == QMessageBox::Yes)
                        {
                            disconnect(popupTimer, SIGNAL(timeout()), this, SLOT(switch_route()));
                            popupTimer->stop();
                            if((int)route_type < route_data.count()-1)
                            {
                                route_type = (route_type_t)((int)route_type+1);
                            }
                            else
                                if(route_type == route_data.count()-1)
                                {
                                    route_type = ROUTE_1;
                                }

                            aRIdx = routesInfo.routeSel.at((int)route_type);
                            aRouteData = route_data.at((int)route_type);
                            avg_speed = 0;
                            aStnIdx = 0;
                            wake=false;
                            gps_fp=true;
                            gps_ctr=0;
                            gps_pidx=0;
                            nDist_p=0;
                            nDist_Ctr=0;
                            kmRt=0;
                            emit slave_route_send();

                            //ui->label_tr_name->setText(routesInfo.routeList.at(aRIdx).tr_no_en+", "+routesInfo.routeList.at(aRIdx).tr_nm_en);
                            //ui->label_tr_route->setText(routesInfo.routeList.at(aRIdx).tr_route_en);
                            //ui->label_tr_via->setText(routesInfo.routeList.at(aRIdx).tr_via_en);
                            //ui->label_curr_stn->setText(aRouteData->st_list.at(aStnIdx).st_name_en);
                            //ui->label_next_stn->setText(aRouteData->st_list.at(aStnIdx+1).st_name_en);
                            //                            ui->label_sch_time->setText(QString::number(aRouteData->st_list.at(aStnIdx+1).time_arr_hh)+":"+QString::number(aRouteData->st_list.at(aStnIdx+1).time_arr_mm));

                            if(systemconfig.gps_source == "FILE")
                            {
                                QString gfp = base_dir+"routes/"+routesInfo.routeList.at(aRIdx).tr_no_en+"/"+
                                        routesInfo.routeList.at(aRIdx).tr_no_en+".nmea";
                                if(QFile::exists(gfp))
                                {
                                    gpsFile = new QFile(gfp);

                                    nmeaSource->stopUpdates();
                                    disconnect(nmeaSource, SIGNAL(positionUpdated(QGeoPositionInfo)),this, SLOT(positionUpdated(QGeoPositionInfo)));
                                    disconnect(nmeaSource, SIGNAL(updateTimeout()), this, SLOT(updateTimeout()));
                                    disconnect(nmeaSource,SIGNAL(error(QGeoPositionInfoSource::Error)),this,SLOT(error(QGeoPositionInfoSource::Error)));

                                    nmeaSource = new QNmeaPositionInfoSource(QNmeaPositionInfoSource::SimulationMode);
                                    nmeaSource->setDevice(gpsFile);
                                    connect(nmeaSource, SIGNAL(positionUpdated(QGeoPositionInfo)),this, SLOT(positionUpdated(QGeoPositionInfo)));
                                    connect(nmeaSource, SIGNAL(updateTimeout()), this, SLOT(updateTimeout()));
                                    connect(nmeaSource,SIGNAL(error(QGeoPositionInfoSource::Error)),this,SLOT(error(QGeoPositionInfoSource::Error)));
                                    nmeaSource->startUpdates();
                                }
                            }

                            //put next route
                            papis_sequence=PAPIS_GEN_WELCOME;
                        }

                    }
                        break;
                    case PAPIS_NEXT_WAIT:
                        break;
                    }
                }
            }
        }

        gps_ctr++;
        if(gps_ctr==10)
        {
            if((papis_sequence==PAPIS_ARRIVAL)&&(papis_sequence==PAPIS_DEPARTURE)&&(papis_sequence==PAPIS_DESTINATION))
            {
                kmRt = aRouteData->st_list.at(aStnIdx).dist_km;
                emit slave_distance_send(kmRt);
            }
            else
                if(papis_sequence==PAPIS_NEXT)
                {

                }
                else
                    if(aStnIdx!=-1||route_type!=UNKNOWN)
                    {
                        emit slave_distance_send(kmRt);
                    }
        }

        if(gps_ctr==20)
        {
            gps_ctr=0;
        }
        gpsFix=true;
    }
    else
    {
        if(gpsFix)
        {
            //it has just lost fix
            //            ui->label_gps_sat->setText((positionInfo.isValid())?"Fix":"No-Fix");
            ui->label_gps->setStyleSheet("color: rgb(255, 255, 255); border-color: rgb(255, 255, 255);");
        }
        else
        {
            //            ui->label_gps_sat->setText((positionInfo.isValid())?"Fix":"No-Fix");
            //            ui->label_gps_lat->setText("");
            //            ui->label_gps_lon->setText("");
            //            ui->label_gps_ele->setText("");
            //            ui->label_gps_speed->setText("");
        }

        gpsFix=false;
    }
}

void MainWindow::draw_route()
{

}

void MainWindow::load_info()
{
    load_runs();

    for(int rIdx=0;rIdx<kmRun.count();rIdx++)
    {
        kmR+=kmRun.at(rIdx)->km;
        if(kmRun.at(rIdx)->date==QDate::currentDate())
        {
            kmT+=kmRun.at(rIdx)->km;
        }
    }

    //    ui->label_mpu_id->setText(systemconfig.mpu_id);
    //    ui->label_hw_id->setText(systemconfig.hw_id);
    //    ui->label_coach->setText(systemconfig.coach_id+" , "+systemconfig.coach_no);
    //    ui->label_km_age->setText(QString::number(kmR,'_f',1)+" km");
    //    ui->label_km_today->setText(QString::number(kmT,'_f',1)+" km");

    update();
}

bool MainWindow::load_runs()
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
            //kmRun.reserve(rArray.size());

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

bool MainWindow::dump_runs()
{
    QFile saveFile(QString(base_dir+"sdata/km_age.json"));

    if (!saveFile.open(QIODevice::WriteOnly)) {
        w_fault("Unable to save KM age record to Disk!");
        return false;
    }

    QJsonObject jsonObject;
    QJsonArray rArray;
    for(run *r : kmRun)
    {
        QJsonObject rObject;
        r->write(rObject);
        rArray.append(rObject);
    }
    jsonObject["runs"] = rArray;
    saveFile.write(QJsonDocument(jsonObject).toJson());
    saveFile.close();
    return true;
}

double MainWindow::Calc_distance_between_locations(double longA, double latA, double longB, double latB)
{
    longA = longA * M_PI / 180;
    longB = longB * M_PI / 180;
    latA = latA * M_PI / 180;
    latB = latB * M_PI / 180;

    return 6378.137 * acos(sin(latA)* sin(latB)+ cos(latA) * cos(latB) * cos(longB- longA));
}

void MainWindow::updateTimeout(void)
{
    //    qDebug() << __PRETTY_FUNCTION__;
    ui->label_gps->setStyleSheet("color: rgb(255, 255, 255);image: url(:/images/icons/icon_timeout.png);border-color: rgb(255, 255, 255);");
}

void MainWindow::error(QGeoPositionInfoSource::Error positioningError)
{
    ui->label_gps->setStyleSheet("color: rgb(255, 255, 255);image: url(:/images/icons/error.png);border-color: rgb(255, 255, 255);");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_menu_clicked()
{
    //    mnw = new MenuWindow();
    //    mnw->setWindowFlags(Qt::FramelessWindowHint);
    //    mnw->route_ops(route_ongoing);
    ////    mnw->showFullScreen();

    //    connect(mnw,SIGNAL(start_route()),this,SLOT(start_route()));
    //    connect(mnw,SIGNAL(stop_route()),this,SLOT(stop_route()));

    //    w_log(",Login Attempted");
    //    LoginDialog *loginDialog = new LoginDialog();
    //    loginDialog->setWindowFlags(Qt::FramelessWindowHint);
    //    #ifndef LAPTOP
    //    loginDialog->show();
    //    #else
    //    loginDialog->showFullScreen();
    //    #endif

    //connect with tcpserver
    //    pjsipSocket = new QTcpSocket();
    //    pjsipSocket->connectToHost(/*QHostAddress(QHostAddress::LocalHost)*/"192.168.1.50",1998);
    //    if(pjsipSocket->waitForConnected(3000))
    //    {
    //        //
    //        QString msg = "*ak#";
    //        pjsipSocket->write(msg.toLocal8Bit());
    //    }
    //    else
    //    {
    //        //no connection available
    //    }

    pjsipSocket->disconnect();
}

//

void MainWindow::socketReadyRead()
{
    // read from the server
    //ui->label_led_pis->setText("Connected");
}

void MainWindow::socketConnected()
{
    socketBusy=true;
}

void MainWindow::socketConnectionClosed()
{
    socketBusy=false;
}
void MainWindow::socketClosed()
{

}

void MainWindow::socketError( QAbstractSocket::SocketError e )
{

}

void MainWindow::socketWritten()
{

}

void MainWindow::on_pushButton_5_clicked()
{
    //show slave status
    //    ui->stackedWidget_2->setCurrentIndex(1);
    slaveTimer=new QTimer();
    connect(slaveTimer,SIGNAL(timeout()),this,SLOT(slaveTimeout()));
    slaveTimer->start(10000);
}

void MainWindow::slaveTimeout()
{
    //    ui->stackedWidget_2->setCurrentIndex(0);
    disconnect(slaveTimer,SIGNAL(timeout()),this,SLOT(slaveTimeout()));
}

void MainWindow::routeTimeout()
{
    ui->stackedWidget->setCurrentIndex(0);
    disconnect(routeTimer,SIGNAL(timeout()),this,SLOT(routeTimeout()));
}

void MainWindow::on_pushButton_6_clicked()
{
    //show slave status
    ui->stackedWidget->setCurrentIndex(1);
    routeTimer=new QTimer();
    connect(routeTimer,SIGNAL(timeout()),this,SLOT(routeTimeout()));
    routeTimer->start(10000);
}

void MainWindow::on_pushButton_8_toggled(bool checked)
{
    if(checked)
    {
        mask_dn=true;
        //        ui->label_mode->setStyleSheet("image: url(:/images/icons/icon_night.png);border-color: rgb(255, 255, 255);");
        day_night_mode=MODE_NIGHT;
        emit slave_mode_send("n");
    }
    else
    {
        mask_dn=false;
        emit slave_mode_send("d");
    }
}

void MainWindow::on_pushButton_7_clicked()
{
    //show toilet status
    //    ui->stackedWidget_2->setCurrentIndex(2);
    slaveTimer=new QTimer();
    connect(slaveTimer,SIGNAL(timeout()),this,SLOT(slaveTimeout()));
    slaveTimer->start(15000);
}

//For HCD
void MainWindow::on_pushButton_9_clicked()
{
    //Sending Route to HCD
    hcdRouteSocket = new QTcpSocket();
    hcdRouteSocket->connectToHost("192.168.1.115",1000);
    if(hcdRouteSocket->waitForConnected(3000))
    {

//        QFile file("/home/csemi/kartikeya/HCD_ROUTES/TEST_ROUTES/route_2127.bin");

        QFile file("/home/root/ddc_sw/hcd_routes/ROUTE_12345.bin");
        if (!file.open(QIODevice::ReadOnly)) return;
        QByteArray iContents = file.readAll();

        hcdRouteSocket->write(iContents);
    }
    hcdRouteSocket->disconnect();
    hcdRouteSocket->close();
}

void MainWindow::on_pushButton_11_clicked()
{
    //ddb sl
    drive_ddb_sl_rs485(DISP_REP,SCRL_LEFT,0);
}

void MainWindow::on_pushButton_10_clicked()
{
    //ddb dl
    drive_ddb_dl_rs485(DISP_REP,SCRL_LEFT,0);
}

void MainWindow::message_pass(QString msg)
{
    //ui->label_msg->setText(msg);
}

double km=0;
void MainWindow::on_pushButton_12_clicked()
{
    km+=1;
    emit slave_distance_send(km);
}

int t=0;
void MainWindow::on_pushButton_13_clicked()
{
    t++;
    if(t==5)t=1;
    emit slave_alert_send(t);
}

void MainWindow::on_pushButton_14_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    routeTimer=new QTimer();
    connect(routeTimer,SIGNAL(timeout()),this,SLOT(routeTimeout()));
    routeTimer->start(15000);
}

void MainWindow::on_pushButton_15_toggled(bool checked)
{
    if(checked)
    {
        mask_dn=true;
        //        ui->label_mode->setStyleSheet("image: url(:/images/icons/icon_day.png);border-color: rgb(255, 255, 255);");
        day_night_mode=MODE_DAY;
        emit slave_mode_send("d");
    }
    else
    {
        mask_dn=false;
        day_night_mode=MODE_NIGHT;
        emit slave_mode_send("n");
    }
}

void MainWindow::on_pushButton_16_clicked()
{
    //reload config
    load_config();
    //    ui->label_mpu_id->setText(systemconfig.mpu_id);
    //    ui->label_hw_id->setText(systemconfig.hw_id);
    //    ui->label_coach->setText(systemconfig.coach_id+" , "+systemconfig.coach_no);
    //    ui->label_km_age->setText(QString::number(kmR,'_f',1)+" km");
    //    ui->label_km_today->setText(QString::number(kmT,'_f',1)+" km");
}

void MainWindow::on_pushButton_17_clicked()
{
    //reload routes
    QString mR;
    QList <int> skipL;

    if(reload_routes())
    {
        for(int idx=0;idx<routesInfo.routeSel.count();idx++)
        {
            route_ok=reload_routeData(idx);
            if(!route_ok)
            {
                route r = routesInfo.routeList.at(routesInfo.routeSel.at(idx));
                r.d_av = false;
                w_fault(",No Data found for Route: "+r.tr_no_en);
                routesInfo.routeList.replace(routesInfo.routeSel.at(idx),r);
                mR+=routesInfo.routeList.at(routesInfo.routeSel.at(idx)).tr_no_en+" ";
                skipL.append(idx);
            }
        }
    }

    if(!route_ok)
    {
        //remove routes for which data is not found
        for(int idx=0;idx<skipL.count();idx++)
        {
            routesInfo.routeSel.removeAt(skipL.at(id)-idx);
        }

        //ui->label_msg->setText(" Error: Data for configured route(s): "+mR+"NOT found!!");
        w_fault(",Data for configured route(s): "+mR+"NOT found!!");
        msgTimer = new QTimer();
        connect(msgTimer, SIGNAL(timeout()), this, SLOT(clearMessage()));
        msgTimer->setSingleShot(true);
        msgTimer->start(5000);
        update();
    }
}

void MainWindow::on_pushButton_18_clicked()
{
    //wifi on
    system(QString("connmanctl connect wifi_d03745f32c2f_50415049535f3247_managed_psk").toLocal8Bit());
}

void MainWindow::on_pushButton_19_clicked()
{
    t--;
    if(t<1)t=4;
    emit slave_clear_send(t);
}

//emu-memu specific
void MainWindow::on_pushButton_call_pick_clicked()
{
    pjsipSocket = new QTcpSocket();
    pjsipSocket->connectToHost("192.168.1.50",1998);
    if(pjsipSocket->waitForConnected(3000))
    {
        if(buddyNum!="")
        {
            //initiate call
            QString msg = "o"+buddyNum;      // o for outgoing call
            pjsipSocket->write(msg.toLocal8Bit());
            ui->label_call_status->setText("Call Initiated");
            callTimeCtr=0;
            callTimer->start(1000);
        }
        else
        {
            //pickup incoming call
            QString msg = "a";
            pjsipSocket->write(msg.toLocal8Bit());
            ui->label_call_status->setText("Call Picked");

            callTimeCtr=0;
            callTimer->start(1000);
        }
    }
    else
    {
        //no connection available
    }

    pjsipSocket->disconnect();
    pjsipSocket->close();
}


void MainWindow::on_pushButton_call_reject_clicked()
{
    pjsipSocket = new QTcpSocket();
    pjsipSocket->connectToHost("192.168.1.50",1998);
    if(pjsipSocket->waitForConnected(3000))
    {
        //
        QString msg = "h";
        pjsipSocket->write(msg.toLocal8Bit());
        ui->label_call_status->setText("");
        buddyNum="";
        ui->label_emergency->setStyleSheet(QString("image: url(:/images/icons/icon_siren_bw.png);background-color: rgb(238, 238, 236);"));

        //TBD: do for all
        ui->pushButton_select_etb1->setStyleSheet(QString("background-color: rgb(255, 255, 255);"));
        ui->pushButton_select_etb1->setChecked(false);

        if(callTimer->isActive())
        {
            callTimer->stop();
        }

    }
    else
    {
        //no connection available
    }

    pjsipSocket->disconnect();
    pjsipSocket->close();
//    QString sendRec2Hmi = "sshpass -p 'hmi@root' scp /home/root/rec_* hmi@192.168.1.70:/home/hmi/ETBArchives";
//    system(qPrintable(sendRec2Hmi));

//    QString moveRecFile = "mv /home/root/rec_* /home/root/arec";
//    system(qPrintable(moveRecFile));



    //Sending String to STOP CCTV Feed on HMI
//    etbCallSocket = new QTcpSocket();
//    etbCallSocket->connectToHost("192.168.1.70",1999);
//    if(etbCallSocket->waitForConnected(3000))
//    {
//        QString msg = "EN:";
//        etbCallSocket->write(msg.toLocal8Bit());
//    }

//    etbCallSocket->disconnect();
//    etbCallSocket->close();
}

void MainWindow::callTimerTimeout()
{
    callTimeCtr++;
    ui->label_call_duration->setText(QString::number(callTimeCtr/60)+":"+QString::number(callTimeCtr%60));

    //give notification for autocut
    if(callAutoCut)
    {
        if(callTimeCtr>=callAutoCutTime-10 && callTimeCtr<callAutoCutTime)
        {
            ui->label_call_status->setText("AUTO HANGUP in "+QString::number(callAutoCutTime-callTimeCtr)+"s");
        }
        else
            if(callTimeCtr==callAutoCutTime)
            {
                //autocut
                ui->label_call_status->setText("");
                ui->label_call_duration->setText("");
                callTimer->stop();
                on_pushButton_call_reject_clicked();

                //TBD: call disconnected announement
            }
    }
}

void MainWindow::on_pushButton_select_etb1_toggled(bool checked)
{
    if(checked)
    {
        ui->pushButton_select_etb1->setStyleSheet(QString("background-color: rgb(255, 255, 0);"));
        ui->pushButton_select_etb2->setChecked(false);
        ui->pushButton_select_etb_dg->setChecked(false);
        ui->pushButton_select_etb_occ->setChecked(false);
        buddyNum="4";
    }
    else
    {
        ui->pushButton_select_etb1->setStyleSheet(QString("background-color: rgb(255, 255, 255);"));
        buddyNum="";
    }
}

void MainWindow::on_pushButton_select_etb2_toggled(bool checked)
{
    if(checked)
    {
        ui->pushButton_select_etb2->setStyleSheet(QString("background-color: rgb(255, 255, 0);"));
        ui->pushButton_select_etb1->setChecked(false);
        ui->pushButton_select_etb_dg->setChecked(false);
        ui->pushButton_select_etb_occ->setChecked(false);
        buddyNum="6";
    }
    else
    {
        ui->pushButton_select_etb2->setStyleSheet(QString("background-color: rgb(255, 255, 255);"));
        buddyNum="";
    }
}

void MainWindow::on_pushButton_select_etb4_toggled(bool checked)
{

}

void MainWindow::on_pushButton_select_etb_dg_toggled(bool checked)
{
    if(checked)
    {
        ui->pushButton_select_etb_dg->setStyleSheet(QString("background-color: rgb(255, 255, 0);"));
        ui->pushButton_select_etb1->setChecked(false);
        ui->pushButton_select_etb2->setChecked(false);
        ui->pushButton_select_etb_occ->setChecked(false);
    }
    else
    {
        ui->pushButton_select_etb_dg->setStyleSheet(QString("background-color: rgb(255, 255, 255);"));
    }
}


void MainWindow::on_pushButton_select_etb_occ_toggled(bool checked)
{
    if(checked)
    {
        ui->pushButton_select_etb_occ->setStyleSheet(QString("background-color: rgb(255, 255, 0);"));
        ui->pushButton_select_etb1->setChecked(false);
        ui->pushButton_select_etb2->setChecked(false);
        ui->pushButton_select_etb_dg->setChecked(false);
    }
    else
    {
        ui->pushButton_select_etb_occ->setStyleSheet(QString("background-color: rgb(255, 255, 255);"));
    }
}

void MainWindow::on_pushButton_next_clicked()
{
    ui->stackedWidget_announcements->setCurrentIndex(0);
}


void MainWindow::on_pushButton_back_clicked()
{
    ui->stackedWidget_announcements->setCurrentIndex(1);
}


void MainWindow::on_pushButton_cabmic_set_clicked()
{
    system(QString("amixer set 'Capture' "+QString::number(cab_spk_vol)+"%").toLocal8Bit());
}


void MainWindow::on_pushButton_cabspk_set_clicked()
{
    system(QString("amixer set 'Headphone' "+QString::number(cab_spk_vol)+"%").toLocal8Bit());
}


void MainWindow::on_pushButton_paspk_set_clicked()
{
    //todo: we will need to have a loop here with iterating over a dynamic list
    QTcpSocket *volSocket1 = new QTcpSocket();
    volSocket1->connectToHost("192.168.1.80",5655);
    if(volSocket1->waitForConnected(3000))
    {
        volSocket1->write(QString::number(pa_spk_vol).toLocal8Bit());
    }
    else
    {
        //no connection available
    }

    volSocket1->disconnect();
    volSocket1->close();

    QTcpSocket *volSocket2 = new QTcpSocket();
    volSocket2->connectToHost("192.168.1.81",5655);
    if(volSocket2->waitForConnected(3000))
    {
        volSocket2->write(QString::number(pa_spk_vol).toLocal8Bit());
    }
    else
    {
        //no connection available
    }

    volSocket2->disconnect();
    volSocket2->close();

    QTcpSocket *volSocket3 = new QTcpSocket();
    volSocket3->connectToHost("192.168.1.82",5655);
    if(volSocket3->waitForConnected(3000))
    {
        volSocket3->write(QString::number(pa_spk_vol).toLocal8Bit());
    }
    else
    {
        //no connection available
    }

    volSocket3->disconnect();
    volSocket3->close();

    QTcpSocket *volSocket4 = new QTcpSocket();
    volSocket4->connectToHost("192.168.1.83",5655);
    if(volSocket4->waitForConnected(3000))
    {
        volSocket4->write(QString::number(pa_spk_vol).toLocal8Bit());
    }
    else
    {
        //no connection available
    }

    volSocket4->disconnect();
    volSocket4->close();
}


void MainWindow::on_pushButton_pa_auto_clicked()
{

}


void MainWindow::on_pushButton_pa_manual_clicked()
{

}


void MainWindow::on_pushButton_pa_auto_toggled(bool checked)
{
    ui->pushButton_pa_manual->setChecked(!checked);
    if(checked)
    {

    }
    else
    {

    }
}

void MainWindow::on_pushButton_pa_manual_toggled(bool checked)
{
    ui->pushButton_pa_auto->setChecked(!checked);
    if(checked)
    {

    }
    else
    {

    }
}

void MainWindow::on_pushButton_speak_toggled(bool checked)
{
    //    if(checked)
    //    {
    //        system(QString("gst-launch-1.0 pulsesrc ! tee name=t ! queue ! audioconvert ! autoaudiosink t. ! queue ! audioconvert ! audio/x-raw,channels=1,depth=16,width=16,rate=44100 ! rtpL16pay pt=97 ! udpsink host=239.1.1.1 auto-multicast=true port=6000 &").toLocal8Bit());
    //    }
    //    else
    //    {
    //        system(QString("./stop_gst.sh").toLocal8Bit());
    //    }
}

void MainWindow::on_pushButton_mute_clicked()
{

}


void MainWindow::on_pushButton_mute_toggled(bool checked)
{
    if(checked)
    {
        system(QString("amixer set 'Capture' 0%").toLocal8Bit());
    }
    else
    {
        system(QString("amixer set 'Capture' "+QString::number(cab_spk_vol)+"%").toLocal8Bit());
    }
}

void MainWindow::on_pushButton_spk_toggled(bool checked)
{
    if(checked)
    {
        system(QString("gst-launch-1.0 pulsesrc ! tee name=t ! queue ! audioconvert ! autoaudiosink t. ! queue ! audioconvert ! audio/x-raw,channels=1,depth=16,width=16,rate=44100 ! rtpL16pay pt=97 ! udpsink host=239.1.1.1 auto-multicast=true port=6000 &").toLocal8Bit());
    }
    else
    {
        system(QString("ps -A | grep gst-launch-1.0 | awk '{ printf $1 }' | xargs kill -2 $1").toLocal8Bit());
    }
}


void MainWindow::load_prerec_files()
{
    //load announcement files from base director
    ui->listView_rec->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);

    QDir dir(base_dir+"/prerec_audio");
    audio_files = dir.entryList(QStringList() << "*.mp3" << "*.MP3",QDir::Files);
    QStringList labels = {"Sound Name"};
    aModel = new QStandardItemModel(audio_files.count(),1);
    aModel->setHorizontalHeaderLabels(labels);

    for (int row = 0; row < aModel->rowCount(); ++row) {

        QStandardItem *item_filename = new QStandardItem(audio_files.at(row));
        aModel->setItem(row,0,item_filename);
    }

    ui->listView_rec->setModel(aModel);

    ui->listView_rec->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
    ui->listView_rec->scrollToTop();
}

void MainWindow::on_pushButton_up_clicked()
{
    //list up
    selectedIndex = scrlIdx;
    if(scrlIdx>0)
    {
        scrlIdx--;
    }
    ui->listView_rec->verticalScrollBar()->setValue(scrlIdx);
    highlight_row(scrlIdx);
}


void MainWindow::on_pushButton_down_clicked()
{
    //list down
    selectedIndex = scrlIdx;
    if(scrlIdx<aModel->rowCount())
    {
        scrlIdx++;
    }
    ui->listView_rec->verticalScrollBar()->setValue(scrlIdx);
    highlight_row(scrlIdx);
}

void MainWindow::highlight_row(int row)
{
    selectedIndex=row;
    QModelIndex index = ui->listView_rec->model()->index(row, 0);
    ui->listView_rec->setCurrentIndex(index);
}


void MainWindow::on_pushButton_stoprec_clicked()
{
    //stop announcement
    //     system(QString("./stop_gst.sh").toLocal8Bit());
    system(QString("ps -A | grep gst-launch-1.0 | awk '{ printf $1 }' | xargs kill -2 $1").toLocal8Bit());
}


void MainWindow::on_pushButton_cast_clicked()
{
    //broadcast announcement
    emit audio_add(base_dir+"/prerec_audio/"+audio_files.at(selectedIndex),ANN_NETWORK);
    emit audio_play();
}


void MainWindow::on_pushButton_playrec_clicked()
{
    //play announcement locally
    emit audio_add(base_dir+"/prerec_audio/"+audio_files.at(selectedIndex),ANN_LOCAL);
    emit audio_play();
}


void MainWindow::on_listView_rec_clicked(const QModelIndex &index)
{
    selectedIndex = index.row();
}


void MainWindow::on_verticalSlider_cabmic_valueChanged(int value)
{
    cab_mic_vol = value;
    ui->label_cabmic_vol->setText(QString::number(cab_mic_vol)+"%");
}


void MainWindow::on_verticalSlider_cabspk_valueChanged(int value)
{
    cab_spk_vol = value;
    ui->label_cabspk_vol->setText(QString::number(cab_spk_vol)+"%");
}


void MainWindow::on_verticalSlider_paspk_valueChanged(int value)
{
    pa_spk_vol = value;
    ui->label_paspkr_vol->setText(QString::number(pa_spk_vol)+"%");
}


void MainWindow::on_pushButton_cabmic_p_clicked()
{
    if(cab_mic_vol<100)
    {
        cab_mic_vol++;
        ui->label_cabmic_vol->setText(QString::number(cab_mic_vol)+"%");
    }
}


void MainWindow::on_pushButton_cabmic_m_clicked()
{
    if(cab_mic_vol>0)
    {
        cab_mic_vol--;
        ui->label_cabmic_vol->setText(QString::number(cab_mic_vol)+"%");
    }
}


void MainWindow::on_pushButton_cabspk_p_clicked()
{
    if(cab_spk_vol<100)
    {
        cab_spk_vol++;
        ui->label_cabspk_vol->setText(QString::number(cab_spk_vol)+"%");
    }
}


void MainWindow::on_pushButton_cabspk_m_clicked()
{
    if(cab_spk_vol>0)
    {
        cab_spk_vol--;
        ui->label_cabspk_vol->setText(QString::number(cab_spk_vol)+"%");
    }
}


void MainWindow::on_pushButton_paspk_p_clicked()
{
    if(pa_spk_vol<100)
    {
        pa_spk_vol++;
        ui->label_paspkr_vol->setText(QString::number(pa_spk_vol)+"%");
    }
}


void MainWindow::on_pushButton_paspk_m_clicked()
{
    if(pa_spk_vol>0)
    {
        pa_spk_vol--;
        ui->label_paspkr_vol->setText(QString::number(pa_spk_vol)+"%");
    }
}

