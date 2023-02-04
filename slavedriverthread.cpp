#include "slavedriverthread.h"

extern systemconfig systemconfig;
slaveDriverThread::slaveDriverThread()
{
    crcInit();
    rs485Port = new QSerialPort(this);
#ifndef LAPTOP
    rs485Port->setPortName("/dev/apalis-uart3");
#else
    rs485Port->setPortName("/dev/ttyUSB0");
#endif
    rs485Port->setBaudRate(QSerialPort::Baud115200);
    rs485Port->setDataBits(QSerialPort::Data8);
    rs485Port->setParity(QSerialPort::NoParity);
    rs485Port->setStopBits(QSerialPort::OneStop);
    rs485Port->setFlowControl(QSerialPort::NoFlowControl);
    connect(rs485Port,SIGNAL(readyRead()),this,SLOT(rs485_Received()));
    if(rs485Port->open(QIODevice::ReadWrite))
    {
        tmsTimer = new QTimer();
        connect(tmsTimer,SIGNAL(timeout()),this,SLOT(tmsTimeout()));
//        tmsTimer->start(3000);

        onTimer = new QTimer();
        connect(onTimer, SIGNAL(timeout()),this, SLOT(onTimeout()));
        onTimer->setSingleShot(true);

        offTimer = new QTimer();
        connect(offTimer, SIGNAL(timeout()),this, SLOT(offTimeout()));
        offTimer->setSingleShot(true);

        connect(this,SIGNAL(shoot_data()),this,SLOT(shoot()));
    }
}

void slaveDriverThread::shoot()
{
#ifndef LAPTOP
    gpio.gpio_set_value(RS485_IO,OP_OFF);
#endif
    offTimer->start(10);
}

void slaveDriverThread::tmsTimeout()
{
#ifndef LAPTOP
    gpio.gpio_set_value(RS485_IO,OP_OFF);
#endif
    offTimer->start(10);
}

void slaveDriverThread::offTimeout()
{
    rs485Port->write(cmdp,dsize);
#ifndef LAPTOP
        onTimer->start(100);
#else
        onTimer->start(100);
#endif
}

void slaveDriverThread::onTimeout()
{
    #ifndef LAPTOP
    gpio.gpio_set_value(RS485_IO,OP_ON);
    #endif
    wc_w.wakeAll();
}

void slaveDriverThread::rs485_Received()
{
    rep.append(rs485Port->readAll());
    dataR=true;
    wc_r.wakeAll();
}
extern const char* ptr_led_pis;

void slaveDriverThread::run()
{
    forever
    {
        switch(s_driver_task)
        {
        case SD_IDLE:
        {
            if(!sd_queue.isEmpty())
            {
                cTask = sd_queue.dequeue();
                s_driver_task = cTask->s_driver_task;
            }
            else
            {
                if(int_neg==true)
                {
                    s_driver_task=SD_SILENT_SCAN;
                }
                this->msleep(100);
            }
        }
            break;
        case SD_SILENT_SCAN:
        {
            //talk  to tms
            for(int idx=0; idx< papis_slaves.count();idx++)
            {
                if(papis_slaves.at(idx)->installStatus==INSTALLED&&papis_slaves.at(idx)->activeStatus==ACTIVE
                    &&papis_slaves.at(idx)->conn_mode=="RS485"&&papis_slaves.at(idx)->device_type==DEV_TMS)
                {
                    for(int idx=0;idx<6;idx++){cmd[idx]=0;}
                    cmd[0]='#';
                    cmd[1]=papis_slaves.at(idx)->rs485_addr;
                    cmd[2]='r';
                    unsigned short cc = crcFast(cmd,3);
                    cmd[3]=cc&0xff;
                    cmd[4]=(cc>>8)&0xff;
                    cmd[5]='*';
                    cmdp=cmd;
                    dsize=6;

                    for(int retr=0;retr<1/*3*/;retr++)
                    {
                        rep.clear();
                        emit message_pass("Toilet-"+QString::number(papis_slaves.at(idx)->rs485_addr-60)+" Get Status");
                        emit shoot_data();
                        m_w.lock();
                        wc_w.wait(&m_w);
                        m_w.unlock();

                        m_r.lock();
                        wc_r.wait(&m_r,QDeadlineTimer(3000));
                        m_r.unlock();
                        this->msleep(1000);

                        if(((rep[0]='#')&&((int)rep[1]==papis_slaves.at(idx)->rs485_addr)
                                &&(rep[12]=='*')))
                        {
                            emit message_pass("Status Acquired");
                            t_stat=*(toilet_status_t*)rep.data();
                            toilet_stat toilet;
//                            toilet.addr = t_stat.addr-60;
//                            toilet.clogged=t_stat.clogged;
//                            toilet.curtain=t_stat.curtain;
//                            toilet.occupancy=t_stat.occupancy;
//                            toilet.stationery=t_stat.stationery;
//                            toilet.sw=t_stat.sw;
//                            toilet.water_level=t_stat.water_level;

                            emit toilet_status(toilet);
                           break;
                        }
                        else
                        {
                            emit message_pass("No response!");
                        }
                    }
                }
            }

            //talk to one slave
            int idx;
            for(idx=slaveIdx; idx< papis_slaves.count();idx++)
            {
                if(papis_slaves.at(idx)->installStatus==INSTALLED&&papis_slaves.at(idx)->conn_mode=="RS485")
                {
                    for(int idx=0;idx<6;idx++){cmd[idx]=0;}
                    cmd[0]='#';
                    cmd[1]=papis_slaves.at(idx)->rs485_addr;
                    cmd[2]='s';
                    unsigned short cc = crcFast(cmd,3);
                    cmd[3]=cc&0xff;
                    cmd[4]=(cc>>8)&0xff;
                    cmd[5]='*';
                    cmdp=cmd;
                    dsize=6;

                    papis_slaves.at(idx)->activeStatus=INACTIVE;
                    for(int retr=0;retr<1/*3*/;retr++)
                    {
                        rep.clear();
                        emit message_pass("Scan Slave Addr: "+QString::number(papis_slaves.at(idx)->rs485_addr));
                        emit shoot_data();
                        m_w.lock();
                        wc_w.wait(&m_w);
                        m_w.unlock();

                        m_r.lock();
                        wc_r.wait(&m_r,QDeadlineTimer(3000));
                        m_r.unlock();
                        this->msleep(1000);

                        if(rep.count()>=6)
                        {
                            if(((rep[0]='#')&&((int)rep[1]==papis_slaves.at(idx)->rs485_addr)
                                    &&(rep[2]=='t')&&(rep[5]=='*')))
                            {
                                emit message_pass("Slave Detected");
                               papis_slaves.at(idx)->activeStatus=ACTIVE;
                               break;
                            }
                            else
                            {
                                emit message_pass("No response, retrying");
                            }
                        }
                    }
                    break;
                }
            }
            slaveIdx=idx+1;
            if(slaveIdx>=papis_slaves.count())
            {
                slaveIdx=0;
            }
            emit message_pass("");
            emit slave_silent_scan(papis_slaves);
            this->msleep(2500);
            s_driver_task=SD_IDLE;
        }
            break;
        case SD_NEG:
            break;
        case SD_NEG_ALL:
        {
            int cnt=0;
            int scn=0;
            for(int idx=0; idx< papis_slaves.count();idx++)
            {
                if(papis_slaves.at(idx)->conn_mode=="RS485" &&
                        papis_slaves.at(idx)->installStatus==INSTALLED)
                {
                    cnt++;
                }
            }

            for(int idx=0; idx< papis_slaves.count();idx++)
            {
                if(papis_slaves.at(idx)->conn_mode=="RS485" &&
                        papis_slaves.at(idx)->installStatus==INSTALLED)
                {
                    scn++;
                    for(int idx=0;idx<6;idx++){cmd[idx]=0;}
                    cmd[0]='#';
                    cmd[1]=papis_slaves.at(idx)->rs485_addr;
                    cmd[2]='s';
                    unsigned short cc = crcFast(cmd,3);
                    cmd[3]=cc&0xff;
                    cmd[4]=(cc>>8)&0xff;
                    cmd[5]='*';
                    cmdp=cmd;
                    dsize=6;

                    papis_slaves.at(idx)->activeStatus=INACTIVE;
                    for(int retr=0;retr<1/*3*/;retr++)
                    {
                        rep.clear();
                        emit message_pass("Search Addr: "+QString::number(papis_slaves.at(idx)->rs485_addr));
                        emit shoot_data();
                        m_w.lock();
                        wc_w.wait(&m_w);
                        m_w.unlock();

                        m_r.lock();
                        wc_r.wait(&m_r,QDeadlineTimer(3000));
                        m_r.unlock();
                        this->msleep(1000);

                        if(rep.count()>=6)
                        {
                            if(((rep[0]='#')&&((int)rep[1]==papis_slaves.at(idx)->rs485_addr)
                                    &&(rep[2]=='t')&&(rep[5]=='*')))
                            {
                                emit message_pass("Slave Found");
                               papis_slaves.at(idx)->activeStatus=ACTIVE;
                               break;
                            }
                            else
                            {
                                emit message_pass("No response, retrying");
                            }
                        }

                    }
                    emit slave_scan_progress(100*scn/cnt,papis_slaves);
                }
            }
            if(!int_neg){int_neg=true;}
            s_driver_task=SD_IDLE;
        }
            break;
        case SD_DRIVE_DDB_SL:
        {
            emit message_pass("");
            dp1.head='#';
            dp1.foot='*';
            dp1.pIdx=1;
            dp1.sspeed=(unsigned char)systemconfig.led_pis_speed;
            dp1.dispType = (int)cTask->dtype;
            dp1.scrlDir = (int)cTask->sdir;
            dp1.statCols = 0;
            for(int idx=0;idx<512;idx++)
            {
                dp1.fb[idx] = cTask->data[idx];
            }
            dp1.crc = (int)crcFast((unsigned char*)dp1.fb,512);

            dp2.head='#';
            dp2.foot='*';
            dp2.pIdx=2;
            for(int idx=512;idx<1024;idx++)
            {
                dp2.fb[idx-512] = cTask->data[idx];
            }
            dp2.crc = crcFast((unsigned char*)dp2.fb,512);

            for(int idx=0; idx< papis_slaves.count();idx++)
            {
                if((papis_slaves.at(idx)->installStatus==INSTALLED&&papis_slaves.at(idx)->activeStatus==ACTIVE
                    &&papis_slaves.at(idx)->conn_mode=="RS485"))
                {
                   switch(papis_slaves.at(idx)->device_type)
                   {
                   case DEV_LCD_PIS_18:
                   case DEV_LCD_PIS_10:
                       break;
                   case DEV_DDB_SL:
                   {
                        dp1.addr=papis_slaves.at(idx)->rs485_addr;
                        dp2.addr=papis_slaves.at(idx)->rs485_addr;

                       for(int idx=0;idx<2048;idx++){cmd[idx]=0;}
                       cmd[0]='#';
                       cmd[1]=papis_slaves.at(idx)->rs485_addr;
                       cmd[2]='r';
                       unsigned short cc = crcFast(cmd,3);
                       cmd[3]=cc&0xff;
                       cmd[4]=(cc>>8)&0xff;
                       cmd[5]='*';
                       memcpy((void*)cmdp,(void*)cmd,6);
                        dsize=6;

                        rep.clear();
                        emit message_pass("Search Addr: "+QString::number(papis_slaves.at(idx)->rs485_addr));
                        emit shoot_data();
                        m_w.lock();
                        wc_w.wait(&m_w);
                        m_w.unlock();

                        m_r.lock();
                        wc_r.wait(&m_r,QDeadlineTimer(5000));
                        m_r.unlock();

                        if(!((rep[0]='#')&&((int)rep[1]==papis_slaves.at(idx)->rs485_addr)
                                &&(rep[2]=='y')&&(rep[5]=='*')))
                        {
                            emit message_pass("Not found");
                           break;
                        }

                        emit message_pass("Slave Found, Sending Data-1");
                        this->msleep(1000);
                        dsize=sizeof(dataPacket1_t);
                        memcpy((void*)cmdp,(void*)&dp1,dsize);

                       rep.clear();
                       emit shoot_data();
                       m_w.lock();
                       wc_w.wait(&m_w);
                       m_w.unlock();
                       emit message_pass("Data Sent");

                        m_r.lock();
                        wc_r.wait(&m_r,QDeadlineTimer(5000));
                        m_r.unlock();

                        if(!((rep[0]='#')&&((int)rep[1]==papis_slaves.at(idx)->rs485_addr)
                                &&(rep[2]=='1')&&(rep[5]=='*')))
                        {
                            emit message_pass("No Response");
                           break;
                        }

                        emit message_pass("Accepted");
                        this->msleep(1000);
                        dsize=sizeof(dataPacket2_t);
                        memcpy((void*)cmdp,(void*)&dp2,dsize);

                        emit message_pass("Sending Data-2");
                       rep.clear();
                       emit shoot_data();
                       m_w.lock();
                       wc_w.wait(&m_w);
                       m_w.unlock();

                        m_r.lock();
                        wc_r.wait(&m_r,QDeadlineTimer(5000));
                        m_r.unlock();

                        if(!((rep[0]='#')&&((int)rep[1]==papis_slaves.at(idx)->rs485_addr)
                                &&(rep[2]=='2')&&(rep[5]=='*')))
                        {
                            emit message_pass("No Response");
                           break;
                        }

                        emit message_pass("Accepted");
                        this->msleep(1000);

                        for(int idx=0;idx<2048;idx++){cmd[idx]=0;}
                        //send disp cmd
                        cmd[0]='#';
                        cmd[1]=papis_slaves.at(idx)->rs485_addr;
                        cmd[2]='d';
                        cc = crcFast(cmd,3);
                        cmd[3]=cc&0xff;
                        cmd[4]=(cc>>8)&0xff;
                        cmd[5]='*';
                         dsize=6;
                         memcpy((void*)cmdp,(void*)cmd,6);
                        emit message_pass("Displayed");
                         this->msleep(1000);

                         rep.clear();
                         emit shoot_data();
                         m_w.lock();
                         wc_w.wait(&m_w);
                         m_w.unlock();

                   }
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
            s_driver_task=SD_IDLE;
        }
            break;
        case SD_DRIVE_DDB_DL:
        {
            dp1.head='#';
            dp1.foot='*';
            dp1.pIdx=1;
            dp1.sspeed=(unsigned char)systemconfig.led_pis_speed;
            dp1.dispType = (int)cTask->dtype;
            dp1.scrlDir = (int)cTask->sdir;
            dp1.statCols = 0;
            for(int idx=0;idx<512;idx++)
            {
                dp1.fb[idx] = cTask->data[idx];
            }
            dp1.crc = (int)crcFast((unsigned char*)dp1.fb,512);

            dp2.head='#';
            dp2.foot='*';
            dp2.pIdx=2;
            for(int idx=512;idx<1024;idx++)
            {
                dp2.fb[idx-512] = cTask->data[idx];
            }
            dp2.crc = crcFast((unsigned char*)dp2.fb,512);

            for(int idx=0; idx< papis_slaves.count();idx++)
//            for(int idx=papis_slaves.count()-1; idx>=0;idx--)
            {
                if((papis_slaves.at(idx)->installStatus==INSTALLED&&papis_slaves.at(idx)->activeStatus==ACTIVE
                    &&papis_slaves.at(idx)->conn_mode=="RS485"))
                {
                   switch(papis_slaves.at(idx)->device_type)
                   {
                   case DEV_LCD_PIS_18:
                   case DEV_LCD_PIS_10:
                       break;
                   case DEV_DDB_SL:
                       break;
                   case DEV_DDB_DL:
                   {
                        dp1.addr=papis_slaves.at(idx)->rs485_addr;
                        dp2.addr=papis_slaves.at(idx)->rs485_addr;

                       for(int idx=0;idx<2048;idx++){cmd[idx]=0;}
                       cmd[0]='#';
                       cmd[1]=papis_slaves.at(idx)->rs485_addr;
                       cmd[2]='r';
                       unsigned short cc = crcFast(cmd,3);
                       cmd[3]=cc&0xff;
                       cmd[4]=(cc>>8)&0xff;
                       cmd[5]='*';
                       memcpy((void*)cmdp,(void*)cmd,6);
                        dsize=6;

                        rep.clear();
                        emit message_pass("Search Addr: "+QString::number(papis_slaves.at(idx)->rs485_addr));
                        emit shoot_data();
                        m_w.lock();
                        wc_w.wait(&m_w);
                        m_w.unlock();

                        m_r.lock();
                        wc_r.wait(&m_r,QDeadlineTimer(5000));
                        m_r.unlock();
                        this->msleep(1000);
                        if(!((rep[0]='#')&&((int)rep[1]==papis_slaves.at(idx)->rs485_addr)
                                &&(rep[2]=='y')&&(rep[5]=='*')))
                        {
                            emit message_pass("Not found");
                           break;
                        }

                        emit message_pass("Slave Found, Sending Data-1");
                        this->msleep(1000);
                        dsize=sizeof(dataPacket1_t);
                        memcpy((void*)cmdp,(void*)&dp1,dsize);

                       rep.clear();
                       emit shoot_data();
                       m_w.lock();
                       wc_w.wait(&m_w);
                       m_w.unlock();
                       emit message_pass("Data Sent");

                        m_r.lock();
                        wc_r.wait(&m_r,QDeadlineTimer(5000));
                        m_r.unlock();

                        if(!((rep[0]='#')&&((int)rep[1]==papis_slaves.at(idx)->rs485_addr)
                                &&(rep[2]=='1')&&(rep[5]=='*')))
                        {
                            emit message_pass("No Response");
                           break;
                        }

                        emit message_pass("Ack Rxd");
                        dsize=sizeof(dataPacket2_t);
                        memcpy((void*)cmdp,(void*)&dp2,dsize);

                        this->msleep(1000);

                        emit message_pass("Sending Data-2");
                       rep.clear();
                       emit shoot_data();
                       m_w.lock();
                       wc_w.wait(&m_w);
                       m_w.unlock();

                        m_r.lock();
                        wc_r.wait(&m_r,QDeadlineTimer(5000));
                        m_r.unlock();

                        if(!((rep[0]='#')&&((int)rep[1]==papis_slaves.at(idx)->rs485_addr)
                                &&(rep[2]=='2')&&(rep[5]=='*')))
                        {
                            emit message_pass("No Response");
                           break;
                        }
                        emit message_pass("Accepted");
                        this->msleep(1000);

                        for(int idx=0;idx<2048;idx++){cmd[idx]=0;}
                        //send disp cmd
                        cmd[0]='#';
                        cmd[1]=papis_slaves.at(idx)->rs485_addr;
                        cmd[2]='d';
                        cc = crcFast(cmd,3);
                        cmd[3]=cc&0xff;
                        cmd[4]=(cc>>8)&0xff;
                        cmd[5]='*';
                         dsize=6;
                         memcpy((void*)cmdp,(void*)cmd,6);
                        emit message_pass("Displayed");
                         this->msleep(1000);

                         rep.clear();
                         emit shoot_data();
                         m_w.lock();
                         wc_w.wait(&m_w);
                         m_w.unlock();

                         this->msleep(500);
                   }
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
            s_driver_task=SD_IDLE;
        }
            break;
        case SD_DRIVE_LED_PIS:
        {
            dp1.head='#';
            dp1.foot='*';
            dp1.pIdx=1;
            dp1.sspeed=(unsigned char)systemconfig.led_pis_speed;
            dp1.dispType = (int)cTask->dtype;
            dp1.scrlDir = (int)cTask->sdir;
            dp1.statCols = 0;
            for(int idx=0;idx<512;idx++)
            {
                dp1.fb[idx] = cTask->data[idx];
            }
            dp1.crc = (int)crcFast((unsigned char*)dp1.fb,512);

            dp2.head='#';
            dp2.foot='*';
            dp2.pIdx=2;
            for(int idx=512;idx<1024;idx++)
            {
                dp2.fb[idx-512] = cTask->data[idx];
            }
            dp2.crc = crcFast((unsigned char*)dp2.fb,512);

            for(int idx=0; idx< papis_slaves.count();idx++)
            {
                if((papis_slaves.at(idx)->installStatus==INSTALLED&&papis_slaves.at(idx)->activeStatus==ACTIVE
                    &&papis_slaves.at(idx)->conn_mode=="RS485"))
                {
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
                   {
                        dp1.addr=papis_slaves.at(idx)->rs485_addr;
                        dp2.addr=papis_slaves.at(idx)->rs485_addr;

                       for(int idx=0;idx<2048;idx++){cmd[idx]=0;}
                       cmd[0]='#';
                       cmd[1]=papis_slaves.at(idx)->rs485_addr;
                       cmd[2]='r';
                       unsigned short cc = crcFast(cmd,3);
                       cmd[3]=cc&0xff;
                       cmd[4]=(cc>>8)&0xff;
                       cmd[5]='*';
                       memcpy((void*)cmdp,(void*)cmd,6);
                        dsize=6;

                        rep.clear();
                        emit message_pass("Search Addr: "+QString::number(papis_slaves.at(idx)->rs485_addr));
                        emit shoot_data();
                        m_w.lock();
                        wc_w.wait(&m_w);
                        m_w.unlock();

                        m_r.lock();
                        wc_r.wait(&m_r,QDeadlineTimer(5000));
                        m_r.unlock();

                        if(!((rep[0]='#')&&((int)rep[1]==papis_slaves.at(idx)->rs485_addr)
                                &&(rep[2]=='y')&&(rep[5]=='*')))
                        {
                            emit message_pass("Not found");
                           break;
                        }

                        emit message_pass("Slave Found, Sending Data-1");
                        this->msleep(1000);
                        dsize=sizeof(dataPacket1_t);
                        memcpy((void*)cmdp,(void*)&dp1,dsize);

                       rep.clear();
                       emit shoot_data();
                       m_w.lock();
                       wc_w.wait(&m_w);
                       m_w.unlock();
                       emit message_pass("Data Sent");

                        m_r.lock();
                        wc_r.wait(&m_r,QDeadlineTimer(5000));
                        m_r.unlock();

                        if(!((rep[0]='#')&&((int)rep[1]==papis_slaves.at(idx)->rs485_addr)
                                &&(rep[2]=='1')&&(rep[5]=='*')))
                        {
                            emit message_pass("No Response");
                           break;
                        }

                        emit message_pass("Ack Rxd");
                        this->msleep(1000);
                        dsize=sizeof(dataPacket2_t);
                        memcpy((void*)cmdp,(void*)&dp2,dsize);

                        this->msleep(1000);

                        emit message_pass("Sending Data-2");
                       rep.clear();
                       emit shoot_data();
                       m_w.lock();
                       wc_w.wait(&m_w);
                       m_w.unlock();

                        m_r.lock();
                        wc_r.wait(&m_r,QDeadlineTimer(5000));
                        m_r.unlock();

                        if(!((rep[0]='#')&&((int)rep[1]==papis_slaves.at(idx)->rs485_addr)
                                &&(rep[2]=='2')&&(rep[5]=='*')))
                        {
                            emit message_pass("No Response");
                           break;
                        }
                        emit message_pass("Accepted");
                        this->msleep(1000);

                        for(int idx=0;idx<2048;idx++){cmd[idx]=0;}
                        //send disp cmd
                        cmd[0]='#';
                        cmd[1]=0;
                        cmd[2]='d';
                        cc = crcFast(cmd,3);
                        cmd[3]=cc&0xff;
                        cmd[4]=(cc>>8)&0xff;
                        cmd[5]='*';
                         dsize=6;
                         memcpy((void*)cmdp,(void*)cmd,6);

                         emit message_pass("Displayed");
                         this->msleep(500);

                         rep.clear();
                         emit shoot_data();
                         m_w.lock();
                         wc_w.wait(&m_w);
                         m_w.unlock();

                   }

                       break;
                   case DEV_TMS:

                       break;
                    default:
                       break;
                   }
                }
            }
            s_driver_task=SD_IDLE;
        }
            break;
        case SD_EMERGENCY:
        {
            for(int idx=0;idx<6;idx++){cmd[idx]=0;}
            cmd[0]='#';
            cmd[1]=t_addr;
            cmd[2]='a';
            unsigned short cc = crcFast(cmd,3);
            cmd[3]=cc&0xff;
            cmd[4]=(cc>>8)&0xff;
            cmd[5]='*';
            cmdp=cmd;
            dsize=6;

            rep.clear();
            emit message_pass("Toilet Ack Sent");
            emit shoot_data();
            m_w.lock();
            wc_w.wait(&m_w);
            m_w.unlock();

            m_r.lock();
            wc_r.wait(&m_r,QDeadlineTimer(5000));
            m_r.unlock();
            this->msleep(1000);

            s_driver_task=SD_IDLE;
        }
            break;

        }
    }
}

void slaveDriverThread::rs485_drive_matrix(s_driver_tasks_t s_d_task,const char* d, int s, dispType_t dt, scrlDir_t sd, day_night_mode_t mode)
{
    sdtask_t *task = new sdtask_t();
    task->s_driver_task=s_d_task;
    task->data=d;
    task->size=s;
    task->dtype=dt;
    task->sdir=sd;
    task->mode=mode;
    sd_queue.enqueue(task);
}

void slaveDriverThread::rs485_emergency_ack(int n)
{
    t_addr=(unsigned char)n;
//    s_driver_task=SD_EMERGENCY;

    sdtask_t *task = new sdtask_t();
    task->s_driver_task=SD_EMERGENCY;
    sd_queue.enqueue(task);
}

void slaveDriverThread::rs485_scan_all(QList <slave*> slaves)
{
    papis_slaves = slaves;
//    s_driver_task=SD_NEG_ALL;

    sdtask_t *task = new sdtask_t();
    task->s_driver_task=SD_NEG_ALL;
    sd_queue.enqueue(task);
}

void slaveDriverThread::rs485_scan(int addr)
{

}

//void slaveDriverThread::rs485_drive_ddb_sl(const char* data, int size, dispType_t dtype, scrlDir_t sdir, int scols)
//{

//}

//void slaveDriverThread::rs485_drive_ddl_dl(const char* data, int size, dispType_t dtype, scrlDir_t sdir, int scols)
//{

//}
