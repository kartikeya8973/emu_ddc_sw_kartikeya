#ifndef SERVICERECORD_H
#define SERVICERECORD_H

#include <QObject>
#include <QDateTime>
#include <QJsonArray>
#include <QJsonObject>
#include "common.h"

class servicerecord : public QObject
{
    Q_OBJECT
public:
    explicit servicerecord(QObject *parent = nullptr);
    servicerecord(int,maintenance_ops_t,QString,QString,QString);

signals:

public:
    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;
    void update_status(QString);

    int sno;
    maintenance_ops_t op;
    QDateTime issue_dt;
    QString issue_src;
    QString user;
    QString status;

private:

};

#endif // SERVICERECORD_H
