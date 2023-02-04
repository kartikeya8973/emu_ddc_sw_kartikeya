#ifndef ROUTES_H
#define ROUTES_H

#include "route.h"
#include <QList>
#include <QJsonObject>
#include <QJsonArray>
class routes
{
public:
    routes();

    QList<int> routeSel;
    QList<route> routeList;
    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;
};

#endif // ROUTES_H
