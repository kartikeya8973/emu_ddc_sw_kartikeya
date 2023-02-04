#ifndef AUDIOPLAYERTHREAD_H
#define AUDIOPLAYERTHREAD_H

#include <QObject>
#include <QThread>
#include <QQueue>
#include <QWaitCondition>
#include <QMutex>
#include "systemconfig.h"
#include "common.h"

class audioplayerthread : public QThread
{
    Q_OBJECT
public:
    audioplayerthread();

private slots:
    void play();
    void add(QString);
    void add(QString,announcementType_t);

signals:

protected:
    void run() override;

private:

    QQueue <QString> pa_queue;
    QWaitCondition wc_a;
    QMutex m_a;
    QQueue <announcementType_t> announcementType_queue;

};

#endif // AUDIOPLAYERTHREAD_H
