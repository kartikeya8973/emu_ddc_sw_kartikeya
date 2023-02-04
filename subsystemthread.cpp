#include "subsystemthread.h"

subsystemthread::subsystemthread()
{
    crcInit();
    ssPort = new QSerialPort(this);

#ifndef LAPTOP
    ssPort->setPortName("/dev/apalis-uart3");
#else
    ssPort->setPortName("/dev/ttyUSB0");
#endif
    ssPort->setBaudRate(QSerialPort::Baud115200);
    ssPort->setDataBits(QSerialPort::Data8);
    ssPort->setParity(QSerialPort::NoParity);
    ssPort->setStopBits(QSerialPort::OneStop);
    ssPort->setFlowControl(QSerialPort::NoFlowControl);

    if(ssPort->open(QIODevice::ReadWrite))
    {
        connect(ssPort,SIGNAL(readyRead()),this,SLOT(ss_Received()));
        connect(this,SIGNAL(transmit(QByteArray,int)),this,SLOT(ss_transmit(QByteArray,int)));
        connect(this,SIGNAL(transmit(QByteArray)),this,SLOT(ss_transmit(QByteArray)));
    }

//    ss_task = SS_IDLE;

}

void subsystemthread::ss_Received()
{
    QByteArray ar = ssPort->readAll();

    switch(ss_task)
    {
    case SS_BOOT:
        /* *SY# */
        if(!ar.compare("*SY#"))
        {
            sctr++;
            if(sctr==3)
            {
                ss_task = SS_LTE;
                sctr=0;
                actr=0;
                emit ss_present();
            }
        }
        else
        {
            sctr=0;
        }

        break;
    case SS_LTE:
        break;
    case SS_IDLE:
        if(ar.at(0)=='*'&&ar.at(3)=='#')
        {
            switch(ar.at(1))
            {
            case 0:
                emit pow_ok();
                break;
            case 1:
                emit pow_lo();
                break;
            case 2:
                emit pow_hi();
                break;
            }

            emit mic_out();

            switch(ar.at(2))
            {
            case 0:
                emit mic_out();
                break;
            case 1:
                emit mic_in();
                break;
            }
        }
        break;
    }
}

void subsystemthread::ss_transmit(QByteArray buf,int size)
{
    ssPort->write(buf,size);
}

void subsystemthread::ss_transmit(QByteArray buf)
{
    ssPort->write(buf);
}

void subsystemthread::run()
{
    int idx;
    forever
    {
        switch(ss_task)
        {
        case SS_BOOT:
            emit transmit("*SY#");
            actr++;
            if(actr==20)
            {
                emit ss_absent();
                sctr=0;
                actr=0;
            }
            this->msleep(1000);
            break;
        case SS_LTE:
            /* Detect LTE */
            if(QFile::exists("/dev/ttyUSB3"))
            {
                emit lte_present();
            }
            else
            {
                emit lte_absent();
            }
            ss_task = SS_IDLE;
            break;
        case SS_IDLE:
            //get data from all toilet units
            emit transmit("*ST#");

            this->msleep(3000);
            break;
        }
    }
}
