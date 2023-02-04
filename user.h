#ifndef USER_H
#define USER_H

#include <QJsonObject>
#include "common.h"
class user
{
public:
    QString name="";
    QString password="";
    access_level_t access_level;
    activeStatus_t activeStatus;

    user();

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;
};

#endif // USER_H
