#ifndef ROUTE_H
#define ROUTE_H

#include <QString>
#include <QJsonObject>
class route
{
public:
    route();

    int     sno;
    QString tr_no_en="";
    QString tr_nm_en="";
    QString tr_route_en="";
    QString tr_via_en="";
    bool    d_av=true;
    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;
    void write_s(QJsonObject &json) const;
};

#endif // ROUTE_H
