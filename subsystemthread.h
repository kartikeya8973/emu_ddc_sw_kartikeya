#ifndef SUBSYSTEMTHREAD_H
#define SUBSYSTEMTHREAD_H

#include <QObject>
#include <QThread>
#include <QSerialPort>
#include <QWaitCondition>
#include <QMutex>
#include <QFile>
#include "common.h"
#include "crc.h"

class subsystemthread : public QThread
{
    Q_OBJECT
public:
    subsystemthread();

private slots:
    void ss_Received();
    void ss_transmit(QByteArray,int);
    void ss_transmit(QByteArray);
//    void ss_transmit();
signals:
    void transmit(QByteArray,int);
    void transmit(QByteArray);
    void ss_present();
    void ss_absent();
    void lte_present();
    void lte_absent();

    void pow_lo();
    void pow_hi();
    void pow_ok();
    void mic_in();
    void mic_out();


//    void transmit();
protected:
        void run() override;
private:
    QSerialPort* ssPort;

    ss_tasks_t ss_task = SS_BOOT;
    int sctr=0;
    int actr=0;
};

#endif // SUBSYSTEMTHREAD_H
