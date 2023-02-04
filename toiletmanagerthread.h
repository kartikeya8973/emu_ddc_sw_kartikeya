#ifndef TOILETMANAGERTHREAD_H
#define TOILETMANAGERTHREAD_H

#include <QObject>
#include <QThread>
#include <QSerialPort>
#include <QWaitCondition>
#include "common.h"
#include "crc.h"
#include "slave.h"
#include "toilet_stat.h"

class toiletmanagerthread : public QThread
{
    Q_OBJECT
public:
    toiletmanagerthread();

signals:
    void message_pass(QString);
    void toilet_status(int,toilet_packet_t);


private slots:
//    void serverReadyRead();
protected:
    void run() override;

private:
    toilet_status_t t_stat;
    QTcpSocket *sock;
    int base=0;
};

#endif // TOILETMANAGERTHREAD_H
