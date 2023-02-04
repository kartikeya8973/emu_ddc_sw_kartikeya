#include "devicetestdialog.h"
#include "ui_devicetestdialog.h"

extern const char* ptr_ddb_sl;
extern const char* ptr_ddb_dl;
extern const char*ptr_led_pis;
extern QList <slave*> *papis_slaves;
extern systemconfig systemconfig;
DeviceTestDialog::DeviceTestDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DeviceTestDialog)
{
    ui->setupUi(this);

//    ui->label_ddb_sl_p1->setFont(font);
//    ui->label_ddb_sl_p2->setFont(font);
//    ui->label_ddb_dl_p1->setFont(font);
//    ui->label_ddb_dl_p2->setFont(font);
//    ui->label_led_pis->setFont(font);
////    ui->label_lcd_pis->setFont(font);

    render();
}

DeviceTestDialog::~DeviceTestDialog()
{
    delete ui;
}

void DeviceTestDialog::on_pushButton_6_clicked()
{
    if(ui->pushButton_6->text()=="START")
    {
        ui->pushButton_6->setText("STOP");
        ui->pushButton_5->setDisabled(true);
        timer = new QTimer();
        connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));
        timer->start(5000);
    }
    else
    {
        ui->pushButton_6->setText("START");
        ui->pushButton_5->setEnabled(true);
        timer->stop();
    }
}

void DeviceTestDialog::render()
{
    if((papis_slaves->at(0)->installStatus==INSTALLED&&papis_slaves->at(0)->activeStatus==ACTIVE))
    {
        ui->label_status_lcd_pis18_1->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
    }
    else
    if((papis_slaves->at(0)->installStatus==INSTALLED&&papis_slaves->at(0)->activeStatus==INACTIVE))
    {
        ui->label_status_lcd_pis18_1->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
    }

    if((papis_slaves->at(1)->installStatus==INSTALLED&&papis_slaves->at(1)->activeStatus==ACTIVE))
    {
        ui->label_status_lcd_pis18_2->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
    }
    else
        if((papis_slaves->at(1)->installStatus==INSTALLED&&papis_slaves->at(1)->activeStatus==INACTIVE))
    {
        ui->label_status_lcd_pis18_2->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
    }

    if((papis_slaves->at(2)->installStatus==INSTALLED&&papis_slaves->at(2)->activeStatus==ACTIVE))
    {
        ui->label_status_lcd_pis18_3->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
    }
    else
        if((papis_slaves->at(2)->installStatus==INSTALLED&&papis_slaves->at(2)->activeStatus==INACTIVE))
    {
        ui->label_status_lcd_pis18_3->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
    }

    if((papis_slaves->at(3)->installStatus==INSTALLED&&papis_slaves->at(3)->activeStatus==ACTIVE))
    {
        ui->label_status_lcd_pis18_4->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
    }
    else
        if((papis_slaves->at(3)->installStatus==INSTALLED&&papis_slaves->at(3)->activeStatus==INACTIVE))
    {
        ui->label_status_lcd_pis18_4->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
    }

    if((papis_slaves->at(4)->installStatus==INSTALLED&&papis_slaves->at(4)->activeStatus==ACTIVE))
    {
        ui->label_status_lcd_pis10_1->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
    }
    else
        if((papis_slaves->at(4)->installStatus==INSTALLED&&papis_slaves->at(4)->activeStatus==INACTIVE))
    {
        ui->label_status_lcd_pis10_1->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
    }

    if((papis_slaves->at(5)->installStatus==INSTALLED&&papis_slaves->at(5)->activeStatus==ACTIVE))
    {
        ui->label_status_lcd_pis10_2->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
    }
    else
        if((papis_slaves->at(5)->installStatus==INSTALLED&&papis_slaves->at(5)->activeStatus==INACTIVE))
    {
        ui->label_status_lcd_pis10_2->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
    }

    if((papis_slaves->at(6)->installStatus==INSTALLED&&papis_slaves->at(6)->activeStatus==ACTIVE))
    {
        ui->label_status_lcd_pis10_3->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
    }
    else
        if((papis_slaves->at(6)->installStatus==INSTALLED&&papis_slaves->at(6)->activeStatus==INACTIVE))
    {
        ui->label_status_lcd_pis10_3->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
    }

    if((papis_slaves->at(7)->installStatus==INSTALLED&&papis_slaves->at(7)->activeStatus==ACTIVE))
    {
        ui->label_status_lcd_pis10_4->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
    }
    else
        if((papis_slaves->at(7)->installStatus==INSTALLED&&papis_slaves->at(7)->activeStatus==INACTIVE))
    {
        ui->label_status_lcd_pis10_4->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
    }

    if((papis_slaves->at(8)->installStatus==INSTALLED&&papis_slaves->at(8)->activeStatus==ACTIVE))
    {
        ui->label_status_lcd_pis10_5->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
    }
    else
        if((papis_slaves->at(8)->installStatus==INSTALLED&&papis_slaves->at(8)->activeStatus==INACTIVE))
    {
        ui->label_status_lcd_pis10_5->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
    }

    if((papis_slaves->at(9)->installStatus==INSTALLED&&papis_slaves->at(9)->activeStatus==ACTIVE))
    {
        ui->label_status_lcd_pis10_6->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
    }
    else
        if((papis_slaves->at(9)->installStatus==INSTALLED&&papis_slaves->at(9)->activeStatus==INACTIVE))
    {
        ui->label_status_lcd_pis10_6->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
    }

    if((papis_slaves->at(10)->installStatus==INSTALLED&&papis_slaves->at(10)->activeStatus==ACTIVE))
    {
        ui->label_status_lcd_pis10_7->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
    }
    else
        if((papis_slaves->at(10)->installStatus==INSTALLED&&papis_slaves->at(10)->activeStatus==INACTIVE))
    {
        ui->label_status_lcd_pis10_7->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
    }

    if((papis_slaves->at(11)->installStatus==INSTALLED&&papis_slaves->at(11)->activeStatus==ACTIVE))
    {
        ui->label_status_lcd_pis10_8->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
    }
    else
        if((papis_slaves->at(11)->installStatus==INSTALLED&&papis_slaves->at(11)->activeStatus==INACTIVE))
    {
        ui->label_status_lcd_pis10_8->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
    }

    if((papis_slaves->at(12)->installStatus==INSTALLED&&papis_slaves->at(12)->activeStatus==ACTIVE))
    {
        ui->label_status_lcd_pis10_9->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
    }
    else
        if((papis_slaves->at(12)->installStatus==INSTALLED&&papis_slaves->at(12)->activeStatus==INACTIVE))
    {
        ui->label_status_lcd_pis10_9->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
    }

    if((papis_slaves->at(13)->installStatus==INSTALLED&&papis_slaves->at(13)->activeStatus==ACTIVE))
    {
        ui->label_status_ddb_sl_1->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
    }
    else
        if((papis_slaves->at(13)->installStatus==INSTALLED&&papis_slaves->at(13)->activeStatus==INACTIVE))
    {
        ui->label_status_ddb_sl_1->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
    }

    if((papis_slaves->at(14)->installStatus==INSTALLED&&papis_slaves->at(14)->activeStatus==ACTIVE))
    {
        ui->label_status_ddb_sl_2->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
    }
    else
        if((papis_slaves->at(14)->installStatus==INSTALLED&&papis_slaves->at(14)->activeStatus==INACTIVE))
    {
        ui->label_status_ddb_sl_2->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
    }

    if((papis_slaves->at(15)->installStatus==INSTALLED&&papis_slaves->at(15)->activeStatus==ACTIVE))
    {
        ui->label_status_ddb_sl_3->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
    }
    else
        if((papis_slaves->at(15)->installStatus==INSTALLED&&papis_slaves->at(15)->activeStatus==INACTIVE))
    {
        ui->label_status_ddb_sl_3->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
    }

    if((papis_slaves->at(16)->installStatus==INSTALLED&&papis_slaves->at(16)->activeStatus==ACTIVE))
    {
        ui->label_status_ddb_sl_4->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
    }
    else
        if((papis_slaves->at(16)->installStatus==INSTALLED&&papis_slaves->at(16)->activeStatus==INACTIVE))
    {
        ui->label_status_ddb_sl_4->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
    }

    if((papis_slaves->at(17)->installStatus==INSTALLED&&papis_slaves->at(17)->activeStatus==ACTIVE))
    {
        ui->label_status_ddb_dl_1->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
    }
    else
        if((papis_slaves->at(17)->installStatus==INSTALLED&&papis_slaves->at(17)->activeStatus==INACTIVE))
    {
        ui->label_status_ddb_dl_1->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
    }

    if((papis_slaves->at(18)->installStatus==INSTALLED&&papis_slaves->at(18)->activeStatus==ACTIVE))
    {
        ui->label_status_ddb_dl_2->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
    }
    else
        if((papis_slaves->at(18)->installStatus==INSTALLED&&papis_slaves->at(18)->activeStatus==INACTIVE))
    {
        ui->label_status_ddb_dl_2->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
    }

    if((papis_slaves->at(19)->installStatus==INSTALLED&&papis_slaves->at(19)->activeStatus==ACTIVE))
    {
        ui->label_status_ddb_dl_3->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
    }
    else
        if((papis_slaves->at(19)->installStatus==INSTALLED&&papis_slaves->at(19)->activeStatus==INACTIVE))
    {
        ui->label_status_ddb_dl_3->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
    }

    if((papis_slaves->at(20)->installStatus==INSTALLED&&papis_slaves->at(20)->activeStatus==ACTIVE))
    {
        ui->label_status_ddb_dl_4->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
    }
    else
        if((papis_slaves->at(20)->installStatus==INSTALLED&&papis_slaves->at(20)->activeStatus==INACTIVE))
    {
        ui->label_status_ddb_dl_4->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
    }

    if((papis_slaves->at(21)->installStatus==INSTALLED&&papis_slaves->at(21)->activeStatus==ACTIVE))
    {
        ui->label_status_led_pis_1->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
    }
    else
        if((papis_slaves->at(21)->installStatus==INSTALLED&&papis_slaves->at(21)->activeStatus==INACTIVE))
    {
        ui->label_status_led_pis_1->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
    }

    if((papis_slaves->at(22)->installStatus==INSTALLED&&papis_slaves->at(22)->activeStatus==ACTIVE))
    {
        ui->label_status_led_pis_2->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
    }
    else
        if((papis_slaves->at(22)->installStatus==INSTALLED&&papis_slaves->at(22)->activeStatus==INACTIVE))
    {
        ui->label_status_led_pis_2->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
    }

    if((papis_slaves->at(23)->installStatus==INSTALLED&&papis_slaves->at(24)->activeStatus==ACTIVE))
    {
        ui->label_status_led_pis_3->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
    }
    else
        if((papis_slaves->at(23)->installStatus==INSTALLED&&papis_slaves->at(24)->activeStatus==INACTIVE))
    {
        ui->label_status_led_pis_3->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
    }

    if((papis_slaves->at(24)->installStatus==INSTALLED&&papis_slaves->at(24)->activeStatus==ACTIVE))
    {
        ui->label_status_led_pis_4->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
    }
    else
        if((papis_slaves->at(24)->installStatus==INSTALLED&&papis_slaves->at(24)->activeStatus==INACTIVE))
    {
        ui->label_status_led_pis_4->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
    }

    if((papis_slaves->at(25)->installStatus==INSTALLED&&papis_slaves->at(25)->activeStatus==ACTIVE))
    {
        ui->label_status_tms_1->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
    }
    else
        if((papis_slaves->at(25)->installStatus==INSTALLED&&papis_slaves->at(25)->activeStatus==INACTIVE))
    {
        ui->label_status_tms_1->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
    }

    if((papis_slaves->at(26)->installStatus==INSTALLED&&papis_slaves->at(26)->activeStatus==ACTIVE))
    {
        ui->label_status_tms_2->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
    }
    else
        if((papis_slaves->at(26)->installStatus==INSTALLED&&papis_slaves->at(26)->activeStatus==INACTIVE))
    {
        ui->label_status_tms_2->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
    }
    if((papis_slaves->at(27)->installStatus==INSTALLED&&papis_slaves->at(27)->activeStatus==ACTIVE))
    {
        ui->label_status_tms_3->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
    }
    else
        if((papis_slaves->at(27)->installStatus==INSTALLED&&papis_slaves->at(27)->activeStatus==INACTIVE))
    {
        ui->label_status_tms_3->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
    }

    if((papis_slaves->at(28)->installStatus==INSTALLED&&papis_slaves->at(28)->activeStatus==ACTIVE))
    {
        ui->label_status_tms_4->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: green;");
    }
    else
        if((papis_slaves->at(28)->installStatus==INSTALLED&&papis_slaves->at(28)->activeStatus==INACTIVE))
    {
        ui->label_status_tms_4->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: red;");
    }

    w_log(",Rendered Slave Dashboard");
    update();

}

void DeviceTestDialog::timeout()
{
    ui->label_ddb_sl_p2->setText(test_strings.at(strIdx));
    ui->label_ddb_dl_p2->setText(test_strings.at(strIdx));
    ui->label_led_pis->setText(test_strings.at(strIdx));
    ui->label_lcd_pis->setText(test_strings.at(strIdx));
    strIdx++;
    if(strIdx==test_strings.count())
    {
        strIdx=0;
    }
    update();
    drive_slaves();
}

void DeviceTestDialog::on_pushButton_5_clicked()
{
    //cancel
    this->close();
}

void DeviceTestDialog::drive_pis()
{
    //    //led_pis
    //extract data
    QPixmap pmap_led_pis;
    QImage imgL;

    const QRect rl = QRect(QPoint(190+80,250+103),QSize(512,16));
    pmap_led_pis = this->grab(rl);
    imgL = pmap_led_pis.toImage().convertToFormat(QImage::Format_Mono,Qt::MonoOnly);

    ushort ba_led_pis[512]={0};
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

    ptr_led_pis = (char*)ba_led_pis;

    int idx;
    for(idx=0; idx< papis_slaves->count();idx++)
    {
        if((papis_slaves->at(idx)->installStatus==INSTALLED&&papis_slaves->at(idx)->activeStatus==ACTIVE))
        {
            papis_slaves->at(idx)->socket = new QTcpSocket();
//            qDebug() << QString("Sending data to %1").arg(papis_slaves->at(idx)->ip_addr);

           switch(papis_slaves->at(idx)->device_type)
           {
           case DEV_LCD_PIS_18:
           case DEV_LCD_PIS_10:
               break;
           case DEV_DDB_SL:

               break;
           case DEV_DDB_DL:

               break;
           case DEV_LED_PIS:
               papis_slaves->at(idx)->socket->connectToHost(papis_slaves->at(idx)->ip_addr,1000);
               papis_slaves->at(idx)->socket->write(ptr_led_pis,1024);
               papis_slaves->at(idx)->socket->disconnect();
               papis_slaves->at(idx)->socket->close();
               break;
           case DEV_TMS:

               break;
            default:
               break;
           }
        }
    }
}

void DeviceTestDialog::drive_lcd_pis()
{
    int idx;
    for(idx=0; idx< papis_slaves->count();idx++)
    {
        if((papis_slaves->at(idx)->installStatus==INSTALLED&&papis_slaves->at(idx)->activeStatus==ACTIVE))
        {
            papis_slaves->at(idx)->socket = new QTcpSocket();
//            qDebug() << QString("Sending data to %1").arg(papis_slaves->at(idx)->ip_addr);

           switch(papis_slaves->at(idx)->device_type)
           {
           case DEV_LCD_PIS_18:
           case DEV_LCD_PIS_10:
               papis_slaves->at(idx)->socket->connectToHost(papis_slaves->at(idx)->ip_addr,3669);
               papis_slaves->at(idx)->socket->write(ui->label_led_pis->text().toLocal8Bit());
               papis_slaves->at(idx)->socket->disconnect();
               papis_slaves->at(idx)->socket->close();
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

void DeviceTestDialog::drive_slaves()
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
    for(idx=0; idx< papis_slaves->count();idx++)
    {
        if((papis_slaves->at(idx)->installStatus==INSTALLED&&papis_slaves->at(idx)->activeStatus==ACTIVE))
        {
            papis_slaves->at(idx)->socket = new QTcpSocket();
//            qDebug() << QString("Sending data to %1").arg(papis_slaves->at(idx)->ip_addr);

           switch(papis_slaves->at(idx)->device_type)
           {
           case DEV_LCD_PIS_18:
           case DEV_LCD_PIS_10:
               //send data to lcd pis
               papis_slaves->at(idx)->socket->connectToHost(papis_slaves->at(idx)->ip_addr,3669);
               papis_slaves->at(idx)->socket->write(ui->label_led_pis->text().toLocal8Bit());
               papis_slaves->at(idx)->socket->disconnect();
               papis_slaves->at(idx)->socket->close();
               break;
           case DEV_DDB_SL:
               papis_slaves->at(idx)->socket->connectToHost(papis_slaves->at(idx)->ip_addr,1000);
               papis_slaves->at(idx)->socket->write(ptr_ddb_sl,1024);
               papis_slaves->at(idx)->socket->disconnect();
               papis_slaves->at(idx)->socket->close();
               break;
           case DEV_DDB_DL:
               papis_slaves->at(idx)->socket->connectToHost(papis_slaves->at(idx)->ip_addr,1000);
               papis_slaves->at(idx)->socket->write(ptr_ddb_dl,1024);
               papis_slaves->at(idx)->socket->disconnect();
               papis_slaves->at(idx)->socket->close();
               break;
           case DEV_LED_PIS:
               papis_slaves->at(idx)->socket->connectToHost(papis_slaves->at(idx)->ip_addr,1000);
               papis_slaves->at(idx)->socket->write(ptr_led_pis,1024);
               papis_slaves->at(idx)->socket->disconnect();
               papis_slaves->at(idx)->socket->close();
               break;
           case DEV_TMS:

               break;
            default:
               break;
           }
        }
    }
}


