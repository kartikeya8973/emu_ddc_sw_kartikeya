#ifndef SLAVEDRIVERTHREAD_H
#define SLAVEDRIVERTHREAD_H

#include <QObject>
#include <QThread>
#include <QSerialPort>
#include <QTimer>
#include <QWaitCondition>
#include <QMutex>
#include "common.h"
#include "systemconfig.h"
#include "gpio_utils.h"
#include "crc.h"
#include "slave.h"
#include "toilet_stat.h"

#define RS485_IO        38
#define OP_ON           1
#define OP_OFF          0

class slaveDriverThread: public QThread
{
    Q_OBJECT

public:
    slaveDriverThread();


private slots:

    void rs485_Received();
    void rs485_drive_matrix(s_driver_tasks_t s_d_task,const char* d, int s, dispType_t dt, scrlDir_t sd, day_night_mode_t mode);
    void rs485_scan_all(QList <slave*> slaves);
    void rs485_scan(int addr);

    void tmsTimeout();
    void onTimeout();
    void offTimeout();

    void shoot();
    void rs485_emergency_ack(int);

public slots:


signals:
    void shoot_data();
    void slave_silent_scan(QList <slave*> slaves);
    void slave_scan_progress(int pc,QList <slave*> slaves);
    void message_pass(QString);
    void toilet_status(toilet_stat);

protected:
    void run() override;

private:
    QSerialPort* rs485Port;
    gpio_utils gpio;
    QTimer *onTimer;
    QTimer *offTimer;

    char tmsSendData[3];
    QTimer* tmsTimer;
    dataPacket1_t dp1;
    dataPacket2_t dp2;

    char cmd[2048];
    const char *cmdp;
    int dsize;
    QByteArray rep;

    QWaitCondition wc_r;
    QWaitCondition wc_w;
    QMutex m_r;
    QMutex m_w;      

    bool dataR=false;

    toilet_status_t t_stat;
    unsigned char t_addr;
    int slaveIdx=0;

    QList <slave*> papis_slaves;
    s_driver_tasks_t s_driver_task=SD_IDLE;
    QQueue <sdtask_t*> sd_queue;
    sdtask_t *cTask;
    bool int_neg=false;

};

#endif // SLAVEDRIVERTHREAD_H
